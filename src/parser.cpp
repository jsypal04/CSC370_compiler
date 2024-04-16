#include "headers/parser.h"

ProgStmtAST* Parser::prog() {
    // parse the first line of the program
    std::cout << "Parsing stmt...\n";
    LineStmtAST* statement = stmt();
    std::cout << "Parsed stmt.\n";
    // if the next token is a newline then parse the rest of the program
    if (lexer->nextToken == NEWLINE) {
        lexer->lex();
        // parses the rest of the program
        std::cout << "Parsing prog...\n";
        ProgStmtAST* next_prog = prog();
        std::cout << "Parsed prog.\n";

        // creates a pointer to a ProgStmtAST class and returns that pointer
        auto program = new ProgStmtAST(statement, next_prog);
        return program;
    }
    // if the next token is not a newline then the program is done
    // creates a pointer to a ProgStmtAST class and returns the pointer
    auto program = new ProgStmtAST(statement, nullptr);
    return program;
}

LineStmtAST* Parser::stmt() {
    // if the next token is a type keyword then parse a variable declaration
    if (lexer->nextToken == INT_KWD || lexer->nextToken == FLOAT_KWD || lexer->nextToken == BOOL_KWD) {
        std::cout << "Parsing declaration...\n";
        DeclarationStmtAST* decl = declaration();
        std::cout << "Parsed declaration.\n";

        auto statement = new LineStmtAST(decl, nullptr);
        return statement;
    }
    // if the next token is an identifier then parse an assignment
    else if (lexer->nextToken == ID) {
        std::cout << "Parsing assignment...\n";
        AssignStmtAST* assign_ptr = assign();
        std::cout << "Parsed assignment.\n";

        auto statement = new LineStmtAST(nullptr, assign_ptr);
        return statement;
    }
    // otherwise this is an undefined statement
    std::cout << "ERROR - invalid statement." << '\n';
    exit(-1);
}

DeclarationStmtAST* Parser::declaration() {
    // get the type token and lexeme
    Token type_tok = lexer->nextToken;
    std::string type_lex = lexer->lexeme;

    // get the variable token and lexeme
    lexer->lex();
    Token var_tok = lexer->nextToken;
    std::string var_lex = lexer->lexeme;

    std::cout << "Type: " << type_lex << ", variable: " << var_lex << '\n'; 

    // check to make sure the nextToken is actually an identifier
    if (lexer->nextToken != ID) {
        std::cout << "ERROR - invalid declaration." << '\n';
        exit(-1);
    }
    // create two IDStmtAST classes for the type and variable and add them to the declaration class
    auto data_type = new IDStmtAST(type_tok, type_lex);
    auto var = new IDStmtAST(var_tok, var_lex);
    auto decl = new DeclarationStmtAST(data_type, var);

    // get the next token
    lexer->lex();
    if (lexer->nextToken == ASSIGN_OP) {
        std::cout << "ERROR - Cannot initialize a variable when declared.\n";
        exit(-1);
    }
    // return the declaration node
    return decl;
}

AssignStmtAST* Parser::assign() {
    // make an IDStmtAST for the variable on the lefthand side
    auto lefthand_side = new IDStmtAST(lexer->nextToken, lexer->lexeme);
    std::cout << "LHS: " << lexer->lexeme << '\n';

    // check to make sure the next token is the assignment operator
    lexer->lex();
    if (lexer->nextToken != ASSIGN_OP) {
        std::cout << "ERROR - invalid assignment." << '\n';
        exit(-1);
    }
    // check to make sure the next token is a valid start to an arithmetic expression
    lexer->lex();
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
        // parse the expression, add it to the assignment node and return the assignment node
        ExprStmtAST* righthand_side = expr();
        auto assignment = new AssignStmtAST(lefthand_side, righthand_side, 'a');
        return assignment;
    }
    else if (lexer->nextToken == LBRACK) {
        lexer->lex();
        std::cout << "Parsing bool_expr...\n";
        BoolExpr* boolean_expr = bool_expr();
        if (lexer->nextToken != RBRACK) {
            std::cout << "ERROR - Need to close boolean expression.\n";
            std::cout << lexer->lexeme << '\n';
            exit(-1);
        }
        lexer->lex();
        std::cout << "Parsed bool_expr.\n";

        auto assignment = new AssignStmtAST(lefthand_side, boolean_expr, 'b');
        return assignment;
    }
    std::cout << "ERROR - invalid assignment." << '\n';
    exit(-1);
}

/*******************************************************
 * PARSE FUNCTION DEFINITIONS FOR ARITHMETIC EXPRESSIONS
*******************************************************/

