#include <iostream>
#include <string>
#include "headers/TACGenerator.h"

void TACGenerator::traverse_prog(ProgStmtAST* prog) {
    traverse_stmt(prog->stmt);
    if (prog->prog != nullptr) {
        traverse_prog(prog->prog);
    }
}

void TACGenerator::traverse_stmt(LineStmtAST* stmt) {
    if (stmt->assign != nullptr) {
        traverse_assign(stmt->assign);
    }
}

void TACGenerator::traverse_assign(AssignStmtAST* assign) {
    std::string exprVar = traverse_expr(assign->RHS);
    *output << assign->varID->lexeme << '=' << exprVar << '\n';
}

std::string TACGenerator::traverse_expr(ExprStmtAST* expr) {
    std::string var1 = traverse_term(expr->term);
    if (expr->expr_p == nullptr) {
        return var1;
    }
    std::string var2 = traverse_expr_p(expr->expr_p);
    return var1 + var2;
}

std::string TACGenerator::traverse_term(TermStmtAST* term) {
    std::string var1 = traverse_factor(term->factor);
    if (term->term_p == nullptr) {
        return var1;
    }
    std::string var2 = traverse_term_p(term->term_p);
    std::string newTemp = "_t" + std::to_string(tempCount);
    *output << newTemp << '=' << var1 << var2 << '\n';
    tempCount++;
    return newTemp;
}

std::string TACGenerator::traverse_expr_p(Expr_PStmtAST* expr_p) {
    std::string var1 = traverse_term(expr_p->term);
    if (expr_p->expr_p == nullptr) {
        switch (expr_p->op) {
            case ADD_OP: return "+" + var1;

            case SUB_OP: return "-" + var1;
        }
        std::cout << "ERROR - We got some major problems.\n";
        exit(-1);
    }
    std::string var2 = traverse_expr_p(expr_p->expr_p);
    std::string newTemp = "_t" + std::to_string(tempCount);
    *output << newTemp << '=' << var1 << var2 << '\n';
    tempCount++;
    switch (expr_p->op) {
        case ADD_OP: return "+" + newTemp;

        case SUB_OP: return "-" + newTemp;
    }
    std::cout << "ERROR - We got some major problems.\n";
    exit(-1);
}

std::string TACGenerator::traverse_term_p(Term_PStmtAST* term_p) {
    std::string var1 = traverse_factor(term_p->factor);
    if (term_p->term_p == nullptr) {
        switch (term_p->op) {
            case MULT_OP: return "*" + var1;

            case DIV_OP: return "/" + var1;

            case MOD_OP: return "%" + var1;
        }
        std::cout << "ERROR - We got some major problems.\n";
        exit(-1);
    }
    std::string var2 = traverse_term_p(term_p->term_p);
    std::string newTemp = "_t" + std::to_string(tempCount);
    *output << newTemp << '=' << var1 << var2 << '\n';
    tempCount++;
    switch (term_p->op) {
        case MULT_OP: return "*" + newTemp;

        case DIV_OP: return "/" + newTemp;

        case MOD_OP: return "%" + newTemp;
    }
    std::cout << "ERROR - We got some major problems.\n";
    exit(-1);
}

std::string TACGenerator::traverse_factor(FactorStmtAST* factor) {
    if (factor->object != nullptr) {
        return factor->object->lexeme;
    }
    else if (factor->expr_object != nullptr) {
        std::string tacExpression = traverse_expr(factor->expr_object);
        std::string newTemp = "_t" + std::to_string(tempCount);
        *output << newTemp << '=' << tacExpression << '\n';
        tempCount++; 
        return newTemp;
    }
    std::cout << "ERROR - We got come major problems.\n";
    exit(-1);
}