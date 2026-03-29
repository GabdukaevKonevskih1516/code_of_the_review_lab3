#include "Header.h"
#include <fstream>
#include <iostream>

// Загрузка графа из файла
bool TGraphProcessor::LoadFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return false;
    }

    infile >> VertexCount;
    if (VertexCount <= 0) {
        std::cerr << "Ошибка: количество вершин должно быть положительным" << std::endl;
        return false;
    }

    AdjacencyMatrix.assign(VertexCount, std::vector<int>(VertexCount, 0));
    for (int i = 0; i < VertexCount; ++i) {
        for (int j = 0; j < VertexCount; ++j) {
            infile >> AdjacencyMatrix[i][j];
            if (AdjacencyMatrix[i][j] != 0 && AdjacencyMatrix[i][j] != 1) {
                std::cerr << "Ошибка: в матрице смежности допустимы только 0 и 1" << std::endl;
                return false;
            }
        }
    }
    infile.close();
    return true;
}

// Вычисление степеней вершин
std::vector<int> TGraphProcessor::CalculateVertexDegrees() const {
    std::vector<int> degrees(VertexCount, 0);
    for (int i = 0; i < VertexCount; ++i) {
        for (int j = 0; j < VertexCount; ++j) {
            if (AdjacencyMatrix[i][j] == 1) {
                if (i == j) {
                    degrees[i] += 2; // петля — +2 к степени
                }
                else {
                    degrees[i] += 1; // обычное ребро — +1
                }
            }
        }
    }
    return degrees;
}

// Вывод результатов
void TGraphProcessor::PrintDegrees(const std::vector<int>& degrees) const {
    std::cout << "Степени вершин графа (в порядке возрастания номеров):" << std::endl;
    for (int i = 0; i < VertexCount; ++i) {
        std::cout << "Вершина " << i << ": " << degrees[i] << std::endl;
    }
}
