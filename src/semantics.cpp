#include <iostream>
#include "headers/semantics.h"

void Semantics::traverse_prog(ProgStmtAST* prog) {
    traverse_stmt(prog->stmt);
    if (prog->prog != nullptr) {
        traverse_prog(prog->prog);
    }
}

void Semantics::traverse_stmt(LineStmtAST* stmt) {
    if (stmt->declaration != nullptr) {
        traverse_decl(stmt->declaration);
    }
    else if (stmt->assign != nullptr) {
        traverse_assign(stmt->assign);
    }
}

void Semantics::traverse_decl(DeclarationStmtAST* decl) {
    Token type = decl->type->token;
    std::string var = decl->variable->lexeme;
    variables[var] = type;
}

void Semantics::traverse_assign(AssignStmtAST* assign) {
    std::string var = assign->varID->lexeme;
    if (variables.count(var) == 0) {
        std::cout << "ERROR - Variable '" << var << "' has not been declared.\n";
        exit(-1);
    }

    Token type = traverse_expr(assign->RHS);
    if (variables[var] != type) {
        std::cout << "ERROR - Incompatible type for variable '" << var << "'.\n";
        exit(-1);
    }
}

Token Semantics::traverse_expr(ExprStmtAST* expr) {
    Token type1 = traverse_term(expr->term);
    if (expr->expr_p == nullptr) {
        return type1;
    }
    Token type2 = traverse_expr_p(expr->expr_p);
    if (type1 != type2) {
        std::cout << "ERROR - Incompatible types.\n";
        exit(-1);
    }
    return type1;
}

Token Semantics::traverse_expr_p(Expr_PStmtAST* expr_p) {
    Token type1 = traverse_term(expr_p->term);
    if (expr_p->expr_p == nullptr) {
        return type1;
    }
    Token type2 = traverse_expr_p(expr_p->expr_p);
    if (type1 != type2) {
        std::cout << "ERROR - Incompatible types.\n";
        exit(-1);
    }
    return type1;
}

Token Semantics::traverse_term(TermStmtAST* term) {
    Token type1 = traverse_factor(term->factor);
    if (term->term_p == nullptr) {
        return type1;
    }
    Token type2 = traverse_term_p(term->term_p);
    if (type1 != type2) {
        std::cout << "ERROR - Incompatible types.\n";
        exit(-1);
    }
    return type1;
}

Token Semantics::traverse_term_p(Term_PStmtAST* term_p) {
    Token type1 = traverse_factor(term_p->factor);
    if (term_p->term_p == nullptr) {
        return type1;
    }
    Token type2 = traverse_term_p(term_p->term_p);
    if (type1 != type2) {
        std::cout << "ERROR - Incompatible types.\n";
        exit(-1);
    }
    return type1;
}

Token Semantics::traverse_factor(FactorStmtAST* factor) {
    if (factor->object != nullptr) {
        Token tok = factor->object->token;
        // if the factor is a variable:
        if (tok == ID && variables.count(factor->object->lexeme) != 0) {
            // lookup in the map of declared variables
            return variables[factor->object->lexeme];
        }
        // if the factor is a literal
        switch (tok) {
            case INT_LIT: return INT_KWD;

            case FLOAT_LIT: return FLOAT_KWD;

            default:
                std::cout << "ERROR - Invalid type for operation.\n";
                break;
        }
        exit(-1);
    }
    else if (factor->expr_object != nullptr) {
        return traverse_expr(factor->expr_object);
    }
    std::cout << "ERROR - Something went wrong.\n";
    exit(-1);
}

