#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
#include "string"
using namespace std;
unordered_map<std::string, int> memoria;
unordered_map<std::string, string> memoria_str;
///////////////////////////////////////////////////////////////////////////////////

int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IFExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdentifierExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int PrintStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int IfStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int WhileStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int IndexExpr::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int StringLiteral::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int ForRangeStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int ForStringStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

std::string StringLiteral::acceptString(Visitor* visitor) {
    return visitor->visitStringLiteral(this);
}

std::string IndexExpr::acceptString(Visitor* visitor) {
    return visitor->visitIndexExpr(this);
}
///////////////////////////////////////////////////////////////////////////////////


int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(IdentifierExp* exp) {
    cout << exp->name;
    return 0;
}

void PrintVisitor::visit(AssignStatement* stm) {
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrintStatement* stm) {
    cout << "print(";
    stm->e->accept(this);
    cout << ");";
}

void PrintVisitor::visit(IfStatement* stm) {
    cout << "if ";
    stm->condition->accept(this);
    cout << " then" << endl;
    for (Stm* s : stm->then) {
        cout << "    ";
        s->accept(this);
        cout << endl;
    }
    if(!stm->els.empty()) cout << "else" << endl;

    for (Stm* s : stm->els) {
        cout << "    ";
        s->accept(this);
        cout << endl;
    }

    cout << "endif";
}

void PrintVisitor::imprimir(Program* program){
    for (Stm* s : program->slist) {
        s->accept(this);
        cout << endl;
    }
};

int PrintVisitor::visit(StringLiteral* exp) {
    cout << "\"" << exp->value << "\"";
    return 0;
}

int PrintVisitor::visit(IndexExpr* exp) {
    cout << exp->id << "[";
    exp->index->accept(this);
    cout << "]";
    return 0;
}



void PrintVisitor::visit(ForRangeStatement* stm) {
    cout << "for " << stm->id << " in range(";
    stm->start->accept(this); cout << ", ";
    stm->end->accept(this); cout << ", ";
    stm->step->accept(this); cout << ") do" << endl;
    for (Stm* s : stm->body) {
        cout << "    "; s->accept(this); cout << endl;
    }
    cout << "endfor";
}

void PrintVisitor::visit(ForStringStatement* stm) {
    cout << "for " << stm->id << " in \"" << stm->str << "\" do" << endl;
    for (Stm* s : stm->body) {
        cout << "    "; s->accept(this); cout << endl;
    }
    cout << "endfor";
}

std::string PrintVisitor::visitStringLiteral(StringLiteral* exp) {
    return exp->value;
}

std::string PrintVisitor::visitIndexExpr(IndexExpr* exp) {
    return "";
}


///////////////////////////////////////////////////////////////////////////////////
std::string EVALVisitor::visitStringLiteral(StringLiteral* exp) {
    return exp->value;
}

int EVALVisitor::visit(BinaryExp* exp) {
    if (exp->op == PLUS_OP) {
        try {
            string left = dynamic_cast<StringLiteral*>(exp->left)->value;
            string right = dynamic_cast<StringLiteral*>(exp->right)->value;
            cout << left + right;
            return 0;
        } catch (...) {
        }
    }
    else if (exp->op == MUL_OP) {
        try {
            string str;
            int times;
            if (StringLiteral* strLit = dynamic_cast<StringLiteral*>(exp->left)) {
                str = strLit->value;
                times = exp->right->accept(this);
            }
            else if (StringLiteral* strLit = dynamic_cast<StringLiteral*>(exp->right)) {
                str = strLit->value;
                times = exp->left->accept(this);
            }

            if (!str.empty()) {
                string result;
                for (int i = 0; i < times; i++) {
                    result += str;
                }
                cout << result;
                return 0;
            }
        } catch (...) {
        }
    }

    int leftVal = exp->left->accept(this);
    int rightVal = exp->right->accept(this);

    switch(exp->op) {
        case PLUS_OP: return leftVal + rightVal;
        case MINUS_OP: return leftVal - rightVal;
        case MUL_OP: return leftVal * rightVal;
        case DIV_OP:
            if(rightVal != 0) return leftVal / rightVal;
            cout << "Error: división por cero" << endl;
            return 0;
        case LT_OP: return leftVal < rightVal;
        case LE_OP: return leftVal <= rightVal;
        case EQ_OP: return leftVal == rightVal;
        default:
            cout << "Operador desconocido" << endl;
            return 0;
    }
}
int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(IdentifierExp* exp) {
    return memoria[exp->name];
}

