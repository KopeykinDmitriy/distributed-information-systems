/* C source code is found in dgemm_example.c */

#define min(x,y) (((x) < (y)) ? (x) : (y))

#include <stdio.h>
#include <stdlib.h>
#include "C:\Program Files (x86)\Intel\oneAPI\mkl\2024.0\include\mkl.h"
#include <fstream>
#include <iostream>

double* A, * B, * C;
int n, i, j;
double alpha, beta;

void get_matrices_from_files()
{
    std::ifstream matrix_file_a;
    std::ifstream matrix_file_b;
    matrix_file_a.open("inputMatrixA.txt");
    matrix_file_b.open("inputMatrixB.txt");
    for (int i = 0; i < n*n; ++i) {
        if (!(matrix_file_a >> A[i])) {
            std::cerr << "Ошибка чтения из файла." << std::endl;
        }
    }
    for (int i = 0; i < n*n; ++i) {
        if (!(matrix_file_b >> B[i])) {
            std::cerr << "Ошибка чтения из файла." << std::endl;
        }
    }
}

int main()
{
    n = 3000;
    alpha = 1.0; beta = 0.0;
    A = (double*)mkl_malloc(n * n * sizeof(double), 64);
    B = (double*)mkl_malloc(n * n * sizeof(double), 64);
    C = (double*)mkl_malloc(n * n * sizeof(double), 64);
    get_matrices_from_files();

    for (i = 0; i < (n * n); i++) {
        C[i] = 0.0;
    }
    time_t start, end;
    time(&start);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        n, n, n, alpha, A, n, B, n, beta, C, n);
    time(&end);
    double seconds = difftime(end, start);
    std::cout << seconds << " секунд - быстрое перемножение";
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);
    return 0;
}