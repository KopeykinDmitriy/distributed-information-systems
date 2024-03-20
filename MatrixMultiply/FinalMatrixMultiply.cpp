/// CPU: Intel Core i3 10105f, 4 Core/8 Threads, Cache L3 = 6,0 Mb
/// row-col-inner = 149377 ms
/// col-row-inner = 148823 ms
/// row-inner-col = 9154 ms

#include <fstream>
#include <iostream>

const int ROWS = 3000;
const int COLUMNS = 3000;

int* matrix_a = (int*)malloc(sizeof(*matrix_a) * ROWS * COLUMNS);
int* matrix_b = (int*)malloc(sizeof(*matrix_b) * ROWS * COLUMNS);
int* result_matrix = (int*)malloc(sizeof(*result_matrix) * ROWS * COLUMNS);

void get_matrices_from_files()
{
    std::ifstream matrix_file_a;
    std::ifstream matrix_file_b;
    matrix_file_a.open("inputMatrixA.txt");
    matrix_file_b.open("inputMatrixB.txt");
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            if (!(matrix_file_a >> matrix_a[i*ROWS + j])) {
                std::cerr << "Ошибка чтения из файла." << std::endl;
            }
        }
    }
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            if (!(matrix_file_b >> matrix_b[i * ROWS + j])) {
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
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                out << std::rand() % 9 + 1 << ' ';
            }
            out << std::endl;
        }
    }
}

void bad_multiply()
{
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLUMNS; col++)
        {
            result_matrix[row * ROWS + col] = 0;
            for (int inner = 0; inner < COLUMNS; inner++)
            {
                result_matrix[row * ROWS + col] += matrix_a[row * ROWS + inner] * matrix_b[inner * ROWS + col];
            }
        }
    }
}

void fast_multiply() {
    for (int col = 0; col < COLUMNS; col++)
    {
        for (int row = 0; row < ROWS; row++)
        {
            for (int inner = 0; inner < COLUMNS; inner++)
            {
                result_matrix[row * ROWS + col] += matrix_a[row * ROWS + inner] * matrix_b[inner * ROWS + col];
            }
        }
    }
}

void another_multiply() {
    for (int row = 0; row < ROWS; row++)
    {
        for (int inner = 0; inner < COLUMNS; inner++)
        {
            for (int col = 0; col < COLUMNS; col++)
            {
                result_matrix[row * ROWS + col] += matrix_a[row * ROWS + inner] * matrix_b[inner * ROWS + col];
            }
        }
    }
}

void reset_result_matrix() {
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLUMNS; col++)
        {
            result_matrix[row * ROWS + col] = 0;
        }
    }
}


int main(int argc, char* argv[])
{
    //input_matrices();
    get_matrices_from_files();
    reset_result_matrix();
    unsigned int start_time = clock();
    bad_multiply();
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    std::cout << search_time << " милисекунд - медленное перемножение\n";
    reset_result_matrix();
    start_time = clock();
    fast_multiply();
    end_time = clock();
    search_time = end_time - start_time;
    std::cout << search_time << " милисекунд - быстрое перемножение\n";
    reset_result_matrix();
    start_time = clock();
    another_multiply();
    end_time = clock();
    search_time = end_time - start_time;
    std::cout << search_time << " милисекунд - очередное перемножение\n";
    reset_result_matrix();
    std::cout << result_matrix[0 * ROWS + 0];
    return 0;
}