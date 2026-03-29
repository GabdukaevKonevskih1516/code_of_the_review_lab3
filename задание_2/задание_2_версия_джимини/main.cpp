#include "Header.h"
#include <iostream>
#include <fstream>

int main()
{
    setlocale(LC_ALL, "Russian");

    std::ifstream InFile("FN1.txt");
    if (!InFile.is_open())
    {
        std::cerr << "Критическая ошибка: Файл FN1.txt не найден.\n";
        return 1;
    }

    std::string Expression;
    std::getline(InFile, Expression);
    InFile.close();

    int32 XValue = 0;
    std::cout << "Выражение: " << Expression << "\nВведите x: ";
    while (!(std::cin >> XValue))
    {
        std::cout << "Ошибка! Введите целое число: ";
        ClearInputBuffer();
    }

    TCalcTree Tree;
    if (Tree.BuildFromInfix(Expression))
    {
        // 1. Считаем результат
        int32 Result = Tree.Evaluate(XValue);

        // 2. Создаем файл FN2 и пишем результат
        std::ofstream OutFile("FN2.txt", std::ios::out);
        if (OutFile.is_open())
        {
            OutFile << "Результат для x = " << XValue << ": " << Result << "\n\n";
            OutFile.close();
        }

        // 3. Печать исходного дерева
        Tree.PrintToFile("FN2.txt", "--- Исходное дерево ---");

        // 4. Оптимизация x*A -> A*x
        Tree.OptimizeVariables();

        // 5. Печать измененного дерева
        Tree.PrintToFile("FN2.txt", "--- Дерево после оптимизации (x*A -> A*x) ---");

        std::cout << "Программа успешно завершена. Проверьте файл FN2.txt\n";
    }
    else
    {
        std::cerr << "Ошибка: Не удалось построить дерево.\n";
    }

    return 0;
}
