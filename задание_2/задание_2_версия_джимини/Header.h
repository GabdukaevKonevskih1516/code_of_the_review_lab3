// Copyright (c) 2026. All Rights Reserved.
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cstdint>

typedef int32_t int32;
typedef uint8_t uint8;

/** Типы узлов в дереве выражения */
enum class ENodeType : uint8
{
    Operator,
    Operand,
    Variable
};

/** Узел бинарного дерева выражения */
struct FExpressionNode
{
    ENodeType Type;
    std::string Value;
    std::shared_ptr<FExpressionNode> Left = nullptr;
    std::shared_ptr<FExpressionNode> Right = nullptr;

    FExpressionNode(ENodeType InType, std::string InValue)
        : Type(InType), Value(InValue) {
    }
};

/** Класс для обработки и преобразования дерева арифметических выражений */
class TCalcTree
{
public:
    TCalcTree() = default;

    /** Строит дерево из инфиксной строки */
    bool BuildFromInfix(const std::string& InExpression);

    /** Вычисляет значение выражения при заданном x */
    int32 Evaluate(int32 InXValue) const;

    /** Преобразует поддеревья вида x*A в A*x */
    void OptimizeVariables();

    /** Печать дерева в файл с поясняющим сообщением */
    void PrintToFile(const std::string& InFileName, const std::string& Message) const;

private:
    std::shared_ptr<FExpressionNode> Root = nullptr;

    int32 GetPriority(char Op) const;
    int32 CalculateRecursive(std::shared_ptr<FExpressionNode> Node, int32 InX) const;
    void OptimizeRecursive(std::shared_ptr<FExpressionNode> Node);
    void DumpRecursive(std::shared_ptr<FExpressionNode> Node, std::string Prefix, bool bIsRoot, bool bIsLast, std::ofstream& OutFile) const;
};

/** Вспомогательные функции */
void ClearInputBuffer();
