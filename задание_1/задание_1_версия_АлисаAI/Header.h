#pragma once

#include <string>
#include <random>

// Определяем типы в стиле Epic Games для обычного компилятора
typedef std::uint8_t uint8;
typedef std::int32_t int32;

/** Уровни сложности пароля */
enum class EPasswordDifficulty : uint8
{
    Low,
    Medium,
    High
};

/**
 * Класс для генерации случайных паролей
 */
class FPasswordGenerator
{
public:
    /**
     * Конструктор по умолчанию
     */
    FPasswordGenerator();

    /**
     * Генерирует случайный пароль на основе выбранного уровня сложности
     * @param InDifficulty - Выбранная сложность
     * @param InLength - Длина генерируемого пароля
     * @return Сгенерированная строка пароля
     */
    std::string GeneratePassword(EPasswordDifficulty InDifficulty, int32 InLength);

private:
    std::random_device Rd;
    std::mt19937 Gen;

    /** Набор символов для генерации пароля */
    const std::string LowercaseLetters = "abcdefghijklmnopqrstuvwxyz";
    const std::string UppercaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string Digits = "0123456789";
    const std::string SpecialSymbols = "!@#$%^&*()_+-=[]{}|;:,.<>?";
};

/**
 * Очищает поток ввода и обрабатывает ошибки ввода пользователя
 */
void ClearInputBuffer();
