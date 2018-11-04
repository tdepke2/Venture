#include "Condition.h"
#include <stdexcept>

using namespace std;

const unordered_map<string, Condition::Type> Condition::STRING_TO_TYPE = {
    {"False", Condition::False}, {"True", Condition::True}
};
const unordered_map<string, Condition::LogicOp> Condition::STRING_TO_LOGIC_OP = {
    {"None", Condition::None}, {"And", Condition::And}, {"Or", Condition::Or}, {"AndNot", Condition::AndNot}, {"OrNot", Condition::OrNot}
};

Condition::Type Condition::stringToType(const string& s) {
    Type type = typeSize;
    auto mapIter = STRING_TO_TYPE.find(s);
    if (mapIter != STRING_TO_TYPE.end()) {
        type = mapIter->second;
    }
    if (type == typeSize) {
        throw runtime_error("Invalid type (" + s + ").");
    }
    return type;
}

const string& Condition::typeToString(Condition::Type t) {
    for (auto mapIter = STRING_TO_TYPE.begin(); mapIter != STRING_TO_TYPE.end(); ++mapIter) {
        if (mapIter->second == t) {
            return mapIter->first;
        }
    }
    throw runtime_error("Invalid type (" + to_string(t) + ").");
}

Condition::LogicOp Condition::stringToLogicOp(const string& s) {
    LogicOp logicOp = logicOpSize;
    auto mapIter = STRING_TO_LOGIC_OP.find(s);
    if (mapIter != STRING_TO_LOGIC_OP.end()) {
        logicOp = mapIter->second;
    }
    if (logicOp == logicOpSize) {
        throw runtime_error("Invalid operator (" + s + ").");
    }
    return logicOp;
}

const string& Condition::logicOpToString(Condition::LogicOp l) {
    for (auto mapIter = STRING_TO_LOGIC_OP.begin(); mapIter != STRING_TO_LOGIC_OP.end(); ++mapIter) {
        if (mapIter->second == l) {
            return mapIter->first;
        }
    }
    throw runtime_error("Invalid operator (" + to_string(l) + ").");
}

Condition::Condition() {
    
}

void Condition::init(const vector<string>& data, int index) {
    
}

string Condition::toString() const {
    return "";
}

bool Condition::evaluate() const {
    if (logicOp == None) {
        return _checkCondition();
    } else if (logicOp == And) {
        return _checkCondition() && nextCondition->_checkCondition();
    } else if (logicOp == Or) {
        return _checkCondition() || nextCondition->_checkCondition();
    } else if (logicOp == AndNot) {
        return _checkCondition() && !nextCondition->_checkCondition();
    } else {
        return _checkCondition() || !nextCondition->_checkCondition();
    }
}

bool Condition::_checkCondition() const {
    if (type == False) {
        return false;
    } else {
        return true;
    }
}