#include <iostream>
#include <tbb/tbb.h>

const int N = 4800; // Размер матрицы
double A[N * (N + 1)]; // Расширенная матрица системы
double X[N]; // Решение

void initialize_system() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = rand() % 10 + 1;
        }
        A[i * N + N] = rand() % N;
    }
}

void gaussian_elimination() {
    // Прямой ход метода Гаусса
    for (int i = 0; i < N - 1; i++) {
        tbb::parallel_for(tbb::blocked_range<int>(i + 1, N), [&](const tbb::blocked_range<int>& range) {
            for (int j = range.begin(); j < range.end(); j++) {
                double ratio = A[j * N + i] / A[i * N + i];
                for (int k = i; k < N + 1; k++) {
                    A[j * N + k] -= ratio * A[i * N + k];
                }
            }
        });
    }

    // Обратный ход метода Гаусса
    for (int i = N - 1; i >= 0; i--) {
        X[i] = A[i * N + N];
        for (int j = i + 1; j < N; j++) {
            X[i] -= A[i * N + j] * X[j];
        }
        X[i] /= A[i * N + i];
    }
}

void CheckAnswer() {
    double sum = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            sum += X[j] * A[i * N + j];
        }
        if (std::abs(sum - A[i * N + N]) > 1e-6)
            std::cout << "Неверное решение. Разница = " << sum - A[i * N + N] << std::endl;
        sum = 0;
    }
}


int main(int argc, char** argv) {

    // Инициализация матрицы A
    initialize_system();
    unsigned int start_time = clock();
    gaussian_elimination();
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    printf("Time: %d ms", search_time);
    CheckAnswer();
    return 0;
}