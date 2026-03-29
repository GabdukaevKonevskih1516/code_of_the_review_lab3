#include "Header.h"
#include <iostream>
#include <limits>

FPasswordGenerator::FPasswordGenerator()
    : Gen(Rd())
{
}

std::string FPasswordGenerator::GeneratePassword(EPasswordDifficulty InDifficulty, int32 InLength)
{
    std::string Password;
    std::string CharacterSet;

    // Формируем набор символов в зависимости от уровня сложности
    switch (InDifficulty)
    {
    case EPasswordDifficulty::Low:
        CharacterSet = LowercaseLetters + UppercaseLetters;
        break;
    case EPasswordDifficulty::Medium:
        CharacterSet = LowercaseLetters + UppercaseLetters + Digits;
        break;
    case EPasswordDifficulty::High:
        CharacterSet = LowercaseLetters + UppercaseLetters + Digits + SpecialSymbols;
        break;
    default:
        throw std::invalid_argument("Неверный уровень сложности пароля!");
    }

    // Генерируем пароль случайной выборкой символов
    for (int32 i = 0; i < InLength; ++i)
    {
        std::uniform_int_distribution<> Dis(0, CharacterSet.length() - 1);
        Password += CharacterSet[Dis(Gen)];
    }

    return Password;
}

/**
 * Очищает буфер ввода и обрабатывает некорректный ввод пользователя
 */
void ClearInputBuffer()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
