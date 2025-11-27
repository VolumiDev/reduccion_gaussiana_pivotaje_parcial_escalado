#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
// Función que valida que se introduce un dato valido, y lo retorna como double
double readDouble(const std::string& prompt) {
    double value{};
    while (true) {
        std::cout << prompt << '\n';
        if (std::cin >> value) {
            return value;
        }
        std::cout << "Error: introduce un numero valido.\n";
        std::cin.clear(); //limpia el error de pantalla
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //descarta la información en entrada hasta el final de la línea
    }
}

// Función que valida que se introduce un dato valido, y lo retorna como int
int readSize(const std::string& prompt) {
    int size{};
    while (true) {
        std::cout << prompt << '\n';
        if ((std::cin >> size) && size > 0) {
            return size;
        }
        std::cout << "Error: introduce un entero positivo.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Crea la matriz de la dimensión deseada como vector de vectores y la retorna
std::vector<std::vector<double>> readMatrix(int rows, int cols, const std::string& name) {
    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::string prompt = "Introduce " + name + " [" + std::to_string(i + 1) + "," +
                                 std::to_string(j + 1) + "]:";
            matrix[i][j] = readDouble(prompt);
        }
    }
    return matrix;
}

// Imprime por pantalla una matriz
void printMatrix(const std::vector<std::vector<double>>& matrix, const std::string& title) {
    std::cout << title << '\n';
    for (const auto& row : matrix) {
        std::cout << "| ";
        for (double value : row) {
            std::cout << std::setw(12) << value << ' ';
        }
        std::cout << "|\n";
    }
}

double maxAbsValue(const std::vector<double>& v) {
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

std::vector<double> getScalars(std::vector<std::vector<double>>& a){
    std::vector<double> scalars (a.size());
    for (int i = 0; i < a.size(); i++){
        std::vector<double> row = a[i];
        scalars[i] = maxAbsValue(row);
    }
    return scalars;
}

//Crea la matriz aumentada añadiendo al a cada una de los vectores los coeficientes y en el ultimo lugar el termino independiente
std::vector<std::vector<double>> buildAugmented(
    const std::vector<std::vector<double>>& a,
    const std::vector<double>& b) {
    std::vector<std::vector<double>> augmented(a.size(), std::vector<double>(a[0].size() + 1));
    for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < a[i].size(); ++j) {
            augmented[i][j] = a[i][j]; //iteramos añadiendo coeficientes
        }
        augmented[i].back() = b[i]; //añadimos el termino independiente al final de la "fila"
    }
    return augmented;
}

std::vector<double> computeScales(const std::vector<std::vector<double>>& augmented, int n) {
    std::vector<double> scales(augmented.size(), 0.0);
    for (std::size_t i = 0; i < augmented.size(); ++i) {
        double maxAbs = 0.0;
        for (int j = 0; j < n; ++j) {
            maxAbs = std::max(maxAbs, std::abs(augmented[i][j]));
        }
        if (maxAbs == 0.0) {
            throw std::runtime_error("El sistema no tiene solucion unica (fila nula).");
        }
        scales[i] = maxAbs;
    }
    return scales;
}

void pivot(std::vector<std::vector<double>>& augmented,
           const std::vector<double>& scales,
           int pivotIndex) {
    int maxRow = pivotIndex;
    double maxRatio = std::abs(augmented[pivotIndex][pivotIndex]) / scales[pivotIndex];
    const int n = static_cast<int>(augmented.size());

    for (int i = pivotIndex + 1; i < n; ++i) {
        double ratio = std::abs(augmented[i][pivotIndex]) / scales[i];
        if (ratio > maxRatio) {
            maxRatio = ratio;
            maxRow = i;
        }
    }

    if (maxRatio == 0.0) {
        throw std::runtime_error("El sistema no tiene solucion unica (pivote nulo).");
    }

    if (maxRow != pivotIndex) {
        std::swap(augmented[pivotIndex], augmented[maxRow]);
    }
}

void forwardElimination(std::vector<std::vector<double>>& augmented, std::vector<double>& scalars) {
    const int n = static_cast<int>(augmented.size());
    // std::vector<double> scales = computeScales(augmented, n);
    for (int k = 0; k < n - 1; ++k) { // k es el numero de fila
        pivot(augmented, scalars, k);
        for (int i = k + 1; i < n; ++i) {
            double factor = augmented[i][k] / augmented[k][k];
            for (int j = k; j <= n; ++j) { // incluye columna de terminos independientes
                augmented[i][j] -= factor * augmented[k][j];
            }
        }
    }
    if (std::abs(augmented[n - 1][n - 1]) == 0.0) {
        throw std::runtime_error("El sistema no tiene solucion unica (pivote final nulo).");
    }
}

std::vector<double> backSubstitution(const std::vector<std::vector<double>>& augmented) {
    const int n = static_cast<int>(augmented.size());
    std::vector<double> x(n, 0.0);

    for (int i = n - 1; i >= 0; --i) {
        double sum = augmented[i][n]; // termino independiente
        for (int j = i + 1; j < n; ++j) {
            sum -= augmented[i][j] * x[j];
        }
        x[i] = sum / augmented[i][i];
    }
    return x;
}

} // namespace

int main() {
    std::cout << std::fixed << std::setprecision(6);

    const int n = readSize("Introduce el tamaño de la matriz:");
    std::cout << "Primero introduce la matriz de coeficientes A (" << n << "x" << n << "):" << std::endl;
    std::vector<std::vector<double>> matrixA = readMatrix(n, n, "A");

    //Calculamos los factores de cada ecuación
    std::vector<double> scalarsFactor = getScalars(matrixA);
    for(double x : scalarsFactor){
        std::cout << x << std::endl;
    }

    std::cout << "Ahora introduce el vector de terminos independientes b (" << n << "x1):" << std::endl;
    std::vector<std::vector<double>> vectorBMatrix = readMatrix(n, 1, "b");

    std::vector<double> vectorB(n);
    for (int i = 0; i < n; ++i) {
        vectorB[i] = vectorBMatrix[i][0];
    }

    try {
        std::vector<std::vector<double>> augmented = buildAugmented(matrixA, vectorB);
        forwardElimination(augmented, scalarsFactor);

        std::cout << "\nMatriz triangular superior obtenida (A|b):" << std::endl;
        printMatrix(augmented, "");

        std::vector<double> solution = backSubstitution(augmented);
        std::cout << "\nSolucion del sistema:" << std::endl;
        for (int i = 0; i < n; ++i) {
            std::cout << "x" << i + 1 << " = " << solution[i]  << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cout << "No se pudo completar la eliminacion: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