ExprStmtAST* Parser::expr() {
    // parse the term node
    TermStmtAST* trm = term();

    // parse the expression prime node
    Expr_PStmtAST* expression_p = expr_p();

    // return expression node
    auto expression = new ExprStmtAST(trm, expression_p);
    return expression;
}

Expr_PStmtAST* Parser::expr_p() {
    // if the next token is a +/- parse another expr_p
    if (lexer->nextToken == ADD_OP || lexer->nextToken == SUB_OP) {
        // get the operator token
        Token op_tok = lexer->nextToken;
        
        // parse the term and next expr_p
        lexer->lex();
        TermStmtAST* trm = term();
        Expr_PStmtAST* next_expression_p = expr_p();
        // may need another lexer->lex() here

        // add the operator, term, and expr_p to the expr_p node and return
        auto expression_p = new Expr_PStmtAST(op_tok, trm, next_expression_p);
        return expression_p;
    }
    // otherwise return nullptr because there is nothing more to parse
    return nullptr;
}

TermStmtAST* Parser::term() {
    // check once more that the first token is a valid term token (may not be necessary but let's be safe shall we?)
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
        // parse the factor node and the term_p node
        FactorStmtAST* fctr = factor();
        Term_PStmtAST* trm_p = term_p();

        // add factor and term_p and return the term node
        auto trm = new TermStmtAST(fctr, trm_p);
        return trm;
    }

    std::cout << "ERROR - invalid term." << '\n';
    exit(-1);
}

Term_PStmtAST* Parser::term_p() {
    // if the next token is a times/divide/modulus parse another term_p
    if (lexer->nextToken == MULT_OP || lexer->nextToken == DIV_OP || lexer->nextToken == MOD_OP) {
        // get the operator
        Token op_tok = lexer->nextToken;

        // parse the factor and term_p, add to the parent term_p node and return it
        lexer->lex();
        FactorStmtAST* fctr = factor();
        Term_PStmtAST* next_trm_p = term_p();
        // may need another lexer->lex() here

        auto trm_p = new Term_PStmtAST(op_tok, fctr, next_trm_p);
        return trm_p;
    }

    // if the next token is not an operator just return nullptr
    return nullptr;
}

FactorStmtAST* Parser::factor() {
    // if the next token is an identifier, int literal, or float literal return the factor
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT) {
        Token fac_tok = lexer->nextToken;
        std::string fac_lex = lexer->lexeme;
        lexer->lex();

        auto fctr_val = new IDStmtAST(fac_tok, fac_lex);
        auto fctr = new FactorStmtAST(fctr_val, nullptr);
        return fctr;
    }
    // otherwise, if the next token is a left parenthesis, parse an expr and return it
    else if (lexer->nextToken == LPAREN) {
        ExprStmtAST* expression;

        // parse the expression if the next token is a valid start to an arithmetic expression
        lexer->lex();
        if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
            expression = expr();
        }
        else {
            std::cout << "ERROR - invalid factor.\n";
            exit(-1);
        }
        // check to make sure there is a close parenthesis
        if (lexer->nextToken == RPAREN) {
            lexer->lex();

            // make the factor and return it
            auto fctr = new FactorStmtAST(nullptr, expression);
            return fctr;
        }
    }
    // Otherwise, the factor is invalid
    std::cout << "ERROR - invalid factor." << '\n';
    exit(-1);
}

/****************************************************
 * PARSE FUNCTION DEFINITIONS FOR BOOLEAN EXPRESSIONS
****************************************************/

BoolExpr* Parser::bool_expr() {
    std::cout << "Parsing bool_term...\n";
    BoolTerm* term = bool_term();
    std::cout << "Parsed bool_term.\n";
    std::cout << "Parsing bool_expr_p...\n";
    BoolExpr* expr_p = bool_expr_p();
    std::cout << "Parsed bool_expr_p.\n";

    auto expr = new BoolExpr(NOP, term, expr_p);
    return expr;
}

BoolExpr* Parser::bool_expr_p() {
    if (lexer->nextToken == OR) {
        lexer->lex();
        std::cout << "Parsing bool_term...\n";
        BoolTerm* term = bool_term();
        std::cout << "Parsed bool_term.\n";
        std::cout << "Parsing bool_expr_p...\n";
        BoolExpr* next_expr_p = bool_expr_p();
        std::cout << "Parsed bool_expr_p.\n";

        auto expr_p = new BoolExpr(OR, term, next_expr_p);
        return expr_p;
    }

    return nullptr;
}

