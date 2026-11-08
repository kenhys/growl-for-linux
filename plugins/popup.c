#include <string.h>

#include <gtk/gtk.h>

#include "popup.h"

#if GTK_CHECK_VERSION(3, 22, 0)
static GdkMonitor*
get_monitor(void) {
  GdkDisplay* const display = gdk_display_get_default();
  GdkMonitor* const monitor = gdk_display_get_primary_monitor(display);
  // Wayland never has a primary monitor, and X11 has one only when the user sets it.
  return monitor ? monitor : gdk_display_get_monitor(display, 0);
}
#endif

void
gol_popup_get_screen_rect(GdkRectangle* const rect) {
  memset(rect, 0, sizeof(*rect));
#if GTK_CHECK_VERSION(3, 22, 0)
  GdkMonitor* const monitor = get_monitor();
  if (monitor) gdk_monitor_get_geometry(monitor, rect);
#else
  GdkScreen* const screen = gdk_screen_get_default();
  const gint monitor_num = gdk_screen_get_primary_monitor(screen);
  gdk_screen_get_monitor_geometry(screen, monitor_num, rect);
#endif
}
