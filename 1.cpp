#include <random>
#include<iomanip>
#include<iostream>
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
unsigned int bubble_algorithm(massive <int>& arr);

/**
 * @brief Сортировка массива методом вставок
 * @param arr - ссылка на массив для сортировки
 * @return unsigned int - количество операций сравнения
 */
unsigned int insert_algorithm(massive<int>& arr);

/**
 * @brief Быстрая сортировка Хоара
 * @param lo - итератор на начало диапазона сортировки
 * @param hi - итератор на конец диапазона сортировки
 * @return unsigned int - количество операций сравнения
 */
unsigned int Hoar_algorithm(massive<int>::iterator lo, massive<int>::iterator hi);

/**
 * @brief Точка входа в программу. Сравнивает три алгоритма сортировки
 *        по среднему количеству операций сравнения на случайных массивах.
 *        Выполняет 50 прогонов на массивах размером 10000 элементов,
 *        после чего выводит таблицу со средними значениями для каждого алгоритма.
 * @return int - код завершения программы (1 - успех)
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
        total_hoare += Hoar_algorithm(data.begin(), data.end());
    }

    cout << setw(20) << left << "Algorithm"
        << setw(25) << right << "Average comparison count" << "\n";

    cout << setw(20) << left << "Bubble algorithm"
        << setw(25) << right << total_bubble / trials << "\n";

    cout << setw(20) << left << "Insert algorithm"
        << setw(25) << right << total_insert / trials << "\n";

    cout << setw(20) << left << "Hoare algorithm"
        << setw(25) << right << total_hoare / trials << "\n";

    return 1;
}

/**
 * @brief Сортировка массива пузырьковым методом
 * @param arr - ссылка на массив для сортировки
 * @return unsigned int - количество операций сравнения
 */
unsigned int bubble_algorithm(massive <int>& arr) {
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
 * @brief Сортировка массива методом вставок
 * @param arr - ссылка на массив для сортировки
 * @return unsigned int - количество операций сравнения
 */
unsigned int insert_algorithm(massive<int>& arr) {
    if (arr.size() <= 1) return 0;
    unsigned int ops = 0;
    for (size_t pos = 1; pos < arr.size(); pos++) {
        int key = arr[pos];
        for (size_t slot = 0; slot < pos; slot++) {
            ops++;
            if (key <= arr[slot]) {
                for (size_t shift = pos; shift > slot; shift--) {
                    arr[shift] = arr[shift - 1];
                }
                arr[slot] = key;
                break;
            }
        }
    }
    return ops;
}

/**
 * @brief Быстрая сортировка Хоара
 * @param lo - итератор на начало диапазона сортировки
 * @param hi - итератор на конец диапазона сортировки
 * @return unsigned int - количество операций сравнения
 */
unsigned int Hoar_algorithm(massive<int>::iterator lo, massive<int>::iterator hi) {
    if (hi - lo <= 1) return 0;
    unsigned int ops = 0;
    const int pivot_val = *(lo + (hi - lo) / 2);
    auto left = lo, right = hi - 1;
    while (left <= right) {
        for (; left <= right; left++) {
            ops++;
            if (*left >= pivot_val)
                break;
        }
        for (; left <= right; right--) {
            ops++;
            if (*right <= pivot_val)
                break;
        }
        if (left <= right) {
            swap(*left, *right);
            left++;
            right--;
        }
    }
    ops += Hoar_algorithm(lo, right + 1);
    ops += Hoar_algorithm(left, hi);
    return ops;
}

/**
 * @brief Заполняет массив случайными числами в диапазоне [-100, 100]
 * @param len - размер массива
 * @return massive<int> - заполненный случайными числами массив
 */
massive<int> random_massive(const int len) {
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(-100, 100);
    massive<int> arr(len);
    for (auto& elem : arr)
        elem = dist(rng);
    return arr;
}