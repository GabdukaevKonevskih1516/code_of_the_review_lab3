// Copyright (c) 2026. All Rights Reserved.

#pragma once

#include <string>
#include <vector>

// Определяем типы в стиле Epic Games для обычного компилятора
typedef std::uint8_t uint8;
typedef std::int32_t int32;

/** Класс для работы с неориентированным графом */
class TGraphProcessor
{
public:
	TGraphProcessor() = default;

	/** Загружает матрицу смежности из файла */
	bool LoadFromFile(const std::string& InFileName);

	/** Вычисляет степени всех вершин */
	std::vector<int32> CalculateVertexDegrees() const;

	/** Выводит результат в консоль */
	void PrintDegrees(const std::vector<int32>& InDegrees) const;

private:
	/** Матрица смежности */
	std::vector<std::vector<int32>> AdjacencyMatrix;

	/** Количество вершин */
	int32 VertexCount = 0;
};

/** Очистка буфера ввода */
void ClearInputBuffer();
