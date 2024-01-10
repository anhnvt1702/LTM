#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

void on_quit_activate (GtkMenuItem *menuitem, gpointer data);

void on_setup_activate (GtkMenuItem *menuitem, gpointer data);

void on_entry_name_activate (GtkEntry *entry, gpointer data);

void on_toggle_button_host_toggled (GtkToggleButton *togglebutton, gpointer data);

void on_toggle_button_connect_toggled (GtkToggleButton *togglebutton, gpointer data);

void on_button_start_clicked (GtkButton *button, gpointer data);

void on_button_exchange_clicked (GtkButton *button, gpointer data);

void on_button_draw_clicked (GtkButton *button, gpointer data);

void on_button_resign_clicked (GtkButton *button, gpointer data);

gboolean on_fixed_button_press_event (GtkWidget *widget, GdkEventButton  *event, gpointer data);

void on_entry_talk_activate (GtkEntry *entry, gpointer data);

void on_button_yes_clicked (GtkButton *button, gpointer data);

void on_button_no_clicked (GtkButton *button, gpointer data);

void on_button_cancel_clicked (GtkButton *button, gpointer data);

#ifdef __cplusplus
}
#endif

#endif /* __CALLBACK_H__ */
