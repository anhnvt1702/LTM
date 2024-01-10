/*****************************************************************************/
/*  cchess.c - Chinese Chess status, functions.                             */
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
#include "cchess.h"


const gint xy_original[16][2] = { {8,9}, {0,9}, {7,9}, {1,9}, {7,7}, {1,7}, {6,9}, {2,9},
                                 {5,9}, {3,9}, {4,9}, {8,6}, {6,6}, {4,6}, {2,6}, {0,6},
                               };

static gint xy[32][2] = { {8,9}, {0,9}, {7,9}, {1,9}, {7,7}, {1,7}, {6,9}, {2,9},
                          {5,9}, {3,9}, {4,9}, {8,6}, {6,6}, {4,6}, {2,6}, {0,6},
                          {0,0}, {8,0}, {1,0}, {7,0}, {1,2}, {7,2}, {2,0}, {6,0},
                          {3,0}, {5,0}, {4,0}, {0,3}, {2,3}, {4,3}, {6,3}, {8,3},
                        };

static GtkFixed *fixed;
static GtkWidget *chessman_image[34];

static gint chess_status = -2;
static gint chess_side = 0;

const static gchar *red_pieces[]   = { N_("R"),  N_("N"),  N_("C"),  N_("M"),  N_("G"),  N_("K"),  N_("P"),  };
const static gchar *black_pieces[] = { N_(" R"), N_(" N"), N_(" C"), N_(" M"), N_(" G"), N_(" K"), N_(" P"), };
const static gchar *move_info[]    = { N_(" +"), N_(" -"), N_("+"),  N_("-"),  N_("="), };
const static gchar *number[]       = { N_("9"),  N_("8"),  N_("7"),  N_("6"),  N_("5"),  N_("4"),  N_("3"),  N_("2"),  N_("1"), };



GtkWidget* get_widget (GtkWidget *widget, const gchar *name) {
  GtkWidget *widget_parent;
  GtkWidget *widget_temp;
// get toplevel widget, data store there.
  while (1) {
    widget_parent = gtk_widget_get_parent(widget);
    if (widget_parent == NULL) {
      break;
    }
    widget = widget_parent;
  }
// get data
  widget_temp = (GtkWidget*) g_object_get_data (G_OBJECT (widget), name);
  if (!widget_temp) {
    g_warning ("Widget not found: %s", name);
  }
  return widget_temp;
}

void init_chess (GtkWidget *widget_fixed, GtkWidget **image) {
  gint j;
  fixed = GTK_FIXED (widget_fixed);
  for (j = 0; j < 34; j++) {
    chessman_image[j] = image[j];
  }
}

void change_chess_side (gint side) {
  if ((side < 0) || (side > 1)){
    if (chess_side == 1) {
      chess_side = 0;
    } else if (chess_side == 0) {
      chess_side = 1;
    }
  } else {
    chess_side = side;
  }
  gint j;
  for (j = 0; j < 34; j ++) {
    if ((chess_side == 0) && (j == 10)) {
      gtk_fixed_move (fixed, chessman_image[j], 4 * DIAMETER, 9 * DIAMETER);
      continue;
    }
    if ((chess_side == 1) && (j == 26)) {
      gtk_fixed_move (fixed, chessman_image[j], 4 * DIAMETER, 9 * DIAMETER);
      continue;
    }
    gtk_fixed_move (fixed, chessman_image[j], FIXEDOUT, FIXEDOUT);
  }
  chess_status = -2;
}

