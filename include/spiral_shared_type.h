//
// Created by Sakata on 2021/3/9.
//

#ifndef SPIRAL_SPIRAL_SHARED_TYPE_H
#define SPIRAL_SPIRAL_SHARED_TYPE_H

#include <memory>

namespace spiral {

class ASTree;

class Parameter;

class IValue;

class IntValue;

class FloatValue;

class StringValue;

class FunctionValue;

class DFA;

class IDFANode;

using SIValue = std::shared_ptr<IValue>;
using SIntValue = std::shared_ptr<IntValue>;
using SFloatValue = std::shared_ptr<FloatValue>;
using SStringValue = std::shared_ptr<StringValue>;
using SFunctionValue = std::shared_ptr<FunctionValue>;
using SParameter = std::shared_ptr<Parameter>;

} // namespace spiral


#endif //SPIRAL_SPIRAL_SHARED_TYPE_H
