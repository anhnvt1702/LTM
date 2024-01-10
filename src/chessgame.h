#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <gtk/gtk.h>
#include <stdlib.h>


#define NUM_COLS_TABLE 2
#define COL_NAME_TABLE 0
#define COL_PLAYERS_TABLE 1
#define MAX_TABLES 100

#define NUM_COLS_ONLINE 1
#define COL_NAME_ONLINE 0
#define MAX_ONLINE_ACCOUNTS 100

typedef struct {
    char table_name[50];
    int num_players;
} ChessTable;

typedef struct {
    char account_name[50];
} Account;

extern ChessTable chess_tables[MAX_TABLES];
extern Account online_accounts[MAX_ONLINE_ACCOUNTS];

extern GtkWidget *chessgame_window;
extern GtkTreeStore *table_list_store;
extern GtkTreeStore *online_list_store;

extern GtkWidget *table_tree_view;
extern GtkWidget *online_tree_view;

extern GtkWidget *create_room_button;
extern GtkWidget *invite_game_button;
extern GtkWidget *exit_button;

void create_room_button_clicked(GtkButton *button, gpointer user_data);
void invite_button_clicked(GtkButton *button, gpointer user_data);
void exit_button_clicked(GtkButton *button, gpointer user_data);

void add_online_account(const char *account_name);
void remove_online_account(const char *account_name) ;
void update_online_accounts();
void run_wait_chinesechess_game(GtkWidget *widget, gpointer data);

#endif /* CHESS_GAME_H */
