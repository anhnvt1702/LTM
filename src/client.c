#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include "signin.h"
#include "message.h"
#include "chessgame.h"
#include "online.h"
#include "createwindow.h"
#include <time.h>

#define BUFFER_SIZE 1024
#define Port 5500
#define SERVER_ADDR "127.0.0.1"

/* Standard gettext macros. */
#ifdef ENABLE_NLS
#include <libintl.h>
#undef _
#define _(String) dgettext(PACKAGE, String)
#ifdef gettext_noop
#define N_(String) gettext_noop(String)
#else
#define N_(String) (String)
#endif
#else
#define textdomain(String) (String)
#define gettext(String) (String)
#define dgettext(Domain, Message) (Message)
#define dcgettext(Domain, Message, Type) (Message)
#define bindtextdomain(Domain, Directory) (Domain)
#define _(String) (String)
#define N_(String) (String)
#endif
char logged_in_username[BUFFER_SIZE];
int client_socket;
struct Response received_response;
struct Request request_send;

void handle_login_response(struct Response *response, struct Request *request);
void *background_task(void *data);
void login(GtkButton *widget, gpointer user_data);
void handle_exit(int signo);

int generate_random_4_digit_number()
{

    srand((unsigned int)time(NULL));

    // Sinh số ngẫu nhiên từ 1000 đến 9999
    return rand() % (9999 - 1000 + 1) + 1000;
}

void *background_task(void *data)
{
    struct Response *received_response = malloc(sizeof(struct Response));
    while (1)
    {
        int recv_size = recv(client_socket, received_response, sizeof(received_response), 0);
        // Nhận gói tin từ server
        printf("Server: %d ",recv_size);
        if (recv_size <= 0)
        {
            // Server disconnected
            printf("Disconnected from server.\n");
            close(client_socket);
            exit(EXIT_SUCCESS);

        }else if (received_response->messageType == INVITE_REQUEST)
        {
            
            if (received_response->data.resinvite.resStatus == RECEIVE)
            {

                printf("okkkkkkkkkkkkkkkkkkkkk");
                // GtkWidget *dialog;
                // GtkWidget *content_area;
                // GtkWidget *label;
                // // Tạo dialog
                // dialog = gtk_dialog_new_with_buttons("Mời chơi cờ", GTK_WINDOW(chessgame_window),
                //                                      GTK_DIALOG_MODAL,
                //                                      "Chấp nhận", GTK_RESPONSE_ACCEPT,
                //                                      "Từ chối", GTK_RESPONSE_REJECT,
                //                                      NULL);

                // // Lấy content area của dialog để thêm nội dung
                // content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

                // // Tạo label với nội dung thông báo
                // char message[1038];
                // snprintf(message, sizeof(message), "Bạn nhận %s", thread_data->received_response.data.resinvite.resInvitefrom);
                // label = gtk_label_new(message);

                // // Thêm label vào content area
                // gtk_container_add(GTK_CONTAINER(content_area), label);

                // // Hiển thị tất cả các widget trong dialog
                // gtk_widget_show_all(dialog);

                // // Chờ đợi phản hồi từ dialog
                // gint result = gtk_dialog_run(GTK_DIALOG(dialog));

                // // Xử lý phản hồi
                // switch (result)
                // {
                // case GTK_RESPONSE_ACCEPT:
                //     // Thực hiện hành động khi chấp nhận
                //     g_print("Đã chấp nhận lời mời từ %s\n", thread_data->received_response.data.resinvite.resInvitefrom);
                //     break;
                // case GTK_RESPONSE_REJECT:
                //     // Thực hiện hành động khi từ chối
                //     g_print("Đã từ chối lời mời từ %s\n", thread_data->received_response.data.resinvite.resInvitefrom);
                //     break;
                // }

                // // Đóng dialog
                // gtk_widget_destroy(dialog);
            }
        }
        
        
    }
    pthread_exit(NULL);
}

void handle_login_response(struct Response *response, struct Request *request)
{
    if (response->data.reslogin.response == OK)
    {
        int port_room = generate_random_4_digit_number();
        strcpy(logged_in_username, response->data.reslogin.user);
        request->data.login.port = port_room;
        gtk_widget_destroy(login_window);
        request->data.login.serv_socket = response->data.reslogin.serv_socket;
        run_wait_chinesechess_game(NULL, request);
    }
    else
    {
        printf("Login failed: ");

        // Hiển thị thông báo lỗi cho người dùng (bạn có thể sử dụng dialog box)
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(login_window),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Login failed: %s ", response->data.reslogin.result);

        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void login(GtkButton *widget, gpointer user_data)
{
    const gchar *username_text = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password_text = gtk_entry_get_text(GTK_ENTRY(password_entry));
    int client_socket = *(int *)user_data;
    request_send.messageType = LOGIN_REQUEST;
    strncpy(request_send.data.login.username, username_text, MAX_SIZE - 1);
    request_send.data.login.username[MAX_SIZE - 1] = '\0';
    strncpy(request_send.data.login.password, password_text, MAX_SIZE - 1);
    request_send.data.login.password[MAX_SIZE - 1] = '\0';
    request_send.data.login.cli_socket = client_socket;
    send(client_socket, &request_send, sizeof(request_send), 0);
    recv(client_socket, &received_response, sizeof(received_response), 0);
    handle_login_response(&received_response, &request_send);
}

void handle_exit(int signo)
{
    // Thực hiện các hành động cần thiết trước khi thoát
    printf("Exiting...\n");
    removeAccount(logged_in_username);

    exit(0);
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Port);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, background_task, NULL) != 0)
    {
        perror("Error creating receive thread");
        exit(EXIT_FAILURE);
    }
    
    login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(login_window), "Login Form");
    gtk_container_set_border_width(GTK_CONTAINER(login_window), 10);
    gtk_widget_set_size_request(login_window, 300, 200);

    // Create entry widgets for login
    username_entry = gtk_entry_new();
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(password_entry), '*');

    // Create buttons for login and registration
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    GtkWidget *register_button = gtk_button_new_with_label("Register");

    // Connect signals to the callback functions
    g_signal_connect(login_button, "clicked", G_CALLBACK(login), &client_socket);
    g_signal_connect(register_button, "clicked", G_CALLBACK(open_register_window), NULL);

    // Pack the widgets into the login window
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(login_window), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), register_button, FALSE, FALSE, 0);

    gtk_widget_show_all(login_window);
    gtk_main();
    return 0;
}
