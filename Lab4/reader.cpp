#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

int main(int argc, char* argv[])
{
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, LPCWSTR("ReaderSemaphore"));
    if (hSemaphore == NULL) {
        std::cout << "eS";
        return GetLastError();
    }
    HANDLE readerEvents[5];
    readerEvents[0] = OpenEvent(SYNCHRONIZE, FALSE, LPCWSTR("ReaderEventA"));
    if (readerEvents[0] == NULL)
    {
        std::cout << "1";
        return GetLastError();
    }
    readerEvents[1] = OpenEvent(SYNCHRONIZE, FALSE, LPCWSTR("ReaderEventB"));

    if (readerEvents[1] == NULL) 
    {
        std::cout << "2";
        return GetLastError();
    }

    readerEvents[2] = OpenEvent(EVENT_MODIFY_STATE, FALSE, LPCWSTR("BossEventA"));

    if (readerEvents[2] == NULL)
    {
        std::cout << "3";
        return GetLastError();
    }

    readerEvents[3] = OpenEvent(EVENT_MODIFY_STATE, FALSE, LPCWSTR("BossEventB"));

    if (readerEvents[3] == NULL)
    {
        std::cout << "4";
        return GetLastError();
    }
    readerEvents[4] = OpenEvent(EVENT_MODIFY_STATE, FALSE, LPCWSTR("ReaderEndEvent"));
    if (readerEvents[4] == NULL)
    {
        std::cout << "5";
        return GetLastError();
    }
    if (readerEvents[2] == NULL || hSemaphore == NULL) {
        return GetLastError();
    }
    int messageNum = atoi(argv[0]);
    WaitForSingleObject(hSemaphore, INFINITE);
    std::cout << "S";
    while (messageNum) {
        DWORD message = WaitForMultipleObjects(2, readerEvents, FALSE, INFINITE);
        switch (message) {
        case 0:
            std::cout << "Recieved message: A\n";
            SetEvent(readerEvents[2]);
            break;
        case 1:
            std::cout << "Recieved message: B\n";
            SetEvent(readerEvents[3]);
            break;
        }
        --messageNum;
    }
    ReleaseSemaphore(hSemaphore, 1, NULL);
    std::string s;
    getline(std::cin, s);
    SetEvent(readerEvents[4]);
    CloseHandle(hSemaphore);
    for (int i = 0; i < 5; ++i) {
        CloseHandle(readerEvents[i]);
    }
    return 0;
}
