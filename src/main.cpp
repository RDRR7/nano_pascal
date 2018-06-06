#include <iostream>
#include <fstream>
#include "nanopascal_lexer.h"

const char *get_symbol_name(Symbol Symbol)
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
    }

    return "Unknown";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    std::ifstream in(argv[1], std::ios::in);

    if (!in.is_open())
    {
        std::cerr << "Cannot open input file '" << argv[1] << "'" << std::endl;
        return 1;
    }

    NanoPascalLexer lexer(in);

    Symbol Symbol;

    while ((Symbol = lexer.get_next_token()) != Symbol::Eof)
    {
        std::cout << "Symbol: " << get_symbol_name(Symbol) << " '" << lexer.get_lexeme() << "'" << std::endl;
    }
}
