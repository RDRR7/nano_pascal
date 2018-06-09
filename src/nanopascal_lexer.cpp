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
    case Symbol::OpenBra:
        return "OpenBra";
    case Symbol::CloseBra:
        return "CloseBra";
    case Symbol::Comma:
        return "Comma";
    case Symbol::Semicolon:
        return "Semicolon";
    case Symbol::OpenPar:
        return "OpenPar";
    case Symbol::ClosePar:
        return "ClosePar";
    case Symbol::OpEqual:
        return "OpEqual";
    case Symbol::OpSub:
        return "OpSub";
    case Symbol::OpAdd:
        return "OpAdd";
    case Symbol::OpMul:
        return "OpMul";
    case Symbol::OpLessThan:
        return "OpLessThan";
    case Symbol::OpGreaterThan:
        return "OpGreaterThan";
    case Symbol::OpNotEqual:
        return "OpNotEqual";
    case Symbol::OpLessThanOrEqual:
        return "OpLessThanOrEqual";
    case Symbol::OpGreaterThanOrEqual:
        return "OpGreaterThanOrEqual";
    case Symbol::Assign:
        return "Assign";
    case Symbol::Colon:
        return "Colon";
    case Symbol::OpLeftShift:
        return "OpLeftShift";
    case Symbol::OpRightShift:
        return "OpRightShift";
    case Symbol::StringConstant:
        return "StringConstant";
    case Symbol::ID:
        return "ID";
    case Symbol::IntConstantDec:
        return "IntConstantDec";
    case Symbol::IntConstantHex:
        return "IntConstantHex";
    case Symbol::IntConstantBin:
        return "IntConstantBin";
    case Symbol::CharConstant:
        return "CharConstant";
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
    return Symbol::ID;
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
        case '[':
            RETURN_TOKEN(Symbol::OpenBra);
        case ']':
            RETURN_TOKEN(Symbol::CloseBra);
        case ',':
            RETURN_TOKEN(Symbol::Comma);
        case ';':
            RETURN_TOKEN(Symbol::Semicolon);
        case '(':
            RETURN_TOKEN(Symbol::OpenPar);
        case ')':
            RETURN_TOKEN(Symbol::ClosePar);
        case '=':
            RETURN_TOKEN(Symbol::OpEqual);
        case '-':
            RETURN_TOKEN(Symbol::OpSub);
        case '+':
            RETURN_TOKEN(Symbol::OpAdd);
        case '*':
            RETURN_TOKEN(Symbol::OpMul);
        case '<':
            get_next_symbol();
            if (this->current_symbol == '>')
            {
                this->lexeme = "<>";
                get_next_symbol();
                return Symbol::OpNotEqual;
            }
            else if (this->current_symbol == '=')
            {
                this->lexeme = "<=";
                get_next_symbol();
                return Symbol::OpLessThanOrEqual;
            }
            else if (this->current_symbol == '<')
            {
                this->lexeme = "<<";
                get_next_symbol();
                return Symbol::OpLeftShift;
            }
            this->lexeme = '<';
            return Symbol::OpLessThan;
        case '>':
            get_next_symbol();
            if (this->current_symbol == '=')
            {
                this->lexeme = ">=";
                get_next_symbol();
                return Symbol::OpGreaterThanOrEqual;
            }
            else if (this->current_symbol == '>')
            {
                this->lexeme = ">>";
                get_next_symbol();
                return Symbol::OpRightShift;
            }
            this->lexeme = '>';
            return Symbol::OpGreaterThan;
        case ':':
            get_next_symbol();
            if (this->current_symbol == '=')
            {
                this->lexeme = ":=";
                get_next_symbol();
                return Symbol::Assign;
            }
            this->lexeme = ':';
            return Symbol::Colon;
        case '\'':
            get_next_symbol();
            append_sequence([this](char ch) {
                if(ch=='\"'){
                    this->current_symbol = '\'';
                }
                return ch != '\'' &&
                       ch != '\n' &&
                       ch != EOF; });
            if (this->current_symbol == '\'')
            {
                get_next_symbol();
                if (this->lexeme.length() == 0)
                {
                    std::cerr << "Invalid empty string" << std::endl;
                    exit(1);
                }
                else if (this->lexeme.length() == 1)
                {
                    return Symbol::CharConstant;
                }
                return Symbol::StringConstant;
            }
            std::cerr << "Missing closing \' in string constant" << std::endl;
            exit(1);
        case EOF:
            RETURN_TOKEN(Symbol::Eof);
        default:
            if (isdigit(this->current_symbol))
            {
                append_sequence([](char ch) { return isdigit(ch); });
                return Symbol::IntConstantDec;
            }
            else if (this->current_symbol == '$')
            {
                get_next_symbol();
                if (isxdigit(this->current_symbol))
                {
                    append_sequence([](char ch) { return isxdigit(ch); });
                    return Symbol::IntConstantHex;
                }
                std::cerr << "Expected hexadecimal int constant" << std::endl;
                exit(1);
            }
            else if (this->current_symbol == '%')
            {
                get_next_symbol();
                if (this->current_symbol == '0' || this->current_symbol == '1')
                {
                    append_sequence([](char ch) { return ch == '0' || ch == '1'; });
                    return Symbol::IntConstantBin;
                }
                std::cerr << "Expected binary int constant" << this->current_symbol << std::endl;
                exit(1);
            }
            else if (isalpha(this->current_symbol) || this->current_symbol == '_')
            {
                append_sequence([](char ch) { return isalnum(ch) ||
                                                     (ch == '_'); });
                return look_up_keyword();
            }
            else
            {
                std::cerr << "Invalid symbol " << this->current_symbol << std::endl;
                exit(1);
            }
        }
    }
}
