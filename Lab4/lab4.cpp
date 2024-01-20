#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

int main()
{
    HANDLE hMutex = CreateMutex(NULL, FALSE, LPCWSTR("WriterMutex"));
    HANDLE hSemaphore = CreateSemaphore(NULL, 2, 2, LPCWSTR("ReaderSemaphore"));
    if (hMutex == NULL || hSemaphore == NULL) {
        return GetLastError();
    }
    HANDLE writerEvents[4];
    HANDLE readerEvents[2];
    readerEvents[0] = CreateEvent(NULL, FALSE, FALSE, LPCWSTR("ReaderEventA"));
    readerEvents[1] = CreateEvent(NULL, FALSE, FALSE, LPCWSTR("ReaderEventB"));
    writerEvents[0] = CreateEvent(NULL, FALSE, FALSE, LPCWSTR("BossEventA"));
  writerEvents[1] = CreateEvent(NULL, FALSE, FALSE, LPCWSTR("BossEventB"));
    if (readerEvents[0] == NULL || readerEvents[1] == NULL) {
        cout << 7;
        return GetLastError();
    }
    writerEvents[2] = CreateEvent(NULL, FALSE, FALSE, LPCWSTR("WriterEndEvent"));
    writerEvents[3] = CreateEvent(NULL, FALSE, FALSE, LPCWSTR("ReaderEndEvent"));
    if (writerEvents[0] == NULL || writerEvents[1] == NULL) {
        cout << 8;
        return GetLastError();
    }
    if (writerEvents[2] == NULL || writerEvents[3] == NULL) {
        cout << 9;
        return GetLastError();
    }
    int processNum, messageNum;
    cout << "Enter number of Writer and Reader processes: ";
    cin >> processNum;
    cout << "Enter number of messages: ";
    cin >> messageNum;
    STARTUPINFO si;
    PROCESS_INFORMATION* writer_pi = new PROCESS_INFORMATION[processNum];
    PROCESS_INFORMATION* reader_pi = new PROCESS_INFORMATION[processNum];
    string writerInfo = to_string(messageNum);
    string readerInfo;

    for (int i = 0; i < processNum; ++i) {
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        if (!CreateProcess(LPCWSTR(L"Writer.exe"), (LPWSTR)(char*)writerInfo.c_str(), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &writer_pi[i])) {
            std::cout << "\nwriter process is not created\n";
            return GetLastError();
        }
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        if (!CreateProcess(LPCWSTR(L"Reader.exe"), (LPWSTR)(char*)writerInfo.c_str(), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &reader_pi[i])) {
            std::cout << "\nreader process is not created\n";
            return GetLastError();
        }
    }

    int writerNum = processNum;
    int readerNum = processNum;
    while (writerNum || readerNum) {
        DWORD message = WaitForMultipleObjects(4, writerEvents, FALSE, INFINITE);
        switch (message) {
        case WAIT_OBJECT_0:
            cout << "Recieved message: A\n";
            --writerNum;
            break;
        case WAIT_OBJECT_0 + 1:
            cout << "Recieved message: B\n";
            --readerNum;
            break;
        case WAIT_OBJECT_0 + 2:
            cout << "Writer Process ended\n";
            --writerNum;
            break;
        case WAIT_OBJECT_0 + 3:
            cout << "Reader Process ended\n";
            --readerNum;
            break;

        }
    }


    CloseHandle(hMutex);
    CloseHandle(hSemaphore);
    for (int i = 0; i < 2; ++i) {
        CloseHandle(writerEvents[i]);
        CloseHandle(readerEvents[i]);
    }
    CloseHandle(writerEvents[2]);
    CloseHandle(writerEvents[3]);
    for (int i = 0; i < processNum; ++i) {
        CloseHandle(writer_pi[i].hThread);
        CloseHandle(writer_pi[i].hProcess);
        CloseHandle(reader_pi[i].hThread);
        CloseHandle(reader_pi[i].hProcess);
    }
    return 0;
}
