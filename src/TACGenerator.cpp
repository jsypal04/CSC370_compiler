#include <iostream>
#include <string>
#include "headers/TACGenerator.h"

/*****************************************
 * FUNCTIONS FOR HIGH LEVEL TAC GENERATION 
*****************************************/

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
    else if (stmt->conditional != nullptr) {
        traverse_if_stmt(stmt->conditional);
    }
}

void TACGenerator::traverse_assign(AssignStmtAST* assign) {
    ExprStmtAST* righthand_side = dynamic_cast<ExprStmtAST*>(assign->RHS);
    std::string exprVar = traverse_expr(righthand_side);
    *output << assign->varID->lexeme << '=' << exprVar << '\n';
}

void TACGenerator::traverse_if_stmt(IfStmtAST* conditional) {
    std::cout << "Running TACGenerator::traverse_if_stmt.\n";
    std::string condition = traverse_bool_expr(conditional->condition);
    std::string label = "L" + std::to_string(labelCount);
    labelCount++;
    *output << "IfZ " << condition << " Goto " << label << '\n';
    traverse_prog(conditional->code);
    *output << label << '\n';
}

/*****************************************
 * FUNCTIONS FOR ARITHMETIC TAC GENERATION
*****************************************/

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

/*************************************************
 * FUNCTIONS FOR BOOLEAN EXPRESSION TAC GENERATION
*************************************************/

std::string TACGenerator::traverse_bool_expr(BoolExpr* bool_expr) {
    std::string var1 = traverse_bool_term(bool_expr->term);
    if (bool_expr->expr == nullptr) {
        std::cout << "ERROR - While generating code in bool_expr. Missing bool_expr_p node\n";
        exit(-1);
    }
    std::string var2 = traverse_bool_expr_p(bool_expr->expr);
    std::string newTemp = "_t" + std::to_string(tempCount);
    tempCount++;
    *output << newTemp << '=' << var1 << var2 << '\n';
    return newTemp;
}

std::string TACGenerator::traverse_bool_expr_p(BoolExpr* bool_expr_p) {
    std::string var1 = traverse_bool_term(bool_expr_p->term);
    if (bool_expr_p == nullptr) {
        return var1;
    }
    std::string var2 = traverse_bool_expr_p(bool_expr_p->expr);
    std::string newTemp = "_t" + std::to_string(tempCount);
    tempCount++;
    // This switch statement might be unnecessary since there should not be any syntax errors at this point
    switch (bool_expr_p->op) {
        case OR:
            *output << newTemp << '=' << var1 << var2 << '\n'; // may need to change the symbol for or
            break;
        
        default:
            std::cout << "ERROR - I think. While generating TAC in bool_expr_p. No operator specified.\n";
            exit(-1);
    }
    return '|' + newTemp;
}

std::string TACGenerator::traverse_bool_term(BoolTerm* bool_term) {
    std::string var1 = traverse_bool_factor(bool_term->factor);
    std::cout << (bool_term->term == nullptr) << '\n';
    if (bool_term->term == nullptr) {
        std::cout << "ERROR - While generating TAC in bool_term. Missing bool_term_p node.\n";
        exit(-1);
    }
    std::string var2 = traverse_bool_term_p(bool_term->term);
    std::string newTemp = "_t" + std::to_string(tempCount);
    tempCount++;
    *output << newTemp << '=' << var1 << var2 << '\n';
    return newTemp;
}

std::string TACGenerator::traverse_bool_term_p(BoolTerm* bool_term_p) {
    std::string var1 = traverse_bool_factor(bool_term_p->factor);
    if (bool_term_p->term == nullptr) {
        return var1;
    }
    std::string var2 = traverse_bool_term_p(bool_term_p->term);
    std::string newTemp = "_t" + std::to_string(tempCount);
    tempCount++;
    // This switch statement might be unnecessary since there should be no syntax errors at this point
    switch (bool_term_p->op) {
        case AND:
            *output << newTemp << '=' << var1 << var2 << '\n';
            break;

        default:
            std::cout << "ERROR - I think. While generating TAC in bool_term_p. Missing operator.\n";
            exit(-1);
    }
    return '&' + newTemp;
}

std::string TACGenerator::traverse_bool_factor(BoolFactor* bool_factor) {
    switch (bool_factor->stmt_class) {
        case 'l': {
            IDStmtAST* literal = dynamic_cast<IDStmtAST*>(bool_factor->obj);
            return literal->lexeme;
        }

        case 's': {
            SuperRel* super_rel = dynamic_cast<SuperRel*>(bool_factor->obj);
            std::string var1 = super_rel->operand->lexeme;
            if (super_rel->relation == nullptr) {
                return var1;
            }
            std::string var2 = traverse_relation(super_rel->relation);
            std::string newTemp = "_t" + std::to_string(tempCount);
            tempCount++;
            *output << newTemp << '=' << var1 << var2 << '\n';
            return newTemp;
        }

        case 'e': {
            BoolExpr* bool_expr = dynamic_cast<BoolExpr*>(bool_factor->obj);
            return traverse_bool_expr(bool_expr);
        }

        default:
            std::cout << "ERROR - Bool factor class not recognized.\n";
            exit(-1);
    }
    exit(-1);
}

std::string TACGenerator::traverse_relation(Relation* rel) {
    std::string var1 = rel->operand->lexeme;
    if (rel->nextRelation == nullptr) {
        switch (rel->op) {
            case LS: return '<' + var1;

            case LSEQ: return "<=" + var1;

            case GR: return '>' + var1;

            case GREQ: return ">=" + var1;

            case EQUAL: return "==" + var1;

            case NEQUAL: return "!=" + var1;

            default:
                std::cout << "ERROR - Relational operator '" << rel->op << "' not recognized.\n";
                exit(-1);
        }
    }
    std::string var2 = traverse_relation(rel->nextRelation);
    std::string newTemp = "_t" + std::to_string(tempCount);
    tempCount++;
    *output << newTemp << '=' << var1 << var2 << '\n';
    switch (rel->op) {
        case LS: return '<' + newTemp;

        case LSEQ: return "<=" + newTemp;

        case GR: return '>' + newTemp;

        case GREQ: return ">=" + newTemp;

        case EQUAL: return "==" + newTemp;

        case NEQUAL: return "!=" + newTemp;

        default:
            std::cout << "ERROR - Relational operator not recognized.\n";
            exit(-1);
    }
    exit(-1);
}
