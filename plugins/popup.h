#ifndef plugins_popup_h_
#define plugins_popup_h_

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

void
gol_popup_get_screen_rect(GdkRectangle*);

GtkWidget*
gol_popup_new(void);

void
gol_popup_move(GtkWidget*, gint, gint);

void
gol_popup_move_resize(GtkWidget*, gint, gint, gint, gint);

#ifdef __cplusplus
}
#endif

#endif /* plugins_popup_h_ */
