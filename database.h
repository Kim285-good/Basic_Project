#define _CRT_SECURE_NO_WARNINGS

#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void error(const char* msg);
bool login(MYSQL* conn, const char* username, const char* password);
void registerUser(MYSQL* conn);

#endif // DATABASE_H
