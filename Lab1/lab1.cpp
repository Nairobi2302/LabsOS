#include <windows.h>
#include <iostream>
#include <ctime>

using namespace std;

DWORD WINAPI worker(LPVOID lpParam)
{
    int* A = (int*)lpParam;
    int n = A[0];
    int s = 0;
    for (int i = 1; i <= n; i++)
    {
        s += A[i];
        Sleep(12);
    }
    double average = double(s) / n;
    cout << "Среднее арифметическое массива: " << average << endl;
    return average;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int n = 0;
    cout << "Введите количество элементов массива: ";
    cin >> n;
    int* A = new int[n + 1];
    A[0] = n;
    srand(time(NULL));
    for (int i = 1; i <= n; i++)
    {
        A[i] = rand() % 10;
    }
    cout << "Исходный массив: ";
    for (int i = 1; i <= n; i++)
    {
        cout << A[i] << " ";
    }
    cout << endl;
    HANDLE	hWorker;
    DWORD	IDWorker;
    int startTime, stopTime;
    cout << "Введите начальное время для остановки потока: " << endl;
    cin >> startTime;
    cout << "Введите конечное время для остановки потока: " << endl;
    cin >> stopTime;
    hWorker = CreateThread(NULL, 0, worker, A, 0, &IDWorker);
    if (hWorker == NULL)
        return GetLastError();
    Sleep(startTime * 1000);
    SuspendThread(hWorker);
    Sleep((stopTime - startTime) * 1000);
    ResumeThread(hWorker);
    WaitForSingleObject(hWorker, INFINITE);
    CloseHandle(hWorker);
    return 0;
}
