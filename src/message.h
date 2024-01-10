#ifndef MESSAGE_H
#define MESSAGE_H

#define MAX_SIZE 1024

typedef enum {
    LOGIN_REQUEST,
    INVITE_REQUEST,
    GAME_REQUEST
} MessageType;

typedef enum {
    OK,
    FAILURE
} ResponseLoginType;

typedef enum {
    ACCEPT,
    REFUSE,
    RECEIVE
} StatusInvite;

typedef enum {
    START,
    END,
    FIND
} StatusGame;

struct Request {
    MessageType messageType;

    union {
        struct {
            char username[MAX_SIZE];
            char password[MAX_SIZE];
            int port ;
            int cli_socket ;
            int serv_socket;
        } login;

        struct {
            MessageType messageType;  
            StatusInvite status;
            int port;
            char invitefrom[MAX_SIZE];
            char inviteto[MAX_SIZE];
        } invite; 
    } data;
};


struct Response {
    MessageType messageType;
    
    union {
        struct {
            ResponseLoginType response;
            char user[MAX_SIZE];
            char result[MAX_SIZE];
            int cli_socket;
            int serv_socket;
        } reslogin;

        struct {
            MessageType resMessageType;  
            StatusInvite resStatus;
            char resInvitefrom[MAX_SIZE];
            int port ;
            char resInviteto[MAX_SIZE];
        } resinvite;
        

    } data;
};





#endif /* MESSAGE_H */
