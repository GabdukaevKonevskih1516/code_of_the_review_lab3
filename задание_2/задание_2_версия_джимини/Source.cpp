#include "Header.h"
#include <stack>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

int32 TCalcTree::GetPriority(char Op) const
{
    if (Op == '+' || Op == '-') return 1;
    if (Op == '*' || Op == '/' || Op == '%') return 2;
    if (Op == '^') return 3;
    return 0;
}

bool TCalcTree::BuildFromInfix(const std::string& InExpression)
{
    std::stack<std::shared_ptr<FExpressionNode>> Nodes;
    std::stack<char> Operators;

    auto ProcessOperator = [&]() {
        if (Nodes.size() < 2 || Operators.empty()) return;
        char Op = Operators.top(); Operators.pop();
        auto Right = Nodes.top(); Nodes.pop();
        auto Left = Nodes.top(); Nodes.pop();
        auto NewNode = std::make_shared<FExpressionNode>(ENodeType::Operator, std::string(1, Op));
        NewNode->Left = Left;
        NewNode->Right = Right;
        Nodes.push(NewNode);
        };

    for (size_t i = 0; i < InExpression.length(); ++i)
    {
        if (isspace(InExpression[i])) continue;

        if (isdigit(InExpression[i]))
        {
            std::string Num;
            while (i < InExpression.length() && isdigit(InExpression[i])) Num += InExpression[i++];
            --i;
            Nodes.push(std::make_shared<FExpressionNode>(ENodeType::Operand, Num));
        }
        else if (InExpression[i] == 'x')
        {
            Nodes.push(std::make_shared<FExpressionNode>(ENodeType::Variable, "x"));
        }
        else if (InExpression[i] == '(')
        {
            Operators.push('(');
        }
        else if (InExpression[i] == ')')
        {
            while (!Operators.empty() && Operators.top() != '(') ProcessOperator();
            if (!Operators.empty()) Operators.pop();
        }
        else
        {
            while (!Operators.empty() && Operators.top() != '(' &&
                GetPriority(Operators.top()) >= GetPriority(InExpression[i]))
            {
                ProcessOperator();
            }
            Operators.push(InExpression[i]);
        }
    }
    while (!Operators.empty()) ProcessOperator();
    if (!Nodes.empty()) Root = Nodes.top();
    return Root != nullptr;
}

int32 TCalcTree::Evaluate(int32 InXValue) const
{
    return CalculateRecursive(Root, InXValue);
}

int32 TCalcTree::CalculateRecursive(std::shared_ptr<FExpressionNode> Node, int32 InX) const
{
    if (!Node) return 0;
    if (Node->Type == ENodeType::Operand) return std::stoi(Node->Value);
    if (Node->Type == ENodeType::Variable) return InX;

    int32 L = CalculateRecursive(Node->Left, InX);
    int32 R = CalculateRecursive(Node->Right, InX);

    if (Node->Value == "+") return L + R;
    if (Node->Value == "-") return L - R;
    if (Node->Value == "*") return L * R;
    if (Node->Value == "/") return (R != 0) ? L / R : 0;
    if (Node->Value == "%") return (R != 0) ? L % R : 0;
    if (Node->Value == "^") return static_cast<int32>(std::pow(L, R));
    return 0;
}

void TCalcTree::OptimizeVariables()
{
    OptimizeRecursive(Root);
}

void TCalcTree::OptimizeRecursive(std::shared_ptr<FExpressionNode> Node)
{
    if (!Node) return;
    OptimizeRecursive(Node->Left);
    OptimizeRecursive(Node->Right);

    if (Node->Value == "*" && Node->Left && Node->Left->Type == ENodeType::Variable)
    {
        std::swap(Node->Left, Node->Right);
    }
}

void TCalcTree::PrintToFile(const std::string& InFileName, const std::string& Message) const
{
    std::ofstream OutFile(InFileName, std::ios::app);
    if (OutFile.is_open())
    {
        OutFile << Message << "\n";
        DumpRecursive(Root, "", true, true, OutFile);
        OutFile << "\n";
        OutFile.close();
    }
}

void TCalcTree::DumpRecursive(std::shared_ptr<FExpressionNode> Node, std::string Prefix, bool bIsRoot, bool bIsLast, std::ofstream& OutFile) const
{
    if (!Node) return;

    OutFile << Prefix << (bIsRoot ? "" : (bIsLast ? "^-- " : "|-- "))
        << std::setw(4) << std::right << Node->Value << "\n";


    std::vector<std::shared_ptr<FExpressionNode>> Children;
    if (Node->Left) Children.push_back(Node->Left);
    if (Node->Right) Children.push_back(Node->Right);

    for (size_t i = 0; i < Children.size(); ++i)
    {
        DumpRecursive(Children[i], Prefix + (bIsRoot ? "" : (bIsLast ? "    " : "|  ")), false, i + 1 >= Children.size(), OutFile);
    }
}

void ClearInputBuffer()
{
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}
