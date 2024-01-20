#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

int main(int argc, char* argv[])
{
    HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, LPCWSTR("WriterMutex"));
    if (hMutex == NULL) {
        cout << "M";
        return GetLastError();
    }
    HANDLE writerEvents[3];
    writerEvents[0] = OpenEvent(EVENT_MODIFY_STATE, FALSE, LPCWSTR("ReaderEventA"));
    if (writerEvents[0] == NULL ) {
        cout << "1";
        return GetLastError();
    }
    writerEvents[1] = OpenEvent(EVENT_MODIFY_STATE, FALSE, LPCWSTR("ReaderEventB"));
    if (writerEvents[1] == NULL) {
        cout << "2";
        return GetLastError();
    }
    writerEvents[2] = OpenEvent(EVENT_MODIFY_STATE, FALSE, LPCWSTR("WriterEndEvent"));
    if (writerEvents[2] == NULL) {
        cout << "3";
        return GetLastError();
    }
    int messageNum = atoi(argv[0]);
    std::cout << "Count: ";
    std::cout << messageNum;
    WaitForSingleObject(hMutex, INFINITE);
    cout << "M";
    char message;

    for (int i = 0; i < messageNum; ++i) {
        std::cout << "Enter message(A, B): ";
        std::cin >> message;
        switch (message) {
        case 'A':
            SetEvent(writerEvents[0]);
            break;
        case 'B':
            SetEvent(writerEvents[1]);
            break;
        }
    }
    SetEvent(writerEvents[2]);
    ReleaseMutex(hMutex);
    std::string s;
    getline(std::cin, s);
    getline(std::cin, s);
   
    CloseHandle(hMutex);
    for (int i = 0; i < 3; ++i) {
        CloseHandle(writerEvents[i]);
    }
    return 0;
}
