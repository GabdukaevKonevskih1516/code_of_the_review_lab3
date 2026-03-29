// Copyright (c) 2026. All Rights Reserved.

#include "Header.h"
#include <iostream>
#include <fstream>

bool TGraphProcessor::LoadFromFile(const std::string& InFileName)
{
    std::ifstream InFile(InFileName);
    if (!InFile.is_open())
    {
        return false;
    }

    if (!(InFile >> VertexCount) || VertexCount <= 0)
    {
        return false;
    }

    // Инициализация матрицы
    AdjacencyMatrix.assign(VertexCount, std::vector<int32>(VertexCount, 0));

    for (int32 i = 0; i < VertexCount; ++i)
    {
        for (int32 j = 0; j < VertexCount; ++j)
        {
            if (!(InFile >> AdjacencyMatrix[i][j]))
            {
                return false;
            }
        }
    }

    InFile.close();
    return true;
}

std::vector<int32> TGraphProcessor::CalculateVertexDegrees() const
{
    std::vector<int32> Degrees(VertexCount, 0);

    for (int32 i = 0; i < VertexCount; ++i)
    {
        for (int32 j = 0; j < VertexCount; ++j)
        {
            if (AdjacencyMatrix[i][j] != 0)
            {
                // Если это петля (i == j), прибавляем 2, иначе 1
                if (i == j)
                {
                    Degrees[i] += 2;
                }
                else
                {
                    Degrees[i] += 1;
                }
            }
        }
    }

    return Degrees;
}

void TGraphProcessor::PrintDegrees(const std::vector<int32>& InDegrees) const
{
    std::cout << "Степени вершин графа:\n";
    for (int32 i = 0; i < static_cast<int32>(InDegrees.size()); ++i)
    {
        std::cout << "Вершина " << (i + 1) << ": " << InDegrees[i] << "\n";
    }
}

void ClearInputBuffer()
{
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}
