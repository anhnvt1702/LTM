#include <gtk/gtk.h>
#include <stdlib.h>
#include "chessgame.h"
#include "createwindow.h"
#include "message.h"
#include <sys/socket.h>


GtkWidget *chessgame_window;
GtkTreeStore *table_list_store;
GtkTreeStore *online_list_store;

GtkWidget *table_tree_view;
GtkWidget *online_tree_view;

GtkWidget *create_room_button;
GtkWidget *invite_game_button;
GtkWidget *exit_button;


void create_room_button_clicked(GtkButton *button, gpointer user_data)
{
    if (user_data == NULL) {
        g_print("Invalid user_data\n");
        return;
    }

    struct Request *request = (struct Request *)user_data;

    // Kiểm tra xem messageType có là LOGIN_REQUEST không
    if (request->messageType == LOGIN_REQUEST) {
        const char *username = request->data.login.username;
        g_print("Username: %s\n", username);
        create_window_main(request->data.login.port,username);
        g_print("Invite button clicked\n");
    }
}

void invite_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    struct Request *request = (struct Request *)user_data;
    
    // Cấp phát bộ nhớ cho request_send
    struct Request *request_send = malloc(sizeof(struct Request));

    // Kiểm tra xem có thể cấp phát bộ nhớ không
    if (request_send == NULL) {
        g_print("Memory allocation error\n");
        return;
    }

    // Get the selected user from the online tree view
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(online_tree_view));
    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gchar *selected_user;
        gtk_tree_model_get(model, &iter, COL_NAME_ONLINE, &selected_user, -1);
        char *selected_user_char = g_strdup(selected_user);
        
        // Perform actions with the selected_user, for example, send an invitation
        g_print("Invite button clicked for user: %s\n", selected_user);
        
        // Populate request_send
        request_send->messageType = INVITE_REQUEST;
        strcpy(request_send->data.invite.invitefrom, request->data.login.username);
        strcpy(request_send->data.invite.inviteto, selected_user_char);
        request_send->data.invite.port = request->data.login.port;
        g_print("Port:%d\n",request->data.login.cli_socket);

        // Send the invitation
        send(request->data.login.cli_socket, request_send, sizeof(struct Request), 0);

        // Free allocated memory
        free(request_send);
        g_free(selected_user_char);
    }
    else
    {
        // No user selected, show a message or perform other actions
        g_print("No user selected for invitation\n");

        // Free allocated memory
        free(request_send);
    }
}


void exit_button_clicked(GtkButton *button, gpointer user_data)
{
    // Xử lý sự kiện khi nút "Thoát" được nhấn
    gtk_main_quit();
}

// Function to add an online account to the list
void add_online_account(const char *account_name)
{
    GtkTreeIter iter;
    gtk_tree_store_append(online_list_store, &iter, NULL);
    gtk_tree_store_set(online_list_store, &iter, COL_NAME_ONLINE, account_name, -1);
}

// Function to remove an online account from the list
void remove_online_account(const char *account_name)
{
    GtkTreeIter iter;
    gboolean valid;
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(online_list_store), &iter);
    while (valid)
    {
        gchar *name;
        gtk_tree_model_get(GTK_TREE_MODEL(online_list_store), &iter, COL_NAME_ONLINE, &name, -1);
        if (strcmp(name, account_name) == 0)
        {
            gtk_tree_store_remove(online_list_store, &iter);
            g_free(name);
            break;
        }
        g_free(name);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(online_list_store), &iter);
    }
}

// Function to update the list of online accounts from the "acc_onl.txt" file
void update_online_accounts()
{
    FILE *file = fopen("account_online.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Clear existing data in online_list_store
    gtk_tree_store_clear(online_list_store);

    char account_name[50];

    // Read account names from the file and add them to the online_list_store
    while (fscanf(file, "%s", account_name) == 1)
    {
        add_online_account(account_name);
    }

    // Close the file
    fclose(file);
}
guint update_timer = 0;

void on_window_destroy(GtkWidget *widget, gpointer data)
{
    // Hủy bỏ hàm đếm thời gian khi cửa sổ đóng
    if (update_timer != 0)
    {
        g_source_remove(update_timer);
    }
    gtk_main_quit();
}

// Hàm cập nhật danh sách người chơi online
gboolean update_online_list(gpointer user_data)
{
    update_online_accounts();
    return G_SOURCE_CONTINUE; // Tiếp tục gọi hàm sau mỗi khoảng thời gian
}



// Các hàm add_online_account, remove_online_account và update_online_accounts giữ nguyên như trong ví dụ trước đó.

void run_wait_chinesechess_game(GtkWidget *widget, gpointer data)
{
    // Trong hàm run_wait_chinesechess_game
struct Request *originalRequest = (struct Request *)data;
struct Request *copiedRequest = (struct Request *)g_memdup2(originalRequest, sizeof(struct Request));



    // Tạo một GtkListStore cho danh sách người chơi online
    online_list_store = gtk_tree_store_new(NUM_COLS_ONLINE, G_TYPE_STRING);

    // Tạo GtkTreeView và thiết lập cột cho người chơi online
    online_tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(online_list_store));

    GtkTreeViewColumn *online_col;
    GtkCellRenderer *online_renderer;

    online_col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(online_col, "Online Players");
    gtk_tree_view_append_column(GTK_TREE_VIEW(online_tree_view), online_col);

    online_renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(online_col, online_renderer, TRUE);
    gtk_tree_view_column_add_attribute(online_col, online_renderer, "text", COL_NAME_ONLINE);
     update_online_accounts();
    update_timer = g_timeout_add_seconds(3, update_online_list, NULL);

    g_signal_connect(chessgame_window, "destroy", G_CALLBACK(on_window_destroy), NULL);
    // Thực hiện update danh sách người chơi online mỗi khoảng thời gian hoặc khi có sự thay đổi
    // update_online_accounts();

    create_room_button = gtk_button_new_with_label("Tạo Phòng");
    invite_game_button = gtk_button_new_with_label("Mời");
    exit_button = gtk_button_new_with_label("Thoát");

    g_signal_connect(create_room_button, "clicked", G_CALLBACK(create_room_button_clicked), copiedRequest);
    g_signal_connect(invite_game_button, "clicked", G_CALLBACK(invite_button_clicked), copiedRequest);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(exit_button_clicked), NULL);

    // Pack các nút vào một vbox
    GtkWidget *vbox_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), create_room_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), invite_game_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), exit_button, FALSE, FALSE, 0);

    // Pack các thành phần vào một vbox lớn
    GtkWidget *vbox_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox_main), online_tree_view, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_main), vbox_buttons, FALSE, FALSE, 0);

    chessgame_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(chessgame_window), "Chess Game");
    gtk_window_set_default_size(GTK_WINDOW(chessgame_window), 400, 300);
    gtk_container_add(GTK_CONTAINER(chessgame_window), vbox_main);
    // Pack vbox_main vào window
    gtk_widget_show_all(chessgame_window);
    
    
}
