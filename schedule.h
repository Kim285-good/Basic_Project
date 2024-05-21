#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <mysql.h>
#include <stdio.h>
#include <string.h>

void manageSchedule(MYSQL* conn, const char* username);

#endif // SCHEDULE_H
