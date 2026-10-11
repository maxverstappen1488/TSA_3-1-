#include <random>
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
using namespace std;

template<typename T>
using massive = vector<T>;

/**
 * @brief Заполняет массив случайными числами в диапазоне [-100, 100]
 * @param len - размер массива
 * @return massive<int> - заполненный случайными числами массив
 */
massive<int> random_massive(const int len);

/**
 * @brief Сортировка массива пузырьковым методом
 * @param arr - ссылка на массив для сортировки
 * @return unsigned int - количество операций сравнения
 */
unsigned int bubble_algorithm(massive<int>& arr);

/**
 * @brief Сортировка вставками с бинарным поиском места вставки
 *        Делит отсортированную часть пополам для быстрого нахождения позиции
 * @param arr - ссылка на массив для сортировки
 * @return unsigned int - количество операций сравнения
 */
unsigned int insert_algorithm(massive<int>& arr);

/**
 * @brief Быстрая сортировка методом Хоара
 *        Делит массив на три части: < pivot, == pivot, > pivot, 
 *        рекурсивно сортирует крайние и собирает их вместе.
 * @param arr - ссылка на массив для сортировки
 * @return unsigned int - количество операций сравнения
 */
unsigned int Hoar_algorithm(massive<int>& arr);

/**
 * @brief Точка входа в программу. Сравнивает три алгоритма сортировки
 *        по среднему количеству операций сравнения на случайных массивах.
 * @return int - код завершения программы (0 - успех)
 */
int main() {
    const size_t trials = 50, arrLen = 10000;
    unsigned long long int total_bubble = 0, total_insert = 0, total_hoare = 0;
    
    for (size_t i = 0; i < trials; i++) {
        massive<int> data = random_massive(arrLen);
        
        massive<int> data_copy = data;
        total_bubble += bubble_algorithm(data_copy);
        
        data_copy = data;
        total_insert += insert_algorithm(data_copy);
        
        data_copy = data;
        total_hoare += Hoar_algorithm(data_copy);
    }

    cout << setw(25) << left << "Algorithm"
         << setw(25) << right << "Average comparison count" << "\n";
    cout << string(50, '-') << "\n";

    cout << setw(25) << left << "Bubble algorithm"
         << setw(25) << right << total_bubble / trials << "\n";

    cout << setw(25) << left << "Insert algorithm (Binary)"
         << setw(25) << right << total_insert / trials << "\n";

    cout << setw(25) << left << "Hoare algorithm (Piles)"
         << setw(25) << right << total_hoare / trials << "\n";

    return 0;
}

/**
 * @brief Сортировка массива пузырьковым методом
 */
unsigned int bubble_algorithm(massive<int>& arr) {
    if (arr.size() <= 1) return 0;
    unsigned int ops = 0;
    bool sorted;
    for (size_t pass = 0; pass < arr.size() - 1; pass++) {
        sorted = true;
        for (size_t idx = 1; idx < arr.size() - pass; idx++) {
            ops++;
            if (arr[idx - 1] > arr[idx]) {
                swap(arr[idx - 1], arr[idx]);
                sorted = false;
            }
        }
        if (sorted) break;
    }
    return ops;
}

/**
 * @brief Сортировка вставками
 */
unsigned int insert_algorithm(massive<int>& arr) {
    if (arr.size() <= 1) return 0;
    unsigned int ops = 0;
    
    for (size_t i = 1; i < arr.size(); i++) {
        int val = arr[i];
        
        // Бинарный поиск места для вставки в отсортированной части [0, i)
        size_t start = 0, end = i;
        while (start < end) {
            size_t mid = start + (end - start) / 2;
            ops++; // Считаем сравнение
            if (arr[mid] < val) {
                start = mid + 1; // Ищем в правой половине
            } else {
                end = mid;       // Ищем в левой половине (включая mid)
            }
        }
        
        // Сдвиг элементов вправо, чтобы освободить место
        for (size_t j = i; j > start; j--) {
            arr[j] = arr[j - 1];
        }
        // Вставка элемента
        arr[start] = val;
    }
    return ops;
}

/**
 * @brief Быстрая сортировка методом Хоара
 */
unsigned int Hoar_algorithm(massive<int>& arr) {
    if (arr.size() <= 1) return 0;
    unsigned int ops = 0;
    
    int pivot = arr[arr.size() / 2];
    
    massive<int> left_pile;
    massive<int> middle_pile;
    massive<int> right_pile;
    
    // Резервируем память, чтобы избежать лишних перевыделений
    left_pile.reserve(arr.size());
    middle_pile.reserve(arr.size());
    right_pile.reserve(arr.size());
    
    // Раскидываем элементы по трем кучкам
    for (int x : arr) {
        ops++; //x < pivot
        if (x < pivot) {
            left_pile.push_back(x);
        } else {
            ops++; //x == pivot (если первое условие ложно)
            if (x == pivot) {
                middle_pile.push_back(x);
            } else {
                right_pile.push_back(x);
            }
        }
    }
    
    // Рекурсивная сортировка левой и правой кучек
    ops += Hoar_algorithm(left_pile);
    ops += Hoar_algorithm(right_pile);
    
    // Собираем массив обратно
    size_t idx = 0;
    for (int x : left_pile)   arr[idx++] = x;
    for (int x : middle_pile) arr[idx++] = x;
    for (int x : right_pile)  arr[idx++] = x;
    
    return ops;
}

/**
 * @brief Заполняет массив случайными числами в диапазоне [-100, 100]
 */
massive<int> random_massive(const int len) {
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(-100, 100);
    massive<int> arr(len);
    for (auto& elem : arr)
        elem = dist(rng);
    return arr;
}
