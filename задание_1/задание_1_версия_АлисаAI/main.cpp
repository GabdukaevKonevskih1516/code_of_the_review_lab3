#include "Header.h"
#include <iostream>

int main()
{
    // Установка кодировки для корректного отображения кириллицы (для Windows)
    setlocale(LC_ALL, "Russian");

    FPasswordGenerator PasswordGenerator;
    int32 PasswordLength = 0;
    EPasswordDifficulty Difficulty = EPasswordDifficulty::Low;

    while (true)
    {
        // Запрос длины пароля с валидацией
        std::cout << "Введите длину пароля (от 6 до 32 символов): ";
        while (!(std::cin >> PasswordLength) || PasswordLength < 6 || PasswordLength > 32)
        {
            ClearInputBuffer();
            std::cerr << "Ошибка! Длина должна быть числом от 6 до 32.\n";
            std::cout << "Повторите ввод: ";
        }

        // Запрос уровня сложности с валидацией
        std::cout << "\nВыберите уровень сложности пароля:\n";
        std::cout << "1 - Низкий (только буквы)\n";
        std::cout << "2 - Средний (буквы и цифры)\n";
        std::cout << "3 - Высокий (буквы, цифры и символы)\n";
        std::cout << "Ваш выбор: ";

        int32 Choice = 0;
        while (!(std::cin >> Choice) || Choice < 1 || Choice > 3)
        {
            ClearInputBuffer();
            std::cerr << "Ошибка! Выберите число от 1 до 3.\n";
            std::cout << "Повторите ввод: ";
        }

        // Преобразуем выбор пользователя в enum
        switch (Choice)
        {
        case 1: Difficulty = EPasswordDifficulty::Low; break;
        case 2: Difficulty = EPasswordDifficulty::Medium; break;
        case 3: Difficulty = EPasswordDifficulty::High; break;
        }

        // Генерируем и выводим пароль
        try
        {
            std::string GeneratedPassword = PasswordGenerator.GeneratePassword(Difficulty, PasswordLength);
            std::cout << "Сгенерированный пароль: " << GeneratedPassword << "\n\n";
        }
        catch (const std::exception& Error)
        {
            std::cerr << "Ошибка генерации пароля: " << Error.what() << "\n";
        }

        // Запрос на продолжение
        char Continue;
        std::cout << "Хотите сгенерировать ещё один пароль? (y/n): ";
        std::cin >> Continue;
        ClearInputBuffer();

        if (Continue != 'y' && Continue != 'Y')
        {
            break;
        }
    }

    return 0;
}
