displaydir = $(pkglibdir)/display
display_LTLIBRARIES = libnico2.la
libnico2_la_SOURCES = nico2.c display_nico2.xpm
libnico2_la_LDFLAGS = -L../../plugins
if USE_GTK3
libnico2_la_CFLAGS = $(GTK3_CFLAGS) $(LIBCURL_CFLAGS) $(SQLITE3_CFLAGS) -I$(srcdir)/../..
libnico2_la_LIBADD = $(GTK3_LIBS) $(LIBCURL_LIBS) $(SQLITE3_LIBS) -lgolplug
else
libnico2_la_CFLAGS = $(GTK2_CFLAGS) $(LIBCURL_CFLAGS) $(SQLITE3_CFLAGS) -I$(srcdir)/../..
libnico2_la_LIBADD = $(GTK2_LIBS) $(LIBCURL_LIBS) $(SQLITE3_LIBS) -lgolplug
endif

EXTRA_DIST = Makefile.w32