void EVALVisitor::visit(AssignStatement* stm) {
    if (StringLiteral* strLit = dynamic_cast<StringLiteral*>(stm->rhs)) {
        memoria_str[stm->id] = strLit->value;
    }
    else if (IndexExpr* idxExp = dynamic_cast<IndexExpr*>(stm->rhs)) {
        memoria_str[stm->id] = idxExp->acceptString(this);
    }
    else {
        memoria[stm->id] = stm->rhs->accept(this);
    }
}

void EVALVisitor::visit(PrintStatement* stm) {
    if (StringLiteral* strLit = dynamic_cast<StringLiteral*>(stm->e)) {
        cout << strLit->value << endl;
    }
    else if (IndexExpr* idxExp = dynamic_cast<IndexExpr*>(stm->e)) {
        cout << idxExp->acceptString(this) << endl;
    }
    else if (BinaryExp* binExp = dynamic_cast<BinaryExp*>(stm->e)) {
        if (binExp->op == PLUS_OP) {
            string left, right;
            if (StringLiteral* leftStr = dynamic_cast<StringLiteral*>(binExp->left)) {
                left = leftStr->value;
            }
            else if (IndexExpr* leftIdx = dynamic_cast<IndexExpr*>(binExp->left)) {
                left = leftIdx->acceptString(this);
            }
            else if (IdentifierExp* leftId = dynamic_cast<IdentifierExp*>(binExp->left)) {
                if (memoria_str.count(leftId->name)) {
                    left = memoria_str[leftId->name];
                } else {
                    left = to_string(memoria[leftId->name]);
                }
            }
            if (StringLiteral* rightStr = dynamic_cast<StringLiteral*>(binExp->right)) {
                right = rightStr->value;
            }
            else if (IndexExpr* rightIdx = dynamic_cast<IndexExpr*>(binExp->right)) {
                right = rightIdx->acceptString(this);
            }
            else if (IdentifierExp* rightId = dynamic_cast<IdentifierExp*>(binExp->right)) {
                if (memoria_str.count(rightId->name)) {
                    right = memoria_str[rightId->name];
                } else {
                    right = to_string(memoria[rightId->name]);
                }
            }

            cout << left + right << endl;
            return;
        }
    }
    cout << stm->e->accept(this) << endl;
}


void EVALVisitor::ejecutar(Program* program){
    for (Stm* s : program->slist) {
        s->accept(this);
    }
};

void EVALVisitor::visit(IfStatement* stm) {
    if(stm->condition->accept(this)){
        for (Stm* s : stm->then) {
            s->accept(this);
        }
    }
    else{
        for (Stm* s : stm->els) {
            s->accept(this);
        }
    }
}

int EVALVisitor::visit(StringLiteral* exp) {
    return 0;
}

int EVALVisitor::visit(IndexExpr* exp) {
    string str = memoria_str[exp->id];
    int index = exp->index->accept(this);
    if (index >= 0 && index < str.length()) {
        return str[index];
    }
    return 0;
}

void EVALVisitor::visit(ForRangeStatement* stm) {
    int start = stm->start->accept(this);
    int end = stm->end->accept(this);
    int step = stm->step->accept(this);

    for (int i = start; i < end; i += step) {
        memoria[stm->id] = i;
        for (Stm* s : stm->body) {
            s->accept(this);
        }
    }
}

void EVALVisitor::visit(ForStringStatement* stm) {
    string str;

    if (!stm->str.empty()) {
        str = stm->str;
    } else {
        str = memoria_str[stm->id];
    }

    for (char c : str) {
        memoria_str[stm->id] = string(1, c);

        for (Stm* s : stm->body) {
            s->accept(this);
        }
    }
}



void PrintVisitor::visit(WhileStatement* stm) {
    cout << "while " ;
    stm->condition->accept(this);
    cout << " do " << endl;
    for (Stm* s : stm->body) {
        cout << "    ";
        s->accept(this);
        cout <<endl;
    }
    cout << "endwhile" ;
}

void EVALVisitor::visit(WhileStatement* stm) {
    while (stm->condition->accept(this)) {
        for (Stm* s : stm->body) {
            s->accept(this);
        }
    }
}


int PrintVisitor::visit(IFExp *exp) {
    cout << "ifexp(";
    exp->e1->accept(this);
    cout << ",";
    exp->e2->accept(this);
    cout << ",";
    exp->e3->accept(this);
    cout << ")";
    return 0;
}

int EVALVisitor::visit(IFExp *exp) {
    if (exp->e1->accept(this)) {
        return exp->e2->accept(this);
    }
    else {
        return exp->e3->accept(this);
    }
}


std::string EVALVisitor::visitIndexExpr(IndexExpr* exp) {
    if (memoria_str.count(exp->id)) {
        string str = memoria_str[exp->id];
        int index = exp->index->accept(this);
        if (index >= 0 && index < str.size()) {
            return string(1, str[index]);
        }
    }
    return "";
}

