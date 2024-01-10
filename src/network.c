/*****************************************************************************/
/*  network.c - network fuctions, read, write socket ...                     */
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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <gtk/gtk.h>
#include "network.h"
#include "cchess.h"
#include "createwindow.h"

#define BUF_LEN 2048

static gint server_sockfd = -1;
static gint new_sockfd = -1;
static gint sockfd = -1;
static gchar local_name[MAX_NAME_LEN] = "";
static gchar remote_name[MAX_NAME_LEN] = "";

const gchar *cmds[] = {
     /*  0  */         "program",
     /*  1  */         "version",
     /*  2  */         "error",
     /*  3  */         "start",
     /*  4  */         "exchange",
     /*  5  */         "draw",
     /*  6  */         "resign",
     /*  7  */         "name",
     /*  8  */         "ixy",
     /*  9  */         "chat",
     /*  10  */        "quit",
                       NULL
                       };
                
gint parse_message (gpointer data, const gchar *message) {
  gint n = 0;
  while (cmds[n] != NULL) {
    if (g_str_has_prefix (message, cmds[n])) {
      break;
    }
    n++;
  }
  switch (n) {
    case 0:
      if (strcmp (message,cmds[n]) == 0) {
        gchar *tmp = g_strdup_printf ("%s %s", cmds[n], PACKAGE);
        write_socket (tmp);
        g_free ((gpointer) tmp);
        break;
      }
      if (sockfd == new_sockfd) {
        if (strcmp (&message[strlen (cmds[n]) + 1], PACKAGE) == 0) {
          gchar *tmp = g_strdup_printf ("%s OK!\n%s", &message[strlen (cmds[n]) + 1], _("Checking version ... "));
          write_textview (tmp, 1);
          write_socket (cmds[1]);
          g_free ((gpointer) tmp);
        } else {
          write_textview (_("FAILURE!\nDisconnected !\n"), 1);
          gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (data), FALSE);
        }
      }
      break;
    case 1:
      if (strcmp (message,cmds[n]) == 0) {
        gchar *tmp = g_strdup_printf ("%s %s", cmds[n], VERSION);
        write_socket (tmp);
        g_free ((gpointer) tmp);
        break;
      }
      if (sockfd == new_sockfd) {
        if (atof (&message[strlen (cmds[n]) + 1]) >= atof (VERSION)) {
          new_sockfd = -1;
          gchar *tmp = g_strdup_printf ("%s OK!\n%s", &message[strlen (cmds[n]) + 1], _("Geting nickname ...\n"));
          write_textview (tmp, 1);
          write_socket (cmds[7]);
          g_free ((gpointer) tmp);
          tmp = g_strdup_printf ("%s %s", cmds[7], local_name);
          write_socket (tmp);
          g_free ((gpointer) tmp);
        } else {
          write_textview (_("FAILURE!\nDisconnected !\n"), 1);
          gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (data), FALSE);
        }
      }
      break;
    case 2:
      /***********************************
          for some error to do here.
      ***********************************/
        write_textview (message, 1);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (data), FALSE);
        break;
    case 3:
      if (strlen (message) == strlen (cmds[n])) {
        init_remote_chessman ();
        break;
      }
      break;
    case 4:
      if (strcmp (message,cmds[n]) == 0) {
        GtkWidget *dialog = create_dialog_question (n);
        gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (data))));
        gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
        gtk_widget_show (dialog);
        break;
      }
      if (strcmp (&message[strlen (cmds[n]) + 1], "y") == 0) {
        write_textview (_("Your opponent accept exchanging the pieces.\n"), 1);
        change_chess_side (-1);
        destroy_dialog_modal ();
        break;
      }
      if (strcmp (&message[strlen (cmds[n]) + 1], "n") == 0) {
        write_textview (_("Your opponent refuse exchanging the pieces.\n"), 0);
        destroy_dialog_modal ();
        break;
      }
      if (strcmp (&message[strlen (cmds[n]) + 1], "c") == 0) {
        destroy_dialog_modal ();
        break;
      }
      break;
    case 5:
      if (strcmp (message,cmds[n]) == 0) {
        GtkWidget *dialog = create_dialog_question (n);
        gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (data))));
        gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
        gtk_widget_show (dialog);
        break;
      }
      if (strcmp (&message[strlen (cmds[n]) + 1], "y") == 0) {
        write_textview (_("Your opponent accept the proposal.\n"), 1);
        game_over (2);
        destroy_dialog_modal ();
        break;
      }
      if (strcmp (&message[strlen (cmds[n]) + 1], "n") == 0) {
        write_textview (_("Your opponent refuse the proposal.\n"), 0);
        destroy_dialog_modal ();
        break;
      }
      if (strcmp (&message[strlen (cmds[n]) + 1], "c") == 0) {
        destroy_dialog_modal ();
        break;
      }
      break;
    case 6:
      if (strcmp (message,cmds[n]) == 0) {
        write_textview (_("Your opponent resign the game.\n"), 0);
        game_over (0);
        break;
      }
      break;
    case 7:
      if (strcmp (message,cmds[n]) == 0) {
        gchar *tmp = g_strdup_printf ("%s %s", cmds[n], local_name);
        write_socket (tmp);
        g_free ((gpointer) tmp);
        break;
      }
      if (strlen (&message[strlen (cmds[n]) + 1]) < MAX_NAME_LEN) {
        strcpy (remote_name, &message[strlen (cmds[n]) + 1]);
        write_textview (_("Your opponent set his nickname : "), 1);
        gchar *tmp = g_strdup_printf ("%s\n", &message[strlen (cmds[n]) + 1]);
        write_textview (tmp, -1);
        g_free ((gpointer) tmp);
        break;
      }
      break;
    case 8:
      if (strcmp (message,cmds[n]) == 0) {
        break;
      }
      gint i = atoi (&message[strlen (cmds[n]) + 1]);
      gint x = (i / 10) % 10;
      gint y = i % 10;
      i = i / 100;
      if (check_move (i, x, y)) {
        move_chessman (i,x,y);
      }
      break;
    case 9:
      if (strcmp (message,cmds[n]) == 0) {
        break;
      } else {
        gchar *tmp = g_strdup_printf ("%s : %s\n", remote_name, &message[strlen (cmds[n]) + 1]);
        write_textview (tmp, -1);
        g_free ((gpointer) tmp);
        break;
      }
      break;
    case 10:
      if (strcmp (message,cmds[n]) == 0) {
        write_textview ("Your opponent has left the game.\n", 0);
        game_over (0);
        break;
      }
      break;
    default :
      g_warning ("recieved unknown message : \"%s\"", message);
      break;
  } /* end switch (n) */
  return 0;
}


