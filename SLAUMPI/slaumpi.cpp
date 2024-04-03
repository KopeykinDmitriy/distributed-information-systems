#include <iostream>
#include <mpi.h>

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

void back_step_gaussian_elimination() {
    for (int i = N - 1; i >= 0; i--) {
        X[i] = A[i * N + N];
        for (int j = i + 1; j < N; j++) {
            X[i] -= A[i * N + j] * X[j];
        }
        X[i] /= A[i * N + i];
    }
}

void gaussian_elimination(double* a, double* x, int n) {
    int i, j, k;
    double ratio;
    // Прямой ход метода Гаусса
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            ratio = a[j * n + i] / a[i * n + i];
            for (k = i; k < n + 1; k++) {
                a[j * n + k] -= ratio * a[i * n + k];
            }
        }
    }

    // Обратный ход метода Гаусса
    for (i = n - 1; i >= 0; i--) {
        x[i] = a[i * n + n];
        for (j = i + 1; j < n; j++) {
            x[i] -= a[i * n + j] * x[j];
        }
        x[i] /= a[i * n + i];
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
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rows_per_proc = N / (size - 1);
    double elementsDouble = (float)N / (size - 1);
    if (rows_per_proc != elementsDouble)
    {
        printf("Невозможно вычислить с таким количеством процессов.");
        return -1;
    }
    if (rank == 0)
    {
        double startTime, endTime;
        startTime = MPI_Wtime();
        double* local_a = new double[rows_per_proc * (N + 1)];
        int index = 0;
        for (int proc = 1; proc < size; proc++)
        {
            for (int i = 0; i < rows_per_proc * (N + 1); i++)
            {
                local_a[i] = A[index];
                index++;
            }
            MPI_Send(local_a, rows_per_proc * (N + 1), MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
        }
        index = 0;
        for (int proc = 1; proc < size; proc++)
        {
            MPI_Recv(local_a, rows_per_proc * (N + 1), MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < rows_per_proc * (N + 1); i++)
            {
                A[index] = local_a[i];
                index++;
            }
        }
        delete[] local_a;
        back_step_gaussian_elimination();
        endTime = MPI_Wtime();
        printf("Time = %f s", endTime - startTime);
        CheckAnswer();
    }
    else
    {
        int rowsStart = rows_per_proc * (rank - 1);
        int rowsEnd = rank * rows_per_proc - 1;
        double* local_a = new double[rows_per_proc * (N + 1)];
        double* local_b = new double[rows_per_proc * (N + 1)];

        MPI_Recv(local_a, rows_per_proc * (N + 1), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

        for (int proc = 1; proc < rank; proc++)
        {
            MPI_Recv(local_b, rows_per_proc * (N + 1), MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            for (int i = 0; i < rows_per_proc; i++) {
                for (int j = 0; j < rows_per_proc; j++) {
                    double ratio = local_b[i * N + j + rows_per_proc * (proc - 1)];
                    for (int k = N + 1; k >= j; k--) {
                        local_a[i * N + k] -= ratio * local_b[j * N + k];
                    }
                }
            }
        }
        delete[] local_b;
        int counter = 0;
        for (int i = 0; i < rows_per_proc; i++) {
            double ratio = local_a[i * N + i + rowsStart + counter];

            for (int j = (N + 1); j >= rowsStart + counter; j--)
                local_a[i * N + j] /= ratio;
            for (int j = i + 1; j < rows_per_proc; j++) {
                ratio = local_a[j * N + rowsStart + counter];
                for (int k = N + 1; k >= rowsStart + counter; k--)
                    local_a[j * N + k] -= ratio * local_a[i * N + k];
            }
        }
        for (int proc = rank+1; proc < size; proc++)
        {
            MPI_Send(local_a, rows_per_proc * (N + 1), MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
        }
        MPI_Send(local_a, rows_per_proc * (N + 1), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        delete[] local_a;
    }
    
    MPI_Finalize();
    return 0;
}