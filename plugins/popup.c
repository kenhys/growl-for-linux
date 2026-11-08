#include <string.h>

#include <gtk/gtk.h>
#ifdef HAVE_GTK_LAYER_SHELL
# include <gtk-layer-shell.h>
#endif

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

GtkWidget*
gol_popup_new(void) {
#ifdef HAVE_GTK_LAYER_SHELL
  if (gtk_layer_is_supported()) {
    // Wayland does not map a popup window without a parent, so use a toplevel one.
    GtkWidget* const popup = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWindow* const window = GTK_WINDOW(popup);
    gtk_layer_init_for_window(window);
    gtk_layer_set_namespace(window, "growl-for-linux");
    gtk_layer_set_layer(window, GTK_LAYER_SHELL_LAYER_OVERLAY);
    gtk_layer_set_monitor(window, get_monitor());
    // Margins are from the edges of the monitor only when panels are ignored.
    gtk_layer_set_exclusive_zone(window, -1);
    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_TOP, TRUE);
    return popup;
  }
#endif
  return gtk_window_new(GTK_WINDOW_POPUP);
}

#ifdef HAVE_GTK_LAYER_SHELL
static void
set_vertical_anchor(GtkWindow* const window, const GtkLayerShellEdge edge) {
  const GtkLayerShellEdge other =
    edge == GTK_LAYER_SHELL_EDGE_TOP ? GTK_LAYER_SHELL_EDGE_BOTTOM : GTK_LAYER_SHELL_EDGE_TOP;
  if (gtk_layer_get_anchor(window, edge)) return;
  gtk_layer_set_anchor(window, other, FALSE);
  gtk_layer_set_anchor(window, edge, TRUE);
}
#endif

void
gol_popup_move(GtkWidget* const popup, const gint x, const gint y) {
#ifdef HAVE_GTK_LAYER_SHELL
  if (gtk_layer_is_layer_window(GTK_WINDOW(popup))) {
    GtkWindow* const window = GTK_WINDOW(popup);
    GdkRectangle rect;
    gol_popup_get_screen_rect(&rect);
    set_vertical_anchor(window, GTK_LAYER_SHELL_EDGE_TOP);
    // A layer surface that is mapped mostly out of the monitor is not shown
    // even after it is moved in, so keep the whole popup on the monitor.
    GtkRequisition size;
    gtk_widget_get_preferred_size(popup, NULL, &size);
    gtk_layer_set_margin(window, GTK_LAYER_SHELL_EDGE_LEFT, MIN(x - rect.x, rect.width - size.width));
    gtk_layer_set_margin(window, GTK_LAYER_SHELL_EDGE_TOP, MIN(y - rect.y, rect.height - size.height));
    return;
  }
#endif
  gtk_window_move(GTK_WINDOW(popup), x, y);
}

void
gol_popup_move_resize(GtkWidget* const popup, const gint x, const gint y, const gint width, const gint height) {
#ifdef HAVE_GTK_LAYER_SHELL
  if (gtk_layer_is_layer_window(GTK_WINDOW(popup))) {
    // A layer surface is not clipped by the monitor and can not be smaller than
    // its children. Show only the part on the monitor, and keep its bottom edge
    // there, so that the popup grows up like on X11.
    GtkWindow* const window = GTK_WINDOW(popup);
    GdkRectangle rect;
    gol_popup_get_screen_rect(&rect);
    const gint bottom = MIN(y + height, rect.y + rect.height);
    set_vertical_anchor(window, GTK_LAYER_SHELL_EDGE_BOTTOM);
    gtk_layer_set_margin(window, GTK_LAYER_SHELL_EDGE_LEFT, MIN(x - rect.x, rect.width - 1));
    gtk_layer_set_margin(window, GTK_LAYER_SHELL_EDGE_BOTTOM, rect.y + rect.height - bottom);
    gtk_widget_set_size_request(popup, width, MAX(bottom - y, 1));
    return;
  }
#endif
  gdk_window_move_resize(gtk_widget_get_window(popup), x, y, width, height);
}
