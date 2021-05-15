#include <iostream>
using namespace std;

struct Transformer;
struct Number;
struct BinaryOperation;
struct FunctionCall;
struct Variable;
struct Expression
{
    virtual ~Expression() { }
    virtual double evaluate() const = 0;
    virtual Expression* transform(Transformer* tr) const = 0;
};

struct Transformer //pattern Visitor
{
    virtual ~Transformer() { }
    virtual Expression* transformNumber(Number const*) = 0;
    virtual Expression* transformBinaryOperation(BinaryOperation const*) = 0;
    virtual Expression* transformFunctionCall(FunctionCall const*) = 0;
    virtual Expression* transformVariable(Variable const*) = 0;
};

struct Number : Expression
{
    Number(double value) : value_(value) {}
    double value() const { return value_; }
    double evaluate() const { return value_; }
    Expression* transform(Transformer* tr) const { return tr->transformNumber(this); }
private:
    double value_;
};

struct BinaryOperation : Expression
{
    enum {
        PLUS = '+',
        MINUS = '-',
        DIV = '/',
        MUL = '*'
    };
    BinaryOperation(Expression const* left, int op, Expression const* right) : left_(left), op_(op), right_(right) {}
    ~BinaryOperation() {
        delete left_;
        delete right_;
    }
    double evaluate() const {
        if (op_ == '+')
        {
            return left_->evaluate() + right_->evaluate();
        }
        if (op_ == '-')
        {
            return left_->evaluate() - right_->evaluate();
        }
        if (op_ == '*')
        {
            return left_->evaluate() * right_->evaluate();
        }
        if (op_ == '/')
        {
            return left_->evaluate() / right_->evaluate();
        }
    }
    Expression* transform(Transformer* tr) const { return tr->transformBinaryOperation(this); }
    Expression const* left() const { return left_; }
    Expression const* right() const { return right_; }
    int operation() const { return op_; }
private:
    Expression const* left_;
    Expression const* right_;
    int op_;
};

struct FunctionCall : Expression
{
    FunctionCall(std::string const& name, Expression const* arg) : name_(name), arg_(arg) {}
    ~FunctionCall() { delete arg_; }
    double evaluate() const { return arg_->evaluate(); }
    Expression* transform(Transformer* tr) const { return tr->transformFunctionCall(this); }
    std::string const& name() const { return name_; }
    Expression const* arg() const { return arg_; }
private:
    std::string const name_;
    Expression const* arg_;
};

struct Variable : Expression
{
    Variable(std::string const name) : name_(name) {}
    std::string const& name() const { return name_; }
    double evaluate() const { return 0; }
    Expression* transform(Transformer* tr) const { return tr->transformVariable(this); }
private:
    std::string const name_;
};


/**
* реализуйте все необходимые методы
* если считаете нужным, то можете
* заводить любые вспомогательные
* методы
*/

struct FoldConstants : Transformer
{
    Expression* transformNumber(Number const* number)
    {
        Number* tmp = new Number(number->value());
        return tmp;
    }

    Expression* transformBinaryOperation(BinaryOperation const* binop)
    {
        Transformer* t = new FoldConstants();
        Expression* a = binop->left()->transform(t);
        Expression* b = binop->right()->transform(t);
        Number* n1 = dynamic_cast<Number*>(a);
        Number* n2 = dynamic_cast<Number*>(b);
        BinaryOperation* tmp = new BinaryOperation(a, binop->operation(), b);
        if (n1 && n2) {
            Number* ans = new Number(tmp->evaluate());
            delete tmp;
            delete t;
            return ans;
        }
        else {
            delete t;
            return tmp;
        }
    }

    Expression* transformFunctionCall(FunctionCall const* fcall)
    {
        Transformer* t = new FoldConstants();
        Expression* targ = fcall->arg()->transform(t);
        Number* n = dynamic_cast<Number*>(targ);
        Expression* tmp = new FunctionCall(fcall->name(), targ);
        if (n) {
            Number* ans = new Number(tmp->evaluate());
            delete tmp;
            delete t;
            return ans;
        }
        else {
            delete t;
            return tmp;
        }
    }

    Expression* transformVariable(Variable const* var)
    {
        Variable* tmp = new Variable(var->name());
        return tmp;
    }
};


//Проверьте работу созданной структуры, например, на следующем коде
int main() {
    Number* n32 = new Number(32.0);
    Number* n16 = new Number(16.0);
    BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
    FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
    Variable* var = new Variable("var");
    BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
    FunctionCall* callAbs = new FunctionCall("abs", mult);
    FoldConstants FC;
    Expression* newExpr = callAbs->transform(&FC);
     
}