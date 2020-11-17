#include <iostream>
#include <pthread.h>
#include <ctime>
#include "cmath"

// Кузнецов Михаил Алексндрович БПИ199
// 13 вариант

pthread_mutex_t mutexIndex; // Мьютекс для работы с последним обработанным индексом i
pthread_mutex_t mutexWrite; // Мьютекс для вывода подходящих индексов
uint *a; // Массив A
uint *b; // Массив B
int i = -1; // Хранит последний обработанный индекс
uint arrays_size; // Хранит размер массивов
uint thread_num = 6; // Число потоков
uint upperBound; // Верхняя граница длины массива

/// Генерирует длину массива
void generateSize() {
    arrays_size = rand() % (upperBound - 999) + 1000;
}

/// Генерирует массив
/// \return Сгенерированный массив
uint *generateArray() {
    uint *array = new uint[arrays_size];
    for (int j = 0; j < arrays_size; ++j) {
        array[j] = (unsigned) rand();
    }
    return array;
}

/// Проверяет является ли переданное число простым
/// \param num Число для проверки на простоту
/// \return Результат проверки
bool isPrime(long num) {
    for (long j = 2; j < sqrt(abs(num)); ++j) {
        if (num % j == 0) return false;
    }
    return true;
}

/// Ищет индексы массивов подходящие под условия
void *func(void *param) {
    int c;
    while (true) {
        pthread_mutex_lock(&mutexIndex);
        if (i + 1 >= arrays_size) {
            pthread_mutex_unlock(&mutexIndex);
            break;
        }
        i++;
        c = i;
        pthread_mutex_unlock(&mutexIndex);

        if (isPrime(a[c] + b[c]) || isPrime(a[c] - b[c])) {
            pthread_mutex_lock(&mutexWrite);
            std::cout << c << std::endl;
            pthread_mutex_unlock(&mutexWrite);
        }
    }
    return nullptr;
}

/// Выводит массив
/// \param arrayName Название массива
/// \param ar Массив
void printArray(std::string arrayName, uint *ar) {
    for (int j = 0; j < arrays_size; ++j) {
        std::cout << arrayName << "[" << j << "] = " << ar[j] << std::endl;
    }
}


int main(int argc, char **argv) {
    srand((unsigned) time(NULL));
    if (argc != 3) { // Проверяем число аргументов
        std::cout << "Wrong number of console arguments" << std::endl;
        return 1;
    }
    if (std::stol(argv[1]) < 1001) { // Проверяем верхнюю границу
        std::cout << "Wrong upper bound" << std::endl;
        return 1;
    }
    if (std::stol(argv[2]) <= 0) { // Проверяем число потоков
        std::cout << "Wrong thread number" << std::endl;
        return 1;
    }
    upperBound = std::stoul(argv[1]); // Получаем верхнюю границу
    thread_num = std::stoul(argv[2]); // Получаем число потоков
    pthread_mutex_init(&mutexIndex, NULL); // Инициализация мьютексов
    pthread_mutex_init(&mutexWrite, NULL);

    generateSize();
    a = generateArray();
    b = generateArray();
    printArray("a", a);
    printArray("b", b);
    std::cout << "Set of indices i:" << std::endl;
    pthread_t pthreads[thread_num];
    for (uint j = 0; j < thread_num; ++j) { // Создаем потоки
        pthread_create(&pthreads[j], nullptr, func, (void *) (&(j)));
    }
    for (uint j = 0; j < thread_num; ++j) { // Синхронизируем потоки
        pthread_join(pthreads[j], nullptr);
    }
    delete[] a;
    delete[] b;
    return 0;
}
