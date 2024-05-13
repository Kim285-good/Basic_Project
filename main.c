#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.c"

int main(int argc, char* argv[]) {
    unsigned int tid;
    HANDLE mainthread;
    char message[MAXBYTE] = "";

    printf("\nUsage: mcodes_server [port_number]\n");
    printf("       e.g., mcodes_server.exe 9999\n");
    printf("       e.g., mcodes_server.exe\n\n");

    if (argv[1] != NULL)
        port_number = atoi(argv[1]);

    mainthread = (HANDLE)_beginthreadex(NULL, 0, do_chat_service, (void*)0, 0, &tid);
    if (mainthread) {
        while (1) {
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
