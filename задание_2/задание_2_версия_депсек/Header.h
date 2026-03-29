#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <vector>

enum class ENodeType { Number, Variable, Operator };

struct FTreeNode {
    ENodeType Type;
    int Value;
    char Operator;
    std::shared_ptr<FTreeNode> Left;
    std::shared_ptr<FTreeNode> Right;

    FTreeNode(int InValue);
    FTreeNode(char InOp);
    FTreeNode(const std::string& InVar);
};

class FExpressionTree {
public:
    FExpressionTree();

    bool BuildFromString(const std::string& Expression);
    int Evaluate(int XValue) const;
    void Transform();
    void DumpToFile(std::ofstream& OutFile, const std::string& Title) const;

    std::shared_ptr<FTreeNode> GetRoot() const { return Root; }

private:
    std::shared_ptr<FTreeNode> Root;

    // Парсер
    std::shared_ptr<FTreeNode> ParseExpression(const std::string& Expr, size_t& Pos);
    std::shared_ptr<FTreeNode> ParseTerm(const std::string& Expr, size_t& Pos);
    std::shared_ptr<FTreeNode> ParseFactor(const std::string& Expr, size_t& Pos);
    std::shared_ptr<FTreeNode> ParsePrimary(const std::string& Expr, size_t& Pos);

    // Рекурсивные помощники
    int EvaluateNode(const std::shared_ptr<FTreeNode>& Node, int XValue) const;
    void TransformNode(std::shared_ptr<FTreeNode>& Node);
    void DumpNode(std::ofstream& Out, const std::shared_ptr<FTreeNode>& Node,
        std::string Prefix, bool bIsRoot, bool bIsLast) const;
};
