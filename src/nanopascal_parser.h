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

	UP_ProgramNode program();												   // Done
	VariableDeclList variable_section();									   // Done
	UP_VariableDeclNode variable_decl();									   // Done
	std::list<std::string> id_list();										   // Done
	std::tuple<ReturnType, int, int> type();								   // Done
	std::tuple<ReturnType, int, int> array_type();							   // Done
	UP_SubprogramDeclNode subprogram_decl();								   // Done
	std::tuple<std::string, ArgumentDeclList, ReturnType> subprogram_header(); // Done
	std::tuple<std::string, ArgumentDeclList, ReturnType> function_header();   // Done
	std::tuple<std::string, ArgumentDeclList, ReturnType> procedure_header();  // Done
	ArgumentDeclList argument_decl_list();									   // Done
	UP_ArgumentDeclNode argument_decl();									   // Done
	StatementList statement_list();											   // Done
	UP_StatementNode statement();											   // Done
	UP_ASTNode expr();														   // Done
	UP_ASTNode exprpfi();													   // Done
	UP_ASTNode exprpse();													   // Done
	UP_ASTNode exprpth();													   // Done
	UP_ASTNode exprpla();													   //TO DO
	UP_StatementNode subprogram_call();										   // Done
	ASTNodelList expr_list();												   // Done
	ASTNodelList argument_list();											   // Done
	UP_ASTNode argument();													   // Done
	UP_ASTNode constant();													   // Done
	StatementList block();													   // Done
};

#endif
