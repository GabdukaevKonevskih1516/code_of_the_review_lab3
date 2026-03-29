#pragma once

#include <string>
#include <vector>
#include <random>

enum class EPasswordComplexity
{
    Low,      // 6 символов, только строчные буквы
    Medium,   // 10 символов, строчные + прописные + цифры
    High      // 14 символов, строчные + прописные + цифры + спецсимволы
};

class FPasswordGenerator
{
public:
    FPasswordGenerator();

    // Генерирует пароль на основе выбранного уровня сложности
    std::string GeneratePassword(EPasswordComplexity Complexity);

    // Получает описание уровня сложности
    static std::string GetComplexityDescription(EPasswordComplexity Complexity);

    // Получает длину пароля для уровня сложности
    static int GetPasswordLength(EPasswordComplexity Complexity);

private:
    std::mt19937 RandomEngine;

    // Наборы символов
    const std::string LowercaseLetters = "abcdefghijklmnopqrstuvwxyz";
    const std::string UppercaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string Digits = "0123456789";
    const std::string SpecialChars = "!@#$%^&*()_+-=[]{}|;:,.<>?";

    // Формирует набор символов для заданного уровня сложности
    std::string GetCharacterSet(EPasswordComplexity Complexity) const;

    // Генерирует случайный символ из набора
    char GetRandomChar(const std::string& CharSet);
};
