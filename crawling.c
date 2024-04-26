#include <WINDOWS.H> // Windows 헤더 파일
#include <wininet.h> // WinINet API 헤더 파일
#include <stdio.h> // 표준 입출력 헤더 파일
#include <stdlib.h> // 표준 라이브러리 헤더 파일
#include <string.h> // 문자열 처리 헤더 파일

#define BUFFER_SIZE 4096 // 버퍼 크기 상수 정의

// 기사 제목을 가져오는 함수 선언
void GetArticleTitles(const char* url);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    char url[] = "https://news.daum.net/"; // 대상 URL

    // 기사 제목을 가져와서 출력
    GetArticleTitles(url);

    return 0;
}

// 기사 제목을 가져오는 함수 정의
void GetArticleTitles(const char* url) {
    // 인터넷 연결을 열기 위한 핸들 생성
    HINTERNET h_internet = InternetOpenA("NewsCrawler", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (h_internet == NULL) { // 연결 실패 시
        printf("인터넷 연결 열기에 실패했습니다.\n");
        return;
    }

    // URL에 연결하기 위한 핸들 생성
    HINTERNET h_connect = InternetOpenUrlA(h_internet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (h_connect == NULL) { // 연결 실패 시
        printf("URL에 연결하는 데 실패했습니다.\n");
        InternetCloseHandle(h_internet);
        return;
    }

    char buffer[BUFFER_SIZE]; // 데이터를 읽을 버퍼
    DWORD read_byte = 0; // 읽은 바이트 수
    if (InternetReadFile(h_connect, buffer, BUFFER_SIZE - 1, &read_byte)) { // 데이터 읽기 시도
        buffer[read_byte] = '\0'; // 문자열 종료를 위한 널 문자 추가

        // HTML에서 기사 제목을 찾기
        char* p_title_start = strstr(buffer, "<title>");
        if (p_title_start != NULL) { // 시작 태그 찾음
            p_title_start += strlen("<title>"); // 시작 태그 길이만큼 포인터 이동
            for (int i = 0; i < 5; ++i) { // 상위 5개의 기사 제목만 출력
                char* p_title_end = strstr(p_title_start, "</title>"); // 끝 태그 찾기
                if (p_title_end != NULL) { // 끝 태그 찾음
                    *p_title_end = '\0'; // 끝 태그 위치에 널 문자 추가하여 문자열 종료
                    printf("%d: %s\n", i + 1, p_title_start); // 기사 제목 출력
                    p_title_start = strstr(p_title_end + strlen("</title>"), "<title>"); // 다음 기사 제목 시작 위치로 이동
                    if (p_title_start != NULL)
                        p_title_start += strlen("</title>");
                    else
                        break; // 더 이상 기사 제목이 없으면 반복 종료
                }
                else {
                    printf("기사 제목을 찾을 수 없습니다.\n");
                    break;
                }
            }
        }
        else {
            printf("기사 제목을 찾을 수 없습니다.\n");
        }
    }


    InternetCloseHandle(h_connect);
    InternetCloseHandle(h_internet);
}
