#include "Header.h"
#include <algorithm>
#include <chrono>

FPasswordGenerator::FPasswordGenerator()
{
    // Инициализация генератора случайных чисел с использованием энтропии
    auto Seed = std::chrono::steady_clock::now().time_since_epoch().count();
    RandomEngine.seed(static_cast<unsigned int>(Seed));
}

std::string FPasswordGenerator::GetCharacterSet(EPasswordComplexity Complexity) const
{
    switch (Complexity)
    {
    case EPasswordComplexity::Low:
        return LowercaseLetters;
    case EPasswordComplexity::Medium:
        return LowercaseLetters + UppercaseLetters + Digits;
    case EPasswordComplexity::High:
        return LowercaseLetters + UppercaseLetters + Digits + SpecialChars;
    default:
        return LowercaseLetters;
    }
}

char FPasswordGenerator::GetRandomChar(const std::string& CharSet)
{
    std::uniform_int_distribution<> Distribution(0, static_cast<int>(CharSet.size()) - 1);
    return CharSet[Distribution(RandomEngine)];
}

std::string FPasswordGenerator::GeneratePassword(EPasswordComplexity Complexity)
{
    const int Length = GetPasswordLength(Complexity);
    const std::string CharSet = GetCharacterSet(Complexity);

    if (CharSet.empty())
    {
        return "";
    }

    std::string Password;
    Password.reserve(Length);

    for (int i = 0; i < Length; ++i)
    {
        Password += GetRandomChar(CharSet);
    }

    // Для высокого уровня сложности гарантируем наличие хотя бы одного символа из каждой категории
    if (Complexity == EPasswordComplexity::High)
    {
        bool bHasLower = false;
        bool bHasUpper = false;
        bool bHasDigit = false;
        bool bHasSpecial = false;

        for (char C : Password)
        {
            if (islower(C)) bHasLower = true;
            else if (isupper(C)) bHasUpper = true;
            else if (isdigit(C)) bHasDigit = true;
            else bHasSpecial = true;
        }

        // Если какой-то категории не хватает, перегенерируем пароль
        if (!bHasLower || !bHasUpper || !bHasDigit || !bHasSpecial)
        {
            return GeneratePassword(Complexity);
        }
    }
    // Для среднего уровня гарантируем наличие строчных, прописных и цифр
    else if (Complexity == EPasswordComplexity::Medium)
    {
        bool bHasLower = false;
        bool bHasUpper = false;
        bool bHasDigit = false;

        for (char C : Password)
        {
            if (islower(C)) bHasLower = true;
            else if (isupper(C)) bHasUpper = true;
            else if (isdigit(C)) bHasDigit = true;
        }

        if (!bHasLower || !bHasUpper || !bHasDigit)
        {
            return GeneratePassword(Complexity);
        }
    }

    return Password;
}

int FPasswordGenerator::GetPasswordLength(EPasswordComplexity Complexity)
{
    switch (Complexity)
    {
    case EPasswordComplexity::Low:
        return 6;
    case EPasswordComplexity::Medium:
        return 10;
    case EPasswordComplexity::High:
        return 14;
    default:
        return 8;
    }
}

std::string FPasswordGenerator::GetComplexityDescription(EPasswordComplexity Complexity)
{
    switch (Complexity)
    {
    case EPasswordComplexity::Low:
        return "Низкий (6 символов, только строчные буквы)";
    case EPasswordComplexity::Medium:
        return "Средний (10 символов, строчные + прописные + цифры)";
    case EPasswordComplexity::High:
        return "Высокий (14 символов, строчные + прописные + цифры + спецсимволы)";
    default:
        return "Неизвестный уровень";
    }
}
