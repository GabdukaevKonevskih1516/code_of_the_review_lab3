#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

typedef int int32;

enum class ENodeType : unsigned char {
    Number,
    Variable,
    Operation
};

enum class EOperationType : unsigned char {
    Addition, Subtraction, Multiplication, Division, Modulo, Exponentiation, Parenthesis
};

struct FTreeNode {
    ENodeType NodeType;
    EOperationType OperationType;
    int32 Value;
    std::unique_ptr<FTreeNode> LeftChild;
    std::unique_ptr<FTreeNode> RightChild;

    FTreeNode(ENodeType type, EOperationType op = EOperationType::Addition, int32 val = 0)
        : NodeType(type), OperationType(op), Value(val) {
    }
};

// Прототипы функций
std::unique_ptr<FTreeNode> BuildExpressionTree(const std::string& InExpression);
int32 EvaluateTree(const FTreeNode* InNode, int32 InX);
void TransformTree(std::unique_ptr<FTreeNode>& InNode);
// Твой формат dump0, адаптированный под FTreeNode
void DumpTree(const FTreeNode* node, std::ostream& out, std::string const& prefix = "", bool root = true, bool last = true);
