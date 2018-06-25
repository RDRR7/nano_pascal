#ifndef _NANOPASCAL_NODES_H
#define _NANOPASCAL_NODES_H

#include <memory>
#include <string>
#include <list>

#define DEFINE_BINARY_EXPR(name, prec, oper)                                                                         \
	class name##ExprNode : public BinaryExprNode                                                                     \
	{                                                                                                                \
	  public:                                                                                                        \
		name##ExprNode(UP_ExprNode expr1, UP_ExprNode expr2) : BinaryExprNode(std::move(expr1), std::move(expr2)) {} \
		int get_precedence()                                                                                         \
		{                                                                                                            \
			return prec;                                                                                             \
		}                                                                                                            \
		std::string get_oper()                                                                                       \
		{                                                                                                            \
			return oper;                                                                                             \
		}                                                                                                            \
	}

class ASTNode;
class ProgramNode;
class SubprogramDeclNode;
class StatementNode;
class VariableDeclNode;
class ArgumentDeclNode;
class AssignNode;
class ExprNode;
class SubprogramCallNode;
class ArgumentNode;
class IfNode;
class WhileNode;
class RepeatNode;
class ForNode;
class BranchingStatementNode;
class BinaryExprNode;
class NotExprNode;
class UnaryExprNode;
class NumberNode;
class ParExprNode;

using UP_ASTNode = std::unique_ptr<ASTNode>;
using UP_ProgramNode = std::unique_ptr<ProgramNode>;
using UP_SubprogramDeclNode = std::unique_ptr<SubprogramDeclNode>;
using UP_StatementNode = std::unique_ptr<StatementNode>;
using UP_VariableDeclNode = std::unique_ptr<VariableDeclNode>;
using UP_ArgumentDeclNode = std::unique_ptr<ArgumentDeclNode>;
using UP_AssignNode = std::unique_ptr<AssignNode>;
using UP_ExprNode = std::unique_ptr<ExprNode>;
using UP_SubprogramCallNode = std::unique_ptr<SubprogramCallNode>;
using UP_ArgumentNode = std::unique_ptr<ArgumentNode>;
using UP_IfNode = std::unique_ptr<IfNode>;
using UP_WhileNode = std::unique_ptr<WhileNode>;
using UP_RepeatNode = std::unique_ptr<RepeatNode>;
using UP_ForNode = std::unique_ptr<ForNode>;
using UP_BranchingStatementNode = std::unique_ptr<BranchingStatementNode>;
using UP_BinaryExprNode = std::unique_ptr<BinaryExprNode>;
using UP_NotExprNode = std::unique_ptr<NotExprNode>;
using UP_UnaryExprNode = std::unique_ptr<UnaryExprNode>;
using UP_NumberNode = std::unique_ptr<NumberNode>;
using UP_ParExprNode = std::unique_ptr<ParExprNode>;

using SubprogramDeclList = std::list<UP_SubprogramDeclNode>;
using StatementList = std::list<UP_StatementNode>;
using VariableDeclList = std::list<UP_VariableDeclNode>;
using ArgumentDeclList = std::list<UP_ArgumentDeclNode>;
using ExprList = std::list<UP_ExprNode>;
using ArgumentList = std::list<UP_ArgumentNode>;

enum class ReturnType : unsigned int
{
	Non = 0,
	Integer = 1,
	Boolean = 2,
	Char = 3,
};

enum class BranchingStatement : unsigned int
{
	Continue = 0,
	Break = 1,
};

class ASTNode
{
  public:
	ASTNode() {}
	virtual ~ASTNode() {}

	virtual std::string to_string() = 0;
};

class VariableDeclNode : public ASTNode
{
  public:
	VariableDeclNode(std::list<std::string> id_list,
					 ReturnType type,
					 int index1,
					 int index2) : id_list(id_list),
								   type(type),
								   index1(index1),
								   index2(index2) {}

	std::list<std::string> id_list;
	ReturnType type;
	int index1;
	int index2;

