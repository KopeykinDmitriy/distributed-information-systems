/// CPU: Intel Core i3 10105f, 4 Core/8 Threads, Cache L3 = 6,0 Mb
/// row-col-inner = 149377 ms
/// col-row-inner = 148823 ms
/// row-inner-col = 9154 ms

#include <fstream>
#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int N = 2400;

//const int BLOCK_SIZE = 10; // Размер блока



void get_matrices_from_files(int* A, int* B)
{
    std::ifstream matrix_file_a;
    std::ifstream matrix_file_b;
    matrix_file_a.open("inputMatrixA.txt");
    matrix_file_b.open("inputMatrixB.txt");
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (!(matrix_file_a >> A[i * N + j])) {
                std::cerr << "Ошибка чтения из файла." << std::endl;
            }
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (!(matrix_file_b >> B[i * N + j])) {
                std::cerr << "Ошибка чтения из файла." << std::endl;
            }
        }
    }
}

void input_matrices()
{
    std::ofstream out;
    out.open("inputMatrixA.txt");
    std::srand(std::time(nullptr));
    if (out.is_open())
    {
        out.clear();
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                out << std::rand() % 9 + 1 << ' ';
            }
            out << std::endl;
        }
    }
}

void matrix_multiply(int* A, int* B, int* C, int blockSize, int commRank) {
    for (int row = 0; row < blockSize; row++)
    {
        for (int inner = 0; inner < N; inner++)
        {
            for (int col = 0; col < N; col++)
            {
                C[row * N + col] += A[row * N + inner] * B[inner * N + col];
            }
        }
    }
}

void clear_matrix(int* matrix, int size) {
    for (int i = 0; i < size; i++)
        matrix[i] = 0;
}

void print_matrix(int* matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%d ", matrix[row * size + col]);
        }
        printf("\n");
    }
}

int get_block_size(int nCommSize) {
    int countElements = N * N;
    double squareRoot = sqrt(countElements / nCommSize);
    int roundedSquareRoot = round(squareRoot);
    if (roundedSquareRoot * roundedSquareRoot == countElements / nCommSize)
        return roundedSquareRoot;
    return -1;
}


int main(int argc, char* argv[])
{
    //input_matrices();
    int nCommRank, nCommSize, nameLen, nCounter, nIntervale;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    double t1, t2;
    MPI_Status status;
    
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &nCommRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nCommSize);
    MPI_Get_processor_name(processor_name, &nameLen);
    int* A = new int[N * N];
    int* B = new int[N * N];
    int* C = new int[N * N];
    if (nCommRank == 0) {
        get_matrices_from_files(A, B);
        clear_matrix(C, N*N);
        t1 = MPI_Wtime();
    }
    
    double blockSizeDouble = (float)N / nCommSize;
    int blockSize = N/nCommSize;
    if (blockSizeDouble != blockSize) {
        printf("Невозможно вычислить с таким количеством процессов.");
        MPI_Finalize();
        return 1;
    }
        
    int elementsPerProcess = blockSize * N;

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    int* localA = new int[elementsPerProcess];
    int* localC = new int[elementsPerProcess];
    clear_matrix(localC, elementsPerProcess);

    MPI_Scatter(A, elementsPerProcess, MPI_INT, localA, elementsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);
    matrix_multiply(localA, B, localC, blockSize, nCommRank);
    MPI_Gather(localC, elementsPerProcess, MPI_INT, C, elementsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);
    if (nCommRank == 0){
        //print_matrix(C, N);
        t2 = MPI_Wtime();
        printf("\nTime is %f seconds\n", t2 - t1);
    }
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] localA;
    delete[] localC;
    MPI_Finalize();
    return 0;
}
