/*****************************************************************************/
/*  createwindow.c -  GUI code                                               */
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


static GtkWidget *dialog_modal = NULL;

void create_window_main (int port,const char *username) {
  GtkWidget *window_main;
  GtkWidget *image[34];
  GtkWidget *vbox_main;
  GtkWidget *menubar;
  GtkWidget *menu_game;
  GtkWidget *menu_game_menu;
  GtkWidget *quit;
  GtkWidget *menu_preference;
  GtkWidget *menu_preference_menu;
  GtkWidget *setup;
  GtkWidget *menu_help;
  GtkWidget *menu_help_menu;
  GtkWidget *about;
  GtkWidget *box_tool;
  GtkWidget *label_ip;
  GtkWidget *entry_ip;
  GtkWidget *label_port;
  GtkWidget *entry_port;
  GtkWidget *label_name;
  GtkWidget *entry_name;
  GtkWidget *hbox_handl;
  GtkWidget *hbuttonbox_toggle;
  GtkWidget *toggle_button_host;
  GtkWidget *toggle_button_connect;
  GtkWidget *hbuttonbox_action;
  GtkWidget *button_start;
  GtkWidget *button_exchange;
  GtkWidget *button_draw;
  GtkWidget *button_resign;
  GtkWidget *hpaned;
  GtkWidget *hbox_chess;
  GtkWidget *event_box;
  GtkWidget *cchessboard;
  GtkWidget *fixed;
  GtkWidget *vpaned;
  GtkWidget *scrolledwindow_record;
  GtkWidget *textview_record;
  GtkWidget *vbox_talk;
  GtkWidget *scrolledwindow_talk;
  GtkWidget *textview_talk;
  GtkWidget *entry_talk;
  GtkWidget *statusbar;
  GdkPixbuf *pixbuf;
  GtkAccelGroup *accel_group;
  GtkTextMark *mark;
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  gint i;
  
  accel_group = gtk_accel_group_new ();

  window_main = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_object_set_data (G_OBJECT (window_main), "window_main", window_main);
  gtk_window_set_title (GTK_WINDOW (window_main), _("Chinese Chess"));

  vbox_main = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
  gtk_widget_show (vbox_main);
  gtk_container_add (GTK_CONTAINER (window_main), vbox_main);

  menubar = gtk_menu_bar_new ();
  gtk_widget_show (menubar);
  gtk_box_pack_start (GTK_BOX (vbox_main), menubar, FALSE, FALSE, 0);

  menu_game = gtk_menu_item_new_with_mnemonic (_("_Game"));
  gtk_widget_show (menu_game);
  gtk_container_add (GTK_CONTAINER (menubar), menu_game);

  menu_game_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_game), menu_game_menu);

  quit = gtk_menu_item_new_with_mnemonic (_("_Quit"));
  g_object_set_data_full (G_OBJECT (window_main), "quit", g_object_ref (quit), (GDestroyNotify) g_object_unref);
  gtk_widget_show (quit);
  gtk_container_add (GTK_CONTAINER (menu_game_menu), quit);
  gtk_widget_add_accelerator (quit, "activate", accel_group, 81, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); //GDK_Q Ctrl+Q

  menu_preference = gtk_menu_item_new_with_mnemonic (_("_Preference"));
  gtk_widget_show (menu_preference);
  gtk_container_add (GTK_CONTAINER (menubar), menu_preference);

  menu_preference_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_preference), menu_preference_menu);

  setup = gtk_menu_item_new_with_mnemonic (_("_Setup"));
  g_object_set_data_full (G_OBJECT (window_main), "setup", g_object_ref (setup), (GDestroyNotify) g_object_unref);
  gtk_widget_show (setup);
  gtk_container_add (GTK_CONTAINER (menu_preference_menu), setup);
  gtk_widget_add_accelerator (setup, "activate", accel_group, 80, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);//GDK_P Ctrl+P

  menu_help = gtk_menu_item_new_with_mnemonic (_("_Help"));
  gtk_widget_show (menu_help);
  gtk_container_add (GTK_CONTAINER (menubar), menu_help);

  menu_help_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_help), menu_help_menu);

  about = gtk_menu_item_new_with_mnemonic (_("_About"));
  g_object_set_data_full (G_OBJECT (window_main), "about", g_object_ref (about), (GDestroyNotify) g_object_unref);
  gtk_widget_show (about);
  gtk_container_add (GTK_CONTAINER (menu_help_menu), about);
  gtk_widget_add_accelerator (about, "activate", accel_group, 65, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);//GDK_A Ctrl+A

  box_tool = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_show (box_tool);
  gtk_box_pack_start (GTK_BOX (vbox_main), box_tool, FALSE, FALSE, 0);

  label_ip = gtk_label_new (_("HostIP"));
  gtk_widget_show (label_ip);
  gtk_box_pack_start (GTK_BOX (box_tool), label_ip, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_ip), GTK_JUSTIFY_LEFT);

  entry_ip = gtk_entry_new ();
  g_object_set_data_full (G_OBJECT (window_main), "entry_ip", g_object_ref (entry_ip), (GDestroyNotify) g_object_unref);
  gtk_widget_show (entry_ip);
  gtk_box_pack_start (GTK_BOX (box_tool), entry_ip, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry_ip), 15);
  gtk_entry_set_text (GTK_ENTRY (entry_ip), "127.0.0.1");

  label_port = gtk_label_new (_("port"));
  gtk_widget_show (label_port);
  gtk_box_pack_start (GTK_BOX (box_tool), label_port, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_port), GTK_JUSTIFY_LEFT);

  entry_port = gtk_entry_new ();
  g_object_set_data_full (G_OBJECT (window_main), "entry_port", g_object_ref (entry_port), (GDestroyNotify) g_object_unref);
  gtk_widget_show (entry_port);
  gtk_box_pack_start (GTK_BOX (box_tool), entry_port, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry_port), 5);
  char portbumber[10];
  sprintf(portbumber, "%d", port);
  gtk_entry_set_text (GTK_ENTRY (entry_port), portbumber);

  label_name = gtk_label_new (_("name"));
  gtk_widget_show (label_name);
  gtk_box_pack_start (GTK_BOX (box_tool), label_name, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_name), GTK_JUSTIFY_LEFT);

  entry_name = gtk_entry_new ();
  g_object_set_data_full (G_OBJECT (window_main), "entry_name", g_object_ref (entry_name), (GDestroyNotify) g_object_unref);
  gtk_widget_show (entry_name);
  gtk_box_pack_start (GTK_BOX (box_tool), entry_name, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry_name), MAX_NAME_LEN - 2);
  gtk_entry_set_text (GTK_ENTRY (entry_name), username );

  hbox_handl = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 16);
  gtk_widget_show (hbox_handl);
  gtk_container_add (GTK_CONTAINER (vbox_main), hbox_handl);

  hbuttonbox_toggle = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_show (hbuttonbox_toggle);
  gtk_box_pack_start (GTK_BOX (hbox_handl), hbuttonbox_toggle, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbuttonbox_toggle), 4);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox_toggle), GTK_BUTTONBOX_START);
  gtk_box_set_spacing (GTK_BOX (hbuttonbox_toggle), 4);

  toggle_button_host = gtk_toggle_button_new_with_mnemonic (_("H_ost"));
  g_object_set_data_full (G_OBJECT (window_main), "toggle_button_host", g_object_ref (toggle_button_host), (GDestroyNotify) g_object_unref);
  gtk_widget_show (toggle_button_host);
  gtk_container_add (GTK_CONTAINER (hbuttonbox_toggle), toggle_button_host);
  gtk_widget_set_can_focus (toggle_button_host, TRUE);

  toggle_button_connect = gtk_toggle_button_new_with_mnemonic (_("_Connect"));
  g_object_set_data_full (G_OBJECT (window_main), "toggle_button_connect", g_object_ref (toggle_button_connect), (GDestroyNotify) g_object_unref);
  gtk_widget_show (toggle_button_connect);
  gtk_container_add (GTK_CONTAINER (hbuttonbox_toggle), toggle_button_connect);
  gtk_widget_set_can_focus (toggle_button_connect, TRUE);

  hbuttonbox_action = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_show (hbuttonbox_action);
  gtk_box_pack_start (GTK_BOX (hbox_handl), hbuttonbox_action, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbuttonbox_action), 4);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox_action), GTK_BUTTONBOX_START);
  gtk_box_set_spacing (GTK_BOX (hbuttonbox_action), 4);

  button_start = gtk_button_new_with_mnemonic (_("_Start"));
  g_object_set_data_full (G_OBJECT (window_main), "button_start", g_object_ref (button_start), (GDestroyNotify) g_object_unref);
  gtk_widget_show (button_start);
  gtk_container_add (GTK_CONTAINER (hbuttonbox_action), button_start);
  gtk_widget_set_can_focus (button_start, TRUE);

  button_exchange = gtk_button_new_with_mnemonic (_("_Exchange"));
  g_object_set_data_full (G_OBJECT (window_main), "button_exchange", g_object_ref (button_exchange), (GDestroyNotify) g_object_unref);
  gtk_widget_show (button_exchange);
  gtk_container_add (GTK_CONTAINER (hbuttonbox_action), button_exchange);
  gtk_widget_set_can_focus (button_exchange, TRUE);

  button_draw = gtk_button_new_with_mnemonic (_("_Draw"));
  g_object_set_data_full (G_OBJECT (window_main), "button_draw", g_object_ref (button_draw), (GDestroyNotify) g_object_unref);
  gtk_widget_show (button_draw);
  gtk_container_add (GTK_CONTAINER (hbuttonbox_action), button_draw);
  gtk_widget_set_can_focus (button_draw, TRUE);

  button_resign = gtk_button_new_with_mnemonic (_("_Resign"));
  g_object_set_data_full (G_OBJECT (window_main), "button_resign", g_object_ref (button_resign), (GDestroyNotify) g_object_unref);
  gtk_widget_show (button_resign);
  gtk_container_add (GTK_CONTAINER (hbuttonbox_action), button_resign);
  gtk_widget_set_can_focus (button_resign, TRUE);

  hpaned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_show (hpaned);
  gtk_box_pack_start (GTK_BOX (vbox_main), hpaned, TRUE, TRUE, 0);

  hbox_chess = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_show (hbox_chess);
  gtk_paned_pack1 (GTK_PANED (hpaned), hbox_chess, TRUE, FALSE);
  gtk_container_set_border_width (GTK_CONTAINER (hbox_chess), 2);

  event_box = gtk_event_box_new ();
  gtk_widget_show (event_box);
  gtk_box_pack_start (GTK_BOX (hbox_chess), event_box, TRUE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (event_box), 2);

  fixed = gtk_fixed_new ();
  g_object_set_data_full (G_OBJECT (window_main), "fixed", g_object_ref (fixed), (GDestroyNotify) g_object_unref);
  gtk_widget_show (fixed);
  gtk_container_add (GTK_CONTAINER (event_box), fixed);

  cchessboard = gtk_image_new_from_file ("cchessboard.png");
  g_object_set_data_full (G_OBJECT (window_main), "cchessboard", g_object_ref (cchessboard), (GDestroyNotify) g_object_unref);
  gtk_widget_show (cchessboard);
  gtk_fixed_put (GTK_FIXED (fixed), cchessboard, 0, 0);

  pixbuf = gdk_pixbuf_new_from_file ("cchessman.png", NULL);
  image[0] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 0, 0, 50, 50));
  image[1] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 0, 0, 50, 50));
  image[2] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 50, 0, 50, 50));
  image[3] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 50, 0, 50, 50));
  image[4] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 100, 0, 50, 50));
  image[5] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 100, 0, 50, 50));
  image[6] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 150, 0, 50, 50));
  image[7] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 150, 0, 50, 50));
  image[8] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 200, 0, 50, 50));
  image[9] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 200, 0, 50, 50));
  image[10] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 250, 0, 50, 50));
  image[11] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 300, 0, 50, 50));
  image[12] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 300, 0, 50, 50));
  image[13] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 300, 0, 50, 50));
  image[14] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 300, 0, 50, 50));
  image[15] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 300, 0, 50, 50));
  image[16] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 0, 50, 50, 50));
  image[17] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 0, 50, 50, 50));
  image[18] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 50, 50, 50, 50));
  image[19] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 50, 50, 50, 50));
  image[20] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 100, 50, 50, 50));
  image[21] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 100, 50, 50, 50));
  image[22] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 150, 50, 50, 50));
  image[23] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 150, 50, 50, 50));
  image[24] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 200, 50, 50, 50));
  image[25] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 200, 50, 50, 50));
  image[26] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 250, 50, 50, 50));
  image[27] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 300, 50, 50, 50));
  image[28] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 300, 50, 50, 50));
  image[29] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 300, 50, 50, 50));
  image[30] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 300, 50, 50, 50));
  image[31] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 300, 50, 50, 50));
  image[32] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 350, 0, 50, 50));
  image[33] = gtk_image_new_from_pixbuf (gdk_pixbuf_new_subpixbuf (pixbuf, 350, 0, 50, 50));
  for (i = 0; i < 34; i++) {
    gtk_widget_show (image[i]);
    gtk_fixed_put (GTK_FIXED (fixed), image[i], FIXEDOUT, FIXEDOUT);
  }

  vpaned = gtk_paned_new (GTK_ORIENTATION_VERTICAL);
  gtk_widget_show (vpaned);
  gtk_paned_pack2 (GTK_PANED (hpaned), vpaned, TRUE, TRUE);

  scrolledwindow_record = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow_record);
  gtk_paned_pack1 (GTK_PANED (vpaned), scrolledwindow_record, FALSE, TRUE);
  gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow_record), 2);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow_record), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  textview_record = gtk_text_view_new ();
  g_object_set_data_full (G_OBJECT (window_main), "textview_record", g_object_ref (textview_record), (GDestroyNotify) g_object_unref);
  gtk_widget_show (textview_record);
  gtk_container_add (GTK_CONTAINER (scrolledwindow_record), textview_record);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_record), FALSE);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview_record), GTK_WRAP_WORD);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (textview_record), FALSE);
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(textview_record));
  gtk_text_buffer_get_end_iter (buffer, &iter);
  mark = gtk_text_buffer_create_mark (buffer,"mark",&iter,TRUE);

  vbox_talk = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
  gtk_widget_show (vbox_talk);
  gtk_paned_pack2 (GTK_PANED (vpaned), vbox_talk, TRUE, TRUE);
  gtk_container_set_border_width (GTK_CONTAINER (vbox_talk), 2);

  scrolledwindow_talk = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow_talk);
  gtk_box_pack_start (GTK_BOX (vbox_talk), scrolledwindow_talk, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow_talk), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  textview_talk = gtk_text_view_new ();
  g_object_set_data_full (G_OBJECT (window_main), "textview_talk", g_object_ref (textview_talk), (GDestroyNotify) g_object_unref);
  gtk_widget_show (textview_talk);
  gtk_container_add (GTK_CONTAINER (scrolledwindow_talk), textview_talk);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_talk), FALSE);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview_talk), GTK_WRAP_WORD);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (textview_talk), FALSE);
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(textview_talk));
  gtk_text_buffer_create_tag (buffer, "red_fg", "foreground", "red", NULL);
  gtk_text_buffer_create_tag (buffer, "blue_fg", "foreground", "blue", NULL);
  gtk_text_buffer_create_tag (buffer, "yellow_bg", "background", "yellow", NULL);
  gtk_text_buffer_get_end_iter (buffer, &iter);
  gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, _("Chinese Chess"), -1, "red_fg", NULL);
  gtk_text_buffer_insert (buffer, &iter, "\n", -1);
  gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, _("Version"), -1, "blue_fg", NULL);
  gtk_text_buffer_insert (buffer, &iter, ":"VERSION"\n", -1);
  mark = gtk_text_buffer_create_mark (buffer,"mark",&iter,TRUE);

  entry_talk = gtk_entry_new ();
  g_object_set_data_full (G_OBJECT (window_main), "entry_talk", g_object_ref (entry_talk), (GDestroyNotify) g_object_unref);
  gtk_widget_show (entry_talk);
  gtk_box_pack_start (GTK_BOX (vbox_talk), entry_talk, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry_talk), 500);

  statusbar = gtk_statusbar_new ();
  gtk_widget_show (statusbar);
  gtk_box_pack_start (GTK_BOX (vbox_main), statusbar, FALSE, FALSE, 0);

  g_signal_connect (G_OBJECT (window_main), "destroy", G_CALLBACK (on_quit_activate), NULL);
  g_signal_connect (G_OBJECT (quit), "activate", G_CALLBACK (on_quit_activate), NULL);
  g_signal_connect (G_OBJECT (setup), "activate", G_CALLBACK (on_setup_activate), NULL);
  g_signal_connect (G_OBJECT (about), "activate", G_CALLBACK (create_window_about), (gpointer) window_main);
  g_signal_connect (G_OBJECT (entry_ip), "activate", G_CALLBACK (on_entry_name_activate), NULL);
  g_signal_connect (G_OBJECT (entry_port), "activate", G_CALLBACK (on_entry_name_activate), NULL);
  g_signal_connect (G_OBJECT (entry_name), "activate", G_CALLBACK (on_entry_name_activate), NULL);
  g_signal_connect (G_OBJECT (toggle_button_host), "toggled", G_CALLBACK (on_toggle_button_host_toggled), NULL);
  g_signal_connect (G_OBJECT (toggle_button_connect), "toggled", G_CALLBACK (on_toggle_button_connect_toggled), NULL);
  g_signal_connect (G_OBJECT (button_start), "clicked", G_CALLBACK (on_button_start_clicked), NULL);
  g_signal_connect (G_OBJECT (button_exchange), "clicked", G_CALLBACK (on_button_exchange_clicked), NULL);
  g_signal_connect (G_OBJECT (button_draw), "clicked", G_CALLBACK (on_button_draw_clicked), NULL);
  g_signal_connect (G_OBJECT (button_resign), "clicked", G_CALLBACK (on_button_resign_clicked), NULL);
  g_signal_connect (G_OBJECT (event_box), "button_press_event", G_CALLBACK (on_fixed_button_press_event), NULL);
  g_signal_connect (G_OBJECT (entry_talk), "activate", G_CALLBACK (on_entry_talk_activate), NULL);

  gtk_window_set_icon (GTK_WINDOW (window_main), gdk_pixbuf_new_subpixbuf (pixbuf, 350, 50, 50, 50));
  gtk_window_add_accel_group (GTK_WINDOW (window_main), accel_group);

  init_chess (fixed, image);

  gtk_widget_show (window_main);
}

