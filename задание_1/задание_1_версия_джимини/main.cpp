// Copyright (c) 2026. All Rights Reserved.

#include "Header.h"
#include <iostream>
#include <string>

/**
 * Точка входа в программу.
 * Реализует цикл взаимодействия с пользователем и защиту от некорректного ввода.
 */
int main()
{
	// Установка кодировки для корректного отображения кириллицы (для Windows)
	setlocale(LC_ALL, "Russian");

	int32 UserChoice = 0;
	bool bIsRunning = true;

	while (bIsRunning)
	{
		std::cout << "========== ГЕНЕРАТОР ПАРОЛЕЙ ==========\n";
		std::cout << "Выберите уровень сложности:\n";
		std::cout << "1 - Низкий   (8 символов: строчные буквы и цифры)\n";
		std::cout << "2 - Средний  (12 символов: буквы обоих регистров и цифры)\n";
		std::cout << "3 - Высокий  (16 символов: буквы, цифры и спецсимволы)\n";
		std::cout << "4 - Выход из программы\n";
		std::cout << "---------------------------------------\n";
		std::cout << "Ваш выбор: ";

		// Проверка на ввод (защита от ввода букв вместо цифр)
		if (!(std::cin >> UserChoice))
		{
			std::cout << "\n[Ошибка]: Пожалуйста, введите число от 1 до 4.\n\n";
			ClearInputBuffer();
			continue;
		}

		if (UserChoice == 4)
		{
			std::cout << "Завершение работы. До свидания!\n";
			bIsRunning = false;
			continue;
		}

		EPasswordDifficulty SelectedDifficulty;

		// Обработка выбора пользователя
		if (UserChoice == 1)
		{
			SelectedDifficulty = EPasswordDifficulty::Low;
		}
		else if (UserChoice == 2)
		{
			SelectedDifficulty = EPasswordDifficulty::Medium;
		}
		else if (UserChoice == 3)
		{
			SelectedDifficulty = EPasswordDifficulty::High;
		}
		else
		{
			std::cout << "\n[Ошибка]: Выбран несуществующий пункт меню. Попробуйте снова.\n\n";
			continue;
		}

		// Генерация и вывод результата
		std::string NewPassword = GeneratePassword(SelectedDifficulty);

		std::cout << "\n>>> ВАШ НОВЫЙ ПАРОЛЬ: " << NewPassword << " <<<\n";
		std::cout << "=======================================\n\n";
	}

	return 0;
}
