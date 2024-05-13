<<<<<<< HEAD
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
    int count = 0;// ´º½º 5°³Ã£±â
    hInternet = InternetOpen("MyBrowser", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("WinINet ÃÊ±âÈ­¿¡ ½ÇÆÐÇß½À´Ï´Ù: %d\n", GetLastError());
        return;
    }

    hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("URL¿¡ ¿¬°áÇÒ ¼ö ¾ø½À´Ï´Ù: %d\n", GetLastError());
        InternetCloseHandle(hInternet);
        return;
    }

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {

        char* start_tag = strstr(buffer, "<strong class=\"sa_text_strong\">");
        //printf("%s", start_tag);
        if (start_tag != NULL) {
            // "</strong>" ÅÂ±× Ã£±â
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

    ////100 Á¤Ä¡, 101 °æÁ¦,102 »çÈ¸, 103 »ýÈ°/¹®È­, 104 ¼¼°è, 105 IT/°úÇÐ
    crawl_webpage(new_url);
    return 0;
}
*/
=======
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
    int count = 0;// ë‰´ìŠ¤ 5ê°œì°¾ê¸°
    hInternet = InternetOpen("MyBrowser", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("WinINet ì´ˆê¸°í™”ì— ì‹¤íŒ¨í–ˆìŠµë‹ˆë‹¤: %d\n", GetLastError());
        return;
    }

    hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("URLì— ì—°ê²°í•  ìˆ˜ ì—†ìŠµë‹ˆë‹¤: %d\n", GetLastError());
        InternetCloseHandle(hInternet);
        return;
    }

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {

        char* start_tag = strstr(buffer, "<strong class=\"sa_text_strong\">");
        //printf("%s", start_tag);
        if (start_tag != NULL) {
            // "</strong>" íƒœê·¸ ì°¾ê¸°
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
        strcpy(str1,"104");
    }
    else if (select == 6) {
        strcpy(str1,"105");
    }
     char* new_url[100];
     strcpy(new_url, url);
     strcat(new_url, str1);

     ////100 ì •ì¹˜, 101 ê²½ì œ,102 ì‚¬íšŒ, 103 ìƒí™œ/ë¬¸í™”, 104 ì„¸ê³„, 105 IT/ê³¼í•™
    crawl_webpage(new_url);
    return 0;
}
>>>>>>> 3c68c8352474548f60c05eecd64dc3d8af3e0ab6