void init_local_chessman () {
  gint j;
  if (chess_side == 0) {
    for (j = 0; j < 16; j++) {
      xy[j][0] = xy_original[j][0];
      xy[j][1] = xy_original[j][1];
      gtk_fixed_move (fixed, chessman_image[j], xy[j][0] * DIAMETER, xy[j][1] * DIAMETER);
    }
  }
  if (chess_side == 1) {
    for (j = 16; j < 32; j++) {
      xy[j][0] = HCCHESS - xy_original[j - 16][0];
      xy[j][1] = VCCHESS - xy_original[j - 16][1];
      gtk_fixed_move (fixed, chessman_image[j], (HCCHESS - xy[j][0]) * DIAMETER, (VCCHESS - xy[j][1]) * DIAMETER);
    }
  }
  if (chess_status == -2) {
    chess_status = -1;
  }
  if (chess_status == 0) {
    chess_status = 1;
  }
}

void init_remote_chessman () {
  if (chess_status == 0) {
    return;
  }
  gint j;
  if (chess_side == 0) {
    for (j = 16; j < 32; j++) {
      xy[j][0] = HCCHESS - xy_original[j - 16][0];
      xy[j][1] = VCCHESS - xy_original[j - 16][1];
      gtk_fixed_move (fixed, chessman_image[j], xy[j][0] * DIAMETER, xy[j][1] * DIAMETER);
    }
  }
  if (chess_side == 1) {
    for (j = 0; j < 16; j++) {
      xy[j][0] = xy_original[j][0];
      xy[j][1] = xy_original[j][1];
      gtk_fixed_move (fixed, chessman_image[j], (HCCHESS - xy[j][0]) * DIAMETER, (VCCHESS - xy[j][1]) * DIAMETER);
    }
  }
  if (chess_status == -2) {
    chess_status = 0;
  }
  if (chess_status == -1) {
    chess_status = 1;
  }
}

gint get_chess_status () {
  return chess_status;
}

void convert_event_xy (gdouble event_x, gdouble event_y, gint *x, gint *y) {
  if (chess_side == 0) {
    *x = ((gint) event_x) / DIAMETER;
    *y = ((gint) event_y) / DIAMETER;
  }
  if (chess_side == 1) {
    *x = HCCHESS - (((gint) event_x) / DIAMETER);
    *y = VCCHESS - (((gint) event_y) / DIAMETER);
  }
}

gboolean check_action () {
  if ((chess_status < 1) || ((chess_status % 2) == chess_side)) {
    return FALSE;
  }
  return TRUE;
}

gint get_chessman_by_xy (gint x, gint y) {
  gint j;
  for (j = 0; j < 32; j++) {
    if ((x == xy[j][0]) && (y == xy[j][1])) {
      return j;
    }
  }
  return -1;
}

gboolean select_chessman (gint i) {
  if ((chess_side == 0) && (i > -1) && (i < 16)) {
    gtk_fixed_move (fixed, chessman_image[i], xy[i][0] * DIAMETER - 5, xy[i][1] * DIAMETER - 5);
    return TRUE;
  }
  if ((chess_side == 1) && (i > 15) && (i < 32)) {
    gtk_fixed_move (fixed, chessman_image[i], (HCCHESS - xy[i][0]) * DIAMETER - 5, (VCCHESS - xy[i][1]) * DIAMETER - 5);
    return TRUE;
  }
  return FALSE;
}

void unselect_chessman (gint i){
  if ((chess_side == 0) && (i > -1) && (i < 16)) {
    gtk_fixed_move (fixed, chessman_image[i], xy[i][0] * DIAMETER, xy[i][1] * DIAMETER);
    return;
  }
  if ((chess_side == 1) && (i > 15) && (i < 32)) {
    gtk_fixed_move (fixed, chessman_image[i], (HCCHESS - xy[i][0]) * DIAMETER, (VCCHESS - xy[i][1]) * DIAMETER);
    return;
  }
  return;
}

void get_xy_by_chessman (gint i, gint *x, gint *y) {
  if ((i < 0) || (i > 31)) {
    g_warning ("Invalid chessman : %d. It mast > 0 and < 32.\n", i);
    return;
  }
  *x = xy[i][0];
  *y = xy[i][1];
}

