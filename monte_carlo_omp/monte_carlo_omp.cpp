#include <iostream>
#include <random>
#include <chrono>
#include <cstdlib>
#include <omp.h>
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);

    long long N;
    int num_threads;

    if (argc > 1) {
        N = atoll(argv[1]);
    }
    else {
        do {
            std::cout << "Введіть кількість точок N: ";
            std::cin >> N;
        } while (N <= 0);
    }

    int maxThreads = omp_get_max_threads();
    if (argc > 2) {
        num_threads = atoi(argv[2]);
    }
    else {
        do {
            std::cout << "Введіть кількість потоків (1.." << maxThreads << "): ";
            std::cin >> num_threads;
        } while (num_threads <= 0 || num_threads > maxThreads);
    }

    omp_set_num_threads(num_threads);   
    long long inside = 0;
    int actual_threads = 0;             
    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel reduction(+:inside)     
    {
        int tid = omp_get_thread_num();
        if (tid == 0) actual_threads = omp_get_num_threads();   
        std::mt19937_64 rng(42 + tid);
        std::uniform_real_distribution<double> dist(0.0, 1.0);

#pragma omp for
        for (long long i = 0; i < N; i++) {
            double x = dist(rng);
            double y = dist(rng);
            if (x * x + y * y <= 1.0)
                inside++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();

    std::cout << "=== Паралельна версія OpenMP (Монте-Карло) ===" << std::endl;
    std::cout << "Запитано потоків: " << num_threads << std::endl;
    std::cout << "Фактично стартувало: " << actual_threads << std::endl;
    std::cout << "Доступно ядер (max): " << maxThreads << std::endl;
    std::cout << "Кількість точок N = " << N << std::endl;
    std::cout << "Pi = " << 4.0 * inside / N << std::endl;
    std::cout << "Час виконання: " << elapsed << " сек" << std::endl;

    return 0;
}