void create_window_about (GtkMenuItem *menuitem, gpointer data) {
  static GtkWidget *window_about = NULL;
  if (window_about) {
    gtk_window_present (GTK_WINDOW (window_about));
    return;
  } else {
    GtkWidget *vbox_about;
    GtkWidget *image;
    GtkWidget *label_prog;
    GtkWidget *label_thank;
    GtkWidget *label_cr;
    GtkWidget *hbuttonbox;
    GtkWidget *button_info;
    GtkWidget *button_close;
    gchar* tmp;

    window_about = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window_about), _("About"));

    vbox_about = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_show (vbox_about);
    gtk_container_add (GTK_CONTAINER (window_about), vbox_about);
    gtk_container_set_border_width (GTK_CONTAINER (vbox_about), 8);

    image = gtk_image_new_from_file ("cchess.png");
    gtk_widget_show (image);
    gtk_box_pack_start (GTK_BOX (vbox_about), image, FALSE, FALSE, 12);

    tmp = g_strdup_printf ("<span><big><b>%s %s</b></big></span>", _("Chinese Chess"), VERSION);
    label_prog = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (label_prog), tmp);
    gtk_widget_show (label_prog);
    gtk_box_pack_start (GTK_BOX (vbox_about), label_prog, FALSE, FALSE, 0);
    gtk_label_set_selectable (GTK_LABEL (label_prog), TRUE);
    g_free ((gpointer) tmp);

    label_thank = gtk_label_new (_("Thank to RenHaibin, XuYonggang and Yangnengsheng"));
    gtk_widget_show (label_thank);
    gtk_box_pack_start (GTK_BOX (vbox_about), label_thank, FALSE, FALSE, 0);
    gtk_label_set_selectable (GTK_LABEL (label_thank), TRUE);

    label_cr = gtk_label_new (_("Copyright (C) 2003 980501427"));
    gtk_widget_show (label_cr);
    gtk_box_pack_start (GTK_BOX (vbox_about), label_cr, FALSE, FALSE, 0);
    gtk_label_set_selectable (GTK_LABEL (label_cr), TRUE);

    hbuttonbox = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox_about), hbuttonbox, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbuttonbox), 4);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_SPREAD);

    button_info = gtk_button_new_with_label (_("Info"));
    gtk_widget_show (button_info);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), button_info);
    gtk_widget_set_can_focus (button_info, TRUE);

    button_close = gtk_button_new_with_label (_("Close"));
    gtk_widget_show (button_close);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), button_close);
    gtk_widget_set_can_focus (button_close, TRUE);

    gtk_window_set_transient_for (GTK_WINDOW (window_about), GTK_WINDOW (data));
    gtk_window_set_position (GTK_WINDOW (window_about), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_destroy_with_parent (GTK_WINDOW (window_about), TRUE);
    gtk_window_set_resizable (GTK_WINDOW (window_about), FALSE);

    g_signal_connect (G_OBJECT (window_about), "destroy", G_CALLBACK (gtk_widget_destroyed), &window_about);
    g_signal_connect (G_OBJECT (button_info), "clicked", G_CALLBACK (create_window_info), (gpointer) window_about);
    g_signal_connect_swapped (G_OBJECT (button_close), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (window_about));

    gtk_widget_show (window_about);
  }
}

