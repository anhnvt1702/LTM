/*****************************************************************************/
/*  cchess.h - cchess common fuctions                                       */
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

#ifndef __CCHESS_H__
#define __CCHESS_H__

#include <gtk/gtk.h>

#define FIXEDOUT -100
#define DIAMETER 50
#define HCCHESS  8
#define VCCHESS  9
#define MAX_NAME_LEN 1024

/* Standard gettext macros. */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

#ifdef __cplusplus
extern "C" {
#endif

GtkWidget* get_widget (GtkWidget *widget, const gchar *name);

void init_chess (GtkWidget *widget_fixed, GtkWidget **image);

void change_chess_side (gint side);

void init_local_chessman ();

void init_remote_chessman ();

gint get_chess_status ();

void convert_event_xy (gdouble event_x, gdouble event_y, gint *x, gint *y);

gint get_chessman_by_xy (gint x, gint y);

gboolean select_chessman (gint i);

void unselect_chessman (gint i);

gboolean check_action ();

gboolean check_move (gint i, gint x, gint y);

void move_message (gint i, gint x, gint y);

void move_chessman (gint i, gint x, gint y);

void write_textview (const gchar *data, gint tag);

void game_over (gint status);

void write_textview_to_file(const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* __CCHESS_H__ */
