#include <iostream>
#include "headers/semantics.h"

/*******************************************************
 * FUNCTION DEFINITIONS FOR HIGH LEVEL SEMANTIC ANALYSIS
*******************************************************/

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

    switch (assign->RHS_type) {
        case 'a': {
            const ExprStmtAST* righthand_side = dynamic_cast<const ExprStmtAST*>(assign->RHS);
            Token type = traverse_expr(righthand_side);
            if (variables[var] != type) {
                std::cout << "ERROR - Incompatible type for variable '" << var << "'.\n";
                exit(-1);
            }
	    return;
        }

        case 'b': {
            const BoolExpr* righthand_side = dynamic_cast<const BoolExpr*>(assign->RHS);
            Token type = traverse_bool_expr(righthand_side);
            if (variables[var] != type) {
                std::cout << "ERROR - Incompatible type for variable '" << var << "'.\n";
                exit(-1);
            }
	    return;
        }
    }
    std::cout << "ERROR - Right hand side not recognized for variable '" << var << "' definition.\n";
    exit(-1);
}

void Semantics::traverse_if_stmt(IfStmtAST* conditional) {
    Token type = traverse_bool_expr(conditional->condition);
    if (type != BOOL_KWD) {
        std::cout << "ERROR - If statement conditions must be boolean type.\n";
        exit(-1);
    }
    traverse_prog(conditional->code);
}

/******************************************************************
 * FUNCTION DEFINITIONS FOR ARITHMETIC EXPRESSION SEMANTIC ANALYSIS
******************************************************************/

Token Semantics::traverse_expr(const ExprStmtAST* expr) {
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

/***************************************************************
 * FUNCTION DEFINITIONS FOR BOOLEAN EXPRESSION SEMANTIC ANALYSIS
***************************************************************/

Token Semantics::traverse_bool_expr(const BoolExpr* expr) {
    Token type1 = traverse_bool_term(expr->term);
    if (expr->expr == nullptr) {
        return type1;
    }
    Token type2 = traverse_bool_expr(expr->expr);
    if (type1 != type2) {
        std::cout << "ERROR - Incompatable type for '|' operation.\n";
        exit(-1);
    }
    return type1;
}

Token Semantics::traverse_bool_term(BoolTerm* term) {
    Token type1 = traverse_bool_factor(term->factor);
    if (term->term == nullptr) {
        return type1;
    }
    Token type2 = traverse_bool_term(term->term);
    if (type1 != type2) {
        std::cout << "ERROR - Incompatible types for '&' operatioin.\n";
        exit(-1);
    }
    return type1;
}

Token Semantics::traverse_bool_factor(BoolFactor* factor) {
    switch (factor->stmt_class) {
        case 's': {
            const SuperRel* super_rel = dynamic_cast<const SuperRel*>(factor->obj);
            return traverse_super_rel(super_rel);
        }

        case 'l': {
            return BOOL_KWD;
        }

        case 'e': {
            const BoolExpr* expr = dynamic_cast<const BoolExpr*>(factor->obj);
            return traverse_bool_expr(expr);
        }
    }
    std::cout << "ERROR - Boolean factor not recognized.\n";
    exit(-1);    
}

Token Semantics::traverse_super_rel(const SuperRel* super_rel) {
    Token type1 = traverse_rel_operand(super_rel->operand);
    if (super_rel->relation == nullptr) {
        return BOOL_KWD;
    }
    Token type2 = traverse_relation(super_rel->relation);
    if (type1 != type2) {
        std::cout << "ERROR - Invalid types for relation operation.\n";
        exit(-1);
    }
    return BOOL_KWD;
} 

Token Semantics::traverse_relation(Relation* rel) {
    Token type1 = traverse_rel_operand(rel->operand);
    if (rel->nextRelation == nullptr) {
        return type1;
    }
    Token type2 = traverse_relation(rel->nextRelation);
    if (type1 != type2) {
        std::cout << "ERROR - Invalid types for relation operation.\n";
        exit(-1);
    }
    return type1;
}

Token Semantics::traverse_rel_operand(IDStmtAST* operand) {
    switch (operand->token)
    {
        case INT_LIT: return INT_KWD;    

        case FLOAT_LIT: return FLOAT_KWD; 
        
        default:
            std::cout << "ERROR - Invalid type for relation operand.\n";
            exit(-1);
    }
}
