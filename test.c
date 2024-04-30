#define _CRT_SECURE_NO_WARNINGS

#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <wininet.h>

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "dh0103052"
#define DATABASE_USERS "users"
#define DATABASE_SCHEDULE "schedule"

void error(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void crawl_webpage(const char* url) {
    HINTERNET hInternet = NULL, hConnect = NULL;
    char buffer[4096];
    DWORD bytesRead;
    char* end_tag;
    int count = 0; // 최신 뉴스 5개만 출력하기 위한 카운터
    hInternet = InternetOpen("MyBrowser", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("WinINet 초기화에 실패했습니다: %d\n", GetLastError());
        return;
    }

    hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("URL에 연결할 수 없습니다: %d\n", GetLastError());
        InternetCloseHandle(hInternet);
        return;
    }

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {

        char* start_tag = strstr(buffer, "<strong class=\"sa_text_strong\">");
        if (start_tag != NULL) {
            // "</strong>" 태그 찾기
            end_tag = strstr(start_tag, "</strong>");
            if (end_tag != NULL) {
                start_tag += strlen("<strong class=\"sa_text_strong\">");
                for (char* ptr = start_tag; ptr < end_tag; ++ptr) {
                    if (*ptr == '&') {
                        if (strncmp(ptr, "&quot;", 6) == 0) {
                            ptr += 5;
                        }
                        else if (strncmp(ptr, "&#x27;", 6) == 0) {
                            ptr += 5;
                        }
                    }
                    else {
                        putchar(*ptr);
                    }
                }
                putchar('\n');
                count++;
                if (count >= 5) {
                    break;
                }
            }
        }
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

void manageSchedule(MYSQL* conn, const char* username) {
    printf("Welcome to the schedule management program, %s!\n", username);
    printf("1. Add Schedule\n");
    printf("2. View Schedule\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        char date[20];
        char schedule[255];

        printf("Enter date (YYYY-MM-DD): ");
        scanf("%s", date);
        printf("Enter schedule: ");
        scanf(" %[^\n]", schedule);

        char query[1000];
        sprintf(query, "INSERT INTO schedule.schedules (username, date, schedule) VALUES ('%s', '%s', '%s')", username, date, schedule);

        if (mysql_query(conn, query)) {
            error(mysql_error(conn));
        }

        printf("Schedule added successfully!\n");
    }
    else if (choice == 2) {
        char date[20];

        printf("Enter date to view schedule (YYYY-MM-DD): ");
        scanf("%s", date);

        char query[1000];
        sprintf(query, "SELECT schedule FROM schedule.schedules WHERE username='%s' AND date='%s'", username, date);

        if (mysql_query(conn, query)) {
            error(mysql_error(conn));
        }

        MYSQL_RES* result = mysql_store_result(conn);

        if (result == NULL) {
            error(mysql_error(conn));
        }

        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;

        printf("Schedule for %s:\n", date);
        while ((row = mysql_fetch_row(result))) {
            printf("%s\n", row[0]);
        }

        mysql_free_result(result);
    }
    else {
        printf("Invalid choice!\n");
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    MYSQL* conn = mysql_init(NULL);

    if (conn == NULL) {
        error("mysql_init failed");
    }

    if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE_USERS, 0, NULL, 0)) {
        error(mysql_error(conn));
    }

    printf("1. Login\n");
    printf("2. Register\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        char username[255];
        char password[255];

        printf("Enter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        char query[1000];
        sprintf(query, "SELECT * FROM %s.users WHERE username='%s' AND password='%s'", DATABASE_USERS, username, password);

        if (mysql_query(conn, query)) {
            error(mysql_error(conn));
        }

        MYSQL_RES* result = mysql_store_result(conn);

        if (result == NULL) {
            error(mysql_error(conn));
        }

        int num_rows = mysql_num_rows(result);

        if (num_rows == 1) {
            printf("Login successful! Welcome %s!\n", username);
            printf("1. View News\n");
            printf("2. Manage Schedule\n");
            printf("Enter your choice: ");
            int option;
            scanf("%d", &option);

            if (option == 1) {
                const char* url = "https://news.naver.com/section/104"; // 세계 뉴스 섹션의 URL
                crawl_webpage(url);
            }
            else if (option == 2) {
                manageSchedule(conn, username);
            }
            else {
                printf("Invalid choice!\n");
            }
        }
        else {
            printf("Login failed!\n");
        }

        mysql_free_result(result);
    }
    else if (choice == 2) {
        char username[255];
        char password[255];

        printf("Enter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        if (mysql_select_db(conn, DATABASE_USERS)) {
            error(mysql_error(conn));
        }

        char query[1000];
        sprintf(query, "INSERT INTO %s.users (username, password) VALUES ('%s', '%s')", DATABASE_USERS, username, password);

        if (mysql_query(conn, query)) {
            error(mysql_error(conn));
        }

        printf("Registration successful!\n");
    }
    else {
        printf("Invalid choice!\n");
    }

    mysql_close(conn);
    return 0;
}
