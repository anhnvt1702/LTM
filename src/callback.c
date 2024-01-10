/*****************************************************************************/
/*  callback.c - callback fuctions for GUI                                   */
/*  Copyright (C) 2003 980501427 <h980501427@163.com>                      */
/*                                                                           */
/*  This program is free software; you can redistribute it and/or modify      */
/*  it under the terms of the GNU General Public License as published by     */
/*  the Free Software Foundation; either version 2 of the License, or        */
/*  (at your option) any later version.                                     */
/*                                                                          */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU General Public License for more details.                             */
/*                                                                          */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program; if not, write to the Free Software              */
/*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/*****************************************************************************/

#include "config.h"
#include <gtk/gtk.h>
#include "createwindow.h"
#include "callback.h"
#include "cchess.h"
#include "network.h"

void on_quit_activate (GtkMenuItem *menuitem, gpointer data) {
  GtkWidget *toggle_button_connect = get_widget (GTK_WIDGET (menuitem), "toggle_button_connect");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (toggle_button_connect))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle_button_connect), FALSE);
  }
  GtkWidget *toggle_button_host = get_widget (GTK_WIDGET (menuitem), "toggle_button_host");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (toggle_button_host))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle_button_host), FALSE);
  }
  gtk_main_quit ();
}

void on_setup_activate (GtkMenuItem *menuitem, gpointer data) {

}

void on_entry_name_activate (GtkEntry *entry, gpointer data) {
  GtkWidget *toggle_button_connect;
  toggle_button_connect = get_widget (GTK_WIDGET (entry), "toggle_button_connect");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (toggle_button_connect))) {
    return;
  } else {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle_button_connect), TRUE);
  }
}

void on_toggle_button_host_toggled (GtkToggleButton *togglebutton, gpointer data) {
  static gint server_sockfd_id = -1;
  if (server_sockfd_id == -1) {
    GtkWidget *entry_port = get_widget (GTK_WIDGET (togglebutton), "entry_port");
    GtkWidget *entry_name = get_widget (GTK_WIDGET (togglebutton), "entry_name");
    gint server_sockfd = create_server (togglebutton, 
                                       gtk_entry_get_text (GTK_ENTRY (entry_port)),
                                       gtk_entry_get_text (GTK_ENTRY (entry_name)));
    if (server_sockfd < 0) {
      server_sockfd_id = -2;
      gtk_toggle_button_set_active (togglebutton, FALSE);
    } else {
      GIOChannel *channel = g_io_channel_unix_new (server_sockfd);
      server_sockfd_id = g_io_add_watch_full (channel,
                                            G_PRIORITY_DEFAULT,
                                            G_IO_IN,
                                            (GIOFunc) read_server_socket,
                                            (gpointer) togglebutton,
                                            (GDestroyNotify) close_server_socket);
    }
  } else {
    if (server_sockfd_id > 0) {
      g_source_remove (server_sockfd_id);
    }
    server_sockfd_id = -1;
  }
}

void on_toggle_button_connect_toggled (GtkToggleButton *togglebutton, gpointer data) {
  static gint sockfd_id = -1;
  if (sockfd_id == -1) {
    GtkWidget *entry_ip = get_widget (GTK_WIDGET (togglebutton), "entry_ip");
    GtkWidget *entry_port = get_widget (GTK_WIDGET (togglebutton), "entry_port");
    GtkWidget *entry_name = get_widget (GTK_WIDGET (togglebutton), "entry_name");
    gint sockfd = do_connect (togglebutton,
                              gtk_entry_get_text (GTK_ENTRY (entry_ip)),
                              gtk_entry_get_text (GTK_ENTRY (entry_port)),
                              gtk_entry_get_text (GTK_ENTRY (entry_name)));
    if (sockfd < 0) {
      sockfd_id = -2;
      gtk_toggle_button_set_active (togglebutton, FALSE);
    } else {
      GIOChannel *channel = g_io_channel_unix_new (sockfd);
      sockfd_id = g_io_add_watch_full (channel,
                                    G_PRIORITY_DEFAULT,
                                    G_IO_IN,
                                    (GIOFunc) read_socket,
                                    (gpointer) togglebutton,
                                    (GDestroyNotify) close_socket);
    }
  } else {
    if (sockfd_id > 0) {
      change_chess_side (0);
      write_socket ("quit");
      g_source_remove (sockfd_id);
    }
    sockfd_id = -1;
  }
}

void on_button_start_clicked (GtkButton *button, gpointer data) {
  gint j = get_chess_status ();
  if ((j == -1) || (j > 0)) {
    return;
  }
  if (write_socket ("start") < 0) {
    return;
  }
  init_local_chessman ();
} 

void on_button_exchange_clicked (GtkButton *button, gpointer data) {
  gint j = get_chess_status ();
  if (j > 0) {
    return;
  }
  if (write_socket ("exchange") < 0) {
    return;
  }
  GtkWidget *dialog = create_dialog_wait (4);
  gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (button))));
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ON_PARENT);
  gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
  gtk_widget_show (dialog);
}

void on_button_draw_clicked (GtkButton *button, gpointer data) {
  gint j = get_chess_status ();
  if (j < 1) {
    return;
  }
  if (write_socket ("draw") < 0) {
    return;
  }
  GtkWidget *dialog = create_dialog_wait (5);
  gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (button))));
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ON_PARENT);
  gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
  gtk_widget_show (dialog);
}

