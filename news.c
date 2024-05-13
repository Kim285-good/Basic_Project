/*
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


void crawl_webpage(const char* url) {
    HINTERNET hInternet = NULL, hConnect = NULL;
    char buffer[4096];
    DWORD bytesRead;
    char* end_tag;
    int count = 0;// 뉴스 5개찾기
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
        //printf("%s", start_tag);
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

int main() {
    SetConsoleOutputCP(CP_UTF8);
    int select = 0;
    char str1[5];
    char* url = "https://news.naver.com/section/";
    printf("1.Politics 2.Economy, 3.Society, 4.Lifestyle/Culture, 5.World 6.IT/Science : ");
    scanf("%d", &select);
    if (select == 1) {
        strcpy(str1, "100");
    }
    else if (select == 2) {
        strcpy(str1, "101");
    }
    else if (select == 3) {
        strcpy(str1, "102");
    }
    else if (select == 4) {
        strcpy(str1, "103");
    }
    else if (select == 5) {
        strcpy(str1, "104");
    }
    else if (select == 6) {
        strcpy(str1, "105");
    }
    char* new_url[100];
    strcpy(new_url, url);
    strcat(new_url, str1);

    ////100 정치, 101 경제,102 사회, 103 생활/문화, 104 세계, 105 IT/과학
    crawl_webpage(new_url);
    return 0;
}
*/