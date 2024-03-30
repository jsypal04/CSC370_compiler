#include <iostream>
#include "headers/semantics.h"

void Semantics::traverse_prog(std::unique_ptr<ProgStmtAST> prog) {
    traverse_stmt(std::move(prog->stmt));
    if (prog->prog != nullptr) {
        traverse_prog(std::move(prog->prog));
    }
}

void Semantics::traverse_stmt(std::unique_ptr<LineStmtAST> stmt) {
    if (stmt->declaration != nullptr) {
        traverse_decl(std::move(stmt->declaration));
    }
    else if (stmt->assign != nullptr) {
        traverse_assign(std::move(stmt->assign));
    }
}

void Semantics::traverse_decl(std::unique_ptr<DeclarationStmtAST> decl) {
    Token type = decl->type->token;
    std::string var = decl->variable->lexeme;
    variables[var] = type;
}

void Semantics::traverse_assign(std::unique_ptr<AssignStmtAST> assign) {
    std::string var = assign->varID->lexeme;
    if (variables.count(var) == 0) {
        std::cout << "ERROR - Variable '" << var << "' has not been declared.\n";
        return;
    }

    Token type = traverse_expr(std::move(assign->RHS));
    if (type == ERROR) {
        return;
    }
    if (variables[var] != type) {
        std::cout << "ERROR - Incompatible type for variable '" << var << "'.\n";
        return;
    }
}

Token Semantics::traverse_expr(std::unique_ptr<ExprStmtAST> expr) {
    Token type1 = traverse_term(std::move(expr->term));
    if (expr->expr_p == nullptr) {
        return type1;
    }
    Token type2 = traverse_expr_p(std::move(expr->expr_p));
    if (type2 == ERROR) {
        return ERROR;
    }
    else if (type1 != type2) {
        std::cout << "ERROR - Incompatible types.\n";
        return ERROR;
    }
    return type1;
}

Token Semantics::traverse_expr_p(std::unique_ptr<Expr_PStmtAST> expr_p) {
    Token type1 = traverse_term(std::move(expr_p->term));
    if (expr_p->expr_p == nullptr) {
        return type1;
    }
    Token type2 = traverse_expr_p(std::move(expr_p->expr_p));
    if (type2 == ERROR) {
        return ERROR;
    }
    else if (type1 != type2) {
        std::cout << "ERROR - Incompatible types.\n";
        return ERROR;
    }
    return type1;
}

Token Semantics::traverse_term(std::unique_ptr<TermStmtAST> term) {
    Token type1 = traverse_factor(std::move(term->factor));
    if (term->term_p == nullptr) {
        return type1;
    }
    Token type2 = traverse_term_p(std::move(term->term_p));
    if (type2 == ERROR) {
        return ERROR;
    }
    else if (type1 != type2) {
        std::cout << "ERROR - Incompatible types.\n";
        return ERROR;
    }
    return type1;
}

Token Semantics::traverse_term_p(std::unique_ptr<Term_PStmtAST> term_p) {
    Token type1 = traverse_factor(std::move(term_p->factor));
    if (term_p->term_p == nullptr) {
        return type1;
    }
    Token type2 = traverse_term_p(std::move(term_p->term_p));
    if (type2 == ERROR) {
        return ERROR;
    }
    else if (type1 != type2) {
        std::cout << "ERROR - Incompatible types.\n";
        return ERROR;
    }
    return type1;
}

Token Semantics::traverse_factor(std::unique_ptr<FactorStmtAST> factor) {
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
        return ERROR;
    }
    else if (factor->expr_object != nullptr) {
        return traverse_expr(std::move(factor->expr_object));
    }
    std::cout << "ERROR - Something went wrong.\n";
    return ERROR;
}

