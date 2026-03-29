#include <iostream>
#include <fstream>
#include <string>
#include "ExpressionTree.h"

int main() {
    setlocale(LC_ALL, "Russian");
    const std::string FN1 = "input.txt";
    const std::string FN2 = "output.txt";

    try {
        // 1. Читаем выражение
        std::ifstream InFile(FN1);
        if (!InFile.is_open()) throw std::runtime_error("Не найден входной файл " + FN1);

        std::string Expression;
        std::getline(InFile, Expression);
        InFile.close();

        // 2. Строим дерево
        FExpressionTree Tree;
        if (!Tree.BuildFromString(Expression)) {
            throw std::runtime_error("Ошибка при построении дерева из строки.");
        }

        // 3. Запрашиваем x
        int XValue;
        std::cout << "Введите значение x: ";
        if (!(std::cin >> XValue)) throw std::runtime_error("Некорректный ввод x.");

        // 4. Вычисляем результат
        int Result = Tree.Evaluate(XValue);

        // 5. Выводим всё в FN2
        std::ofstream OutFile(FN2);
        if (!OutFile.is_open()) throw std::runtime_error("Не удалось создать выходной файл " + FN2);

        OutFile << "Результат вычисления при x = " << XValue << ": " << Result << "\n\n";

        // Печатаем начальное дерево
        Tree.DumpToFile(OutFile, "=== Исходное дерево ===");

        // Преобразуем (x*A -> A*x)
        Tree.Transform();

        // Печатаем измененное дерево
        Tree.DumpToFile(OutFile, "=== Дерево после преобразования (xA -> A*x) ===");

        OutFile.close();
        std::cout << "Готово! Результаты сохранены в " << FN2 << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
