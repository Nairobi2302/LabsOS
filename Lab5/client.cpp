#include<iostream>
#include<string>
#include<windows.h>
#include<vector>
#include<conio.h>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    HANDLE hWritePipe, hReadPipe;
    HANDLE hWritePipe2, hReadPipe2;
    int N, M;
    int size;
    cout << "OK";
    cin >> N;
    if (argc == 1) return 0;
    ifstream in("output.txt");
    in >> hReadPipe2 >> hWritePipe;
   /* hWritePipe = (HANDLE)atoi(argv[1]);
    hReadPipe = (HANDLE)atoi(argv[2]);
    hWritePipe2 = (HANDLE)atoi(argv[3]);
    hReadPipe2 = (HANDLE)atoi(argv[4]);*/
    DWORD bytesWritten;

    if (!ReadFile(hReadPipe2, &size, sizeof(size), &bytesWritten, NULL))
    {
        cout << "не удалось прочитать размер массива" << endl;
        return GetLastError();
    }

    long* A = new long[size];
    if (!ReadFile(hReadPipe2, &M, sizeof(M), &bytesWritten, NULL))
    {
        cout << "не удалось прочитать M" << endl;
        return GetLastError();
    }
    if (!ReadFile(hReadPipe2, &N, sizeof(N), &bytesWritten, NULL))
    {
        cout << "не удалось прочитать N" << endl;
        return GetLastError();
    }

    for (int i = 0; i < size; i++)
    {
        A[i] = N + (long)(rand()) % (M - N + 1);
    }

    _cprintf("элементы массива в диапазоне [N ,M] \n");
    for (int i = 0; i < size; i++)
    {
        _cprintf("%u ", A[i]);
    }



    WriteFile(hWritePipe, A, sizeof(long) * size, &bytesWritten, NULL);

    _cprintf("\nЧтобы выйти нажмите клавишу Q ");
    _getch();

    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe2);
    CloseHandle(hReadPipe2);

}
