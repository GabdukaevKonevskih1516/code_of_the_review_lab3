#include "Header.h"
#include <iostream>
#include <string>
#include <limits>

// Очистка потока ввода
void ClearInputStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Получение имени файла от пользователя
std::string GetFileName(const std::string& Prompt)
{
    std::string FileName;
    std::cout << Prompt;
    std::getline(std::cin, FileName);

    // Удаляем пробелы в начале и конце
    size_t Start = FileName.find_first_not_of(" \t\r\n");
    if (Start != std::string::npos)
    {
        size_t End = FileName.find_last_not_of(" \t\r\n");
        FileName = FileName.substr(Start, End - Start + 1);
    }

    return FileName;
}

// Запрос на продолжение работы
bool ContinueProgram()
{
    std::string Answer;
    std::cout << "\nХотите проанализировать другой граф? (y/n): ";
    std::getline(std::cin, Answer);

    return (Answer == "y" || Answer == "Y" || Answer == "yes" || Answer == "Yes");
}

// Демонстрация формата входного файла
void ShowFileFormatExample()
{
    std::cout << "\nПример формата входного файла (input.txt):\n";
    std::cout << "4\n";
    std::cout << "0 1 1 0\n";
    std::cout << "1 0 1 1\n";
    std::cout << "1 1 0 0\n";
    std::cout << "0 1 0 0\n";
    std::cout << "\nГде:\n";
    std::cout << "- Первая строка: количество вершин\n";
    std::cout << "- Последующие строки: матрица смежности (0 - нет ребра, 1 - есть ребро)\n";
    std::cout << "- Петли (единицы на диагонали) учитываются в степени дважды\n";
    std::cout << "========================================\n";
}

int main(int argc, char* argv[])
{

    // Установка кодировки для корректного отображения кириллицы (для Windows)
    setlocale(LC_ALL, "Russian");

    PrintHeader();
    std::cout << "Программа для анализа неориентированных графов\n";
    std::cout << "Входной файл должен содержать матрицу смежности\n\n";

    ShowFileFormatExample();

    bool bContinue = true;

    while (bContinue)
    {
        std::string InputFileName;

        // Если передан аргумент командной строки, используем его
        if (argc >= 2)
        {
            InputFileName = argv[1];
            std::cout << "Используется файл из аргумента командной строки: " << InputFileName << "\n";
        }
        else
        {
            InputFileName = GetFileName("Введите имя входного файла: ");
        }

        if (InputFileName.empty())
        {
            std::cerr << "Ошибка: имя файла не может быть пустым\n";
            if (argc < 2)
            {
                bContinue = ContinueProgram();
                continue;
            }
            else
            {
                return 1;
            }
        }

        // Создаем анализатор и загружаем граф
        FGraphAnalyzer Analyzer;

        if (!Analyzer.LoadFromFile(InputFileName))
        {
            std::cerr << "Не удалось загрузить граф из файла " << InputFileName << "\n";
            if (argc < 2)
            {
                bContinue = ContinueProgram();
                continue;
            }
            else
            {
                return 1;
            }
        }

        // Вычисляем степени вершин
        Analyzer.CalculateDegrees();

        // Выводим информацию на экран
        Analyzer.PrintGraphInfo();

        // Записываем результаты в файл
        std::string OutputFileName = "output.txt";

        // Если передан второй аргумент, используем его как имя выходного файла
        if (argc >= 3)
        {
            OutputFileName = argv[2];
        }

        if (Analyzer.WriteDegreesToFile(OutputFileName))
        {
            std::cout << "Результаты сохранены в файл: " << OutputFileName << "\n";
        }
        else
        {
            std::cerr << "Не удалось сохранить результаты в файл\n";
        }

        if (argc < 2)
        {
            bContinue = ContinueProgram();
        }
        else
        {
            bContinue = false;
        }
    }


    std::cout << "\nПрограмма завершена.\n";
    return 0;
}
