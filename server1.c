#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#include <process.h>
#include <string.h>


#ifndef MAXBYTE
#define MAXBYTE 1024
#endif

#define CLIENT_COUNT 50

#define MAX_CLIENT_COUNT 100 // 예시로 사용할 상수 정의





int server_init();
int server_close();
unsigned int WINAPI do_chat_service(void* param);
unsigned int WINAPI recv_and_forward(void* param);
int add_client(int index);
int read_client(int index);
void remove_client(int index);
int notify_client(char* message);
char* get_client_ip(int index);

typedef struct sock_info
{
    SOCKET s;
    HANDLE ev;
    char nick[50];
    char ipaddr[50];
} SOCK_INFO;

SOCK_INFO sock_array[CLIENT_COUNT + 1];
int total_socket_count = 0;
int port_number = 9999;

int main(int argc, char* argv[])
{
    unsigned int tid;
    HANDLE mainthread;
    char message[MAXBYTE] = "";

    printf("\nUsage: mcodes_server [port_number]\n");
    printf("       e.g., mcodes_server.exe 9999\n");
    printf("       e.g., mcodes_server.exe\n\n");

    if (argv[1] != NULL)
        port_number = atoi(argv[1]);

    mainthread = (HANDLE)_beginthreadex(NULL, 0, do_chat_service, (void*)0, 0, &tid);
    if (mainthread)
    {
        while (1)
        {
            gets_s(message, MAXBYTE);
            if (strcmp(message, "/x") == 0)
                break;

            notify_client(message);
        }
        server_close();
        WSACleanup();
        CloseHandle(mainthread);
    }

    return 0;
}

int server_init()
{
    WSADATA wsadata;
    SOCKET s;
    SOCKADDR_IN server_address;

    memset(&sock_array, 0, sizeof(sock_array));
    total_socket_count = 0;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        puts("WSAStartup error.");
        return -1;
    }

    if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        puts("socket error.");
        return -1;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port_number);

    if (bind(s, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        puts("bind error");
        return -2;
    }

    if (listen(s, SOMAXCONN) < 0)
    {
        puts("listen error");
        return -3;
    }

    return s;
}

int server_close()
{
    for (int i = 1; i < total_socket_count; i++)
    {
        closesocket(sock_array[i].s);
        WSACloseEvent(sock_array[i].ev);
    }

    return 0;
}

unsigned int WINAPI do_chat_service(void* param)
{
    SOCKET server_socket;
    WSANETWORKEVENTS ev;
    int index;
    int client_count = 100; // 예: 최대 클라이언트 수를 100으로 설정
    WSAEVENT handle_array[client_count + 1];

    server_socket = server_init();
    if (server_socket < 0)
    {
        printf("Initialization error\n");
        exit(0);
    }
    else
    {
        printf("\n >> Server initialized (port number: %d)\n", port_number);

        HANDLE event = WSACreateEvent();
        sock_array[total_socket_count].ev = event;
        sock_array[total_socket_count].s = server_socket;
        strcpy_s(sock_array[total_socket_count].nick, sizeof(sock_array[total_socket_count].nick), "svr");
        strcpy_s(sock_array[total_socket_count].ipaddr, sizeof(sock_array[total_socket_count].ipaddr), "0.0.0.0");

        WSAEventSelect(server_socket, event, FD_ACCEPT);
        total_socket_count++;

        while (1)
        {
            memset(&handle_array, 0, sizeof(handle_array));
            for (int i = 0; i < total_socket_count; i++)
                handle_array[i] = sock_array[i].ev;

            index = WSAWaitForMultipleEvents(total_socket_count, handle_array, 0, INFINITE, 0);
            if ((index != WSA_WAIT_FAILED) && (index != WSA_WAIT_TIMEOUT))
            {
                WSAEnumNetworkEvents(sock_array[index].s, sock_array[index].ev, &ev);
                if (ev.lNetworkEvents == FD_ACCEPT)
                    add_client(index);
                else if (ev.lNetworkEvents == FD_READ)
                    read_client(index);
                else if (ev.lNetworkEvents == FD_CLOSE)
                    remove_client(index);
            }
        }
        closesocket(server_socket);
    }

    WSACleanup();
    _endthreadex(0);

    return 0;
}