	std::string to_string()
	{
		std::string s_id_list = this->id_list.front();
		this->id_list.pop_front();
		for (auto id : this->id_list)
		{
			s_id_list += ", " + id;
		}

		std::string s_type = "";
		if (this->index1 == -1 && this->index2 == -1)
		{
			switch (this->type)
			{
			case ReturnType::Integer:
				s_type = "Integer;";
				break;
			case ReturnType::Boolean:
				s_type = "Boolean;";
				break;
			case ReturnType::Char:
				s_type = "Char;";
				break;
			}
		}
		else
		{
			switch (this->type)
			{
			case ReturnType::Integer:
				s_type = "Array [" + std::to_string(index1) + ".." + std::to_string(index2) + "] of Integer;";
				break;
			case ReturnType::Boolean:
				s_type = "Array [" + std::to_string(index1) + ".." + std::to_string(index2) + "] of Boolean";
				break;
			case ReturnType::Char:
				s_type = "Array [" + std::to_string(index1) + ".." + std::to_string(index2) + "] of Char";
				break;
			}
		}

		return s_id_list +
			   ": " +
			   s_type;
	}
};

class ProgramNode : public ASTNode
{
  public:
	ProgramNode(std::string id,
				VariableDeclList variable_section,
				SubprogramDeclList subprogram_decl,
				StatementList statement_list) : id(id),
												variable_section(std::move(variable_section)),
												subprogram_decl(std::move(subprogram_decl)),
												statement_list(std::move(statement_list)) {}

	std::string id;
	VariableDeclList variable_section;
	SubprogramDeclList subprogram_decl;
	StatementList statement_list;

	std::string to_string()
	{
		std::string s_variable_section = "\nvar ";
		for (auto &variable_decl : this->variable_section)
		{
			s_variable_section += "\n" + variable_decl->to_string();
		}

		return "program " + id + ";" +
			   s_variable_section +
			   "\nbegin"
			   "\nend.";
	}
};

class ArgumentDeclNode : public ASTNode
{
  public:
	ArgumentDeclNode(std::string id,
					 ReturnType type,
					 int index1,
					 int index2) : id(id),
								   type(type),
								   index1(index1),
								   index2(index2) {}

	std::string id;
	ReturnType type;
	int index1;
	int index2;

	std::string to_string()
	{
		return "argumentdecl";
	}
};

class SubprogramDeclNode : public ASTNode
{
  public:
	SubprogramDeclNode(std::string id,
					   ArgumentDeclList argument_decl_list,
					   ReturnType return_type,
					   VariableDeclList variable_section,
					   StatementList statement_list) : id(id),
													   argument_decl_list(std::move(argument_decl_list)),
													   return_type(return_type),
													   variable_section(std::move(variable_section)),
													   statement_list(std::move(statement_list)) {}

	std::string id;
	ArgumentDeclList argument_decl_list;
	ReturnType return_type; // Missing array indexes

	VariableDeclList variable_section;

	StatementList statement_list;

	std::string to_string()
	{
		return "subprogram";
	}
};

class StatementNode : public ASTNode
{
  public:
	~StatementNode() {}
};

class AssignNode : public StatementNode
{
  public:
	AssignNode(std::string id,
			   UP_ExprNode index,
			   UP_ExprNode expr) : id(id),
								   index(std::move(index)),
								   expr(std::move(expr)) {}
	std::string id;
	UP_ExprNode index;

	UP_ExprNode expr;

	std::string to_string()
	{
		return "assign";
	}
};

class SubprogramCallNode : public StatementNode
{
  public:
	SubprogramCallNode(std::string id,
					   ExprList expr_list,
					   ArgumentList argument_list) : id(id),
													 expr_list(std::move(expr_list)),
													 argument_list(std::move(argument_list)) {}

	std::string id;

	ExprList expr_list;
	ArgumentList argument_list;

	std::string to_string()
	{
		return "subprogram call";
	}
};

class ArgumentNode : public ASTNode
{
  public:
	ArgumentNode(std::string string_constant,
				 UP_ExprNode expr) : string_constant(string_constant),
									 expr(std::move(expr)) {}

	std::string string_constant;
	UP_ExprNode expr;
};

class IfNode : public StatementNode
{
  public:
	IfNode(UP_ExprNode expr,
		   StatementList block_true,
		   StatementList block_false) : expr(std::move(expr)),
										block_true(std::move(block_true)),
										block_false(std::move(block_false)) {}

	UP_ExprNode expr;
	StatementList block_true;
	StatementList block_false;

	std::string to_string()
	{
		return "if";
	}
};

