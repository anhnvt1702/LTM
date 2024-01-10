#include <gtk/gtk.h>
#include "signin.h"

GtkWidget *login_window;
GtkWidget *register_window;
GtkWidget *username_entry;
GtkWidget *password_entry;

void open_login_window(GtkWidget *widget, gpointer data);
void open_register_window(GtkWidget *widget, gpointer data);
// void login(GtkWidget *widget, gpointer data);
void register_user(GtkWidget *widget, gpointer data);


void open_login_window(GtkWidget *widget, gpointer data)
{
    // Hide the registration window (if it exists)
    if (register_window != NULL)
        gtk_widget_hide(register_window);

    // Create and show the login window
    login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(login_window), "Login Form");
    gtk_container_set_border_width(GTK_CONTAINER(login_window), 10);
    gtk_widget_set_size_request(login_window, 300, 200);

    // Create entry widgets for login
    username_entry = gtk_entry_new();
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(password_entry), '*');

    // Create a button for registration
    GtkWidget *register_button = gtk_button_new_with_label("Register");
    g_signal_connect(register_button, "clicked", G_CALLBACK(open_register_window), NULL);

    // Pack the widgets into the login window
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(login_window), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), register_button, FALSE, FALSE, 0);

    gtk_widget_show_all(login_window);
}

void open_register_window(GtkWidget *widget, gpointer data)
{
    // Hide the login window (if it exists)
    if (login_window != NULL)
        gtk_widget_hide(login_window);

    // Create and show the registration window
    register_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(register_window), "Registration Form");
    gtk_container_set_border_width(GTK_CONTAINER(register_window), 10);
    gtk_widget_set_size_request(register_window, 300, 200);

    // Create entry widgets for registration
    username_entry = gtk_entry_new();
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(password_entry), '*');

    // Create a button for registration
    GtkWidget *register_button = gtk_button_new_with_label("Register");
    g_signal_connect(register_button, "clicked", G_CALLBACK(register_user), NULL);

    // Pack the widgets into the registration window
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(register_window), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), register_button, FALSE, FALSE, 0);

    gtk_widget_show_all(register_window);
}


void register_user(GtkWidget *widget, gpointer data)
{
    const char *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
    FILE *file = fopen("account.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s 1\n", username, password);

        // Đóng tệp tin sau khi ghi
        fclose(file);
    } 
    gtk_widget_hide(register_window);
    gtk_widget_show_all(login_window);
    
}