BoolTerm* Parser::bool_term() {
    std::cout << "Parsing bool_factor...\n";
    BoolFactor* factor = bool_factor();
    std::cout << "Parsed bool_factor.\n";
    std::cout << "Parsing bool_term_p...\n";
    BoolTerm* term_p = bool_term_p();
    std::cout << "Parsed bool_term_p.\n";

    auto term = new BoolTerm(NOP, factor, term_p);
    return term;
}

BoolTerm* Parser::bool_term_p() {
    if (lexer->nextToken == AND) {
        lexer->lex();
        std::cout << "Parsing bool_factor...\n";
        BoolFactor* factor = bool_factor();
        std::cout << "Parsed bool_factor.\n";
        std::cout << "Parsing bool_term_p...\n";
        BoolTerm* next_term_p = bool_term_p();
        std::cout << "Parsed bool_term_p.\n";

        auto term_p = new BoolTerm(AND, factor, next_term_p);
        return term_p;
    }

    return nullptr;
}

BoolFactor* Parser::bool_factor() {
    if (lexer->nextToken == NOT) {
        lexer->lex();
        if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT) {
            std::cout << "Parsing rel_operand...\n";
            IDStmtAST* operand = rel_operand();
            std::cout << "Parsed rel_operand.\n";
            std::cout << "Parsing relation...\n";
            Relation* rel = relation();
            std::cout << "Parsed relation.\n";

            SuperRel* super = new SuperRel(operand, rel);
            auto factor = new BoolFactor(true, 's', super);
            return factor;
        }
        else if (lexer->nextToken == BOOL_LIT) {
            std::cout << "Boolean literal: " << lexer->lexeme << '\n';
            
            IDStmtAST* literal = new IDStmtAST(lexer->nextToken, lexer->lexeme);
            BoolFactor* factor = new BoolFactor(true, 'l', literal); 
            lexer->lex();
        }
        else if (lexer->nextToken == LPAREN) {
            lexer->lex();
            std::cout << "Parsing bool_expr...\n";
            BoolExpr* expr = bool_expr();
            std::cout << "Parsed bool_expr.\n";
            if (lexer->nextToken != RPAREN) {
                std::cout << "ERROR - Invalid factor.\n";
                exit(-1);
            }
            lexer->lex();

            BoolFactor* factor = new BoolFactor(true, 'e', expr);
        }
        else {
            std::cout << "ERROR - Invalid factor.\n";
        }
    }
    else {
        if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT) {
            std::cout << "Parsing rel_operand...\n";
            IDStmtAST* operand = rel_operand();
            std::cout << "Parsed rel_operand.\n";
            std::cout << "Parsing relation...\n";
            Relation* rel = relation();
            std::cout << "Parsed relation.\n";

            SuperRel* super = new SuperRel(operand, rel);
            auto factor = new BoolFactor(false, 's', super);
            return factor;
        }
        else if (lexer->nextToken == BOOL_LIT) {
            std::cout << "Boolean literal: " << lexer->lexeme << '\n';
            
            IDStmtAST* literal = new IDStmtAST(lexer->nextToken, lexer->lexeme);
            BoolFactor* factor = new BoolFactor(true, 'l', literal); 
            lexer->lex();
        }
        else if (lexer->nextToken == LPAREN) {
            lexer->lex();
            std::cout << "Parsing bool_expr...\n";
            BoolExpr* expr = bool_expr();
            std::cout << "Parsed bool_expr.\n";
            if (lexer->nextToken != RPAREN) {
                std::cout << "ERROR - Invalid factor.\n";
                exit(-1);
            }
            lexer->lex();

            BoolFactor* factor = new BoolFactor(false, 'e', expr);
        }
        else {
            std::cout << "ERROR - Invalid factor.\n";
        }
    }
}

Relation* Parser::relation() {
    if (lexer->nextToken == GR || lexer->nextToken == GREQ || lexer->nextToken == LS || lexer->nextToken == LSEQ || lexer->nextToken == EQUAL || lexer->nextToken == NEQUAL) {
        lexer->lex();
        std::cout << "Parsing rel_operand...\n";
        IDStmtAST* operand = rel_operand();
        std::cout << "Parsed rel_operand.\n";
        std::cout << "Parsing relation...\n";
        Relation* rel = relation();
        std::cout << "Parsed relation.\n";

        Relation* next_rel = new Relation(lexer->nextToken, operand, rel);
        return next_rel;
    }

    return nullptr;
}

IDStmtAST* Parser::rel_operand() {
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT) {
        std::cout << "Relational operand: " << lexer->lexeme << '\n';
        IDStmtAST* operand = new IDStmtAST(lexer->nextToken, lexer->lexeme);
        
        lexer->lex();
        return operand;
    }

    std::cout << "Error - Invalid operand.\n";
    exit(-1);
}