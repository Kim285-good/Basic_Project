#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mysql.h>
#include "database.h"
#include "webcrawler.h"
#include "schedule.h"
#include "chat.h"

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "dh0103052"
#define DATABASE_USERS "users"
#define DATABASE_SCHEDULE "schedule"

void startChattingService();


void mainMenu(MYSQL* conn, const char* username) {
    while (1) {
        printf("1. View News Title\n");
        printf("2. View today's weather\n");
        printf("3. Manage Schedule\n");
        printf("4. Chatting Service\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        int option;
        scanf("%d", &option);

        if (option == 1) {
            int select = 0;
            char str1[5];
            const char* url = "https://news.naver.com/section/";
            printf("1.Politics 2.Economy 3.Society 4.Lifestyle/Culture 5.World 6.IT/Science: ");
            scanf("%d", &select);
            if (select == 1) strcpy(str1, "100");
            else if (select == 2) strcpy(str1, "101");
            else if (select == 3) strcpy(str1, "102");
            else if (select == 4) strcpy(str1, "103");
            else if (select == 5) strcpy(str1, "104");
            else if (select == 6) strcpy(str1, "105");

            char new_url[100];
            strcpy(new_url, url);
            strcat(new_url, str1);
            crawl_webpage(new_url);
        }
        else if (option == 2) {
            const char* url = "https://search.daum.net/search?w=tot&DA=YZR&t__nil_searchbox=btn&q=%EC%84%9C%EC%9A%B8%ED%8A%B9%EB%B3%84%EC%8B%9C+%EB%82%A0%EC%94%A8";
            crawl_weather(url);
        }
        else if (option == 3) {
            manageSchedule(conn, username);
        }
        else if (option == 4) {
            startChattingService();
        }
        else if (option == 5) {
            break; // Logout and return to main menu
        }
        else {
            printf("Invalid choice!\n");
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    MYSQL* conn = mysql_init(NULL);

    if (conn == NULL) {
        printf("mysql_init failed\n");
        exit(1);
    }

    if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE_USERS, 0, NULL, 0)) {
        printf("Error connecting to database: %s\n", mysql_error(conn));
        exit(1);
    }

    while (1) {
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

            if (login(conn, username, password)) {
                printf("Login successful! Welcome %s!\n", username);
                mainMenu(conn, username);
            }
            else {
                printf("Login failed!\n");
            }
        }
        else if (choice == 2) {
            registerUser(conn);
        }
        else {
            printf("Invalid choice!\n");
        }
    }

    mysql_close(conn);
    return 0;
}
