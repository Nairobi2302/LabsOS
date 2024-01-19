#include <iostream>
#include <windows.h>

using namespace std;

int main(int argc, char* argv[]) {
    setlocale(0, "Russian");
    if (argc < 3) {
        cout << "Недостаточно аргументов командной строки.\n";
        return 1;
    }
    int size = atoi(argv[1]);
    int k = atoi(argv[2]);
    float* array = (float*)malloc(size * sizeof(float));
    for (int i = 0; i < size; i++) {
        array[i]= atoi(argv[i+3]);
    }
    float sum = 0;
    for (int i = 0; i < k; i++) {
        sum += array[i];
    }
    cout << "Сумма элементов до позиции ",k, ": ", sum;
    free(array);
    return 0;
}
