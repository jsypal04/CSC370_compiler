#ifndef AST_H
#define AST_H

#include <string>
#include <memory>
#include "tokens.h"

// class declarations
class StmtAST;
class ProgStmtAST;
class LineStmtAST;
class DeclarationStmtAST;
class AssignStmtAST;

// class declarations for arithmetic expressions
class ExprStmtAST;
class Expr_PStmtAST;
class TermStmtAST;
class Term_PStmtAST;
class FactorStmtAST;
class IDStmtAST;

// class declarations for boolean expressions
class BoolExpr;
class BoolExpr_P;
class BoolTerm;
class BoolTerm_P;
class BoolFactor;
class Relation;
class RelOperand;

// base class for all statements
class StmtAST {
public:
    virtual ~StmtAST() = default;
};

/*************************************************
BEGIN CLASS DEFINITIONS FOR ARITHMETIC EXPRESSIONS
*************************************************/

// class to represent identifiers and keywords
class IDStmtAST : public StmtAST {
public:
    Token token;
    std::string lexeme;

    IDStmtAST(Token tok, std::string lex) {
        token = tok;
        lexeme = lex;
    }
};

class FactorStmtAST : public StmtAST {
public:
    IDStmtAST* object;
    ExprStmtAST* expr_object;

    // Enter (nullptr, ExprStmtAST) if the factor is an expression and (IDStmtAST, nullptr) if the factor is a variable/literal
    FactorStmtAST(IDStmtAST* obj, ExprStmtAST* expr_obj) {
        object = obj;
        expr_object = expr_obj;
    }

    ~FactorStmtAST() {
        delete object;
        delete expr_object;
        object = nullptr;
        expr_object = nullptr;
    }
};

class Term_PStmtAST : public StmtAST {
public:
    Token op;
    FactorStmtAST* factor;
    Term_PStmtAST* term_p;

    Term_PStmtAST(Token o, FactorStmtAST* fctr, Term_PStmtAST* trm_p) {
        op = o;
        factor = fctr;
        term_p = trm_p;
    }

    ~Term_PStmtAST() {
        delete factor;
        delete term_p;
        factor = nullptr;
        term_p = nullptr;
    }
};

class TermStmtAST : public StmtAST {
public:
    FactorStmtAST* factor;
    Term_PStmtAST* term_p;

    TermStmtAST(FactorStmtAST* fctr, Term_PStmtAST* trm_p) {
        factor = fctr;
        term_p = trm_p;
    }

    ~TermStmtAST() {
        delete factor;
        delete term_p;
        factor = nullptr;
        term_p = nullptr;
    }
};

class Expr_PStmtAST : public StmtAST {
public:
    Token op;
    TermStmtAST* term;
    Expr_PStmtAST* expr_p;

    Expr_PStmtAST(Token o, TermStmtAST* trm, Expr_PStmtAST* exp) {
        op = o;
        term = trm;
        expr_p = exp;
    }

    ~Expr_PStmtAST() {
        delete term;
        delete expr_p;
        term = nullptr;
        expr_p = nullptr;
    }
};

// class for arithmetic expression nodes
class ExprStmtAST : public StmtAST {
public:
    TermStmtAST* term;
    Expr_PStmtAST* expr_p;

    ExprStmtAST(TermStmtAST* trm, Expr_PStmtAST* exp) {
        term = trm;
        expr_p = exp;
    }

    ~ExprStmtAST() {
        delete term;
        delete expr_p;
        term = nullptr;
        expr_p = nullptr;
    }
};

/*************************************************
 * BEGIN CLASS DEFINITIONS FOR BOOLEAN EXPRESSIONS
*************************************************/

class RelOperand : public StmtAST {
public:
    IDStmtAST* operand;

    RelOperand(IDStmtAST* opr) {
        operand = opr;
    }

    ~RelOperand() {
        delete operand;
        operand = nullptr;
    }
};

class Relation : public StmtAST {
public:
    Token op;
    RelOperand* operand;
    Relation* nextRelation;

    Relation(Token o, RelOperand* opr, Relation* nextRel) {
        op = o;
        operand = opr;
        nextRelation = nextRel;
    }

    ~Relation() {
        delete operand;
        delete nextRelation;
        operand = nullptr;
        nextRelation = nullptr;
    }
};

class BoolFactor : public StmtAST {
public:
    bool negated;
    IDStmtAST* factor;
    Relation* relation;

    BoolFactor(bool neg, IDStmtAST* fctr, Relation* rel) {
        negated = neg;
        factor = fctr;
        relation = rel;
    }

    ~BoolFactor() {
        if (factor != nullptr) {
            delete factor;
            factor = nullptr;
        }
        if (relation != nullptr) {
            delete relation;
            relation = nullptr;
        }
    }
};

class BoolTerm : StmtAST {
public:
    Token op; 
    BoolFactor* factor;
    BoolTerm* term;

    BoolTerm(Token o, BoolFactor* fctr, BoolTerm* trm) {
        op = o;
        factor = fctr;
        term = trm;
    }

    ~BoolTerm() {
        delete factor;
        factor = nullptr;
        if (term != nullptr) {
            delete term;
            term = nullptr;
        }
    }
};

// to be continued

/*********************************************
 * BEGIN CLASS DEFINITIONS FOR TOP LEVEL NODES
*********************************************/

// class for assignment nodes
class AssignStmtAST : public StmtAST {
public:
    IDStmtAST* varID;
    ExprStmtAST* RHS;

    AssignStmtAST(IDStmtAST* name, ExprStmtAST* rhs) {
        varID = name;
        RHS = rhs;
    }

    ~AssignStmtAST() {
        delete varID;
        delete RHS;
        varID = nullptr;
        RHS = nullptr;
    }
};

// class for declaration nodes
class DeclarationStmtAST : public StmtAST {
public:
    IDStmtAST* type;
    IDStmtAST* variable;
    
    DeclarationStmtAST(IDStmtAST* data_type, IDStmtAST* varName) {
        type = data_type;
        variable = varName;
    }

    ~DeclarationStmtAST() {
        delete type;
        delete variable;
        type = nullptr;
        variable = nullptr;
    }
};

// class for Stmts
class LineStmtAST : public StmtAST {
public:
    DeclarationStmtAST* declaration;
    AssignStmtAST* assign;

    LineStmtAST(DeclarationStmtAST* decl_ptr, AssignStmtAST* assign_ptr) {
        declaration = decl_ptr;
        assign = assign_ptr;
    }

    ~LineStmtAST() {
        delete declaration;
        delete assign;
        declaration = nullptr;
        assign = nullptr;
    }
};

// class for prog
class ProgStmtAST : public StmtAST {
public:
    LineStmtAST* stmt;
    ProgStmtAST* prog;

    ProgStmtAST(LineStmtAST* first_stmt, ProgStmtAST* program) {
        stmt = first_stmt;
        prog = program;
    }

    ~ProgStmtAST() {
        delete stmt;
        delete prog;
        stmt = nullptr;
        prog = nullptr;
    }
};

#endif