void create_window_info (GtkButton *button, gpointer data) {
  static GtkWidget *window_info = NULL;
  if (window_info) {
    gtk_window_present (GTK_WINDOW (window_info));
    return;
  } else {
    GtkWidget *vbox_info;
    GtkWidget *notebook;
    GtkWidget *scrolledwindow_author;
    GtkWidget *viewport_author;
    GtkWidget *vbox_author;
    GtkWidget *label_author_1;
    GtkWidget *label_author;
    GtkWidget *scrolledwindow_translator;
    GtkWidget *viewport_translator;
    GtkWidget *vbox_translator;
    GtkWidget *label_translator_zh_CN;
    GtkWidget *label_translator;
    GtkWidget *scrolledwindow_lisence;
    GtkWidget *textview_lisence;
    GtkWidget *label_lisence;
    GtkWidget *hbuttonbox;
    GtkWidget *button_close;

    window_info = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window_info), _("Information"));
    gtk_window_set_default_size (GTK_WINDOW (window_info), 300, 200);

    vbox_info = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_show (vbox_info);
    gtk_container_add (GTK_CONTAINER (window_info), vbox_info);
    gtk_container_set_border_width (GTK_CONTAINER (vbox_info), 8);

    notebook = gtk_notebook_new ();
    gtk_widget_show (notebook);
    gtk_box_pack_start (GTK_BOX (vbox_info), notebook, TRUE, TRUE, 0);

    scrolledwindow_author = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow_author);
    gtk_container_add (GTK_CONTAINER (notebook), scrolledwindow_author);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow_author), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    viewport_author = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport_author);
    gtk_container_add (GTK_CONTAINER (scrolledwindow_author), viewport_author);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport_author), GTK_SHADOW_NONE);

    vbox_author = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show (vbox_author);
    gtk_container_add (GTK_CONTAINER (viewport_author), vbox_author);
    gtk_container_set_border_width (GTK_CONTAINER (vbox_author), 8);

    label_author_1 = gtk_label_new ("980501427 ("PACKAGE_BUGREPORT")");
    gtk_widget_show (label_author_1);
    gtk_box_pack_start (GTK_BOX (vbox_author), label_author_1, FALSE, FALSE, 0);
    gtk_label_set_selectable (GTK_LABEL (label_author_1), TRUE);
    gtk_widget_set_margin_top (label_author_1, 0.5);

    label_author = gtk_label_new_with_mnemonic (_("_Author"));
    gtk_widget_show (label_author);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0), label_author);

    scrolledwindow_translator = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow_translator);
    gtk_container_add (GTK_CONTAINER (notebook), scrolledwindow_translator);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow_translator), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    viewport_translator = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport_translator);
    gtk_container_add (GTK_CONTAINER (scrolledwindow_translator), viewport_translator);
    gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport_translator), GTK_SHADOW_NONE);

    vbox_translator = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show (vbox_translator);
    gtk_container_add (GTK_CONTAINER (viewport_translator), vbox_translator);
    gtk_container_set_border_width (GTK_CONTAINER (vbox_translator), 8);

    label_translator_zh_CN = gtk_label_new ("980501427 <"PACKAGE_BUGREPORT">");
    gtk_widget_show (label_translator_zh_CN);
    gtk_box_pack_start (GTK_BOX (vbox_translator), label_translator_zh_CN, FALSE, FALSE, 0);
    gtk_label_set_selectable (GTK_LABEL (label_translator_zh_CN), TRUE);
    gtk_widget_set_margin_top (label_translator_zh_CN, 0.5);

    label_translator = gtk_label_new_with_mnemonic (_("_Translator"));
    gtk_widget_show (label_translator);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 1), label_translator);

    scrolledwindow_lisence = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow_lisence);
    gtk_container_add (GTK_CONTAINER (notebook), scrolledwindow_lisence);
    gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow_lisence), 8);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow_lisence), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    textview_lisence = gtk_text_view_new ();
    gtk_widget_show (textview_lisence);
    gtk_container_add (GTK_CONTAINER (scrolledwindow_lisence), textview_lisence);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_lisence), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview_lisence), GTK_WRAP_WORD);
    gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview_lisence)),_(
             "This program is free software; you can redistribute it and/or modify it "
             "under the terms of the GNU General Public License as published by the Free Software Foundation; "
             "either version 2 of the License, or (at your option) any later version.\n\n"
             "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; "
             "without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."
             "  See the GNU General Public License for more details.\n\n"
             "You should have received a copy of the GNU General Public License along with this program;"
             " if not, write to the Free Software Foundation, Inc., "
             "59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n"
             ), -1);

    label_lisence = gtk_label_new_with_mnemonic (_("_License"));
    gtk_widget_show (label_lisence);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 2), label_lisence);

    hbuttonbox = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox_info), hbuttonbox, FALSE, FALSE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);

    button_close = gtk_button_new_with_label (_("Close"));
    gtk_widget_show (button_close);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), button_close);
    gtk_widget_set_can_focus (button_close, TRUE);

    gtk_window_set_transient_for (GTK_WINDOW (window_info), GTK_WINDOW (data));
    gtk_window_set_position (GTK_WINDOW (window_info), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_destroy_with_parent (GTK_WINDOW (window_info), TRUE);
    gtk_window_set_icon (GTK_WINDOW (window_info), gtk_window_get_icon (gtk_window_get_transient_for (GTK_WINDOW (data))));

    g_signal_connect (G_OBJECT (window_info), "destroy", G_CALLBACK (gtk_widget_destroyed), &window_info);
    g_signal_connect_swapped (G_OBJECT (button_close), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (window_info));

    gtk_widget_show (window_info);
  }
}

