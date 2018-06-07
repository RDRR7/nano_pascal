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
    "array",
    "of",
    "integer",
    "boolean",
    "char",
    "not",
    "and",
    "or",
    "xor",
    "shl",
    "shr",
    "div",
    "mod",
    "begin",
    "end",
    "break",
    "if",
    "then",
    "else",
    "while",
    "repeat",
    "until",
    "for",
    "do",
    "write",
    "writeln",
    "to",
    0,
};

static Symbol kwTk[] = {
    Symbol::KwProgram,
    Symbol::KwVar,
    Symbol::KwArray,
    Symbol::KwOf,
    Symbol::KwInteger,
    Symbol::KwBoolean,
    Symbol::KwChar,
    Symbol::KwNot,
    Symbol::KwAnd,
    Symbol::KwOr,
    Symbol::KwXor,
    Symbol::KwShl,
    Symbol::KwShr,
    Symbol::KwDiv,
    Symbol::KwMod,
    Symbol::KwBegin,
    Symbol::KwEnd,
    Symbol::KwBreak,
    Symbol::KwIf,
    Symbol::KwThen,
    Symbol::KwElse,
    Symbol::KwWhile,
    Symbol::KwRepeat,
    Symbol::KwUntil,
    Symbol::KwFor,
    Symbol::KwDo,
    Symbol::KwWrite,
    Symbol::KwWriteln,
    Symbol::KwTo,
};

const char *NanoPascalLexer::get_symbol_name(Symbol Symbol)
{

    switch (Symbol)
    {
    case Symbol::Number:
        return "Number";
    case Symbol::Ident:
        return "Identifier";
    case Symbol::OpAdd:
        return "Add";
    case Symbol::OpSub:
        return "Sub";
    case Symbol::OpMul:
        return "Mul";
    case Symbol::OpenPar:
        return "Open Par";
    case Symbol::ClosePar:
        return "Close Par";
    case Symbol::Semicolon:
        return "Semicolon";
    case Symbol::Eof:
        return "End of Input";
    case Symbol::KwProgram:
        return "Program";
    case Symbol::KwVar:
        return "Var";
    case Symbol::KwArray:
        return "Array";
    case Symbol::KwOf:
        return "Of";
    case Symbol::KwInteger:
        return "Integer";
    case Symbol::KwBoolean:
        return "Boolean";
    case Symbol::KwChar:
        return "Char";
    case Symbol::KwNot:
        return "Not";
    case Symbol::KwAnd:
        return "And";
    case Symbol::KwOr:
        return "Or";
    case Symbol::KwXor:
        return "Xor";
    case Symbol::KwShl:
        return "Shl";
    case Symbol::KwShr:
        return "Shr";
    case Symbol::KwDiv:
        return "Div";
    case Symbol::KwMod:
        return "Mod";
    case Symbol::KwBegin:
        return "Begin";
    case Symbol::KwEnd:
        return "End";
    case Symbol::KwBreak:
        return "Break";
    case Symbol::KwIf:
        return "If";
    case Symbol::KwThen:
        return "Then";
    case Symbol::KwElse:
        return "Else";
    case Symbol::KwWhile:
        return "While";
    case Symbol::KwRepeat:
        return "Repeat";
    case Symbol::KwUntil:
        return "Until";
    case Symbol::KwFor:
        return "For";
    case Symbol::KwDo:
        return "Do";
    case Symbol::KwWrite:
        return "Write";
    case Symbol::KwWriteln:
        return "Writeln";
    case Symbol::KwTo:
        return "To";
    }

    return "Unknown";
}

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
