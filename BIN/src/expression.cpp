#include "expression.h"
#include <sstream>

Expression::Expression(Function* function)
    : childs(std::vector<Expression*>())
    , function(function)
{

}

truthTable_t Expression::interpret(context_t* context) {
    return this->function->eval(this, context);
}

std::vector<Expression*> Expression::allNodes() {
    std::vector<Expression*> nodes;
    std::queue<Expression*> queue;
    queue.push(this);
    Expression* node;

    while (! queue.empty()) {
        node = queue.front();
        queue.pop();
        nodes.push_back(node);
        for(auto n: node->childs) {
            queue.push(n);
        }
    }

    return nodes;
}

std::vector<Expression*> Expression::nodesInDeep(int n) {
    std::vector<Expression*> nodes;
    std::queue<std::pair<int, Expression*>> queue;
    queue.push(std::pair<int, Expression*>(n, this));
    std::pair<int, Expression*> node;

    while (! queue.empty()) {
        node = queue.front();
        queue.pop();

        if (node.first > 1) {
            for(auto child: node.second->childs) {
                queue.push(std::pair<int, Expression*>(node.first-1, child));
            }
        }
        else {
            nodes.push_back(node.second);
        }
    }

    return nodes;
}

int Expression::deep() {
    std::queue<std::pair<int, Expression*>> queue;
    std::pair<int, Expression*> node;
    queue.push(std::pair<int, Expression*>(1, this));
    int max_ = 0;
    while (! queue.empty()) {
        node = queue.front();
        queue.pop();
        max_ = std::max(max_, node.first);

        for(auto child: node.second->childs) {
            queue.push(std::pair<int, Expression*>(node.first+1, child));
        }
    }

    return max_;
}

Expression* Expression::copy() {
    Expression* expr = new Expression(this->function);

    for (auto child: this->childs) {
        expr->childs.push_back(child->copy());
    }

    return expr;
}

std::string Expression::toString() {
    std::stringstream str;
    for (size_t i = 0; i < this->childs.size(); ++i) {
        str << this->childs[i]->toString() << " ";
    }
    str << this->function->toString();
    return str.str();
}

Expression::~Expression() {
    for(auto c: this->childs) {
        delete c;
    }
    this->childs.clear();
}
