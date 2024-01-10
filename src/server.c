#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "message.h"
#include "online.h"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
struct Account
{
    char username[50];
    char password[50];
    int status;
    int elo;
    struct Account *next;
};
struct Account *accounts;
struct onlineAccounts *onlineaccounts;

// struct onlineAccounts {
//     char username[50];
// };

struct Account *getAccountFromFile(const char *fileName) // Lấy tất cả tài khoản và mật khẩu có trong file lưu vào dslk
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Khong the mo tep %s\n", fileName);
        return NULL;
    }
    // Khởi tạo con trỏ đầu và cuối
    struct Account *head = NULL;
    struct Account *tail = NULL;

    char username[50], password[50];
    int status, elo;

    while (fscanf(file, "%s %s %d %d", username, password, &status, &elo) == 3) // Nếu đọc đủ 3 trường của 1 account thì tiếp tục đọc
    {
        // Khởi tạo 1 account mới
        struct Account *newAccount = (struct Account *)malloc(sizeof(struct Account));
        strcpy(newAccount->username, username);
        strcpy(newAccount->password, password);
        newAccount->status = status;
        newAccount->elo = elo;
        newAccount->next = NULL;

        // Thực hiện đưa account mới đọc được vào dslk
        if (head == NULL)
        {
            head = newAccount;
            tail = newAccount;
        }
        else
        {
            tail->next = newAccount;
            tail = newAccount;
        }
    }

    fclose(file);
    return head; // Trả về con trỏ đầu tiên của dslk
}

int isUsernameExistOrBlocked(char username[]) // Kiểm tra tài khoản có tồn tại hay không
{
    struct Account *current = accounts;
    while (current != NULL)
    {
        if (strcmp(username, current->username) == 0)
        {
            if (current->status == 1)
            {
                return 1; // Nếu tồn tại tài khoản trả về 1 và không bị block
            }
            else
            {
                return 2; // Nếu bị block trả về 2
            }
        }
        current = current->next;
    }
    return 0; // Tài khoản không tồn tại trả về 0
}

int isPasswordCorrect(char username[], char password[]) // Kiểm tra tài khoản và mật khẩu nhập đúng chưa
{
    struct Account *current = accounts;
    while (current != NULL)
    {
        if (strcmp(username, current->username) == 0 && strcmp(password, current->password) == 0)
        {
            return 1; // Nếu mật khẩu đúng trả về 1
        }
        current = current->next;
    }
    return 0; // Mật khẩu sai trả về 0
}

int sendStatusToClient(char *username, char *password, struct sockaddr_in client, int server_sock, int client_sock)
{
    int readOnline = readAccountsFromFile(onlineaccounts);
    if (readOnline != -1)
    {
        int status = isUsernameExistOrBlocked(username);
        struct Response login_response;
        switch (status)
        {
        case 1:
            if (isPasswordCorrect(username, password))
            {
                if ((readOnline != 0) && (findAccount(onlineaccounts, username) != -1))
                {
                    // login_response.messageType = LOGIN_REQUEST;
                    login_response.data.reslogin.response = FAILURE;
                    strcpy(login_response.data.reslogin.result, "account logged in");
                    send(client_sock, &login_response, sizeof(login_response), 0);
                    return 4; // Nếu tài khoản đã đăng nhập trả về 4
                }
                else
                {
                    writeAccountsToFile(username);
                    // login_response.messageType = LOGIN_REQUEST;
                    login_response.data.reslogin.response = OK;
                    strcpy(login_response.data.reslogin.user, username);
                    strcpy(login_response.data.reslogin.result, "Loged in");
                    login_response.data.reslogin.serv_socket = client_sock;
                    send(client_sock, &login_response, sizeof(login_response), 0);
                    return 1; // Neu mat khau dung tra ve 1
                }
            }
            else
            {
                // login_response.messageType = LOGIN_REQUEST;
                login_response.data.reslogin.response = FAILURE;
                strcpy(login_response.data.reslogin.result, "not OK");
                send(client_sock, &login_response, sizeof(login_response), 0);
                return 0; // Neu mat khau sai tra ve 0
            }

            break;
        case 2:
            // login_response.messageType = LOGIN_REQUEST;
            login_response.data.reslogin.response = FAILURE;
            strcpy(login_response.data.reslogin.result, "account not ready");
            send(client_sock, &login_response, sizeof(login_response), 0);
            return 2; // Neu tai khoan chua duoc kich hoat tra ve 2
        case 0:
            // login_response.messageType = LOGIN_REQUEST;
            login_response.data.reslogin.response = FAILURE;
            strcpy(login_response.data.reslogin.result, "account not exist");
            send(client_sock, &login_response, sizeof(login_response), 0);
            return 3; // Neu tai khoan khong ton tai tra ve 3
        default:
            break;
        }
    }
}

