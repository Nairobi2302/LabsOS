#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

int main() {
    int size, k;
    setlocale(0, "Russian");
    string commandLine ="Child.exe ",el;
    printf("Введите размер массива: ");
    cin >> el;
    commandLine += el;
    size = atoi(el.c_str());
    cout << "Введите значение K: ";
    cin >> el;
    commandLine += " ";
    commandLine += el;
    cout << "Введите элементы массива : \n";
    for (int i = 0; i < size; i++) {
        cin >> el;
        commandLine += " ";
        commandLine+= el;
    }
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFOA));
    si.cb = sizeof(STARTUPINFOA);
    if (!CreateProcessA(NULL, (LPSTR)commandLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        cout << "Ошибка при создании дочернего процесса. Код ошибки: %d\n", GetLastError();
        return 0;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
