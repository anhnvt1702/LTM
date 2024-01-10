/*****************************************************************************/
/*  createwindow.h -  GUI functions                                          */
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

#ifndef __CREATEWINDOW_H__
#define __CREATEWINDOW_H__

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

void create_window_main (int port,const char *username);

void create_window_about (GtkMenuItem *menuitem, gpointer data);

void create_window_info (GtkButton *button, gpointer data);

GtkWidget* create_dialog_question (gint question);

GtkWidget* create_dialog_wait (gint request);

void destroy_dialog_modal ();

#ifdef __cplusplus
}
#endif

#endif /* __CREATEWINDOW_H__ */
