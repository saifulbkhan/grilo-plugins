#
# Makefile.am
#
# Author: Iago Toral Quiroga <itoral@igalia.com>
#
# Copyright (C) 2010, 2011 Igalia S.L. All rights reserved.

include $(top_srcdir)/gtester.mk

ext_LTLIBRARIES = libgrlyoutube.la

libgrlyoutube_la_CFLAGS =	\
	$(DEPS_YOUTUBE_CFLAGS)	\
	-DG_LOG_DOMAIN=\"GrlYoutube\" \
	-DLOCALEDIR=\"$(localedir)\"

libgrlyoutube_la_LIBADD =	\
	$(DEPS_YOUTUBE_LIBS)

libgrlyoutube_la_LDFLAGS = \
	-no-undefined      \
	-module		   \
	-avoid-version

libgrlyoutube_la_SOURCES = grl-youtube.c grl-youtube.h
nodist_libgrlyoutube_la_SOURCES = \
	youtuberesources.h \
	youtuberesources.c

extdir				= $(GRL_PLUGINS_DIR)

youtuberesources.h: youtube.gresource.xml
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) $(srcdir)/youtube.gresource.xml \
		--target=$@ --sourcedir=$(srcdir) --c-name _grl_youtube --generate-header
youtuberesources.c: youtube.gresource.xml youtuberesources.h channel-youtube.svg
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) $(srcdir)/youtube.gresource.xml \
		--target=$@ --sourcedir=$(srcdir) --c-name _grl_youtube --generate-source

resource_sources = \
	youtuberesources.c \
	youtuberesources.h \
	$(NULL)
CLEANFILES = $(resource_sources)

EXTRA_DIST += channel-youtube.svg youtube.gresource.xml

-include $(top_srcdir)/git.mk