gboolean check_move (gint i, gint x, gint y) {
  if ((i < 0) || (i > 31) || (x < 0) || (x > 8) || (y < 0) || (y > 9)) {//|| (chess_status < 1)) {
    return FALSE;
  }
  gint j = 0;
  gint k = 0;
  gint z = -1;
  for (j = 0; j < 32; j++) {
    if (j == i) continue;
    if ((xy[j][0] == x) && (xy[j][1] == y) && (((i < 16) && (j < 16)) || ((i > 15) && (j > 15)))) {
      return FALSE;
    }
  }
  switch (i) {
    case 0:
    case 1:
    case 16:
    case 17:
      if ((x == xy[i][0]) && (y != xy[i][1])) {
        k = 0;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][0] == xy[i][0]) && (((xy[j][1] > xy[i][1]) && (xy[j][1] < y)) || ((xy[j][1] < xy[i][1]) && (xy[j][1] > y)))) {
            k++;
            break;
          }
        }
        if (k == 0) return TRUE;
      }
      if ((y == xy[i][1]) && (x != xy[i][0])) {
        k = 0;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][1] == xy[i][1]) && (((xy[j][0] > xy[i][0]) && (xy[j][0] < x)) || ((xy[j][0] < xy[i][0]) && (xy[j][0] > x)))) {
            k++;
            break;
          }
        }
        if (k == 0) return TRUE;
      }
      return FALSE;
    case 2:
    case 3:
    case 18:
    case 19:
      if ((ABS (x - xy[i][0]) == 1) && (y - xy[i][1] == 2)) {
        k = 0;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][0] == xy[i][0]) && (xy[j][1] - xy[i][1] == 1)) {
            k++;
            break;
          }
        }
        if (k == 0) return TRUE;
      }
      if ((ABS (x - xy[i][0]) == 1) && (y - xy[i][1] == -2)) {
        k = 0;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][0] == xy[i][0]) && (xy[j][1] - xy[i][1] == -1)) {
            k++;
            break;
          }
        }
        if (k == 0) return TRUE;
      }
      if ((ABS (y - xy[i][1]) == 1) && (x - xy[i][0] == 2)) {
        k = 0;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][1] == xy[i][1]) && (xy[j][0] - xy[i][0] == 1)) {
            k++;
            break;
          }
        }
        if (k == 0) return TRUE;
      }
      if ((ABS (y - xy[i][1]) == 1) && (x - xy[i][0] == -2)) {
        k = 0;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][1] == xy[i][1]) && (xy[j][0] - xy[i][0] == -1)) {
            k++;
            break;
          }
        }
        if (k == 0) return TRUE;
      }
      return FALSE;
    case 4:
    case 5:
    case 20:
    case 21:
      if ((x == xy[i][0]) && (y != xy[i][1])) {
        k = 0;
        z = -1; 
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][0] == x) && (xy[j][1] == y)) {
            z = j;
          }
          if ((xy[j][0] == xy[i][0]) && (((xy[j][1] > xy[i][1]) && (xy[j][1] < y)) || ((xy[j][1] < xy[i][1]) && (xy[j][1] > y)))) {
            k++;
          }
        }
        if ((k == 0) && (z == -1)) return TRUE;
        if ((z > -1) && (k == 1) && (((i < 16) && (z > 15)) || ((i > 15) && (z < 16)))) return TRUE;
      }
      if ((y == xy[i][1]) && (x != xy[i][0])) {
        k = 0;
        z = -1;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][0] == x) && (xy[j][1] == y)) {
            z = j;
          }
          if ((xy[j][1] == xy[i][1]) && (((xy[j][0] > xy[i][0]) && (xy[j][0] < x)) || ((xy[j][0] < xy[i][0]) && (xy[j][0] > x)))) {
            k++;
          }
        }
        if ((k == 0) && (z == -1)) return TRUE;
        if ((z > -1) && (k == 1) && (((i < 16) && (z > 15)) || ((i > 15) && (z < 16)))) return TRUE;
      }
      return FALSE;
    case 6:
    case 7:
      if ((ABS (x - xy[i][0]) == 2) && (ABS (y - xy[i][1]) == 2) && (y > 4)) {
        k = 0;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][0] == (x + xy[i][0]) / 2) && (xy[j][1] == (y + xy[i][1]) / 2)) {
            k++;
            break;
          }
        }
        if (k == 0) return TRUE;
      }
      return FALSE;
    case 22:
    case 23:
      if ((ABS (x - xy[i][0]) == 2) && (ABS (y - xy[i][1]) == 2) && (y < 5)) {
        k = 0;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][0] == (x + xy[i][0]) / 2) && (xy[j][1] == (y + xy[i][1]) / 2)) {
            k++;
            break;
          }
        }
        if (k == 0) return TRUE;
      }
      return FALSE;
    case 8:
    case 9:
      if ((ABS (x - xy[i][0]) == 1) && (ABS (y - xy[i][1]) == 1) && (y > 6) && (x > 2) && (x < 6)) {
        return TRUE;
      }
      return FALSE;
    case 24:
    case 25:
      if ((ABS (x - xy[i][0]) == 1) && (ABS (y - xy[i][1]) == 1) && (y < 3) && (x > 2) && (x < 6)) {
        return TRUE;
      }
      return FALSE;
    case 10:
      if ((((ABS (x - xy[i][0]) == 1) && (y == xy[i][1])) || ((ABS (y - xy[i][1]) == 1) && (x == xy[i][0]))) 
             && (y > 6) && (x > 2) && (x < 6)) {
        return TRUE;
      }
      if ((xy[10][0] == xy [26][0]) && (x == xy[26][0]) && (y == xy[26][1])) {
        k = 0;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][0] == xy [10][0]) && (xy[j][1] > xy [26][1]) && (xy[j][1] < xy[10][1])) {
            k++;
            break;
          }
        }
        if (k == 0) return TRUE;
      }
      return FALSE;
    case 26:
      if ((((ABS (x - xy[i][0]) == 1) && (y == xy[i][1])) || ((ABS (y - xy[i][1]) == 1) && (x == xy[i][0])))
             && (y < 3) && (x > 2) && (x < 6)) {
        return TRUE;
      }
      if ((xy[10][0] == xy [26][0]) && (x == xy[10][0]) && (y == xy[10][1])) {
        k = 0;
        for (j = 0; j < 32; j++) {
          if (j == i) continue;
          if ((xy[j][0] == xy[10][0]) && (xy[j][1] > xy [26][1]) && (xy[j][1] < xy[10][1])) {
            k++;
            break;
          }
        }
        if (k == 0) return TRUE;
      }
      return FALSE;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      if ((y - xy[i][1] == -1) && (x == xy[i][0])) {
        return TRUE;
      }
      if ((xy[i][1] < 5) && (ABS (x - xy[i][0]) == 1) && (y == xy[i][1])) {
        return TRUE;
      }
      return FALSE; 
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
      if ((y - xy[i][1] == 1) && (x == xy[i][0])) {
        return TRUE;
      }
      if ((xy[i][1] > 4) && (ABS (x - xy[i][0]) == 1) && (y == xy[i][1])) {
        return TRUE;
      }
      return FALSE; 
    default :
      break;
      return FALSE;
  } /* end switch (i) */
  return FALSE;
}

