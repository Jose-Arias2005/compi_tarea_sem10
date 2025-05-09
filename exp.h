#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
using namespace std;
enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP,LT_OP, LE_OP, EQ_OP };

class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    static string binopToChar(BinaryOp op);
};

class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    int accept(Visitor* visitor);
    ~BinaryExp();
};

class IFExp : public Exp {
public:
    Exp *e1,*e2, *e3;
    IFExp(Exp* a1, Exp* a2, Exp* a3);
    int accept(Visitor* visitor);
    ~IFExp();
};


class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    int accept(Visitor* visitor);
    ~NumberExp();
};

class IdentifierExp : public Exp {
public:
    std::string name;
    IdentifierExp(const std::string& n);
    int accept(Visitor* visitor);
    ~IdentifierExp();
};

class Stm {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

class AssignStatement : public Stm {
public:
    std::string id;
    Exp* rhs;
    AssignStatement(std::string id, Exp* e);
    int accept(Visitor* visitor);
    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    Exp* e;
    PrintStatement(Exp* e);
    int accept(Visitor* visitor);
    ~PrintStatement();
};


class IfStatement : public Stm {
public:
    Exp* condition;
    list<Stm*> then;
    list<Stm*> els;
    IfStatement(Exp* condition, list<Stm*> then, list<Stm*> els);
    int accept(Visitor* visitor);
    ~IfStatement();
};


class WhileStatement : public Stm {
public:
    Exp* condition;
    list<Stm*> body;
    WhileStatement(Exp* c, list<Stm*> b);
    int accept(Visitor* visitor);
    ~WhileStatement();
};

class StringLiteral : public Exp {
public:
    std::string value;
    StringLiteral(const std::string& v);
    int accept(Visitor* visitor);
    string acceptString(Visitor* visitor);
    ~StringLiteral();
};

class IndexExpr : public Exp {
public:
    std::string id;
    Exp* index;
    IndexExpr(const std::string& id, Exp* index);
    int accept(Visitor* visitor);
    string acceptString(Visitor* visitor);
    ~IndexExpr();
};

class ForRangeStatement : public Stm {
public:
    std::string id;
    Exp* start;
    Exp* end;
    Exp* step;
    list<Stm*> body;
    ForRangeStatement(const std::string& id, Exp* s, Exp* e, Exp* st, list<Stm*> b);
    int accept(Visitor* visitor);
    ~ForRangeStatement();
};

class ForStringStatement : public Stm {
public:
    std::string id;
    std::string str;
    list<Stm*> body;
    ForStringStatement(const std::string& id, const std::string& s, list<Stm*> b);
    int accept(Visitor* visitor);
    ~ForStringStatement();
};


class Program {
public:
    std::list<Stm*> slist;
    Program();
    void add(Stm* s);
    ~Program();
};

#endif // EXP_H