gint create_server (GtkToggleButton *togglebutton, const gchar *port, const gchar *name) {
  if (server_sockfd > 0) {
    write_textview (_("server:server is running!\n"), 1);
    return server_sockfd;
  }
  strcpy (local_name, name);
  struct sockaddr_in server_addr;
  if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    write_textview (_("server:can't open stream socket!\n"), 1);
    server_sockfd = -1;
    return server_sockfd;
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
  server_addr.sin_port = htons (atoi (port));
  if (bind (server_sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    write_textview (_("server:can't bind local address!\n"), 1);
    server_sockfd = -1;
    return server_sockfd;
  }
  if ((listen (server_sockfd, MAX_CLIENT)) < 0) {
    write_textview (_("server:listen socket error!\n"), 1);
    server_sockfd = -1;
    return server_sockfd;
  }
  change_chess_side (0);
  write_textview (_("server:server created!\n"), 1);
  return server_sockfd;
}

gint do_connect (GtkToggleButton *togglebutton, const gchar *ip, const gchar *port, const gchar *name) {
  if (sockfd > 0) {
    return sockfd;
  }
  strcpy (local_name, name);
  struct sockaddr_in client_addr;
  gint client_len = sizeof(client_addr);
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = inet_addr (ip);
  client_addr.sin_port = htons (atoi (port));
  if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
    write_textview (_("client:can't open stream socket!\n"), 1);
    sockfd = -1;
    return sockfd;
  }
  if (connect (sockfd,(struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
    write_textview (_("client:can't connect to server!\n"), 1);
    sockfd = -1;
    return sockfd;
  }
  if (getpeername (sockfd, (struct sockaddr *) &client_addr, &client_len) == 0 ) {
    gchar *tmp = g_strdup_printf (_("server socket set %s has port %d\n"),
                                 inet_ntoa (client_addr.sin_addr), ntohs (client_addr.sin_port));
    write_textview (tmp, 1);
    g_free ((gpointer) tmp);
  }
  change_chess_side (1);
  write_textview (_("client:client connected!\n"), 1);
  return sockfd;
}

gboolean read_server_socket (GIOChannel *source, GIOCondition condition, gpointer data) {
  struct sockaddr_in client_addr;
  gint client_len = sizeof(client_addr);
  gint len;
  if ((new_sockfd = accept(server_sockfd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
    write_textview (_("server:accept error!\n"), 1);
    return FALSE;
  }
  if (sockfd > 0) {
    gchar *tmp = g_strdup_printf ("error %s",_("Too many connections or you connect yourself !\n"));
    len = write (new_sockfd, tmp, strlen (tmp) + 1);
    close (new_sockfd);
    new_sockfd = -1;
    g_free ((gpointer) tmp);
    return TRUE;
  }
  if (getpeername (new_sockfd, (struct sockaddr *) &client_addr, &client_len) == 0 ) {
    gchar *tmp = g_strdup_printf (_("client socket from %s has port %d\n"),
                                 inet_ntoa (client_addr.sin_addr), ntohs (client_addr.sin_port));
    write_textview (tmp, 1);
    g_free ((gpointer) tmp);
  }
  sockfd = new_sockfd;
  len = write (sockfd, cmds[0], strlen (cmds[0]) + 1);
  write_textview (_("Checking program ... "), 1);
  GtkWidget *toggle_button_connect = get_widget (GTK_WIDGET (data), "toggle_button_connect");
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle_button_connect), TRUE);

  return TRUE;
}

gboolean read_socket (GIOChannel *source, GIOCondition condition, gpointer data) {
  gchar* buf = g_strnfill (BUF_LEN, '\0');
  gint j;
  for (j = 0; j < BUF_LEN; j++) {
    buf[j] = '\0';
  }
  gint length = read (sockfd, buf, BUF_LEN - 2);
  if (length == 0) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (data), FALSE);
    return FALSE;
  }
  j = strlen (buf);
  gint k = 0;
  while (j) {
    parse_message (data, &buf[k]);
    k = k + j + 1;
    j = strlen (&buf[k]);
  }
  g_free ((gpointer) buf);

  return TRUE;
}

void close_server_socket (gpointer data) {
  if (server_sockfd < 0) {
    return;
  }
  close (server_sockfd);
  server_sockfd = -1;
  write_textview (_("server:server shutdown!\n"), 1);
}

void close_socket (gpointer data) {
  if (sockfd < 0) {
    return;
  }
  close (sockfd);
  sockfd = -1;
  write_textview (_("client:disconnected!\n"), 1);
}

gint write_socket (const gchar *data) {
  if (sockfd < 0) {
    g_warning ("can't write a invalid socket %d !\n", sockfd);
    return -1;
  }
  gint result;
  result = write (sockfd , data, strlen (data) + 1);
  if (result < 0) {
    g_warning ("write socket %d error!\n", sockfd);
    return result;
  }
  if (g_str_has_prefix (data, "chat")) {
    gchar *tmp = g_strdup_printf ("%s : %s\n", local_name, &data[strlen ("chat") + 1]);
    write_textview (tmp, -1);
    g_free ((gpointer) tmp);
  }
  return 0;
}