void move_message (gint i, gint x, gint y) {
  GtkWidget *textview;
  GtkTextBuffer *buffer;
  GtkTextMark *mark;
  GtkTextIter iter;
  gchar *tmp = NULL;
  gchar *message = NULL;
  textview = get_widget (GTK_WIDGET (fixed), "textview_record");
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));
  mark = gtk_text_buffer_get_mark (buffer, "mark");
  gtk_text_buffer_get_end_iter (buffer, &iter);
  
  gint k = 0;
  if (i % 2) {
    k = i - 1;
  } else {
    k = i + 1;
  }
  switch (i) {
    case 0:
    case 1:
    case 4:
    case 5:
      if (xy[i][0] == xy[k][0]) {
        if (xy[i][1] > xy[k][1]) {
          tmp = g_strdup_printf ("%s %s", _(move_info[1]), _(red_pieces[(i / 2)]));
        } else {
          tmp = g_strdup_printf ("%s %s", _(move_info[0]), _(red_pieces[(i / 2)]));
        }
      } else {
        tmp = g_strdup_printf (" %s %s", _(red_pieces[(i / 2)]), _(number[xy[i][0]]));
      }
      if (y == xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[4]), _(number[x]));
      }
      if (y > xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[3]), _(number[VCCHESS - ABS(y - xy[i][1])]));
      }
      if (y < xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[2]), _(number[VCCHESS - ABS(y - xy[i][1])]));
      }
      break;
    case 2:
    case 3:
      if (xy[i][0] == xy[k][0]) {
        if (xy[i][1] > xy[k][1]) {
          tmp = g_strdup_printf ("%s %s", _(move_info[1]), _(red_pieces[(i / 2)]));
        } else {
          tmp = g_strdup_printf ("%s %s", _(move_info[0]), _(red_pieces[(i / 2)]));
        }
      } else {
        tmp = g_strdup_printf (" %s %s", _(red_pieces[(i / 2)]), _(number[xy[i][0]]));
      }
      if (y > xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[3]), _(number[x]));
      }
      if (y < xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[2]), _(number[x]));
      }
      break;
    case 6:
    case 7:
    case 8:
    case 9:
      tmp = g_strdup_printf (" %s %s", _(red_pieces[(i / 2)]), _(number[xy[i][0]]));
      if (y > xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[3]), _(number[x]));
      }
      if (y < xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[2]), _(number[x]));
      }
      break;
    case 10:
      tmp = g_strdup_printf (" %s %s", _(red_pieces[(i / 2)]), _(number[xy[i][0]]));
      if (y == xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[4]), _(number[x]));
      }
      if (y > xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[3]), _(number[VCCHESS - ABS(y - xy[i][1])]));
      }
      if (y < xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[2]), _(number[VCCHESS - ABS(y - xy[i][1])]));
      }
      break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:// 5 pawns, so sometimes could be canfused with this record method. don't care.
      tmp = g_strdup_printf (" %s %s", _(red_pieces[6]), _(number[xy[i][0]]));
      if (y == xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[4]), _(number[x]));
      }
      if (y > xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[3]), _(number[VCCHESS - ABS(y - xy[i][1])]));
      }
      if (y < xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[2]), _(number[VCCHESS - ABS(y - xy[i][1])]));
      }
      break;
    case 16:
    case 17:
    case 20:
    case 21:
      if (xy[i][0] == xy[k][0]) {
        if (xy[i][1] > xy[k][1]) {
          tmp = g_strdup_printf ("%s%s", _(move_info[0]), _(black_pieces[((i - 16) / 2)]));
        } else {
          tmp = g_strdup_printf ("%s%s", _(move_info[1]), _(black_pieces[((i - 16) / 2)]));
        }
      } else {
        tmp = g_strdup_printf ("%s %s", _(black_pieces[((i - 16) / 2)]), number[HCCHESS - xy[i][0]]);
      }
      if (y == xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[4]), number[HCCHESS - x]);
      }
      if (y > xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[2]), number[VCCHESS - ABS(y - xy[i][1])]);
      }
      if (y < xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[3]), number[VCCHESS - ABS(y - xy[i][1])]);
      }
      break;
    case 18:
    case 19:
      if (xy[i][0] == xy[k][0]) {
        if (xy[i][1] > xy[k][1]) {
          tmp = g_strdup_printf ("%s%s", _(move_info[0]), _(black_pieces[((i - 16) / 2)]));
        } else {
          tmp = g_strdup_printf ("%s%s", _(move_info[1]), _(black_pieces[((i - 16) / 2)]));
        }
      } else {
        tmp = g_strdup_printf ("%s %s", _(black_pieces[((i - 16) / 2)]), number[HCCHESS - xy[i][0]]);
      }
      if (y > xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[2]), number[HCCHESS - x]);
      }
      if (y < xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[3]), number[HCCHESS - x]);
      }
      break;
    case 22:
    case 23:
    case 24:
    case 25:
      tmp = g_strdup_printf ("%s %s", _(black_pieces[((i - 16) / 2)]), number[HCCHESS - xy[i][0]]);
      if (y > xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[2]), number[HCCHESS - x]);
      }
      if (y < xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[3]), number[HCCHESS - x]);
      }
      break;
    case 26:
      tmp = g_strdup_printf ("%s %s", _(black_pieces[((i - 16) / 2)]), number[HCCHESS - xy[i][0]]);
      if (y == xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[4]), number[HCCHESS - x]);
      }
      if (y > xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[2]), number[VCCHESS - ABS(y - xy[i][1])]);
      }
      if (y < xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[3]), number[VCCHESS - ABS(y - xy[i][1])]);
      }
      break;
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
      tmp = g_strdup_printf ("%s %s", _(black_pieces[6]), number[HCCHESS - xy[i][0]]);
      if (y == xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[4]), number[HCCHESS - x]);
      }
      if (y > xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[2]), number[VCCHESS - ABS(y - xy[i][1])]);
      }
      if (y < xy[i][1]) {
        message = g_strdup_printf ("%d: %s %s %s\n", chess_status, tmp, _(move_info[3]), number[VCCHESS - ABS(y - xy[i][1])]);
      }
      break;
    default :
      message = g_strdup_printf ("ERROR: %s i = %d, x = %d, y = %d\n", _("Can not convert !"), i, x, y);
      break;
  } /* end switch (i) */
  gtk_text_buffer_insert (buffer, &iter, message, -1);
  gtk_text_buffer_move_mark (buffer, mark, &iter);
  gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (textview), mark, 0.0, FALSE, 0.0, 0.0);
  g_free ((gpointer) tmp);
  g_free ((gpointer) message);
}

