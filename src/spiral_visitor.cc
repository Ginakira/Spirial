//
// Created by Sakata on 2021/3/1.
//

#include "spiral_visitor.h"

namespace spiral {

void ConvertToStringVisitor::visit(spiral::IntValue *obj) {
    this->_result = std::to_string(obj->val());
}

void ConvertToStringVisitor::visit(spiral::FloatValue *obj) {
    this->_result = std::to_string(obj->val());
}

void ConvertToStringVisitor::visit(spiral::StringValue *obj) {
    this->_result = obj->val();
}

void ConvertToStringVisitor::visit(FunctionValue *obj) {
    this->_result = obj->val();
}

std::string ConvertToStringVisitor::result() const {
    return this->_result;
}

// Base operator
ValueOperator::ValueOperator(IValue *lvalue, op_type op) : lvalue(lvalue), op(op) {}

void ValueOperator::visit(IntValue *obj) { (this->lvalue->*op)(*obj); }

void ValueOperator::visit(FloatValue *obj) { (this->lvalue->*op)(*obj); }

void ValueOperator::visit(StringValue *obj) { (this->lvalue->*op)(*obj); }

void ValueOperator::visit(FunctionValue *obj) { (this->lvalue->*op)(*obj); }

// Value base Operator
IntValueOperator::IntValueOperator(IntValue *left, op_type op) : ValueOperator(left, op), left(left) {}

FloatValueOperator::FloatValueOperator(FloatValue *left, op_type op) : ValueOperator(left, op), left(left) {}

StringValueOperator::StringValueOperator(StringValue *left, op_type op) : ValueOperator(left, op), left(left) {}


// Int operators
// Int plus
IntValuePlusOperatorVisitor::IntValuePlusOperatorVisitor(IntValue *left)
        : IntValueOperator(left, &IValue::operator_plus_error) {}

void IntValuePlusOperatorVisitor::visit(IntValue *obj) {
    this->_result = std::make_shared<IntValue>(this->left->val() + obj->val());
}

void IntValuePlusOperatorVisitor::visit(FloatValue *obj) {
    this->_result = std::make_shared<FloatValue>(this->left->val() + obj->val());
}

SIValue IntValuePlusOperatorVisitor::result() const {
    return this->_result;
}

// Int Minus
IntValueMinusOperatorVisitor::IntValueMinusOperatorVisitor(IntValue *left)
        : IntValueOperator(left, &IValue::operator_minus_error) {}

void IntValueMinusOperatorVisitor::visit(IntValue *obj) {
    this->_result = std::make_shared<IntValue>(this->left->val() - obj->val());
}

void IntValueMinusOperatorVisitor::visit(FloatValue *obj) {
    this->_result = std::make_shared<FloatValue>(this->left->val() - obj->val());
}

SIValue IntValueMinusOperatorVisitor::result() const {
    return this->_result;
}

// Int Times
IntValueTimesOperatorVisitor::IntValueTimesOperatorVisitor(IntValue *left)
        : IntValueOperator(left, &IValue::operator_times_error) {}

void IntValueTimesOperatorVisitor::visit(IntValue *obj) {
    this->_result = std::make_shared<IntValue>(this->left->val() * obj->val());
}

void IntValueTimesOperatorVisitor::visit(FloatValue *obj) {
    this->_result = std::make_shared<FloatValue>(this->left->val() * obj->val());
}

SIValue IntValueTimesOperatorVisitor::result() const {
    return this->_result;
}

// Int Div
IntValueDivOperatorVisitor::IntValueDivOperatorVisitor(IntValue *left)
        : IntValueOperator(left, &IValue::operator_div_error) {}

void IntValueDivOperatorVisitor::visit(IntValue *obj) {
    if (obj->val() == 0) {
        throw std::runtime_error("Division by zero, left value is " + std::to_string(this->left->val()));
    }
    this->_result = std::make_shared<IntValue>(this->left->val() / obj->val());
}

void IntValueDivOperatorVisitor::visit(FloatValue *obj) {
    if (obj->val() == 0.0) {
        throw std::runtime_error("Division by zero, left value is " + std::to_string(this->left->val()));
    }
    this->_result = std::make_shared<FloatValue>(this->left->val() / obj->val());
}

SIValue IntValueDivOperatorVisitor::result() const {
    return this->_result;
}

// Int Mod
IntValueModOperatorVisitor::IntValueModOperatorVisitor(IntValue *left)
        : IntValueOperator(left, &IValue::operator_mod_error) {}

void IntValueModOperatorVisitor::visit(IntValue *obj) {
    if (obj->val() == 0) {
        throw std::runtime_error("Mod by zero, left value is " + std::to_string(this->left->val()));
    }
    this->_result = std::make_shared<IntValue>(this->left->val() % obj->val());
}


SIValue IntValueModOperatorVisitor::result() const {
    return this->_result;
}

// Int Little
IntValueLittleOperatorVisitor::IntValueLittleOperatorVisitor(IntValue *left)
        : IntValueOperator(left, &IValue::operator_compare_error) {}

void IntValueLittleOperatorVisitor::visit(IntValue *obj) {
    this->_result = (left->val() < obj->val()) ? spiral::true_val : spiral::false_val;
}

void IntValueLittleOperatorVisitor::visit(FloatValue *obj) {
    this->_result = (left->val() < obj->val()) ? spiral::true_val : spiral::false_val;
}

SIValue IntValueLittleOperatorVisitor::result() const {
    return this->_result;
}


// Float operators
// Float plus
FloatValuePlusOperatorVisitor::FloatValuePlusOperatorVisitor(FloatValue *left)
        : FloatValueOperator(left, &IValue::operator_plus_error) {}

void FloatValuePlusOperatorVisitor::visit(IntValue *obj) {
    this->_result = std::make_shared<FloatValue>(this->left->val() + obj->val());
}

void FloatValuePlusOperatorVisitor::visit(FloatValue *obj) {
    this->_result = std::make_shared<FloatValue>(this->left->val() + obj->val());
}

SIValue FloatValuePlusOperatorVisitor::result() const {
    return this->_result;
}

// Float Minus
FloatValueMinusOperatorVisitor::FloatValueMinusOperatorVisitor(FloatValue *left)
        : FloatValueOperator(left, &IValue::operator_minus_error) {}

void FloatValueMinusOperatorVisitor::visit(IntValue *obj) {
    this->_result = std::make_shared<FloatValue>(this->left->val() - obj->val());
}

void FloatValueMinusOperatorVisitor::visit(FloatValue *obj) {
    this->_result = std::make_shared<FloatValue>(this->left->val() - obj->val());
}

SIValue FloatValueMinusOperatorVisitor::result() const {
    return this->_result;
}

// Float Times
FloatValueTimesOperatorVisitor::FloatValueTimesOperatorVisitor(FloatValue *left)
        : FloatValueOperator(left, &IValue::operator_times_error) {}

void FloatValueTimesOperatorVisitor::visit(IntValue *obj) {
    this->_result = std::make_shared<FloatValue>(this->left->val() * obj->val());
}

void FloatValueTimesOperatorVisitor::visit(FloatValue *obj) {
    this->_result = std::make_shared<FloatValue>(this->left->val() * obj->val());
}

SIValue FloatValueTimesOperatorVisitor::result() const {
    return this->_result;
}

// Float Div
FloatValueDivOperatorVisitor::FloatValueDivOperatorVisitor(FloatValue *left)
        : FloatValueOperator(left, &IValue::operator_div_error) {}

void FloatValueDivOperatorVisitor::visit(IntValue *obj) {
    if (obj->val() == 0) {
        throw std::runtime_error("Division by zero, left value is " + std::to_string(this->left->val()));
    }
    this->_result = std::make_shared<FloatValue>(this->left->val() / obj->val());
}

void FloatValueDivOperatorVisitor::visit(FloatValue *obj) {
    if (obj->val() == 0.0) {
        throw std::runtime_error("Division by zero, left value is " + std::to_string(this->left->val()));
    }
    this->_result = std::make_shared<FloatValue>(this->left->val() / obj->val());
}

SIValue FloatValueDivOperatorVisitor::result() const {
    return this->_result;
}

// Float Little
FloatValueLittleOperatorVisitor::FloatValueLittleOperatorVisitor(FloatValue *left)
        : FloatValueOperator(left, &IValue::operator_compare_error) {}

void FloatValueLittleOperatorVisitor::visit(IntValue *obj) {
    this->_result = (left->val() < obj->val()) ? spiral::true_val : spiral::false_val;
}

void FloatValueLittleOperatorVisitor::visit(FloatValue *obj) {
    this->_result = (left->val() < obj->val()) ? spiral::true_val : spiral::false_val;
}

SIValue FloatValueLittleOperatorVisitor::result() const {
    return this->_result;
}


// String operators
// String plus
StringValuePlusOperatorVisitor::StringValuePlusOperatorVisitor(StringValue *left)
        : StringValueOperator(left, &IValue::operator_plus_error) {}

void StringValuePlusOperatorVisitor::visit(IntValue *obj) {
    ConvertToStringVisitor vis;
    obj->accept(&vis);
    this->_result = std::make_shared<StringValue>(this->left->val() + vis.result());
}

void StringValuePlusOperatorVisitor::visit(FloatValue *obj) {
    ConvertToStringVisitor vis;
    obj->accept(&vis);
    this->_result = std::make_shared<StringValue>(this->left->val() + vis.result());
}

void StringValuePlusOperatorVisitor::visit(StringValue *obj) {
    this->_result = std::make_shared<StringValue>(this->left->val() + obj->val());
}

SIValue StringValuePlusOperatorVisitor::result() const {
    return this->_result;
}

// String times
StringValueTimesOperatorVisitor::StringValueTimesOperatorVisitor(StringValue *left)
        : StringValueOperator(left, &IValue::operator_times_error) {}


void StringValueTimesOperatorVisitor::visit(IntValue *obj) {
    std::string ret;
    for (int i = 0, n = obj->val(); i < n; ++i) {
        ret += this->left->val();
    }
    this->_result = std::make_shared<StringValue>(ret);
}

SIValue StringValueTimesOperatorVisitor::result() const {
    return this->_result;
}

// String Little
StringValueLittleOperatorVisitor::StringValueLittleOperatorVisitor(StringValue *left)
        : StringValueOperator(left, &IValue::operator_compare_error) {}

void StringValueLittleOperatorVisitor::visit(StringValue *obj) {
    this->_result = (left->val() < obj->val()) ? spiral::true_val : spiral::false_val;
}

SIValue StringValueLittleOperatorVisitor::result() const {
    return this->_result;
}

} // namespace spiral