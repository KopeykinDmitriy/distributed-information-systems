#include <fstream>
#include <iostream>

const int ROWS = 3000;
const int COLUMNS = 3000;

int matrix_a[ROWS][COLUMNS];
int matrix_b[ROWS][COLUMNS];
int result_matrix[ROWS][COLUMNS];

void get_matrices_from_files()
{
    std::ifstream matrix_file_a;
    std::ifstream matrix_file_b;
    matrix_file_a.open("inputMatrixA.txt");
    matrix_file_b.open("inputMatrixB.txt");
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            if (!(matrix_file_a >> matrix_a[i][j])) {
                std::cerr << "Ошибка чтения из файла." << std::endl;
            }
        }
    }
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            if (!(matrix_file_b >> matrix_b[i][j])) {
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
            result_matrix[row][col] = 0;
            for (int inner = 0; inner < COLUMNS; inner++)
            {
                result_matrix[row][col] += matrix_a[row][inner] * matrix_b[inner][col];
            }
        }
    }
}


int main(int argc, char* argv[])
{
    //input_matrices();
    get_matrices_from_files();
    time_t start, end;
    time(&start);
    bad_multiply();
    time(&end);
    double seconds = difftime(end, start);
    std::cout << seconds << " секунд - медленное перемножение";
    return 0;
}