GtkWidget* create_dialog_question (gint question) {
  if (!dialog_modal) {
    GtkWidget *vbox_dialog;
    GtkWidget *hbox_dialog;
    GtkWidget *image_question;
    GtkWidget *label_question;
    GtkWidget *hseparator;
    GtkWidget *hbuttonbox;
    GtkWidget *button_yes;
    GtkWidget *button_no;

    dialog_modal = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (dialog_modal), 8);

    vbox_dialog = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show (vbox_dialog);
    gtk_container_add (GTK_CONTAINER (dialog_modal), vbox_dialog);

    hbox_dialog = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_widget_show (hbox_dialog);
    gtk_box_pack_start (GTK_BOX (vbox_dialog), hbox_dialog, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox_dialog), 8);

    image_question = gtk_image_new_from_icon_name ("dialog-question", GTK_ICON_SIZE_DIALOG);
    gtk_widget_show (image_question);
    gtk_box_pack_start (GTK_BOX (hbox_dialog), image_question, FALSE, FALSE, 0);

    switch (question) {
      case 4:
        label_question = gtk_label_new (_("Your opponent want to\nexchange the pieces.\n\nDo you accept ?"));
        gtk_window_set_title (GTK_WINDOW (dialog_modal), _("Exchange"));
        break;
      case 5:
        label_question = gtk_label_new (_("Your opponent propose a draw.\n\nDo you accept ?"));
        gtk_window_set_title (GTK_WINDOW (dialog_modal), _("Draw"));
        break;
      default:
        label_question = gtk_label_new (_("Recieve a unknow request."));
        gtk_window_set_title (GTK_WINDOW (dialog_modal), _("Error"));
        break;
    }
    gtk_widget_show (label_question);
    gtk_box_pack_start (GTK_BOX (hbox_dialog), label_question, FALSE, FALSE, 16);

    hseparator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox_dialog), hseparator, FALSE, FALSE, 8);

    hbuttonbox = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox_dialog), hbuttonbox, FALSE, FALSE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_SPREAD);

    button_yes = gtk_button_new_with_label (_("Yes"));
    gtk_widget_show (button_yes);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), button_yes);
    gtk_widget_set_can_focus (button_yes, TRUE);

    button_no = gtk_button_new_with_label (_("No"));
    gtk_widget_show (button_no);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), button_no);
    gtk_widget_set_can_focus (button_no, TRUE);

    gtk_window_set_resizable (GTK_WINDOW (dialog_modal), FALSE);
    gtk_window_set_modal (GTK_WINDOW (dialog_modal), TRUE);

    g_signal_connect (G_OBJECT (dialog_modal), "delete_event", G_CALLBACK (gtk_true), NULL);
    g_signal_connect (G_OBJECT (button_yes), "clicked", G_CALLBACK (on_button_yes_clicked), GINT_TO_POINTER (question));
    g_signal_connect (G_OBJECT (button_no), "clicked", G_CALLBACK (on_button_no_clicked), GINT_TO_POINTER (question));
  }
  return dialog_modal;
}