int add_client(int index)
{
    SOCKADDR_IN addr;
    int len = 0;
    SOCKET accept_sock;

    if (total_socket_count == FD_SETSIZE)
        return 1;
    else
    {

        len = sizeof(addr);
        memset(&addr, 0, sizeof(addr));
        accept_sock = accept(sock_array[0].s, (SOCKADDR*)&addr, &len);

        HANDLE event = WSACreateEvent();
        sock_array[total_socket_count].ev = event;
        sock_array[total_socket_count].s = accept_sock;
        strcpy_s(sock_array[total_socket_count].ipaddr, sizeof(sock_array[total_socket_count].ipaddr), inet_ntoa(addr.sin_addr));

        WSAEventSelect(accept_sock, event, FD_READ | FD_CLOSE);

        total_socket_count++;
        printf(" >> New client connected (IP: %s)\n", inet_ntoa(addr.sin_addr));

        char msg[256];
        strcpy_s(msg, sizeof(msg), " >> New client connected (IP: ");
        strcat_s(msg, sizeof(msg), inet_ntoa(addr.sin_addr));
        strcat_s(msg, sizeof(msg), ")\n");
        notify_client(msg);
    }

    return 0;
}

int read_client(int index)
{
    unsigned int tid;
    HANDLE mainthread = (HANDLE)_beginthreadex(NULL, 0, recv_and_forward, (void*)index, 0, &tid);
    WaitForSingleObject(mainthread, INFINITE);

    CloseHandle(mainthread);

    return 0;
}

unsigned int WINAPI recv_and_forward(void* param)
{
    int index = (int)param;
    char message[MAXBYTE], share_message[MAXBYTE];
    SOCKADDR_IN client_address;
    int recv_len = 0, addr_len = 0;
    char* token1 = NULL;
    char* next_token = NULL;

    memset(&client_address, 0, sizeof(client_address));

    if ((recv_len = recv(sock_array[index].s, message, MAXBYTE, 0)) > 0)
    {
        addr_len = sizeof(client_address);
        getpeername(sock_array[index].s, (SOCKADDR*)&client_address, &addr_len);
        strcpy_s(share_message, sizeof(share_message), message);

        if (strlen(sock_array[index].nick) <= 0)
        {
            token1 = strtok_s(message, "]", &next_token);
            strcpy_s(sock_array[index].nick, sizeof(sock_array[index].nick), token1 + 1);
        }
        for (int i = 1; i < total_socket_count; i++)
            send(sock_array[i].s, share_message, MAXBYTE, 0);
    }

    _endthreadex(0);
    return 0;
}

void remove_client(int index)
{
    char remove_ip[256];
    char message[MAXBYTE];

    strcpy_s(remove_ip, sizeof(remove_ip), get_client_ip(index));
    printf(" >> Client disconnected (Index: %d, IP: %s, Nickname: %s)\n", index, remove_ip, sock_array[index].nick);
    sprintf_s(message, sizeof(message), " >> Client disconnected (IP: %s, Nickname: %s)\n", remove_ip, sock_array[index].nick);

    closesocket(sock_array[index].s);
    WSACloseEvent(sock_array[index].ev);

    total_socket_count--;
    sock_array[index].s = sock_array[total_socket_count].s;
    sock_array[index].ev = sock_array[total_socket_count].ev;
    strcpy_s(sock_array[index].ipaddr, sizeof(sock_array[index].ipaddr), sock_array[total_socket_count].ipaddr);
    strcpy_s(sock_array[index].nick, sizeof(sock_array[index].nick), sock_array[total_socket_count].nick);

    notify_client(message);
}

char* get_client_ip(int index)
{
    static char ipaddress[256];
    int addr_len;
    struct sockaddr_in sock;

    addr_len = sizeof(sock);
    if (getpeername(sock_array[index].s, (struct sockaddr*)&sock, &addr_len) < 0)
        return NULL;

    strcpy_s(ipaddress, sizeof(ipaddress), inet_ntoa(sock.sin_addr));
    return ipaddress;
}

int notify_client(char* message)
{
    for (int i = 1; i < total_socket_count; i++)
        send(sock_array[i].s, message, strlen(message) + 1, 0);

    return 0;
}
