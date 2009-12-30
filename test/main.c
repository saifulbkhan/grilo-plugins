#include <glib.h>
#include <string.h>

#include <media-store.h>

static void
print_supported_ops (MsMetadataSource *source)
{
  g_print ("  Operations available in '%s'\n",
	   ms_metadata_source_get_name (source));

  MsSupportedOps caps = ms_metadata_source_supported_operations (source);

  if (caps & MS_OP_METADATA) {
    g_print ("    + Metadata\n");
  }
  if (caps & MS_OP_RESOLVE) {
    g_print ("    + Resolution\n");
  }
  if (caps & MS_OP_BROWSE) {
    g_print ("    + Browse\n");
  }
  if (caps & MS_OP_SEARCH) {
    g_print ("    + Search\n");
  }
}

static void
print_metadata (MsContent *content, MsKeyID key_id)
{
  if (key_id == MS_METADATA_KEY_DESCRIPTION) {
    return;
  }

  MsPluginRegistry *registry = ms_plugin_registry_get_instance ();
  const MsMetadataKey *key = 
    ms_plugin_registry_lookup_metadata_key (registry,
					    GPOINTER_TO_UINT (key_id));

  const GValue *value = ms_content_get (MS_CONTENT(content), key_id);
  if (value && G_VALUE_HOLDS_STRING (value)) {
    g_print ("\t%s: %s\n", MS_METADATA_KEY_GET_NAME (key),
	     g_value_get_string (value));
  } else if (value && G_VALUE_HOLDS_INT (value)) {
    g_print ("\t%s: %d\n",  MS_METADATA_KEY_GET_NAME (key),
	     g_value_get_int (value));
  }
}

static void 
browse_cb (MsMediaSource *source,
	   guint browse_id,
           MsContent *media,
	   guint remaining,
	   gpointer user_data,
	   const GError *error)
{
  MsKeyID *keys;
  gint size;
  gint i;
  static guint index = 0;

  g_print ("  browse result (%d - %d|%d)\n",
	   browse_id, index++, remaining);

  if (error) {
    g_error ("Got error from browse: %s", error->message);
  }

  if (!media && remaining == 0) {
    g_print ("  No results\n");
    return;
  }

  g_print ("\tContainer: %s\n",
	   ms_content_is_container (media) ? "yes" : "no");

  keys = ms_content_get_keys (media, &size);
  for (i = 0; i < size; i++) {
    print_metadata (media, keys[i]);
  }
  g_free (keys);
  g_object_unref (media);

  if (remaining == 0) {
    g_print ("  Browse operation finished\n");
  }
}

static void 
metadata_cb (MsMetadataSource *source,
	     MsContent *media,
	     gpointer user_data,
	     const GError *error)
{
  MsKeyID *keys;
  gint size;
  gint i;

  g_print ("  metadata_cb\n");

  if (error) {
    g_print ("Error: %s\n", error->message);
    return;
  }
  
  g_print ("    Got metadata for object '%s'\n",
	   ms_content_media_get_id (MS_CONTENT_MEDIA (media)));

  keys = ms_content_get_keys (media, &size);
  for (i = 0; i < size; i++) {
    print_metadata (media, keys[i]);
  }
  g_free (keys);
  g_object_unref (media);

  g_print ("  Metadata operation finished\n");
}

gint
main (void)
{
  GList *keys;

  g_type_init ();

  keys = ms_metadata_key_list_new (MS_METADATA_KEY_ID,
				   MS_METADATA_KEY_TITLE,
                                   MS_METADATA_KEY_ALBUM,
                                   MS_METADATA_KEY_ARTIST,
                                   MS_METADATA_KEY_GENRE,
                                   MS_METADATA_KEY_THUMBNAIL,
				   MS_METADATA_KEY_DESCRIPTION,
				   MS_METADATA_KEY_AUTHOR,
                                   MS_METADATA_KEY_LYRICS,
				   MS_METADATA_KEY_DURATION,
                                   NULL);

  g_print ("start\n");

  g_print ("loading plugins\n");

  MsPluginRegistry *registry = ms_plugin_registry_get_instance ();
  ms_plugin_registry_load (registry, "../plugins/youtube/.libs/libmsyoutube.so");
  ms_plugin_registry_load (registry, "../plugins/fake-metadata/.libs/libfakemetadata.so");

  g_print ("Obtaining sources\n");

  MsMediaSource *youtube = 
    (MsMediaSource *) ms_plugin_registry_lookup_source (registry, "ms-youtube");

  MsMetadataSource *fake = 
    (MsMetadataSource *) ms_plugin_registry_lookup_source (registry, "ms-fake-metadata");

  g_assert (youtube);

  g_print ("Supported operations\n");

  print_supported_ops (MS_METADATA_SOURCE (youtube));
  print_supported_ops (fake);

  g_print ("testing\n");

  if (0) ms_media_source_browse (youtube, NULL, keys, 0, 1, MS_RESOLVE_IDLE_RELAY, browse_cb, NULL);
  if (1) ms_media_source_browse (youtube, "standard-feeds/most-viewed", keys, 100, 5, MS_RESOLVE_IDLE_RELAY, browse_cb, NULL);
  if (0) ms_media_source_browse (youtube, "categories", keys, 28, 10, 0, browse_cb, NULL);
  if (0) ms_media_source_search (youtube, "igalia", keys, NULL, 1, 3, MS_RESOLVE_IDLE_RELAY, browse_cb, NULL);
  if (0) ms_media_source_search (youtube, "igalia", keys, NULL, 1, 10, 0, browse_cb, NULL);
  if (0) ms_metadata_source_get (MS_METADATA_SOURCE (youtube), "okVW_YSHSPU", keys, 0, metadata_cb, NULL);

  g_print ("Running main loop\n");

  GMainLoop *loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);

  g_print ("done\n");

  return 0;
}
