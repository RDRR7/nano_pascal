#include <iostream>
#include <fstream>
#include "nanopascal_lexer.h"

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
        std::cout << "Symbol: " << NanoPascalLexer::get_symbol_name(Symbol) << " '" << lexer.get_lexeme() << "'" << std::endl;
    }
}
