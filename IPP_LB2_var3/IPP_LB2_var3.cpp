#include <iostream>
#include <windows.h>
#include <thread>
#include <mutex>
#include <random>

#define BUFFER_SIZE 1000

std::mutex mtx;  // М'ютекс для синхронізації доступу до буфера
int buffer[BUFFER_SIZE];  // Буфер для зберігання даних

// Функція для запису даних в буфер
DWORD WINAPI writeToBuffer(LPVOID lpParam)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(33, 150);

    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        // Захоплення м'ютексу перед записом в буфер
        mtx.lock();

        // Запис даних в буфер
        buffer[i] = dist(rng);

        // Звільнення м'ютексу після запису
        mtx.unlock();
    }
    return 0;
}

// Функція для зчитування даних з буфера і виводу на екран
DWORD WINAPI readFromBuffer(LPVOID lpParam)
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        // Захоплення м'ютексу перед зчитуванням з буфера
        mtx.lock();

        // Зчитування даних з буфера і вивід їх числового значення на екран
        std::cout << buffer[i] << " ";

        // Звільнення м'ютексу після зчитування
        mtx.unlock();
    }
    return 0;
}

int main()
{
    char* buff = new char[BUFFER_SIZE];
    std::cout << "synchOFF" << std::endl;
    HANDLE threads[2];
    threads[0] = CreateThread(NULL, 0, writeToBuffer, buff, 0, 0);
    threads[1] = CreateThread(NULL, 0, readFromBuffer, buff, 0, 0);
    WaitForMultipleObjects(2, threads, FALSE, INFINITE); //true - очікує завершення двох (усіх), false - хоча б одного
    delete[] buff;
    return 0;
}
