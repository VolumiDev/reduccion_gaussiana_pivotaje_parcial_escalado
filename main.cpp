#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

/*
    Funcion que retorna un float. Tambien realiza la validación de que nos ingresan un dato valido.
*/
float input_number(string placeholder)
{
    double num;

    do
    {
        cout << placeholder << endl;

        if (!(cin >> num))
        { // Error en caso de que nos ingresen un dato no valido
            cout << "¡Error! El dato introducido no es valido. Intentelo de nuevo.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            break;
        }
    } while (true);
    return num;
}

/*
    Funcion que genera una matriz de determinadas filas y columnas. Los datos nos los pide por pantalla
    Retorna la matriz
*/
vector<vector<double>> matrix_generator(int rows, int cols)
{
    vector<vector<double>> matrix;

    for (int i = 0; i < rows; i++)
    {
        vector<double> current_row;
        for (int j = 0; j < cols; j++)
        {
            string prompt = "Introduce un numero para la fila " + to_string(i + 1) + " columna " + to_string(j + 1) + ":";
            double value = input_number(prompt);
            current_row.push_back(value);
        }
        matrix.push_back(current_row);
    }
    return matrix;
}

/*
    Imprime por pantalla la matriz que le pasamos como parámetro.
*/
void print_matrix(vector<vector<double>> &matrix, string message = "")
{
    cout << message << endl;
    for (const auto &row : matrix)
    {
        int i = 0;
        cout << "|\t";
        for (double number : row)
        {
            cout << number << "\t";
            i++;
        }
        cout << "|" << endl;
    }
}

void augmented_matrix(vector<vector<double>> main_matrix, vector<vector<double>> independiente_matrix, string message)
{
    cout << message << endl;
    int rows = main_matrix.size();
    int cols = independiente_matrix.size();

    for (int i = 0; i < main_matrix.size(); i++) // filas de la main matrix
    {
        for (int j = 0; j < independiente_matrix.size(); j++) // filas de la independient_matrix
        {
            for (int z = 0; z < independiente_matrix[j].size(); z++) // columnas de la independient_matrix
            {
                main_matrix[i].push_back(independiente_matrix[j][z]);
            }
        }
    }
    
}

double max_abs_value(const std::vector<double>& v) {
    if (v.empty()) return 0; // o lanza excepción

    auto iterator = std::max_element(
        v.begin(),
        v.end(),
        [](double a, double b) {
            return std::abs(a) < std::abs(b);
        }
    );
    return abs(*iterator);
}


vector<double> scale_factor(vector<vector<double>> main_matrix){
    vector<double> scales = {};
    for (int i = 0; i < main_matrix.size(); i++){
        auto row = main_matrix[i];
        auto scale_factor = max_abs_value(row);
        scales.push_back(scale_factor);
    };
    return scales;
}

vector<int> 





int main()
{


    vector<vector<double>> matriz = {
        { 1.5,   -3.2,   7.0,   -0.5 },
        { -10.0,  2.3,  -4.4,   8.8 },
        { 6.1,   -9.9,   0.0,   3.14 },
        { -2.2,   5.5,  -7.7,  12.0 }
    };

    vector<double> scales = scale_factor(matriz);
    for (double x : scales){
        cout << x << " ";
    }




return 0;
}

















    // std::vector<double> prueba1 = {23, -1.2345, -25.0, 12.0};
    // std::vector<double> prueba2 = {-100.5, 3.14, 50.2};
    // std::vector<double> prueba3 = {0.001, -0.002, 0.0005};

    // std::cout << max_abs_value(prueba1) << std::endl;
    // std::cout << max_abs_value(prueba2) << std::endl;
    // std::cout << max_abs_value(prueba3) << std::endl;



    // const int main_row_number = 4;
    // const int main_col_number = 4;
    // const int result_row_number = main_row_number;
    // const int result_col_number = 1;

    // vector<vector<double>> matrix = matrix_generator(main_row_number, main_col_number);
    // cout << "Ahora introduce la matriz de terminos independientes" << endl;
    // vector<vector<double>> result_matrix = matrix_generator(result_row_number, result_col_number);

    // print_matrix(matrix, "Matriz principal");
    // print_matrix(result_matrix, "Matriz de terminos independientes");