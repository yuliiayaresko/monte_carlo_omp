#include <iostream>
#include <random>
#include <chrono>
#include <cstdlib>
#include <omp.h>
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);
    long long N = (argc > 1) ? atoll(argv[1]) : 1000000;
    long long inside = 0;

    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel reduction(+:inside)
    {
        int tid = omp_get_thread_num();
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
    std::cout << "Кількість потоків: " << omp_get_max_threads() << std::endl;
    std::cout << "Кількість точок N = " << N << std::endl;
    std::cout << "Pi = " << 4.0 * inside / N << std::endl;
    std::cout << "Час виконання: " << elapsed << " сек" << std::endl;

    return 0;
}