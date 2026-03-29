#pragma once

#include <vector>
#include <string>

class TGraphProcessor {
private:
    int VertexCount;           // количество вершин
    std::vector<std::vector<int>> AdjacencyMatrix; // матрица смежности

public:
    // Загрузка графа из файла
    bool LoadFromFile(const std::string& filename);

    // Вычисление степеней вершин (петли учитываются дважды)
    std::vector<int> CalculateVertexDegrees() const;

    // Вывод степеней вершин в порядке возрастания номеров
    void PrintDegrees(const std::vector<int>& degrees) const;
};