class WhileNode : public StatementNode
{
  public:
	WhileNode(UP_ExprNode expr,
			  StatementList block) : expr(std::move(expr)),
									 block(std::move(block)) {}

	UP_ExprNode expr;
	StatementList block;

	std::string to_string()
	{
		return "while";
	}
};

class RepeatNode : public StatementNode
{
  public:
	RepeatNode(StatementList block,
			   UP_ExprNode expr) : block(std::move(block)),
								   expr(std::move(expr)) {}

	StatementList block;
	UP_ExprNode expr;

	std::string to_string()
	{
		return "repeat";
	}
};

class ForNode : public StatementNode
{
  public:
	ForNode(UP_AssignNode assign,
			UP_ExprNode expr,
			StatementList block) : assign(std::move(assign)),
								   expr(std::move(expr)),
								   block(std::move(block)) {}

	UP_AssignNode assign;
	UP_ExprNode expr;
	StatementList block;

	std::string to_string()
	{
		return "for";
	}
};

class BranchingStatementNode : public StatementNode
{
  public:
	BranchingStatementNode(BranchingStatement branching_statement) : branching_statement(branching_statement) {}

	BranchingStatement branching_statement;

	std::string to_string()
	{
		return "branching";
	}
};

class ExprNode : public ASTNode
{
  public:
	~ExprNode() {}

	virtual int get_precedence() = 0;
	virtual std::string get_oper() = 0;
};

class NotExprNode : public ExprNode
{
  public:
	NotExprNode(UP_ExprNode expr1) : expr1(std::move(expr1)) {}
	UP_ExprNode expr1;

	int get_precedence()
	{
		return 3;
	}

	std::string get_oper()
	{
		return "not";
	}

	std::string to_string()
	{
		return "not";
	}
};

class UnaryExprNode : public ExprNode
{
  public:
	UnaryExprNode(UP_ExprNode expr1) : expr1(std::move(expr1)) {}
	UP_ExprNode expr1;

	int get_precedence()
	{
		return 3;
	}

	std::string get_oper()
	{
		return "-";
	}

	std::string to_string()
	{
		return "-";
	}
};

class BinaryExprNode : public ExprNode
{
  public:
	BinaryExprNode(UP_ExprNode expr1,
				   UP_ExprNode expr2) : expr1(std::move(expr1)),
										expr2(std::move(expr2)) {}
	UP_ExprNode expr1;
	UP_ExprNode expr2;

	std::string to_string()
	{
		return "binary";
	}
};

DEFINE_BINARY_EXPR(Mul, 2, "*");
DEFINE_BINARY_EXPR(Div, 2, "div");
DEFINE_BINARY_EXPR(Mod, 2, "mod");
DEFINE_BINARY_EXPR(And, 2, "and");
DEFINE_BINARY_EXPR(Shl, 2, "shl");
DEFINE_BINARY_EXPR(Shr, 2, "shr");
DEFINE_BINARY_EXPR(LS, 2, "<<");
DEFINE_BINARY_EXPR(RS, 2, ">>");

DEFINE_BINARY_EXPR(Add, 1, "+");
DEFINE_BINARY_EXPR(Sub, 1, "-");
DEFINE_BINARY_EXPR(Or, 1, "or");
DEFINE_BINARY_EXPR(Xor, 1, "xor");

DEFINE_BINARY_EXPR(Eq, 0, "=");
DEFINE_BINARY_EXPR(NEq, 0, "<>");
DEFINE_BINARY_EXPR(LT, 0, "<");
DEFINE_BINARY_EXPR(GT, 0, ">");
DEFINE_BINARY_EXPR(LToE, 0, "<=");
DEFINE_BINARY_EXPR(GToE, 0, ">=");

class ParExprNode : public ExprNode
{
  public:
	ParExprNode(UP_ExprNode expr1) : expr1(std::move(expr1)) {}
	UP_ExprNode expr1;

	std::string to_string()
	{
		return "parexpr";
	}

	int get_precedence()
	{
		return 4;
	}

	std::string get_oper()
	{
		return "(";
	}
};

class NumberNode : public ASTNode
{
	NumberNode(int val) : val(val) {}

	int val;

	std::string to_string()
	{
		return "number";
	}
};

class StringNode : public ASTNode
{
	StringNode(std::string val) : val(val) {}

	std::string val;

	std::string to_string()
	{
		return "string";
	}
};

#endif