void move_chessman (gint i, gint x, gint y) {
  gint j = get_chessman_by_xy (x, y);
  move_message (i, x, y);

  if ((j > -1) && (j < 32)) {
    gtk_fixed_move (fixed, chessman_image[j], FIXEDOUT, FIXEDOUT);
    xy[j][0] = 0 - chess_status;
    xy[j][1] = 0 - i;
  }
  if (chess_side == 0) {
    gtk_fixed_move (fixed, chessman_image[i], x * DIAMETER, y * DIAMETER);
    gtk_fixed_move (fixed, chessman_image[32], xy[i][0] * DIAMETER, xy[i][1] * DIAMETER);
    gtk_fixed_move (fixed, chessman_image[33], x * DIAMETER, y * DIAMETER);
  }
  if (chess_side == 1) {
    gtk_fixed_move (fixed, chessman_image[i], (HCCHESS - x) * DIAMETER, (VCCHESS - y) * DIAMETER);
    gtk_fixed_move (fixed, chessman_image[32], (HCCHESS - xy[i][0]) * DIAMETER, (VCCHESS - xy[i][1]) * DIAMETER);
    gtk_fixed_move (fixed, chessman_image[33], (HCCHESS - x) * DIAMETER, (VCCHESS - y) * DIAMETER);
  }
  xy[i][0] = x;
  xy[i][1] = y;

  if (j == 10) {
    if (chess_side == 0) {
      game_over (1);
      return;
    }
    if (chess_side == 1) {
      game_over (0);
      return;
    }
  }
  if (j == 26) {
    if (chess_side == 0) {
      game_over (0);
      return;
    }
    if (chess_side == 1) {
      game_over (1);
      return;
    }
  }

  chess_status++;
  for (j = 0; j < 32; j ++) {
    if ((j < 16) && (check_move (j, xy[26][0], xy[26][1]))) {
      write_textview (_("CHECKMATE !\n"), 0);
      break;
    }
    if ((j > 15) && (check_move (j, xy[10][0], xy[10][1]))) {
      write_textview (_("CHECKMATE !\n"), 0);
      break;
    }
  }
}

