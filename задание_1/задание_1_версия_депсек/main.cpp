#include "Header.h"
#include <iostream>
#include <limits>
#include <cctype>

// Очистка потока ввода после ошибки
void ClearInputStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Получение выбора пользователя с защитой от неверного ввода
int GetUserChoice()
{
    int Choice = 0;
    while (true)
    {
        std::cout << "\nВыберите уровень сложности (1-3): ";

        if (std::cin >> Choice)
        {
            if (Choice >= 1 && Choice <= 3)
            {
                return Choice;
            }
            else
            {
                std::cout << "Ошибка: введите число от 1 до 3.\n";
            }
        }
        else
        {
            std::cout << "Ошибка: пожалуйста, введите числовое значение.\n";
            ClearInputStream();
        }
    }
}

// Получение ответа на вопрос "Хотите продолжить?" с защитой от неверного ввода
bool GetYesNoAnswer()
{
    std::string Answer;
    while (true)
    {
        std::cout << "\nСгенерировать ещё один пароль? (y/n): ";
        std::cin >> Answer;

        if (Answer == "y" || Answer == "Y" || Answer == "yes" || Answer == "Yes")
        {
            return true;
        }
        else if (Answer == "n" || Answer == "N" || Answer == "no" || Answer == "No")
        {
            return false;
        }
        else
        {
            std::cout << "Ошибка: введите 'y' для продолжения или 'n' для выхода.\n";
            ClearInputStream();
        }
    }
}

void DisplayMenu()
{
    std::cout << "\n========================================\n";
    std::cout << "        ГЕНЕРАТОР ПАРОЛЕЙ\n";
    std::cout << "========================================\n";
    std::cout << "1. " << FPasswordGenerator::GetComplexityDescription(EPasswordComplexity::Low) << "\n";
    std::cout << "2. " << FPasswordGenerator::GetComplexityDescription(EPasswordComplexity::Medium) << "\n";
    std::cout << "3. " << FPasswordGenerator::GetComplexityDescription(EPasswordComplexity::High) << "\n";
    std::cout << "========================================\n";
}

int main()
{
    // Установка кодировки для корректного отображения кириллицы (для Windows)
    setlocale(LC_ALL, "Russian");

    FPasswordGenerator Generator;
    bool bRunning = true;

    std::cout << "Добро пожаловать в генератор паролей!\n";

    while (bRunning)
    {
        DisplayMenu();

        int Choice = GetUserChoice();

        EPasswordComplexity Complexity;
        switch (Choice)
        {
        case 1:
            Complexity = EPasswordComplexity::Low;
            break;
        case 2:
            Complexity = EPasswordComplexity::Medium;
            break;
        case 3:
            Complexity = EPasswordComplexity::High;
            break;
        default:
            Complexity = EPasswordComplexity::Low;
            break;
        }

        std::string Password = Generator.GeneratePassword(Complexity);

        std::cout << "\n----------------------------------------\n";
        std::cout << "Сгенерированный пароль: " << Password << "\n";
        std::cout << "----------------------------------------\n";

        bRunning = GetYesNoAnswer();
    }

    std::cout << "\nСпасибо за использование генератора паролей!\n";

    return 0;
}
