#include <iostream>
#include <windows.h>
#include <cctype>

using namespace std;

char* inputArray;
int size_;
double sum = 0;
int K = 0;

CRITICAL_SECTION cs;
CRITICAL_SECTION cs2;
HANDLE SumElementEvent;
bool ready = false;

DWORD WINAPI Work(LPVOID lpParam)
{
	EnterCriticalSection(&cs);
	int sleepTime;
	cout << "Введите временной интервал для отдыха после подготовки элемента: ";
	cin >> sleepTime;
	for (int i = 0; i < size_; ++i) 
	{
		if (isdigit(inputArray[i])) 
		{
			for (int j = i - 1; j >= 0; --j) 
			{
				if (!isdigit(inputArray[j])) 
				{
					swap(inputArray[i], inputArray[j]);
					break;
				}
			}
		}
		Sleep(sleepTime);
	}
	for (int i = 0; i < size_; ++i) {
		if (!isdigit(inputArray[i])) {
			inputArray[i] = ' ';
		}
	}	
	LeaveCriticalSection(&cs);	
	return 0;
}

DWORD WINAPI SumElement(LPVOID lpParam)
{
	EnterCriticalSection(&cs2);
	int count = 0;
	for (int i = 0; i < K && i < size_; i++) {
		if (isdigit(inputArray[i])) {
			sum += inputArray[i] - '0';
			count++;
		}
	}
	if (count > 0) {
		cout << "Среднее арифметическое первых " << count << " чисел: " << static_cast<double>(sum) / count << endl;
	}
	else {
		cout << "Числа не были найдены в первых " << K << " элементах" << endl;
	}	
	LeaveCriticalSection(&cs2);
	SetEvent(SumElementEvent);

	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&cs2);

	
	SumElementEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	cout << "Введите размер массива: ";
	cin >> size_;
	inputArray = new char[size_];

	HANDLE hThread[2];
	DWORD dwThread[2];
	
	hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Work, NULL, CREATE_SUSPENDED, &dwThread[0]);
	EnterCriticalSection(&cs2);
	hThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SumElement, NULL, CREATE_SUSPENDED, &dwThread[1]);

	char a;
	cout << "Введите элементы массива: ";
	for (int i = 0; i < size_; i++)
	{
		cin >> a;
		inputArray[i] = a;
	}

	cout << "Введите число K < размера массива: ";
	cin >> K;
	ResumeThread(hThread[0]);
	ResumeThread(hThread[1]);
	Sleep(1);
	EnterCriticalSection(&cs);
	cout << "Итоговый массив: ";
	for (int i = 0; i < size_; i++)
	{
		cout << inputArray[i] << " ";
	}
	cout << endl;
	LeaveCriticalSection(&cs2);
	LeaveCriticalSection(&cs);
	
	cout << "Итоговый массив ещё раз: ";
	for (int i = 0; i < size_; i++)
	{
		cout << inputArray[i] << " ";
	}
	cout << endl;
	
	WaitForSingleObject(SumElementEvent, INFINITE);
	cout << "результат работы потока SumElement: " << sum;
	DeleteCriticalSection(&cs);
	DeleteCriticalSection(&cs2);
	CloseHandle(SumElementEvent);
	return 0;
}
