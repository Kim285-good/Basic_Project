
#define _CRT_SECURE_NO_WARNINGS

#include "schedule.h"


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


        printf("Schedule added successfully!\n");
    }
    else if (choice == 2) {
        char date[20];

        printf("Enter date to view schedule (YYYY-MM-DD): ");
        scanf("%s", date);

        char query[1000];
        sprintf(query, "SELECT schedule FROM schedule.schedules WHERE username='%s' AND date='%s'", username, date);



        MYSQL_RES* result = mysql_store_result(conn);


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
