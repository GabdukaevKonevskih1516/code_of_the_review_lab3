#include "Header.h"
#include <iostream>
#include <fstream>

int main() {
    setlocale(LC_ALL, "Russian");

    std::string Expression;
    std::ifstream InFile("FN1.txt");
    if (!InFile) {
        std::cerr << "Ошибка: Файл FN1 не найден!" << std::endl;
        return 1;
    }
    std::getline(InFile, Expression);
    InFile.close();

    int32 XValue;
    std::cout << "Введите значение переменной x: ";
    if (!(std::cin >> XValue)) return 1;

    try {
        auto Root = BuildExpressionTree(Expression);

        std::ofstream OutFile("FN2.txt");

        OutFile << "Начальное дерево" << std::endl;
        DumpTree(Root.get(), OutFile);

        int32 Result = EvaluateTree(Root.get(), XValue);

        TransformTree(Root);

        OutFile << "\nПреобразованное дерево (x*A -> A*x)" << std::endl;
        DumpTree(Root.get(), OutFile);

        OutFile << "\nЗначение выражения при x = " << XValue << ": " << Result << std::endl;
        OutFile.close();

        std::cout << "Выполнено! Результаты в файле FN2." << std::endl;
    }
    catch (...) {
        std::cerr << "Ошибка выполнения." << std::endl;
    }

    return 0;
}
