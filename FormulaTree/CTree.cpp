#include "CTree.h"
#include "CNode.h"
#include <iostream> 
#include <string> 
#include <vector> 
#include <sstream>
#include <queue>
using namespace std;

CTree::CTree()
{
    root = nullptr;

}

CTree::~CTree() {
    deleteTree(root);
}

CNode* CTree::getRoot() const {
    return root;
}

CTree& CTree::operator=(const CTree& other) {
    if (this != &other) {
        deleteTree(root);
        root = copyTree(other.root);
    }
    return *this;
}

// Funkcja do wypisywania drzewa w notacji prefiksowej
void CTree::printTree(CNode* node) {
    if (node) {
        cout << node->value << " ";

       
        if (!node->children.empty()) {
            for (size_t i = 0; i < node->children.size(); ++i) {
                printTree(node->children[i]);
            }
        }
    }
}




double CTree::evaluate(CNode* node, const map<string, double>& values) {
    if (node == nullptr) return 0.0;

    if (node->value == op_plus) {
        double result = 0.0;
        for (const auto& child : node->children) {
            result += evaluate(child, values);
        }
        return result;
    }
    else if (node->value == op_minus) {
        return evaluate(node->children[0], values) - evaluate(node->children[1], values);
    }
    else if (node->value == op_multiply) {
        double result = 1.0;
        for (const auto& child : node->children) {
            result *= evaluate(child, values);
        }
        return result;
    }
    else if (node->value == op_division) {
        return evaluate(node->children[0], values) / evaluate(node->children[1], values);
    }
    else if (node->value == op_sinus) {
        return sin(evaluate(node->children[0], values));
    }
    else if (node->value == op_cosinus) {
        return cos(evaluate(node->children[0], values));
    }
    else if (values.find(node->value) != values.end()) {
        return values.at(node->value);
    }
    else {
        return stod(node->value);
    }
}


void CTree::parseExpression(const string& expression) {
    size_t offset = 0;
    root = parseNode(expression, offset);
}

CNode* CTree::copyTree(const CNode* source) {
    if (!source) {
        return nullptr;
    }

    CNode* newNode = new CNode(source->value);
    for (const auto& child : source->children) {
        newNode->children.push_back(copyTree(child));
    }
    return newNode;
}

void CTree::deleteTree(CNode* node) {
    if (node) {
        for (const auto& child : node->children) {
            deleteTree(child);
        }
        node->children.clear();  
        delete node;
    }
    root = nullptr;  
}

CNode* CTree::parseNode(const string& expression, size_t& offset) {
    
    string value;
    while (offset < expression.size() - 1 && expression[offset] == ' ') {
        offset++;
    }
    while (offset < expression.size() && expression[offset] != ' ') {
        value += expression[offset++];
    }


    if (root == nullptr) {
        root = new CNode(value);
    }
    createTree(root, expression, offset);

    return root;
}

void CTree::createTree(CNode* currentNode, const string& expression, size_t& offset) {
  
    if (!currentNode) {
        return;
    }
    while (currentNode->isClosed) {
        currentNode = currentNode->previous;
    }

    if (currentNode->isVariable() || currentNode->isNumber()) {
        currentNode->isClosed = true;
        currentNode = currentNode->previous;
    }

   
    if (currentNode->isOperator() && currentNode->children.size() == 2) {
        currentNode->isClosed = true;
        currentNode = currentNode->previous;
    }

    while (offset < expression.size()) {
        string value;
        while (offset < expression.size() && expression[offset] == ' ') {
            offset++; 
        }
        
        while (offset < expression.size() && expression[offset] != ' ') {
            value += expression[offset++];
        }
        CNode* newNode = new CNode(value);
        while (currentNode->isClosed) {
            currentNode = currentNode->previous;
        }
        if (currentNode->isVariable() || currentNode->isNumber()) {
            cout << full_childs << endl;
            currentNode = currentNode->previous;
            return;
        }
        else {
            newNode->previous = currentNode;
            currentNode->children.push_back(newNode);
            if (currentNode->isSinCos()) {
                currentNode->isClosed = true;
                createTree(currentNode->previous, expression, offset);
            }
            else {
                createTree(newNode, expression, offset);
            }
        }
    }
}


size_t CTree::numberOfVariablesInTree() const {
    set<string> variables;
    collectVariables(root, variables);
    return variables.size();
}


void CTree::collectVariables(const CNode* node, set<string>& variables) const {
    if (!node) return;

    if (node->isVariable()) {
        variables.insert(node->value);
    }

    for (const auto& child : node->children) {
        collectVariables(child, variables);
    }
}


string CTree::getVariableNameAtIndex(size_t index) const {
    set<string> variables;
    collectVariables(root, variables);

    if (index < variables.size()) {
        auto it = next(variables.begin(), index);
        return *it;
    }
    else {
        cerr << index_error << endl;
        return message_empty;
    }
}


CNode* CTree::mergeTrees(CNode* left, CNode* right) {
    if (!left) return new CNode(right->value);
    if (!right) return new CNode(left->value);
    CNode* lastNode = left->findLastNode(left);
    CNode* lastButOneNode = lastNode->previous;
    delete lastNode;
    lastButOneNode->children[lastButOneNode->children.size() - 1] = copyTree(right);

    return left;
}


CTree& CTree::operator+=(const CTree& other) {
    root = mergeTrees(root, other.root);
    return *this;
}



void CTree::printTreeRows(CNode* node) {
    if (!node) {
        cout << message_empty_tree << endl;
        return;
    }

    queue<CNode*> q;
    q.push(node);

    while (!q.empty()) {
        int lSize = q.size();

        for (int i = 0; i < lSize; ++i) {
            CNode* current = q.front();
            q.pop();

            cout << current->value << " ";

            for (const auto& child : current->children) {
                q.push(child);

            }
            
        }

        cout << endl;
    }
}
