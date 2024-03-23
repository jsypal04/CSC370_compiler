#include "headers/parser.h"

std::unique_ptr<ProgStmtAST> Parser::prog() {

    std::cout << "Parsing stmt..." << '\n';
    std::unique_ptr<LineStmtAST> statement = std::move(stmt());
    std::cout << "Stmt parsed." << '\n';
    if (lexer->nextToken == NEWLINE) {
        lexer->lex();
        std::cout << "Parsing prog..." << '\n';
        std::unique_ptr<ProgStmtAST> next_prog = std::move(prog());
        std::cout << "Prog parsed." << '\n';

        auto program = std::make_unique<ProgStmtAST>(statement, next_prog);
        return program;
    }
    return nullptr;
}

std::unique_ptr<LineStmtAST> Parser::stmt() {
    if (lexer->nextToken == INT_KWD || lexer->nextToken == FLOAT_KWD) {
        std::cout << "Parsing declaration..." << '\n';
        std::unique_ptr<DeclarationStmtAST> decl = std::move(declaration());
        std::cout << "Declaration parsed..." << '\n';

        auto statement = std::make_unique<LineStmtAST>(decl, nullptr);
        return statement;
    }
    else if (lexer->nextToken == ID) {
        std::cout << "Parsing assign..." << '\n';
        std::unique_ptr<AssignStmtAST> assign_ptr = std::move(assign());
        std::cout << "Assign parsed." << '\n';

        auto statement = std::make_unique<LineStmtAST>(nullptr, assign_ptr);
        return statement;
    }
    std::cout << "ERROR - invalid statement." << '\n';
    return nullptr;
}

std::unique_ptr<DeclarationStmtAST> Parser::declaration() {
    Token type_tok = lexer->nextToken;
    std::string type_lex = lexer->lexeme;
    std::cout << "Type: " << lexer->lexeme;

    lexer->lex();
    Token var_tok = lexer->nextToken;
    std::string var_lex = lexer->lexeme;
    std::cout << ", variable: " << lexer->lexeme << "\n";

    if (lexer->nextToken != ID) {
        std::cout << "ERROR - invalid declaration." << '\n';
        return nullptr;
    }
    auto data_type = std::make_unique<IDStmtAST>(type_tok, type_lex);
    auto var = std::make_unique<IDStmtAST>(var_tok, var_lex);
    auto decl = std::make_unique<DeclarationStmtAST>(data_type, var);

    lexer->lex();

    return decl;
}

std::unique_ptr<AssignStmtAST> Parser::assign() {
    std::cout << "Lefthand side: " << lexer->lexeme << '\n';
    auto lefthand_side = std::make_unique<IDStmtAST>(lexer->nextToken, lexer->lexeme);

    lexer->lex();
    if (lexer->nextToken != ASSIGN_OP) {
        std::cout << "ERROR - invalid assignment." << '\n';
        return nullptr;
    }
    lexer->lex();
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
        std::cout << "Parsing expr..." << '\n';
        std::unique_ptr<ExprStmtAST> righthand_side = std::move(expr());
        std::cout << "Expr parsed." << '\n';

        auto assignment = std::make_unique<AssignStmtAST>(lefthand_side, righthand_side);
        return assignment;
    }
    std::cout << "ERROR - invalid assignment." << '\n';
    return nullptr;
}

std::unique_ptr<ExprStmtAST> Parser::expr() {
    std::cout << "Parsing term..." << '\n';
    std::unique_ptr<TermStmtAST> trm = std::move(term());
    std::cout << "Term parsed." << '\n';

    std::cout << "Parsing expr_p..." << '\n';
    std::unique_ptr<Expr_PStmtAST> expression_p = std::move(expr_p());
    std::cout << "Expr_p parsed." << '\n';

    auto expression = std::make_unique<ExprStmtAST>(trm, expression_p);
    return expression;
}

std::unique_ptr<Expr_PStmtAST> Parser::expr_p() {
    //cout << "\n\nEXPR_P TOKEN: " << lexer->nextToken << ", LEXEME: " << lexer->lexeme << "\n\n";
    if (lexer->nextToken == ADD_OP || lexer->nextToken == SUB_OP) {
        Token op_tok = lexer->nextToken;
        
        lexer->lex();
        std::cout << "Parsing term..." << '\n';
        std::unique_ptr<TermStmtAST> trm = std::move(term());
        std::cout << "Term parsed." << '\n';
        std::cout << "Parsing expr_p..." << '\n';
        std::unique_ptr<Expr_PStmtAST> next_expression_p = std::move(expr_p());
        std::cout << "Expr_p parsed." << '\n';
        // may need another lexer->lex() here

        auto expression_p = std::make_unique<Expr_PStmtAST>(op_tok, trm, next_expression_p);
        return expression_p;
    }

    return nullptr;
}

std::unique_ptr<TermStmtAST> Parser::term() {
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
        std::cout << "Parsing factor..." << '\n';
        std::unique_ptr<FactorStmtAST> fctr = std::move(factor());
        std::cout << "Factor parsed." << '\n';

        std::cout << "Parsing term_p..." << '\n';
        std::unique_ptr<Term_PStmtAST> trm_p = std::move(term_p());
        std::cout << "Term_p parsed." << '\n';

        auto trm = std::make_unique<TermStmtAST>(fctr, trm_p);
        return trm;
    }

    std::cout << "ERROR - invalid term." << '\n';
    return nullptr;
}

std::unique_ptr<Term_PStmtAST> Parser::term_p() {
    if (lexer->nextToken == MULT_OP || lexer->nextToken == DIV_OP || lexer->nextToken == MOD_OP) {
        Token op_tok = lexer->nextToken;

        lexer->lex();
        std::cout << "Parsing factor..." << '\n';
        std::unique_ptr<FactorStmtAST> fctr = std::move(factor());
        std::cout << "Factor parsed." << '\n';

        std::cout << "Parsing term_p..." << '\n';
        std::unique_ptr<Term_PStmtAST> next_trm_p = std::move(term_p());
        std::cout << "Term_p parsed." << '\n';
        // may need another lexer->lex() here

        auto trm_p = std::make_unique<Term_PStmtAST>(op_tok, fctr, next_trm_p);
        return trm_p;
    }

    return nullptr;
}

std::unique_ptr<FactorStmtAST> Parser::factor() {
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT) {
        Token fac_tok = lexer->nextToken;
        std::string fac_lex = lexer->lexeme;
        std::cout << "Factor: " << lexer->lexeme << '\n';
        lexer->lex();

        auto fctr_val = std::make_unique<IDStmtAST>(fac_tok, fac_lex);
        auto fctr = std::make_unique<FactorStmtAST>(fctr_val);
        return fctr;
    }
    else if (lexer->nextToken == LPAREN) {
        std::unique_ptr<ExprStmtAST> expression;

        lexer->lex();
        if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
            std::cout << "Parsing expr..." << '\n';
            expression = std::move(expr());
            std::cout << "Expr parsed." << '\n';
        }
        else {
            std::cout << "ERROR - invalid factor.\n";
            return nullptr;
        }
        if (lexer->nextToken == RPAREN) {
            lexer->lex();

            auto fctr = std::make_unique<FactorStmtAST>(expression);
            return fctr;
        }
    }
    std::cout << "ERROR - invalid factor." << '\n';
    return nullptr;
}