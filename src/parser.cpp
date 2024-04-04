#include "headers/parser.h"

ProgStmtAST* Parser::prog() {
    // parse the first line of the program
    LineStmtAST* statement = stmt();
    // if the next token is a newline then parse the rest of the program
    if (lexer->nextToken == NEWLINE) {
        lexer->lex();
        // parses the rest of the program
        ProgStmtAST* next_prog = prog();

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
    if (lexer->nextToken == INT_KWD || lexer->nextToken == FLOAT_KWD) {
        DeclarationStmtAST* decl = declaration();

        auto statement = new LineStmtAST(decl, nullptr);
        return statement;
    }
    // if the next token is an identifier then parse an assignment
    else if (lexer->nextToken == ID) {
        AssignStmtAST* assign_ptr = assign();

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
        auto assignment = new AssignStmtAST(lefthand_side, righthand_side);
        return assignment;
    }
    std::cout << "ERROR - invalid assignment." << '\n';
    exit(-1);
}

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