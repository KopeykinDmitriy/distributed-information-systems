#include <iostream>
#include <omp.h>

const int N = 1000; // Размер матрицы
double A[N][N + 1]; // Расширенная матрица системы
double X[N]; // Решение

void initialize_system() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10 + 1;
        }
        A[i][N] = rand() % N;
    }
}

void gaussian_elimination() {
    int i, j, k;
    double ratio;

    // Прямой ход метода Гаусса
    for (i = 0; i < N - 1; i++) {
#pragma omp parallel for private(j, k, ratio) shared(A)
        for (j = i + 1; j < N; j++) {
            ratio = A[j][i] / A[i][i];
            for (k = i; k < N + 1; k++) {
                A[j][k] -= ratio * A[i][k];
            }
        }
    }

    // Обратный ход метода Гаусса
    for (i = N - 1; i >= 0; i--) {
        X[i] = A[i][N];
#pragma omp parallel for private(j) shared(A, X)
        for (j = i + 1; j < N; j++) {
            X[i] -= A[i][j] * X[j];
        }
        X[i] /= A[i][i];
    }
}

int main() {
    // Инициализация матрицы A
    initialize_system();
    // Выполнение метода Гаусса
    gaussian_elimination();

    // Вывод решения
    std::cout << "Solution:\n";
    for (int i = 0; i < N; i++) {
        std::cout << "X[" << i << "] = " << X[i] << std::endl;
    }

    return 0;
}