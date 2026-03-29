#include "ExpressionTree.h"
#include <iostream>
#include <cmath>
#include <cctype>

// Символы для "бокового" вывода (4 позиции на уровень)
const std::string CH_VER_HOR = "|-- ";
const std::string CH_UDIA_HOR = "^-- ";
const std::string CH_VER_SPA = "|   ";
const std::string CH_SPACE = "    ";

FTreeNode::FTreeNode(int InValue) : Type(ENodeType::Number), Value(InValue), Operator('\0'), Left(nullptr), Right(nullptr) {}
FTreeNode::FTreeNode(char InOp) : Type(ENodeType::Operator), Value(0), Operator(InOp), Left(nullptr), Right(nullptr) {}
FTreeNode::FTreeNode(const std::string& InVar) : Type(ENodeType::Variable), Value(0), Operator('\0'), Left(nullptr), Right(nullptr) {}

FExpressionTree::FExpressionTree() : Root(nullptr) {}

bool FExpressionTree::BuildFromString(const std::string& Expression) {
    std::string Clean;
    for (char c : Expression) if (!isspace(c)) Clean += c;
    if (Clean.empty()) return false;

    size_t Pos = 0;
    Root = ParseExpression(Clean, Pos);
    return Root != nullptr && Pos == Clean.length();
}

std::shared_ptr<FTreeNode> FExpressionTree::ParseExpression(const std::string& Expr, size_t& Pos) {
    auto Node = ParseTerm(Expr, Pos);
    while (Pos < Expr.length() && (Expr[Pos] == '+' || Expr[Pos] == '-')) {
        auto NewRoot = std::make_shared<FTreeNode>(Expr[Pos++]);
        NewRoot->Left = Node;
        NewRoot->Right = ParseTerm(Expr, Pos);
        Node = NewRoot;
    }
    return Node;
}

std::shared_ptr<FTreeNode> FExpressionTree::ParseTerm(const std::string& Expr, size_t& Pos) {
    auto Node = ParseFactor(Expr, Pos);
    while (Pos < Expr.length() && (Expr[Pos] == '*' || Expr[Pos] == '/' || Expr[Pos] == '%')) {
        auto NewRoot = std::make_shared<FTreeNode>(Expr[Pos++]);
        NewRoot->Left = Node;
        NewRoot->Right = ParseFactor(Expr, Pos);
        Node = NewRoot;
    }
    return Node;
}

std::shared_ptr<FTreeNode> FExpressionTree::ParseFactor(const std::string& Expr, size_t& Pos) {
    auto Node = ParsePrimary(Expr, Pos);
    if (Pos < Expr.length() && Expr[Pos] == '^') {
        auto NewRoot = std::make_shared<FTreeNode>(Expr[Pos++]);
        NewRoot->Left = Node;
        NewRoot->Right = ParseFactor(Expr, Pos); // Степень обычно правоассоциативна
        Node = NewRoot;
    }
    return Node;
}

std::shared_ptr<FTreeNode> FExpressionTree::ParsePrimary(const std::string& Expr, size_t& Pos) {
    if (Pos >= Expr.length()) return nullptr;

    if (isdigit(Expr[Pos])) {
        int Val = 0;
        while (Pos < Expr.length() && isdigit(Expr[Pos])) {
            Val = Val * 10 + (Expr[Pos++] - '0');
        }
        if (Val < 1 || Val > 30) {
            std::cerr << "Предупреждение: число " << Val << " вне диапазона [1, 30]\n";
        }
        return std::make_shared<FTreeNode>(Val);
    }
    else if (Expr[Pos] == 'x') {
        Pos++;
        return std::make_shared<FTreeNode>("x");
    }
    else if (Expr[Pos] == '(') {
        Pos++;
        auto Node = ParseExpression(Expr, Pos);
        if (Pos < Expr.length() && Expr[Pos] == ')') Pos++;
        return Node;
    }
    return nullptr;
}

int FExpressionTree::Evaluate(int XValue) const {
    if (!Root) return 0;
    return EvaluateNode(Root, XValue);
}

int FExpressionTree::EvaluateNode(const std::shared_ptr<FTreeNode>& Node, int XValue) const {
    if (Node->Type == ENodeType::Number) return Node->Value;
    if (Node->Type == ENodeType::Variable) return XValue;

    int L = EvaluateNode(Node->Left, XValue);
    int R = EvaluateNode(Node->Right, XValue);

    switch (Node->Operator) {
    case '+': return L + R;
    case '-': return L - R;
    case '*': return L * R;
    case '/': return (R != 0) ? L / R : 0;
    case '%': return (R != 0) ? L % R : 0;
    case '^': return static_cast<int>(std::pow(L, R));
    default: return 0;
    }
}

void FExpressionTree::Transform() {
    TransformNode(Root);
}

void FExpressionTree::TransformNode(std::shared_ptr<FTreeNode>& Node) {


    if (!Node) return;
    TransformNode(Node->Left);
    TransformNode(Node->Right);

    if (Node->Type == ENodeType::Operator && Node->Operator == '*') {
        // Если слева x, а справа НЕ x (или поддерево), меняем местами
        if (Node->Left->Type == ENodeType::Variable && Node->Right->Type != ENodeType::Variable) {
            std::swap(Node->Left, Node->Right);
        }
    }
}

void FExpressionTree::DumpToFile(std::ofstream& OutFile, const std::string& Title) const {
    OutFile << Title << "\n";
    DumpNode(OutFile, Root, "", true, true);
    OutFile << "\n";
}

void FExpressionTree::DumpNode(std::ofstream& Out, const std::shared_ptr<FTreeNode>& Node,
    std::string Prefix, bool bIsRoot, bool bIsLast) const {
    if (!Node) return;

    Out << Prefix;
    if (!bIsRoot) {
        Out << (bIsLast ? CH_UDIA_HOR : CH_VER_HOR);
    }

    if (Node->Type == ENodeType::Number) Out << Node->Value;
    else if (Node->Type == ENodeType::Variable) Out << "x";
    else Out << Node->Operator;
    Out << "\n";

    if (Node->Left || Node->Right) {
        std::vector<std::shared_ptr<FTreeNode>> Children;
        if (Node->Left) Children.push_back(Node->Left);
        if (Node->Right) Children.push_back(Node->Right);

        for (size_t i = 0; i < Children.size(); ++i) {
            std::string NewPrefix = Prefix + (bIsRoot ? "" : (bIsLast ? CH_SPACE : CH_VER_SPA));
            DumpNode(Out, Children[i], NewPrefix, false, i == Children.size() - 1);
        }
    }
}