void on_button_resign_clicked (GtkButton *button, gpointer data) {
  gint j = get_chess_status ();
  if (j < 1) {
    return;
  }
  GtkWidget *dialog = gtk_dialog_new_with_buttons (_("Resign"),
                                                  GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (button))),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  _("_Yes"),
                                                  GTK_RESPONSE_YES,
                                                  _("_No"),
                                                  GTK_RESPONSE_NO,
                                                  NULL);
  GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

  GtkWidget *dialog_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_show (dialog_box);
  gtk_box_pack_start (GTK_BOX (content_area), dialog_box, TRUE, TRUE, 8);

  GtkWidget *image_question = gtk_image_new_from_icon_name ("dialog-question", GTK_ICON_SIZE_DIALOG);
  gtk_widget_show (image_question);
  gtk_box_pack_start (GTK_BOX (dialog_box), image_question, TRUE, TRUE, 8);

  GtkWidget *label_question = gtk_label_new (_("Resign the game.\n\nAre you sure ?"));
  gtk_widget_show (label_question);
  gtk_box_pack_start (GTK_BOX (dialog_box), label_question, TRUE, TRUE, 8);

  j = gtk_dialog_run (GTK_DIALOG (dialog));
  switch (j) {
    case GTK_RESPONSE_YES:
      write_socket ("resign");
      gtk_widget_destroy (dialog);
      game_over (1);
      break;
    default:
      gtk_widget_destroy (dialog);
      break;
  }
}

gboolean on_fixed_button_press_event (GtkWidget *widget, GdkEventButton  *event, gpointer data) {
  if ((!check_action ()) || (event->button != 1)) return TRUE;
  static gint selected_chessman = -1;
  gint i = -1;
  gint x = -1;
  gint y = -1;
  convert_event_xy (event->x, event->y, &x, &y);

  i = get_chessman_by_xy (x, y);
  if (i == -1) {
    if (selected_chessman == -1) {
      return TRUE;
    }
    if ((selected_chessman > -1) && (selected_chessman < 32)) {
      if (check_move (selected_chessman, x, y)) {
        gchar *tmp = g_strdup_printf ("ixy %d%d%d", selected_chessman, x, y);
        write_socket (tmp);
        g_free ((gpointer) tmp);
        move_chessman (selected_chessman, x, y);
        selected_chessman = -1;
      }
      return TRUE;
    }
  }
  if (i == selected_chessman) {
    unselect_chessman (selected_chessman);
    selected_chessman = -1;
    return TRUE;
  }
  if ((i > -1) && (i < 16)) {
    if (selected_chessman == -1) {
      if (select_chessman (i)) {
        selected_chessman = i;
      }
      return TRUE;
    }
    if ((selected_chessman > -1) && (selected_chessman < 16)) {
      if (select_chessman (i)) {
        unselect_chessman (selected_chessman);
        selected_chessman = i;
      }
      return TRUE;
    }
    if ((selected_chessman > 15) && (selected_chessman < 32)) {
      if (check_move (selected_chessman, x, y)) {
        gchar *tmp = g_strdup_printf ("ixy %d%d%d", selected_chessman, x, y);
        write_socket (tmp);
        g_free ((gpointer) tmp);
        move_chessman (selected_chessman, x, y);
        selected_chessman = -1;
      }
      return TRUE;
    }
  }
  if ((i > 15) && (i < 32)) {
    if (selected_chessman == -1) {
      if (select_chessman (i)) {
        selected_chessman = i;
      }
      return TRUE;
    }
    if ((selected_chessman > 15) && (selected_chessman < 32)) {
      if (select_chessman (i)) {
        unselect_chessman (selected_chessman);
        selected_chessman = i;
      }
      return TRUE;
    }
    if ((selected_chessman > -1) && (selected_chessman < 16)) {
      if (check_move (selected_chessman, x, y)) {
        gchar *tmp = g_strdup_printf ("ixy %d%d%d", selected_chessman, x, y);
        write_socket (tmp);
        g_free ((gpointer) tmp);
        move_chessman (selected_chessman, x, y);
        selected_chessman = -1;
      }
      return TRUE;
    }
  }
  return TRUE;
}

void on_entry_talk_activate (GtkEntry *entry, gpointer data) {
  gchar *tmp = g_strdup_printf ("chat %s", gtk_entry_get_text (entry));
  write_socket (tmp);
  g_free ((gpointer) tmp);
  gtk_entry_set_text (entry, "");
}
  
void on_button_yes_clicked (GtkButton *button, gpointer data) {
  switch (GPOINTER_TO_INT (data)) {
    case 4:
      write_socket ("exchange y");
      change_chess_side (-1);
      destroy_dialog_modal ();
      break;
    case 5:
      write_socket ("draw y");
      destroy_dialog_modal ();
      game_over (2);
      break;
    default :
      destroy_dialog_modal ();
      break;
  }
}

void on_button_no_clicked (GtkButton *button, gpointer data) {
  switch (GPOINTER_TO_INT (data)) {
    case 4:
      write_socket ("exchange n");
      destroy_dialog_modal ();
      break;
    case 5:
      write_socket ("draw n");
      destroy_dialog_modal ();
      break;
    default :
      destroy_dialog_modal ();
      break;
  }
}

void on_button_cancel_clicked (GtkButton *button, gpointer data) {
  switch (GPOINTER_TO_INT (data)) {
    case 4:
      write_socket ("exchange c");
      destroy_dialog_modal ();
      break;
    case 5:
      write_socket ("draw c");
      destroy_dialog_modal ();
      break;
    default :
      destroy_dialog_modal ();
      break;
  }
}

