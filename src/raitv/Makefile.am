#
# Makefile.am
#
# Author: Marco Piazza <mpiazza@gmail.com>
#

include $(top_srcdir)/gtester.mk

ext_LTLIBRARIES		 = libgrlraitv.la

libgrlraitv_la_CFLAGS =	\
	$(DEPS_RAITV_CFLAGS)	\
	-DG_LOG_DOMAIN=\"GrlRaitv\" \
	-DLOCALEDIR=\"$(localedir)\"

libgrlraitv_la_LIBADD =	\
	$(DEPS_RAITV_LIBS)

libgrlraitv_la_LDFLAGS = \
	-no-undefined	  \
	-module		  \
	-avoid-version

libgrlraitv_la_SOURCES = 		\
	grl-raitv.c			\
	grl-raitv.h
nodist_libgrlraitv_la_SOURCES = \
	$(resource_sources)

extdir 			= $(GRL_PLUGINS_DIR)

raitvresources.h: raitv.gresource.xml
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) $(srcdir)/raitv.gresource.xml \
		--target=$@ --sourcedir=$(srcdir) --c-name _grl_raitv --generate-header
raitvresources.c: raitv.gresource.xml raitvresources.h channel-rai.svg
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) $(srcdir)/raitv.gresource.xml \
		--target=$@ --sourcedir=$(srcdir) --c-name _grl_raitv --generate-source

resource_sources = \
	raitvresources.c \
	raitvresources.h \
	$(NULL)
CLEANFILES = $(resource_sources)

EXTRA_DIST += channel-rai.svg raitv.gresource.xml

-include $(top_srcdir)/git.mk
