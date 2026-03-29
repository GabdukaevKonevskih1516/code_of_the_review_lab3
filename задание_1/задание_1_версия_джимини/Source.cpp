// Copyright (c) 2026. All Rights Reserved.

#include "Header.h"
#include <iostream>
#include <vector>
#include <random>
#include <limits>

std::string GeneratePassword(EPasswordDifficulty InDifficulty)
{
	std::string Characters;
	int32 PasswordLength = 0;

	const std::string Lowercase = "abcdefghijklmnopqrstuvwxyz";
	const std::string Uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const std::string Digits = "0123456789";
	const std::string Symbols = "!@#$%^&*()-_=+[]{}<>?/";

	// Настройка параметров сложности
	switch (InDifficulty)
	{
	case EPasswordDifficulty::Low:
		Characters = Lowercase + Digits;
		PasswordLength = 8;
		break;
	case EPasswordDifficulty::Medium:
		Characters = Lowercase + Uppercase + Digits;
		PasswordLength = 12;
		break;
	case EPasswordDifficulty::High:
		Characters = Lowercase + Uppercase + Digits + Symbols;
		PasswordLength = 16;
		break;
	}

	std::string Result = "";

	// Инициализация генератора случайных чисел
	std::random_device RandomDevice;
	std::mt19937 Generator(RandomDevice());
	std::uniform_int_distribution<> Distribution(0, static_cast<int>(Characters.size() - 1));

	for (int32 i = 0; i < PasswordLength; ++i)
	{
		Result += Characters[Distribution(Generator)];
	}

	return Result;
}

void ClearInputBuffer()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}



