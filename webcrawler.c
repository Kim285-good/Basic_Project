
#define _CRT_SECURE_NO_WARNINGS

#include "webcrawler.h"


void crawl_webpage(const char* url) {
    HINTERNET hInternet = NULL, hConnect = NULL;
    char buffer[4096];
    DWORD bytesRead;
    char* end_tag;
    int count = 0;
    hInternet = InternetOpen("MyBrowser", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("WinINet initialization failed: %d\n", GetLastError());
        return;
    }

    hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("Cannot connect to URL: %d\n", GetLastError());
        InternetCloseHandle(hInternet);
        return;
    }

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        char* start_tag = strstr(buffer, "<strong class=\"sa_text_strong\">");
        if (start_tag != NULL) {
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

void crawl_weather(const char* url) {
    HINTERNET hInternet = NULL, hConnect = NULL;
    char buffer[4096];
    DWORD bytesRead;
    char* end_tag;
    int temp = 0, temp1 = 0;
    int count = 0;
    hInternet = InternetOpen("MyBrowser", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("WinINet initialization failed: %d\n", GetLastError());
        return;
    }

    hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("Cannot connect to URL: %d\n", GetLastError());
        InternetCloseHandle(hInternet);
        return;
    }

    char current_temp[10] = { 0 };
    char dust_status[50] = { 0 };
    char water_status[20] = { 0 };
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        char* start_tag = strstr(buffer, "<strong class=\"txt_temp\">");
        if (start_tag != NULL) {
            end_tag = strstr(start_tag, "</strong>");
            if (end_tag != NULL) {
                for (char* ptr = start_tag + strlen("<strong class=\"txt_temp\">"); ptr < end_tag; ++ptr) {
                    if (*ptr >= '0' && *ptr <= '9' && temp != 2) {
                        strncat(current_temp, ptr, 1);
                        temp++;
                    }
                }
            }
        }

        start_tag = strstr(buffer, "<span class=\"txt_weather\">");
        if (start_tag != NULL) {
            end_tag = strstr(start_tag, "</span>");
            if (end_tag != NULL) {
                dust_status[0] = '\0';
                size_t length = end_tag - (start_tag + strlen("<span class=\"txt_weather\">"));
                strncpy(dust_status, start_tag + strlen("<span class=\"txt_weather\">"), length);
                dust_status[length] = '\0';
            }
        }

        start_tag = strstr(buffer, "<span class=\"txt_tit\">");
        while (start_tag != NULL && count < 1) {
            count++;
            start_tag = strstr(start_tag + 1, "<span class=\"txt_tit\">");
        }
        if (start_tag != NULL) {
            end_tag = strstr(start_tag, "</dd>");
            if (end_tag != NULL) {
                for (char* ptr = start_tag + strlen("</span>"); ptr < end_tag; ++ptr) {
                    if (*ptr >= '0' && *ptr <= '9' && temp1 != 2) {
                        strncat(water_status, ptr, 1);
                        temp1++;
                    }
                }
            }
        }
    }

    printf("temp: %sC\n", current_temp);
    printf("now weather: %s\n", dust_status);
    printf("water: %s%%\n", water_status);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}
