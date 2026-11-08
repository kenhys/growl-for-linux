noinst_LIBRARIES = libgolplug.a
libgolplug_a_SOURCES = memfile.c memfile.h from_url.c from_url.h popup.c popup.h
if USE_GTK3
libgolplug_a_CFLAGS = $(GTK3_CFLAGS) $(GTK_LAYER_SHELL_CFLAGS) $(LIBCURL_CFLAGS) -fPIC -I$(srcdir)/..
else
libgolplug_a_CFLAGS = $(GTK2_CFLAGS) $(LIBCURL_CFLAGS) -fPIC -I$(srcdir)/..
endif
#libgolplug_a_LIBADD = $(GTK2_LIBS) $(LIBCURL_LIBS)

EXTRA_DIST = Makefile.w32
