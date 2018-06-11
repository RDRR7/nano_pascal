#ifndef _NANOPASCAL_LEXER_H
#define _NANOPASCAL_LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <list>
#include <stack>

#include "nanopascal_symbols.h"

class NanoPascalLexer
{
public:
  NanoPascalLexer(std::ifstream &in) : in(in)
  {
    get_next_symbol();
    this->line_number = 1;
    this->column_number = 0;
    this->directives.push_back("NANOPASCAL");
  }

  static const char *get_symbol_name(Symbol Symbol);

  Symbol get_next_token();
  std::string get_lexeme() { return this->lexeme; }
  int get_line_number() { return this->line_number; }
  int get_column_number() { return this->column_number; }

private:
  std::ifstream &in;
  char current_symbol;
  std::string lexeme;
  int line_number;
  int column_number;
  std::list<std::string> directives;
  std::stack<int> directives_stack;

  void append_sequence(std::function<bool(char)> func);
  void get_next_symbol() { this->current_symbol = in.get(); }
  Symbol look_up_keyword();
  bool directive_exists(std::string);
};
#endif
