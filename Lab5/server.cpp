#include<iostream>
#include<ctime>
#include<string>
#include<windows.h>
#include <fstream>

using namespace std;

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");
    int size, N, M;
    cout << "Введите размер массива: " << endl;
    cin >> size;

    long* A = new long[size];

    cout << endl << "Введите N и M: " << endl;
    cin >> N >> M;

    HANDLE hWritePipe, hReadPipe;
    HANDLE hWritePipe2, hReadPipe2;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

   if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0))
    {
        cout << "создать первый канал не удалось" << endl;
        return GetLastError();
    }

    if (!CreatePipe(&hReadPipe2, &h, &saAttr, 0))
    {
        cout << "создать второй канал не удалось" << endl;
        return GetLastError();
    }

    DWORD bytesWritten;
    if (!WriteFile(hWritePipe, &size, sizeof(int), &bytesWritten, NULL))
    {
        cout << "запись размера массива не удалась" << endl;
        return GetLastError();
    }

    if (!WriteFile(hWritePipe, &M, sizeof(M), &bytesWritten, NULL))
    {
        cout << "запись M не удалась" << endl;
        return GetLastError();
    }
    if (!WriteFile(hWritePipe, &N, sizeof(N), &bytesWritten, NULL))
    {
        cout << "запись N не удалась" << endl;
        return GetLastError();
    }
    string client_cmd = "";
    //string client_cmd = to_string((int)hWritePipe) + " " + to_string((int)hReadPipe) + " ";
    wstring Wclient_cmd = wstring(client_cmd.begin(), client_cmd.end());
    LPWSTR  client = &Wclient_cmd[0];
    STARTUPINFOA si;
    //string s = "D:\\Прога\\3 сем\\OC\\Лаба 5\\Lab5\\Debug\\Сlient.exe";
    char *s =(char *) "Сlient.exe";
    wchar_t serv[] = L"D:\\Прога\\3 сем\\OC\\Лаба 5\\Lab5\\Debug\\Сlient.exe";
    PROCESS_INFORMATION pi;
    ofstream out ("output.txt");
    out << hReadPipe<< hWritePipe2 ;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    //ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFO); 
    // if (!CreateProcess(NULL, client, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
 /*   if (!CreateProcess(LPCWSTR(L"Writ"), (LPWSTR)(char*)writerInfo.c_str(), NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &si, &writer_pi[i])) {*/
     if (!CreateProcessA((LPCSTR)s, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
    
      //   if (!CreateProcess((LPCSTR)(char*)s.c_str(), (LPSTR)(char*)client_cmd.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
    {
        cout << "Сlient.exe не был запущен.\n";
        return GetLastError();
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD byteRwad;
    if (!ReadFile(hReadPipe2, A, sizeof(long) * size, &byteRwad, NULL))
    {
        cout << "чтение элементов массива не удалось" << endl;
        return GetLastError();
    }

    cout << "\nэлементы массива в диапазоне [N ,M]\n";
    for (int i = 0; i < size; i++)
    {
        cout << A[i] << " ";
    }

    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe2);
    CloseHandle(hWritePipe2);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

}
