#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

#include "nanopascal_lexer.h"
#include <string>

class NanoPascalParser
{
  public:
	NanoPascalParser(NanoPascalLexer &lexer) : lexer(lexer) {}
	void parse();

  private:
	NanoPascalLexer lexer;
	Symbol current_token;

	void print_error(std::string expected);
	void get_next_token()
	{
		this->current_token = this->lexer.get_next_token();
	}
	void expected_token(Symbol symbol, std::string error)
	{
		if (this->current_token != symbol)
		{
			print_error(error);
			exit(1);
		}
		get_next_token();
	}

	void program();
	void variable_section();
	void variable_decl();
	void id_list();
	void type();
	void type_wo_array();
	void array_type();
	void subprogram_decl();
	void subprogram_header();
	void function_header();
	void procedure_header();
	void argument_decl_list();
	void argument_decl();
	void statement();
	void assign();
	void lvalue();
	void expr();
	void exprpfi();
	void exprpse();
	void exprpth();
	void exprpfo();
	void exprpla();
	void subprogram_call();
	void expr_list();
	void argument_list();
	void argument();
	void constant();
	void bool_constant();
	void block();
};

#endif