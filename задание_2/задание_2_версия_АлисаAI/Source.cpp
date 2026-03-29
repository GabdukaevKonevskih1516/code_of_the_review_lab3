#include "Header.h"
#include <stack>
#include <cctype>
#include <cmath>
#include <vector>

// Символы псевдографики для твоего формата
const std::string ch_udia_hor = "^--";
const std::string ch_ver_hor = "|-- ";
const std::string ch_ver_spa = "|   ";

int32 GetPriority(EOperationType Op) {
    if (Op == EOperationType::Exponentiation) return 3;
    if (Op == EOperationType::Multiplication || Op == EOperationType::Division || Op == EOperationType::Modulo) return 2;
    if (Op == EOperationType::Addition || Op == EOperationType::Subtraction) return 1;
    return 0;
}

std::string NodeToString(const FTreeNode* Node) {
    if (!Node) return "";
    if (Node->NodeType == ENodeType::Number) return std::to_string(Node->Value);
    if (Node->NodeType == ENodeType::Variable) return "x";
    switch (Node->OperationType) {
    case EOperationType::Addition: return "+";
    case EOperationType::Subtraction: return "-";
    case EOperationType::Multiplication: return "*";
    case EOperationType::Division: return "/";
    case EOperationType::Modulo: return "%";
    case EOperationType::Exponentiation: return "^";
    default: return "?";
    }
}

void ProcessOp(std::stack<std::unique_ptr<FTreeNode>>& Nodes, EOperationType Op) {
    if (Nodes.size() < 2) return;
    auto Right = std::move(Nodes.top()); Nodes.pop();
    auto Left = std::move(Nodes.top()); Nodes.pop();
    auto NewNode = std::make_unique<FTreeNode>(ENodeType::Operation, Op);
    NewNode->LeftChild = std::move(Left);
    NewNode->RightChild = std::move(Right);
    Nodes.push(std::move(NewNode));
}

std::unique_ptr<FTreeNode> BuildExpressionTree(const std::string& InExpression) {
    std::stack<std::unique_ptr<FTreeNode>> NodeStack;
    std::stack<EOperationType> OpStack;

    for (size_t i = 0; i < InExpression.length(); ++i) {
        char ch = InExpression[i];
        if (std::isspace(ch)) continue;

        if (std::isdigit(ch)) {
            int val = 0;
            while (i < InExpression.length() && std::isdigit(InExpression[i])) {
                val = val * 10 + (InExpression[i++] - '0');
            }
            --i;
            NodeStack.push(std::make_unique<FTreeNode>(ENodeType::Number, EOperationType::Addition, val));
        }
        else if (ch == 'x') {
            NodeStack.push(std::make_unique<FTreeNode>(ENodeType::Variable));
        }
        else if (ch == '(') {
            OpStack.push(EOperationType::Parenthesis);
        }
        else if (ch == ')') {
            while (!OpStack.empty() && OpStack.top() != EOperationType::Parenthesis) {
                ProcessOp(NodeStack, OpStack.top());
                OpStack.pop();
            }
            if (!OpStack.empty()) OpStack.pop();
        }
        else {
            EOperationType CurrentOp;
            if (ch == '+') CurrentOp = EOperationType::Addition;
            else if (ch == '-') CurrentOp = EOperationType::Subtraction;
            else if (ch == '*') CurrentOp = EOperationType::Multiplication;
            else if (ch == '/') CurrentOp = EOperationType::Division;
            else if (ch == '%') CurrentOp = EOperationType::Modulo;
            else if (ch == '^') CurrentOp = EOperationType::Exponentiation;
            else continue;

            // Левая ассоциативность: >= обеспечивает порядок слева направо
            while (!OpStack.empty() && OpStack.top() != EOperationType::Parenthesis &&
                GetPriority(OpStack.top()) >= GetPriority(CurrentOp)) {
                ProcessOp(NodeStack, OpStack.top());
                OpStack.pop();
            }
            OpStack.push(CurrentOp);
        }
    }
    while (!OpStack.empty()) {
        ProcessOp(NodeStack, OpStack.top());
        OpStack.pop();
    }
    return std::move(NodeStack.top());
}

int32 EvaluateTree(const FTreeNode* InNode, int32 InX) {
    if (!InNode) return 0;
    if (InNode->NodeType == ENodeType::Number) return InNode->Value;
    if (InNode->NodeType ==


        ENodeType::Variable) return InX;

    int32 L = EvaluateTree(InNode->LeftChild.get(), InX);
    int32 R = EvaluateTree(InNode->RightChild.get(), InX);

    switch (InNode->OperationType) {
    case EOperationType::Addition: return L + R;
    case EOperationType::Subtraction: return L - R;
    case EOperationType::Multiplication: return L * R;
    case EOperationType::Division: return (R != 0) ? L / R : 0;
    case EOperationType::Modulo: return (R != 0) ? L % R : 0;
    case EOperationType::Exponentiation: return static_cast<int32>(pow(L, R));
    default: return 0;
    }
}

void TransformTree(std::unique_ptr<FTreeNode>& InNode) {
    if (!InNode) return;
    TransformTree(InNode->LeftChild);
    TransformTree(InNode->RightChild);

    // Замена x * A на A * x
    if (InNode->NodeType == ENodeType::Operation && InNode->OperationType == EOperationType::Multiplication) {
        if (InNode->LeftChild && InNode->LeftChild->NodeType == ENodeType::Variable) {
            std::swap(InNode->LeftChild, InNode->RightChild);
        }
    }
}

void DumpTree(const FTreeNode* node, std::ostream& out, std::string const& prefix, bool root, bool last) {
    // Вывод текущего узла по твоему формату
    out << prefix << (root ? "" : (last ? ch_udia_hor : ch_ver_hor)) << (node ? NodeToString(node) : "") << std::endl;

    if (!node || (!node->LeftChild && !node->RightChild))
        return;

    // Вектор детей для рекурсии
    std::vector<const FTreeNode*> children;
    if (node->LeftChild) children.push_back(node->LeftChild.get());
    if (node->RightChild) children.push_back(node->RightChild.get());

    for (size_t i = 0; i < children.size(); ++i) {
        DumpTree(children[i], out, prefix + (root ? "" : (last ? "    " : ch_ver_spa)), false, i + 1 >= children.size());
    }
}