// Hàm thêm tài khoản vào danh sách liên kết
void addAccount(struct Account **head, const char *username, const char *password, int status)
{
    struct Account *newAccount = (struct Account *)malloc(sizeof(struct Account));
    if (newAccount == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(newAccount->username, username, sizeof(newAccount->username));
    strncpy(newAccount->password, password, sizeof(newAccount->password));
    newAccount->status = status;
    newAccount->next = *head;
    *head = newAccount;
}

// Hàm xóa tài khoản khỏi danh sách liên kết
void deleteAccount(struct Account **head, const char *username)
{
    struct Account *current = *head;
    struct Account *prev = NULL;

    while (current != NULL)
    {
        if (strcmp(current->username, username) == 0)
        {
            if (prev == NULL)
            {
                // Xóa đầu danh sách
                *head = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            free(current);
            return;
        }

        prev = current;
        current = current->next;
    }
}

// Hàm kiểm tra xem có username này trong danh sách liên kết hay không
int isUsernameExists(struct Account *head, const char *username)
{

    while (head != NULL)
    {
        if (strcmp(head->username, username) == 0)
        {
            return 1; // Tìm thấy
        }

        head = head->next;
    }
    return 0; // Không tìm thấy
}

int main(int argc, char *argv[])
{
    accounts = getAccountFromFile("account.txt");
    int server_socket, client_sockets[MAX_CLIENTS], max_clients = MAX_CLIENTS, is_socket_logged[MAX_CLIENTS], is_account_locked[MAX_CLIENTS];
    char userInSocket[MAX_SIZE][max_clients];
    fd_set read_fds;
    onlineaccounts = malloc(MAX_ACCOUNTS * sizeof(struct onlineAccounts));
    int max_sd, activity, i, valread;
    struct Request received_request;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE], account_logged[MAX_CLIENTS][100];
    socklen_t client_address_len = sizeof(client_addr);
    int status = -1;
    // Initialize client_sockets array
    for (i = 0; i < max_clients; i++)
    {
        client_sockets[i] = 0;
        is_socket_logged[i] = 0;
        is_account_locked[i] = 0;
    }

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 3) == -1)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", atoi(argv[1]));

    while (1)
    {
        status = -1;
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        max_sd = server_socket;

        // Add child sockets to set
        for (i = 0; i < max_clients; i++)
        {
            int sd = client_sockets[i];
            if (sd > 0)
            {
                FD_SET(sd, &read_fds);
            }
            if (sd > max_sd)
            {
                max_sd = sd;
            }
        }

        // Wait for an activity on any of the sockets
        activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);

        if ((activity < 0))
        {
            printf("Select error\n");
        }

        // If something happened on the server socket (new connection)
        if (FD_ISSET(server_socket, &read_fds))
        {
            int new_socket;
            if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_address_len)) < 0)
            {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            // Add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                if (client_sockets[i] == 0)
                {
                    client_sockets[i] = new_socket;
                    printf("New connection, socket fd is %d, ip is : %s, port : %d\n", new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    break;
                }
            }
        }

        // Handle data from clients
        for (i = 0; i < max_clients; i++)
        {
            int sd = client_sockets[i];

            if (FD_ISSET(sd, &read_fds))
            {
                recv(sd, &received_request, sizeof(received_request), 0);

                // Xử lý received_request ở đây
                switch (received_request.messageType)
                {
                case LOGIN_REQUEST:
                    if (is_socket_logged[i] == 0)
                    {
                        char *username = received_request.data.login.username;
                        char *password = received_request.data.login.password;
                        printf("%s | %s|\n", username, password);
                        status = sendStatusToClient(username, password, client_addr, server_socket, sd);

                        // Nếu đăng nhập sai thì tăng biến đếm của socket đó thêm 1 , còn nếu đúng reset lại
                        //...
                        if (status != 1)
                        {
                            is_account_locked[i]++;
                        }
                        else
                        {
                            strcpy(userInSocket[i], username);
                            is_socket_logged[i] = 1;
                            is_account_locked[i] = 0;
                        }

                        // Đăng nhập sai quá 3 lần ban acc
                        if (is_account_locked[i] == 3)
                        {
                            FILE *file = fopen("account.txt", "w");
                            if (file != NULL)
                            {
                                struct Account *current = accounts;

                                while (current != NULL)
                                {
                                    if (strcmp(current->username, username) == 0)
                                    {
                                        current->status = 0; // Sửa lại status trong dslk
                                    }
                                    fprintf(file, "%s %s %d\n", current->username, current->password, current->status); // Thực hiện ghi lại file
                                    current = current->next;
                                }
                                fclose(file);
                            }
                            else
                            {
                                printf("Can't open file\n");
                                fclose(file);
                            }
                        }
                    }
                    break;

                case INVITE_REQUEST:
                    printf("Received invite request\n");
                    for (int j = 0; j < max_clients; j++)
                    {
                        int client_sd = client_sockets[j];
                        if (client_sd > 0 && strcmp(userInSocket[j], received_request.data.invite.inviteto) == 0 && is_socket_logged[j] == 1)
                        {
                            // Tìm thấy socket tương ứng với người mời
                            struct Response response; // Declare an instance of the structure
                            printf("%d\n", client_sd);
                            response.messageType = INVITE_REQUEST;
                            response.data.resinvite.resStatus = RECEIVE;
                            strcpy(response.data.resinvite.resInvitefrom, received_request.data.invite.inviteto);
                            strcpy(response.data.resinvite.resInviteto, received_request.data.invite.invitefrom);
                            response.data.resinvite.port = received_request.data.invite.port;
                            printf("%ld\n", send(client_sd, &response, sizeof(response), 0)); // Pass the address of the structure
                            printf("Sent invitation response to %s\n", userInSocket[j]);
                        }
                    }

                    // Thực hiện xử lý yêu cầu mời với received_request.payload.inviteRequest.status, received_request.payload.inviteRequest.invitefrom, received_request.payload.inviteRequest.inviteto
                    break;
                }
            }
        }
    }
    return 0;
}
