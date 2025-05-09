#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include <list>
#include "string"

class Exp;
class BinaryExp;
class NumberExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class WhileStatement;
class IFExp;
class IfStatement;
class Program;
class StringLiteral;
class IndexExpr;
class ForRangeStatement;
class ForStringStatement;

class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(IdentifierExp* exp) = 0;
    virtual int visit(IFExp* exp) = 0;
    virtual void visit(AssignStatement* stm) = 0;
    virtual void visit(PrintStatement* stm) = 0;
    virtual void visit(WhileStatement* stm) = 0;
    virtual void visit(IfStatement* stm) = 0;
    virtual int visit(StringLiteral* stm) = 0;
    virtual int visit(IndexExpr* stm) = 0;
    virtual void visit(ForRangeStatement* stm) = 0;
    virtual void visit(ForStringStatement* stm) = 0;
    virtual std::string visitStringLiteral(StringLiteral* exp) = 0;
    virtual std::string visitIndexExpr(IndexExpr* exp) = 0;

};

class PrintVisitor : public Visitor {
public:
    void imprimir(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(IdentifierExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    int visit(IFExp* exp) override;
    int visit(StringLiteral* stm) override;
    int visit(IndexExpr* stm) override;
    void visit(ForRangeStatement* stm) override;
    void visit(ForStringStatement* stm) override;
    std::string visitStringLiteral(StringLiteral* exp) override;
    std::string visitIndexExpr(IndexExpr* exp) override;
};

class EVALVisitor : public Visitor {
public:
    void ejecutar(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(IdentifierExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    int visit(IFExp* exp) override;
    int visit(StringLiteral* stm) override;
    int visit(IndexExpr* stm) override;
    void visit(ForRangeStatement* stm) override;
    void visit(ForStringStatement* stm) override;
    std::string visitStringLiteral(StringLiteral* exp) override;
    std::string visitIndexExpr(IndexExpr* exp) override;
};

#endif // VISITOR_H