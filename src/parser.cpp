#include "headers/parser.h"

std::unique_ptr<ProgStmtAST> Parser::prog() {
    // parse the first line of the program
    std::unique_ptr<LineStmtAST> statement = std::move(stmt());
    // if the next token is a newline then parse the rest of the program
    if (lexer->nextToken == NEWLINE) {
        lexer->lex();
        // parses the rest of the program
        std::unique_ptr<ProgStmtAST> next_prog = std::move(prog());

        // creates a pointer to a ProgStmtAST class and returns that pointer
        auto program = std::make_unique<ProgStmtAST>(std::move(statement), std::move(next_prog));
        return program;
    }
    // if the next token is not a newline then the program is done
    // creates a pointer to a ProgStmtAST class and returns the pointer
    auto program = std::make_unique<ProgStmtAST>(std::move(statement), nullptr);
    return program;
}

std::unique_ptr<LineStmtAST> Parser::stmt() {
    // if the next token is a type keyword then parse a variable declaration
    if (lexer->nextToken == INT_KWD || lexer->nextToken == FLOAT_KWD) {
        std::unique_ptr<DeclarationStmtAST> decl = std::move(declaration());

        auto statement = std::make_unique<LineStmtAST>(std::move(decl), nullptr);
        return statement;
    }
    // if the next token is an identifier then parse an assignment
    else if (lexer->nextToken == ID) {
        std::unique_ptr<AssignStmtAST> assign_ptr = std::move(assign());

        auto statement = std::make_unique<LineStmtAST>(nullptr, std::move(assign_ptr));
        return statement;
    }
    // otherwise this is an undefined statement
    std::cout << "ERROR - invalid statement." << '\n';
    return nullptr;
}

std::unique_ptr<DeclarationStmtAST> Parser::declaration() {
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
        return nullptr;
    }
    // create two IDStmtAST classes for the type and variable and add them to the declaration class
    auto data_type = std::make_unique<IDStmtAST>(type_tok, type_lex);
    auto var = std::make_unique<IDStmtAST>(var_tok, var_lex);
    auto decl = std::make_unique<DeclarationStmtAST>(std::move(data_type), std::move(var));

    // get the next token
    lexer->lex();
    // return the declaration node
    return decl;
}

std::unique_ptr<AssignStmtAST> Parser::assign() {
    // make an IDStmtAST for the variable on the lefthand side
    auto lefthand_side = std::make_unique<IDStmtAST>(lexer->nextToken, lexer->lexeme);

    // check to make sure the next token is the assignment operator
    lexer->lex();
    if (lexer->nextToken != ASSIGN_OP) {
        std::cout << "ERROR - invalid assignment." << '\n';
        return nullptr;
    }
    // check to make sure the next token is a valid start to an arithmetic expression
    lexer->lex();
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
        // parse the expression, add it to the assignment node and return the assignment node
        std::unique_ptr<ExprStmtAST> righthand_side = std::move(expr());
        auto assignment = std::make_unique<AssignStmtAST>(std::move(lefthand_side), std::move(righthand_side));
        return assignment;
    }
    std::cout << "ERROR - invalid assignment." << '\n';
    return nullptr;
}

std::unique_ptr<ExprStmtAST> Parser::expr() {
    // parse the term node
    std::unique_ptr<TermStmtAST> trm = std::move(term());

    // parse the expression prime node
    std::unique_ptr<Expr_PStmtAST> expression_p = std::move(expr_p());

    // return expression node
    auto expression = std::make_unique<ExprStmtAST>(std::move(trm), std::move(expression_p));
    return expression;
}

std::unique_ptr<Expr_PStmtAST> Parser::expr_p() {
    // if the next token is a +/- parse another expr_p
    if (lexer->nextToken == ADD_OP || lexer->nextToken == SUB_OP) {
        // get the operator token
        Token op_tok = lexer->nextToken;
        
        // parse the term and next expr_p
        lexer->lex();
        std::unique_ptr<TermStmtAST> trm = std::move(term());
        std::unique_ptr<Expr_PStmtAST> next_expression_p = std::move(expr_p());
        // may need another lexer->lex() here

        // add the operator, term, and expr_p to the expr_p node and return
        auto expression_p = std::make_unique<Expr_PStmtAST>(op_tok, std::move(trm), std::move(next_expression_p));
        return expression_p;
    }
    // otherwise return nullptr because there is nothing more to parse
    return nullptr;
}

std::unique_ptr<TermStmtAST> Parser::term() {
    // check once more that the first token is a valid term token (may not be necessary but let's be safe shall we?)
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
        // parse the factor node and the term_p node
        std::unique_ptr<FactorStmtAST> fctr = std::move(factor());
        std::unique_ptr<Term_PStmtAST> trm_p = std::move(term_p());

        // add factor and term_p and return the term node
        auto trm = std::make_unique<TermStmtAST>(std::move(fctr), std::move(trm_p));
        return trm;
    }

    std::cout << "ERROR - invalid term." << '\n';
    return nullptr;
}

std::unique_ptr<Term_PStmtAST> Parser::term_p() {
    // if the next token is a times/divide/modulus parse another term_p
    if (lexer->nextToken == MULT_OP || lexer->nextToken == DIV_OP || lexer->nextToken == MOD_OP) {
        // get the operator
        Token op_tok = lexer->nextToken;

        // parse the factor and term_p, add to the parent term_p node and return it
        lexer->lex();
        std::unique_ptr<FactorStmtAST> fctr = std::move(factor());
        std::unique_ptr<Term_PStmtAST> next_trm_p = std::move(term_p());
        // may need another lexer->lex() here

        auto trm_p = std::make_unique<Term_PStmtAST>(op_tok, std::move(fctr), std::move(next_trm_p));
        return trm_p;
    }

    // if the next token is not an operator just return nullptr
    return nullptr;
}

std::unique_ptr<FactorStmtAST> Parser::factor() {
    // if the next token is an identifier, int literal, or float literal return the factor
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT) {
        Token fac_tok = lexer->nextToken;
        std::string fac_lex = lexer->lexeme;
        lexer->lex();

        auto fctr_val = std::make_unique<IDStmtAST>(fac_tok, fac_lex);
        auto fctr = std::make_unique<FactorStmtAST>(std::move(fctr_val), nullptr);
        return fctr;
    }
    // otherwise, if the next token is a left parenthesis, parse an expr and return it
    else if (lexer->nextToken == LPAREN) {
        std::unique_ptr<ExprStmtAST> expression;

        // parse the expression if the next token is a valid start to an arithmetic expression
        lexer->lex();
        if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
            expression = std::move(expr());
        }
        else {
            std::cout << "ERROR - invalid factor.\n";
            return nullptr;
        }
        // check to make sure there is a close parenthesis
        if (lexer->nextToken == RPAREN) {
            lexer->lex();

            // make the factor and return it
            auto fctr = std::make_unique<FactorStmtAST>(nullptr, std::move(expression));
            return fctr;
        }
    }
    // Otherwise, the factor is invalid
    std::cout << "ERROR - invalid factor." << '\n';
    return nullptr;
}