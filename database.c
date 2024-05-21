
#define _CRT_SECURE_NO_WARNINGS

#include "database.h"


#define SERVER "localhost"
#define USER "root"
#define PASSWORD "dh0103052"
#define DATABASE_USERS "users"
#define DATABASE_SCHEDULE "schedule"

void error(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

bool login(MYSQL* conn, const char* username, const char* password) {
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
    mysql_free_result(result);

    return num_rows == 1;
}

void registerUser(MYSQL* conn) {
    char username[255];
    char password[255];

    printf("Enter new username: ");
    scanf("%s", username);
    printf("Enter new password: ");
    scanf("%s", password);

    char query[1000];
    sprintf(query, "INSERT INTO %s.users (username, password) VALUES ('%s', '%s')", DATABASE_USERS, username, password);

    if (mysql_query(conn, query)) {
        error(mysql_error(conn));
    }

    printf("Registration successful!\n");
}
