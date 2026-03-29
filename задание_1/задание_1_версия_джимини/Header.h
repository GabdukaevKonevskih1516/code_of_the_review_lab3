// Copyright (c) 2026. All Rights Reserved.

#pragma once

#include <string>
#include <cstdint> // Добавляем этот заголовок

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
 * Генерирует случайный пароль на основе выбранного уровня сложности
 * @param InDifficulty - Выбранная сложность
 * @return Сгенерированная строка пароля
 */
std::string GeneratePassword(EPasswordDifficulty InDifficulty);

/**
 * Очищает поток ввода и обрабатывает ошибки ввода пользователя
 */
void ClearInputBuffer();