void write_textview (const gchar *data, gint tag) {
  GtkWidget *textview;
  GtkTextBuffer *buffer;
  GtkTextMark *mark;
  GtkTextIter iter;
  textview = get_widget (GTK_WIDGET (fixed), "textview_talk");
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));
  mark = gtk_text_buffer_get_mark (buffer, "mark");
  gtk_text_buffer_get_end_iter (buffer, &iter);
  switch (tag) {
    case 0:
      gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, data, -1, "red_fg", NULL);
      break;
    case 1:
      gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, data, -1, "blue_fg", NULL);
      break;
    default :
      gtk_text_buffer_insert (buffer, &iter, data, -1);
      break;
  }
  gtk_text_buffer_move_mark (buffer, mark, &iter);
  gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (textview), mark, 0.0, FALSE, 0.0, 0.0);
}

void game_over (gint status) {
  if (chess_status < 1) return;
  GtkWidget *dialog;
  GtkWidget *content_area;
  GtkWidget *dialog_box;
  GtkWidget *image_info;
  GtkWidget *label_info;
  gchar *tmp = NULL;
  dialog = gtk_dialog_new_with_buttons (_("Game over"),
                                       GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (fixed))),
                                       GTK_DIALOG_MODAL |   GTK_DIALOG_DESTROY_WITH_PARENT,
                                       _("_OK"),
                                       GTK_RESPONSE_NONE,
                                       NULL);
  content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

  dialog_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_show (dialog_box);
  gtk_box_pack_start (GTK_BOX (content_area), dialog_box, TRUE, TRUE, 8);

  image_info = gtk_image_new_from_icon_name ("dialog-info", GTK_ICON_SIZE_DIALOG);
  gtk_widget_show (image_info);
  gtk_box_pack_start (GTK_BOX (dialog_box), image_info, TRUE, TRUE, 8);

  switch (status) {
    case 0:
      write_textview ("YOU WIN !\n", 0);
      tmp = g_strdup_printf ("<span><big><b>%s</b></big></span>", _("YOU WIN !\n"));
      break;
    case 1:
      write_textview ("YOU LOSE !\n", 0);
      tmp = g_strdup_printf ("<span><big><b>%s</b></big></span>", _("YOU LOSE !\n"));
      break;
    case 2:
      write_textview ("DRAW GAME !\n", 0);
      tmp = g_strdup_printf ("<span><big><b>%s</b></big></span>", _("DRAW GAME !\n"));
      break;
    default :
      break;
  }

  label_info = gtk_label_new (NULL);
  gtk_label_set_markup (GTK_LABEL (label_info), tmp);
  gtk_widget_show (label_info);
  gtk_box_pack_start (GTK_BOX (dialog_box), label_info, TRUE, TRUE, 8);
  g_free ((gpointer) tmp);
  
  g_signal_connect_swapped (G_OBJECT (dialog), "response", G_CALLBACK (gtk_widget_destroy), (gpointer) dialog);
  
  gtk_widget_show (dialog);

  GtkWidget *textview;
  GtkTextBuffer *buffer;
  textview = get_widget (GTK_WIDGET (fixed), "textview_record");
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));
  gtk_text_buffer_set_text (buffer, "", -1);

  chess_status = -2;
  change_chess_side (-1);
}

void write_textview_to_file(const char *filename) {
    GtkWidget *textview = get_widget(GTK_WIDGET(fixed), "textview_record");

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);

    gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        g_free(text);
        return;
    }

    fprintf(file, "%s", text);

    fclose(file);

    g_free(text);
}