GtkWidget* create_dialog_wait (gint request) {
  if (!dialog_modal) {
    GtkWidget *vbox_dialog;
    GtkWidget *hbox_dialog;
    GtkWidget *image_wait;
    GtkWidget *label_wait;
    GtkWidget *hseparator;
    GtkWidget *hbuttonbox;
    GtkWidget *button_cancel;

    dialog_modal = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (dialog_modal), 8);

    vbox_dialog = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show (vbox_dialog);
    gtk_container_add (GTK_CONTAINER (dialog_modal), vbox_dialog);

    hbox_dialog = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_widget_show (hbox_dialog);
    gtk_box_pack_start (GTK_BOX (vbox_dialog), hbox_dialog, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox_dialog), 8);

    image_wait = gtk_image_new_from_icon_name ("dialog-info", GTK_ICON_SIZE_DIALOG);
    gtk_widget_show (image_wait);
    gtk_box_pack_start (GTK_BOX (hbox_dialog), image_wait, FALSE, FALSE, 0);

    switch (request) {
      case 4:
        gtk_window_set_title (GTK_WINDOW (dialog_modal), _("Exchange"));
        break;
      case 5:
        gtk_window_set_title (GTK_WINDOW (dialog_modal), _("Draw"));
        break;
      default:
        gtk_window_set_title (GTK_WINDOW (dialog_modal), _("Error"));
        break;
    }

    label_wait = gtk_label_new (_("Wait for remote response ...\n"));
    gtk_widget_show (label_wait);
    gtk_box_pack_start (GTK_BOX (hbox_dialog), label_wait, FALSE, FALSE, 16);

    hseparator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox_dialog), hseparator, FALSE, FALSE, 8);

    hbuttonbox = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox_dialog), hbuttonbox, FALSE, FALSE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);

    button_cancel = gtk_button_new_with_label (_("Cancel"));
    gtk_widget_show (button_cancel);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), button_cancel);
    gtk_widget_set_can_focus (button_cancel, TRUE);

    gtk_window_set_resizable (GTK_WINDOW (dialog_modal), FALSE);
    gtk_window_set_modal (GTK_WINDOW (dialog_modal), TRUE);

    g_signal_connect (G_OBJECT (dialog_modal), "delete_event", G_CALLBACK (gtk_true), NULL);
    g_signal_connect (G_OBJECT (button_cancel), "clicked", G_CALLBACK (on_button_cancel_clicked), GINT_TO_POINTER (request));
  }
  return dialog_modal;
}

void destroy_dialog_modal () {
  if (dialog_modal) {
    gtk_widget_destroy (dialog_modal);
    dialog_modal = NULL;
  }
}
