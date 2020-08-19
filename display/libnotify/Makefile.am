displaydir = $(pkglibdir)/display
display_LTLIBRARIES = libnotify_gol.la
libnotify_gol_la_SOURCES = notify.c display_libnotify.xpm
libnotify_gol_la_LDFLAGS = -L../../plugins
if USE_GTK3
libnotify_gol_la_CFLAGS = $(GTK3_CFLAGS) $(NOTIFY_CFLAGS) -I$(srcdir)/../..
libnotify_gol_la_LIBADD = $(GTK3_LIBS) $(NOTIFY_LIBS) -lgolplug
else
libnotify_gol_la_CFLAGS = $(GTK2_CFLAGS) $(NOTIFY_CFLAGS) -I$(srcdir)/../..
libnotify_gol_la_LIBADD = $(GTK2_LIBS) $(NOTIFY_LIBS) -lgolplug
endif

#EXTRA_DIST = Makefile.w32
