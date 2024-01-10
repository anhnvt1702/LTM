/*****************************************************************************/
/*  network.h - network fuctions                                             */
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

#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <gtk/gtk.h>

#define MAX_CLIENT 5

#ifdef __cplusplus
extern "C" {
#endif

gint create_server (GtkToggleButton *togglebutton, const gchar *port, const gchar *name);

gint do_connect (GtkToggleButton *togglebutton, const gchar *ip, const gchar *port, const gchar *name);

gint parse_message (gpointer data, const gchar *message);

gboolean read_server_socket (GIOChannel *source, GIOCondition condition, gpointer data);

gboolean read_socket (GIOChannel *source, GIOCondition condition, gpointer data);

void close_server_socket (gpointer data);

void close_socket (gpointer data);

gint write_socket (const gchar *data);

#ifdef __cplusplus
}
#endif

#endif /* __NETWORK_H__ */
