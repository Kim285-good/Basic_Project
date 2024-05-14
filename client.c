#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#include <string.h>

#ifndef SERVER_MAXBYTE
#define SERVER_MAXBYTE 1024
#endif

// Default server IP address and port number
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 9999

// 클라이언트 초기화 함수 선언
SOCKET client_init(char* ip, int port);

// 채팅 서비스 함수 선언
unsigned int WINAPI do_chat_service(void* params);

int main(int argc, char* argv[])
{
    char ip_addr[256] = DEFAULT_IP; // Default server IP address
    int port_number = DEFAULT_PORT; // Default port number
    char nickname[50] = ""; // Buffer for nickname
    unsigned int tid;
    SOCKET sock;
    char input[SERVER_MAXBYTE] = "";
    char message[SERVER_MAXBYTE] = "";
    char* pexit = NULL;
    HANDLE mainthread;

    // 사용자 닉네임 입력
    if (argc < 2)
    {
        printf("\nUsage: client [nickname]\n\n");
        printf("       e.g., client.exe mainCodes\n");
        exit(0);
    }

    if (argv[1] != NULL)
    {
        strcpy_s(nickname, sizeof(nickname), argv[1]);
    }

    // 클라이언트 초기화
    sock = client_init(ip_addr, port_number);
    if (sock == INVALID_SOCKET)
    {
        printf("Error initializing the client\n");
        exit(0);
    }

    // 채팅 서비스 쓰레드 생성
    mainthread = (HANDLE)_beginthreadex(NULL, 0, do_chat_service, (void*)sock, 0, &tid);
    if (mainthread)
    {
        // 사용자 입력 받기 및 메시지 전송
        while (1)
        {

            fgets(input, SERVER_MAXBYTE, stdin);
            if (strcmp(input, "/x\n") == 0) // 입력이 "/x"일 때 종료
                break;

            sprintf(message, "[%s] %s", nickname, input);
            send(sock, message, strlen(message), 0);
        }

        // 클라이언트 종료 처리
        closesocket(sock);
        CloseHandle(mainthread);
    }

    return 0;
}

// 클라이언트 초기화 함수 정의
SOCKET client_init(char* ip, int port)
{
    SOCKET server_socket;
    WSADATA wsadata;
    SOCKADDR_IN server_address = { 0 };

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        printf("Error initializing Winsock\n");
        return INVALID_SOCKET;
    }

    // 소켓 생성
    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Socket error\n");
        WSACleanup(); // 소켓 생성에 실패한 경우 Winsock을 정리해야 합니다.
        return INVALID_SOCKET;
    }

    // 서버 주소 설정
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);

    // 서버에 연결
    if (connect(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        printf("Error connecting to the server\n");
        closesocket(server_socket); // 서버 연결에 실패한 경우 소켓을 닫아야 합니다.
        WSACleanup(); // Winsock을 정리해야 합니다.
        return INVALID_SOCKET;
    }

    return server_socket;
}

// 채팅 서비스 함수 정의
unsigned int WINAPI do_chat_service(void* params)
{
    SOCKET s = (SOCKET)params;
    char recv_message[SERVER_MAXBYTE];
    WSANETWORKEVENTS ev;
    HANDLE event = WSACreateEvent();

    // 소켓에 이벤트 등록 (수신 및 종료)
    WSAEventSelect(s, event, FD_READ | FD_CLOSE);

    while (1)
    {
        // 이벤트 대기
        if (WSAWaitForMultipleEvents(1, &event, FALSE, WSA_INFINITE, FALSE) != WSA_WAIT_EVENT_0)
            continue;

        WSAEnumNetworkEvents(s, event, &ev);
        if (ev.lNetworkEvents & FD_READ)
        {
            // 메시지 수신
            int len = recv(s, recv_message, SERVER_MAXBYTE - 1, 0);
            if (len > 0)
            {
                recv_message[len] = '\0'; // 수신된 데이터를 문자열로 만들기 위해 NULL 문자 추가
                printf("%s\n", recv_message);
            }
        }
        else if (ev.lNetworkEvents & FD_CLOSE)
        {
            // 서버 종료 메시지 출력 및 소켓 종료
            printf("The server service has been terminated (exit: \"/x\")\n");
            break;
        }
    }

    // 이벤트 객체 제거
    WSACloseEvent(event);

    return 0;
}
