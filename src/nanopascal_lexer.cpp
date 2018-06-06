#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <functional>
#include "nanopascal_lexer.h"

#define RETURN_TOKEN(tk)                     \
    do                                       \
    {                                        \
        this->lexeme = this->current_symbol; \
        get_next_symbol();                   \
        return tk;                           \
    } while (0)

static const char *kw[] = {
    "program",
    "var",
    "begin",
    "end",
    "if",
    "then",
    "else",
    "writeln",
    0,
};

static Symbol kwTk[] = {
    Symbol::KwProgram,
    Symbol::KwVar,
    Symbol::KwBegin,
    Symbol::KwEnd,
    Symbol::KwIf,
    Symbol::KwThen,
    Symbol::KwElse,
    Symbol::KwWriteln,
};

void NanoPascalLexer::append_sequence(std::function<bool(char)> func)
{
    while (func(this->current_symbol))
    {
        this->lexeme += this->current_symbol;
        get_next_symbol();
    }
}

Symbol NanoPascalLexer::look_up_keyword()
{
    for (int i = 0; kw[i] != 0; i++)
    {
        if (strcasecmp(kw[i], lexeme.c_str()) == 0)
        {
            return kwTk[i];
        }
    }

    return Symbol::Ident;
}

Symbol NanoPascalLexer::get_next_token()
{

    lexeme = "";
    while (1)
    {
        if (this->current_symbol == '\n')
        {
            this->line_number++;
            this->column_number = 1;
            get_next_symbol();
            continue;
        }
        if (this->current_symbol == ' ' || this->current_symbol == '\t')
        {
            this->column_number++;
            get_next_symbol();
            continue;
        }

        switch (this->current_symbol)
        {
        case '+':
            RETURN_TOKEN(Symbol::OpAdd);
        case '-':
            RETURN_TOKEN(Symbol::OpSub);
        case '*':
            RETURN_TOKEN(Symbol::OpMul);
        case '/':
            RETURN_TOKEN(Symbol::OpDiv);
        case '(':
            RETURN_TOKEN(Symbol::OpenPar);
        case ')':
            RETURN_TOKEN(Symbol::ClosePar);
        case ',':
            RETURN_TOKEN(Symbol::Comma);
        case ';':
            RETURN_TOKEN(Symbol::Semicolon);
        case EOF:
            RETURN_TOKEN(Symbol::Eof);
        default:
            if (isdigit(this->current_symbol))
            {
                append_sequence([](char ch) { return isdigit(ch); });
                return Symbol::Number;
            }
            else if (isalpha(this->current_symbol) || this->current_symbol == '_')
            {
                append_sequence([](char ch) { return isalpha(ch) ||
                                                     (ch == '_') ||
                                                     isdigit(ch); });
                return look_up_keyword();
            }
            else if (this->current_symbol == '\'')
            {
                get_next_symbol();
                append_sequence([](char ch) { return ch != '\''; });
                get_next_symbol();
                return Symbol::StrLiteral;
            }
            else
            {
                std::cerr << "Invalid symbol " << this->current_symbol << std::endl;
                exit(1);
            }
        }
    }
}
