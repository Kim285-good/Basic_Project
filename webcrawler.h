#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H

#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <string.h>


void crawl_webpage(const char* url);
void crawl_weather(const char* url);

#endif // WEBCRAWLER_H
