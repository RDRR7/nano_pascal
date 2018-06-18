#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

#include "nanopascal_lexer.h"

class NanoPascalParser
{
  public:
	NanoPascalParser(NanoPascalLexer &lexer) : lexer(lexer) {}
	void parse();

  private:
	NanoPascalLexer lexer;
	Symbol current_token;
};

#endif
