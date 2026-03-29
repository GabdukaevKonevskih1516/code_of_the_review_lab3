#pragma once

#include <vector>
#include <string>
#include <fstream>

// Структура для хранения информации о графе
struct FGraph
{
    int VertexCount;                    // Количество вершин
    std::vector<std::vector<int>> AdjacencyMatrix;  // Матрица смежности
    std::vector<int> Degrees;           // Степени вершин

    FGraph() : VertexCount(0) {}
};

// Класс для работы с графом
class FGraphAnalyzer
{
public:
    FGraphAnalyzer();

    // Загрузка графа из файла
    bool LoadFromFile(const std::string& FileName);

    // Вычисление степеней вершин
    void CalculateDegrees();

    // Вывод степеней вершин в файл
    bool WriteDegreesToFile(const std::string& FileName) const;

    // Вывод информации о графе на экран
    void PrintGraphInfo() const;

    // Получение степеней вершин
    const std::vector<int>& GetDegrees() const { return Graph.Degrees; }

    // Получение количества вершин
    int GetVertexCount() const { return Graph.VertexCount; }

private:
    FGraph Graph;

    // Проверка корректности матрицы смежности
    bool ValidateMatrix() const;

    // Проверка, является ли число корректным (0 или 1 для неориентированного графа)
    bool IsValidEntry(int Value) const;

    // Защита от неверного ввода при чтении файла
    bool SafeReadInt(std::ifstream& File, int& Value) const;
};

// Функция для форматированного вывода
void PrintHeader();
void PrintSeparator();
