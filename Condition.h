#ifndef _CONDITION_H
#define _CONDITION_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Condition {
    public:
    enum Type {
        False = 0, True, typeSize
    };
    enum LogicOp {
        None = 0, And, Or, AndNot, OrNot, logicOpSize
    };
    
    static const unordered_map<string, Type> STRING_TO_TYPE;
    static const unordered_map<string, LogicOp> STRING_TO_LOGIC_OP;
    Type type;
    LogicOp logicOp;
    Condition* nextCondition;
    
    static Type stringToType(const string& s);
    static const string& typeToString(Type t);
    static LogicOp stringToLogicOp(const string& s);
    static const string& logicOpToString(LogicOp l);
    Condition();
    void init(const vector<string>& data, int index);
    string toString() const;
    bool evaluate() const;
    
    private:
    bool _checkCondition() const;
};

#endif