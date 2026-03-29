// Copyright (c) 2026. All Rights Reserved.

#include "Header.h"
#include <iostream>

int main()
{
	setlocale(LC_ALL, "Russian");

	std::string FileName;
	std::cout << "Введите имя файла с графом (например, graph_data.txt): ";
	std::cin >> FileName;

	TGraphProcessor Processor;

	if (Processor.LoadFromFile(FileName))
	{
		std::vector<int32> Degrees = Processor.CalculateVertexDegrees();
		std::cout << "-----------------------------------\n";
		Processor.PrintDegrees(Degrees);
		std::cout << "-----------------------------------\n";
	}
	else
	{
		std::cerr << "Ошибка: Не удалось прочитать файл или формат данных неверен.\n";
	}

	return 0;
}
