//
// Created by Sakata on 2021/3/12.
//

#include "spiral_runtime.h"
#include "spiral_parameter.h"
#include "spiral_tree.h"
#include "spiral_type.h"
#include "spiral_dfa.h"

namespace spiral {

std::stack<IDFANode *> DFA::breakPoint;
std::stack<IDFANode *> DFA::continuePoint;
std::stack<IDFANode *> DFA::returnPoint;
int DFA::blockPosition = 100;

DFA::DFA() : head(nullptr), tail(nullptr) {}

DFA::DFA(ASTree *tree) {
    DFA *ret = DFA::build(tree);
    this->head = ret->head;
    this->tail = ret->tail;
}

void DFA::run(SParameter &p) {
    IDFANode *current = this->head;
    while (current) {
        current = current->next(p);
    }
}

DFA *DFA::build(ASTree *tree) {
    DFA *ret = new DFA();
    switch (tree->type()) {
        case BREAK: {
            if (breakPoint.empty()) {
                throw std::runtime_error("Cannot use break outside of a loop!");
            }
            ret->head = ret->tail = new JumpDFANode(breakPoint.top());
            break;
        }
        case CONTINUE: {
            if (continuePoint.empty()) {
                throw std::runtime_error("Cannot use continue outside of a loop!");
            }
            ret->head = ret->tail = new JumpDFANode(continuePoint.top());
            break;
        }
        case IF: {
            DFA *temp;
            ret->head = new ConditionDFANode(tree->at(0));
            ret->tail = new BlankDFANode();
            temp = DFA::build(tree->at(1));
            ret->head->at(0) = temp->head;
            temp->tail->at(0) = ret->tail;
            if (tree->size() == 3) { // 存在else子句
                temp = DFA::build(tree->at(2));
                ret->head->at(1) = temp->head;
                temp->tail->at(0) = ret->tail;
            } else {
                ret->head->at(1) = ret->tail;
            }
            break;
        }
        case BLOCK: {
            ++DFA::blockPosition;
            ret->head = new BlockBeginDFANode(DFA::blockPosition);
            ret->tail = new BlockEndDFANode(DFA::blockPosition);
            DFA *temp;
            IDFANode *p = ret->head;
            for (int i = 0; i < tree->size(); ++i) {
                temp = DFA::build(tree->at(i));
                p->at(0) = temp->head;
                p = temp->tail;
            }
            p->at(0) = ret->tail;
            break;
        }
        case WHILE: {
            ++DFA::blockPosition;
            ret->head = new BlockBeginDFANode(DFA::blockPosition);
            ret->tail = new BlockEndDFANode(DFA::blockPosition);
            IDFANode *blank_node = new BlankDFANode(DFA::blockPosition);
            breakPoint.push(ret->tail);
            continuePoint.push(blank_node);

            IDFANode *condition_node = new ConditionDFANode(tree->at(0));
            DFA *stmt = DFA::build(tree->at(1));
            ret->head->at(0) = condition_node;
            condition_node->at(0) = stmt->head;
            condition_node->at(1) = ret->tail;
            stmt->tail->at(0) = condition_node;
            blank_node->at(0) = condition_node;

            breakPoint.pop();
            continuePoint.pop();
            break;
        }
        case DOWHILE: {
            ++DFA::blockPosition;
            ret->head = new BlockBeginDFANode(DFA::blockPosition);
            ret->tail = new BlockEndDFANode(DFA::blockPosition);
            IDFANode *blank_node = new BlankDFANode(DFA::blockPosition);
            breakPoint.push(ret->tail);
            continuePoint.push(blank_node);

            IDFANode *condition_node = new ConditionDFANode(tree->at(0));
            DFA *stmt = DFA::build(tree->at(1));
            ret->head->at(0) = stmt->head;
            stmt->tail = condition_node;
            condition_node->at(0) = stmt->head;
            condition_node->at(1) = ret->tail;
            blank_node->at(0) = condition_node;

            breakPoint.pop();
            continuePoint.pop();
            break;
        }
        case FOR: {
            ++DFA::blockPosition;
            ret->head = new BlockBeginDFANode(DFA::blockPosition);
            ret->tail = new BlockEndDFANode(DFA::blockPosition);
            IDFANode *blank_node = new BlankDFANode(DFA::blockPosition);
            breakPoint.push(ret->tail);
            continuePoint.push(blank_node);

            IDFANode *init_node = new ExprDFANode(tree->at(0));
            IDFANode *condition_node = new ConditionDFANode(tree->at(1));
            IDFANode *do_node = new ExprDFANode(tree->at(2));

            DFA *stmt = DFA::build(tree->at(3));
            ret->head->at(0) = init_node;
            init_node->at(0) = condition_node;
            condition_node->at(0) = stmt->head;
            condition_node->at(1) = ret->tail;
            stmt->tail->at(0) = do_node;
            do_node->at(0) = condition_node;
            blank_node->at(0) = do_node;

            breakPoint.pop();
            continuePoint.pop();
            break;
        }
        case FUNCTION: {
            auto *return_node = new BlankDFANode();

            returnPoint.push(return_node);
            DFA *dfa = DFA::build(tree->at(2));
            returnPoint.pop();

            return_node->set_pos(dynamic_cast<BlockBeginDFANode *>(dfa->head)->pos());
            return_node->at(0) = dfa->tail;

            ret->head = new DefineFunctionDFANode(tree, dfa);
            ++blockPosition;
            ret->tail = new BlockBeginDFANode(blockPosition);
            ret->head->at(0) = ret->tail;

            break;
        }
        case RETURN: {
            if (returnPoint.empty()) {
                throw std::runtime_error("Cannot use return outside of a function!");
            }
            ret->head = ret->tail = new ReturnDFANode(returnPoint.top(), tree->size() ? tree->at(0) : nullptr);
            break;
        }
        default: {
            ret->head = ret->tail = new ExprDFANode(tree);
            break;
        }
    }
    return ret;
}


// DFA Node constructor
IDFANode::IDFANode(ASTree *tree, int n, std::string _type)
        : tree(tree), children(n), _type(std::move(_type)) {}

SingleDFANode::SingleDFANode(ASTree *tree, std::string _type)
        : IDFANode(tree, 1, std::move(_type)) {}

MultiDFANode::MultiDFANode(ASTree *tree, std::string _type)
        : IDFANode(tree, 2, std::move(_type)) {}

ExprDFANode::ExprDFANode(ASTree *tree)
        : SingleDFANode(tree, "ExprDFANode") {}

BlockBeginDFANode::BlockBeginDFANode(int position)
        : SingleDFANode(nullptr, "BlockBeginDFANode"), _pos(position) {}

BlockEndDFANode::BlockEndDFANode(int position)
        : SingleDFANode(nullptr, "BlockEndDFANode"), _pos(position) {}

ConditionDFANode::ConditionDFANode(ASTree *tree)
        : MultiDFANode(tree, "ConditionDFANode") {}

JumpDFANode::JumpDFANode(IDFANode *node)
        : SingleDFANode(nullptr, "JumpDFANode"), jump_node(node) {}

BlankDFANode::BlankDFANode(int position)
        : SingleDFANode(nullptr, "BlankDFANode"), _pos(position) {}

DefineFunctionDFANode::DefineFunctionDFANode(ASTree *tree, DFA *dfa)
        : SingleDFANode(nullptr, "DefineFunctionDFANode"),
          func(std::make_shared<FunctionValue>(tree, dfa)) {}

ReturnDFANode::ReturnDFANode(IDFANode *jump_node, ASTree *tree) : JumpDFANode(jump_node), tree(tree) {}

IDFANode *&IDFANode::at(int index) {
    return this->children[index];
}

std::string IDFANode::type() {
    return this->_type;
}

IDFANode *ExprDFANode::next(SParameter &p) {
    RuntimeEnv::getValue(this->tree, p);
    return this->at(0);
}

IDFANode *BlockBeginDFANode::next(SParameter &p) {
    p = std::make_shared<Parameter>(p, this->pos());
    return this->at(0);
}

int BlockBeginDFANode::pos() const {
    return this->_pos;
}

IDFANode *BlockEndDFANode::next(SParameter &p) {
    while (p->position() != this->pos()) {
        p = p->next();
    }
    p = p->next();
    return this->at(0);
}

int BlockEndDFANode::pos() const {
    return this->_pos;
}

IDFANode *ConditionDFANode::next(SParameter &p) {
    SIValue val = RuntimeEnv::getValue(this->tree, p);
    return val->isTrue() ? this->at(0) : this->at(1);
}

IDFANode *JumpDFANode::next(SParameter &p) {
    return this->jump_node;
}

IDFANode *BlankDFANode::next(SParameter &p) {
    if (this->pos() > 0) {
        while (p->position() != this->pos()) {
            p = p->next();
        }
    }
    return this->at(0);
}

void BlankDFANode::set_pos(int pos) {
    this->_pos = pos;
}

int BlankDFANode::pos() const {
    return this->_pos;
}

IDFANode *DefineFunctionDFANode::next(SParameter &p) {
    SFunctionValue copy_func = std::make_shared<FunctionValue>(*(this->func));
    p->define_param(copy_func->name());
    copy_func->set_init_param(p);
    p->set(copy_func->name(), copy_func);
    return this->at(0);
}

IDFANode *ReturnDFANode::next(SParameter &p) {
    SIValue ret = spiral::null_val;
    if (this->tree != nullptr) {
        ret = RuntimeEnv::getValue(this->tree, p);
    }
    p->set(spiral::ReturnValueName, ret);
    return JumpDFANode::next(p);

}

} // namespace spiral