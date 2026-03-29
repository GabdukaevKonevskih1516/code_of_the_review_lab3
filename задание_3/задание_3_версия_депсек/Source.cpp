#include "Header.h"
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <iomanip>

FGraphAnalyzer::FGraphAnalyzer()
{
    Graph.VertexCount = 0;
}

bool FGraphAnalyzer::SafeReadInt(std::ifstream& File, int& Value) const
{
    std::string Line;
    if (!std::getline(File, Line))
    {
        return false;
    }

    // Удаляем пробелы в начале и конце
    size_t Start = Line.find_first_not_of(" \t\r\n");
    if (Start == std::string::npos)
    {
        return false;
    }

    size_t End = Line.find_last_not_of(" \t\r\n");
    Line = Line.substr(Start, End - Start + 1);

    // Проверяем, что строка содержит только цифры (возможно с минусом)
    for (char C : Line)
    {
        if (!std::isdigit(C) && C != '-')
        {
            return false;
        }
    }

    try
    {
        Value = std::stoi(Line);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FGraphAnalyzer::IsValidEntry(int Value) const
{
    // Для неориентированного графа без кратных ребер допустимы только 0 и 1
    return Value == 0 || Value == 1;
}

bool FGraphAnalyzer::ValidateMatrix() const
{
    if (Graph.VertexCount <= 0)
    {
        std::cerr << "Ошибка: количество вершин должно быть положительным числом\n";
        return false;
    }

    // Проверяем размерность матрицы
    if (Graph.AdjacencyMatrix.size() != static_cast<size_t>(Graph.VertexCount))
    {
        std::cerr << "Ошибка: количество строк матрицы не соответствует указанному числу вершин\n";
        return false;
    }

    for (int i = 0; i < Graph.VertexCount; ++i)
    {
        if (Graph.AdjacencyMatrix[i].size() != static_cast<size_t>(Graph.VertexCount))
        {
            std::cerr << "Ошибка: размер строки " << i << " не соответствует числу вершин\n";
            return false;
        }

        for (int j = 0; j < Graph.VertexCount; ++j)
        {
            if (!IsValidEntry(Graph.AdjacencyMatrix[i][j]))
            {
                std::cerr << "Ошибка: элемент матрицы [" << i << "][" << j
                    << "] = " << Graph.AdjacencyMatrix[i][j]
                    << " должен быть 0 или 1\n";
                return false;
            }
        }
    }

    // Проверяем симметричность для неориентированного графа
    for (int i = 0; i < Graph.VertexCount; ++i)
    {
        for (int j = i + 1; j < Graph.VertexCount; ++j)
        {
            if (Graph.AdjacencyMatrix[i][j] != Graph.AdjacencyMatrix[j][i])
            {
                std::cerr << "Ошибка: матрица не симметрична. Элемент [" << i << "][" << j
                    << "] = " << Graph.AdjacencyMatrix[i][j]
                    << ", а [" << j << "][" << i
                    << "] = " << Graph.AdjacencyMatrix[j][i] << "\n";
                std::cerr << "Для неориентированного графа матрица должна быть симметричной\n";
                return false;
            }
        }
    }

    return true;
}

bool FGraphAnalyzer::LoadFromFile(const std::string& FileName)
{
    std::ifstream File(FileName);
    if (!File.is_open())
    {
        std::cerr << "Ошибка: не удалось открыть файл " << FileName << "\n";
        return false;
    }

    // Читаем количество вершин
    int VertexCount;
    if (!SafeReadInt(File, VertexCount))
    {
        std::cerr << "Ошибка: не удалось прочитать количество вершин\n";
        return false;
    }

    if (VertexCount <= 0)
    {
        std::cerr << "Ошибка: количество вершин должно быть положительным числом, получено: "
            << VertexCount << "\n";
        return false;
    }

    Graph.VertexCount = VertexCount;
    Graph.AdjacencyMatrix.resize(Graph.VertexCount, std::vector<int>(Graph.VertexCount, 0));

    // Читаем матрицу смежности
    for (int i = 0; i < Graph.VertexCount; ++i)
    {
        for (int j = 0; j < Graph.VertexCount; ++j)
        {


            int Value;
            if (!(File >> Value))
            {
                std::cerr << "Ошибка: недостаточно данных в файле для элемента матрицы ["
                    << i << "][" << j << "]\n";
                return false;
            }

            if (!IsValidEntry(Value))
            {
                std::cerr << "Ошибка: некорректное значение " << Value
                    << " в позиции [" << i << "][" << j << "]\n";
                return false;
            }

            Graph.AdjacencyMatrix[i][j] = Value;
        }
    }

    File.close();

    // Проверяем корректность матрицы
    if (!ValidateMatrix())
    {
        return false;
    }

    return true;
}

void FGraphAnalyzer::CalculateDegrees()
{
    if (Graph.VertexCount <= 0)
    {
        return;
    }

    Graph.Degrees.resize(Graph.VertexCount, 0);

    for (int i = 0; i < Graph.VertexCount; ++i)
    {
        int Degree = 0;
        for (int j = 0; j < Graph.VertexCount; ++j)
        {
            if (Graph.AdjacencyMatrix[i][j] == 1)
            {
                if (i == j)
                {
                    // Петля учитывается дважды
                    Degree += 2;
                }
                else
                {
                    Degree += 1;
                }
            }
        }
        Graph.Degrees[i] = Degree;
    }
}

bool FGraphAnalyzer::WriteDegreesToFile(const std::string& FileName) const
{
    std::ofstream File(FileName);
    if (!File.is_open())
    {
        std::cerr << "Ошибка: не удалось создать файл " << FileName << "\n";
        return false;
    }

    File << "Анализ графа\n";
    File << "========================================\n";
    File << "Количество вершин: " << Graph.VertexCount << "\n\n";

    File << "Степени вершин:\n";
    for (int i = 0; i < Graph.VertexCount; ++i)
    {
        File << "Вершина " << i << ": " << Graph.Degrees[i] << "\n";
    }

    // Дополнительная статистика
    File << "\nСтатистика:\n";
    int MaxDegree = 0;
    int MinDegree = Graph.VertexCount > 0 ? Graph.Degrees[0] : 0;
    int IsolatedCount = 0;
    int LoopCount = 0;

    for (int i = 0; i < Graph.VertexCount; ++i)
    {
        if (Graph.Degrees[i] > MaxDegree)
        {
            MaxDegree = Graph.Degrees[i];
        }
        if (Graph.Degrees[i] < MinDegree)
        {
            MinDegree = Graph.Degrees[i];
        }
        if (Graph.Degrees[i] == 0)
        {
            IsolatedCount++;
        }

        // Проверяем наличие петель
        if (Graph.AdjacencyMatrix[i][i] == 1)
        {
            LoopCount++;
        }
    }

    File << "Максимальная степень: " << MaxDegree << "\n";
    File << "Минимальная степень: " << MinDegree << "\n";
    File << "Количество изолированных вершин: " << IsolatedCount << "\n";
    File << "Количество петель: " << LoopCount << "\n";

    File << "========================================\n";

    File.close();
    return true;
}

void FGraphAnalyzer::PrintGraphInfo() const
{
    PrintHeader();

    std::cout << "Количество вершин: " << Graph.VertexCount << "\n\n";

    std::cout << "Матрица смежности:\n";
    for (int i = 0; i < Graph.VertexCount; ++i)
    {
        std::cout << "  ";
        for (int j = 0; j < Graph.VertexCount; ++j)
        {
            std::cout << Graph.AdjacencyMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nСтепени вершин:\n";
    for (int i = 0; i < Graph.VertexCount; ++i)
    {
        std::cout << "  Вершина " << i << ": " << Graph.Degrees[i] << "\n";
    }

    PrintSeparator();
}

void PrintHeader()
{
    std::cout << "\n========================================\n";
    std::cout << "        АНАЛИЗАТОР ГРАФОВ\n";
    std::cout << "========================================\n";
}

void PrintSeparator()
{
    std::cout <<


        "========================================\n\n";
}
