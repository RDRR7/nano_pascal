#ifndef _NANOPASCAL_PARSER_H
#define _NANOPASCAL_PARSER_H

#include "nanopascal_lexer.h"
#include "nanopascal_nodes.h"
#include <string>
#include <tuple>

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

	UP_ProgramNode program();
	VariableDeclList variable_section();
	UP_VariableDeclNode variable_decl();
	std::list<std::string> id_list();
	std::tuple<ReturnType, int, int> type();
	std::tuple<ReturnType, int, int> array_type();
	UP_SubprogramDeclNode subprogram_decl();
	std::tuple<std::string, ArgumentDeclList, ReturnType, int, int> subprogram_header();
	std::tuple<std::string, ArgumentDeclList, ReturnType, int, int> function_header();
	std::tuple<std::string, ArgumentDeclList, ReturnType, int, int> procedure_header();
	ArgumentDeclList argument_decl_list();
	UP_ArgumentDeclNode argument_decl();
	StatementList statement_list();
	UP_StatementNode statement();
	UP_ASTNode expr();
	UP_ASTNode exprpfi();
	UP_ASTNode exprpse();
	UP_ASTNode exprpth();
	UP_ASTNode exprpla();
	UP_StatementNode subprogram_call();
	ASTNodelList expr_list();
	ASTNodelList argument_list();
	UP_ASTNode argument();
	UP_ASTNode constant();
	StatementList block();
};

#endif
