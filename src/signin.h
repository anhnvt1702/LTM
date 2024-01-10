#ifndef SIGNIN_H
#define SIGNIN_H

#include <gtk/gtk.h>

extern GtkWidget *login_window;
extern GtkWidget *register_window;
extern GtkWidget *username_entry;
extern GtkWidget *password_entry;

void open_login_window(GtkWidget *widget, gpointer data);
void open_register_window(GtkWidget *widget, gpointer data);
// void login(GtkWidget *widget, gpointer data);
void register_user(GtkWidget *widget, gpointer data);

#endif /* SIGNIN_H */
