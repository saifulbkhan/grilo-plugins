#
# Makefile.am
#
# Author: Bastien Nocera <hadess@hadess.net>
#
# Copyright (C) 2014 Red Hat Inc.

include $(top_srcdir)/gtester.mk

TEST_PROGS +=	\
   test_local_metadata

test_local_metadata_defines =                                                    \
   -DLUA_FACTORY_PLUGIN_PATH=\""$(abs_top_builddir)/src/lua-factory/.libs/"\"    \
   -DLUA_SOURCES_PATH=\""$(abs_top_srcdir)/src/lua-factory/sources/"\"

test_local_metadata_SOURCES =	\
	test_local_metadata.c

test_local_metadata_LDADD =	\
	@DEPS_LIBS@

test_local_metadata_CFLAGS =	\
	@DEPS_CFLAGS@					\
	$(test_local_metadata_defines)

noinst_PROGRAMS = $(TEST_PROGS)

-include $(top_srcdir)/git.mk
