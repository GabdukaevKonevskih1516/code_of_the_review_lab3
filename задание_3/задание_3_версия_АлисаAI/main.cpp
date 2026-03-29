#include "Header.h"
#include <iostream>

    int main() {

        // Установка кодировки для корректного отображения кириллицы (для Windows)
        setlocale(LC_ALL, "Russian");

        TGraphProcessor processor;

        // Замените "FileName" на имя вашего файла с матрицей смежности
        if (processor.LoadFromFile("FileName.txt")) {
            std::vector<int> degrees = processor.CalculateVertexDegrees();
            processor.PrintDegrees(degrees);
        }
        else {
            std::cout << "Не удалось обработать граф" << std::endl;
            return 1;
        }

        return 0;
    }
