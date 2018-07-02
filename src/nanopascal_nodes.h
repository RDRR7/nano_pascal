#ifndef _NANOPASCAL_NODES_H
#define _NANOPASCAL_NODES_H

#include <memory>
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <iomanip>

#define DEFINE_BINARY_EXPR(name, prec, oper)                                        \
	class name##ExprNode : public BinaryExprNode                                    \
	{                                                                               \
	  public:                                                                       \
		name##ExprNode(UP_ASTNode expr1, UP_ASTNode expr2)                          \
			: BinaryExprNode(std::move(expr1), std::move(expr2)) {}                 \
		int get_precedence()                                                        \
		{                                                                           \
			return prec;                                                            \
		}                                                                           \
		std::string get_oper()                                                      \
		{                                                                           \
			return oper;                                                            \
		}                                                                           \
		int eval(std::map<std::string, ReturnType> &variables_type,                 \
				 std::map<std::string, std::map<int, int>> &variables_value,        \
				 std::map<std::string, UP_SubprogramDeclNode> &functions) override; \
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
class NotExprNode;
class UnaryExprNode;
class BinaryExprNode;
class NumberNode;
class StringNode;
class BooleanNode;
class IdNode;

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
using UP_NotExprNode = std::unique_ptr<NotExprNode>;
using UP_UnaryExprNode = std::unique_ptr<UnaryExprNode>;
using UP_BinaryExprNode = std::unique_ptr<BinaryExprNode>;
using UP_NumberNode = std::unique_ptr<NumberNode>;
using UP_StringNode = std::unique_ptr<StringNode>;
using UP_BooleanNode = std::unique_ptr<BooleanNode>;
using UP_IdNode = std::unique_ptr<IdNode>;

using ASTNodelList = std::list<UP_ASTNode>;
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

enum class NodeKind : unsigned int
{
	AST = 0,
	StringNode = 1,
	IdNode = 2,
	ExprNode = 3,
	SubprogramCallNode = 4,
	BranchingStatementNode = 5,
};

class ASTNode
{
  public:
	ASTNode() {}
	virtual ~ASTNode() {}

	int get_precedence() { return 10; };
	virtual NodeKind get_kind() { return NodeKind::AST; };
	virtual std::string to_string() = 0;
	virtual void exec(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
	{
		std::cout << "Not implemented" << std::endl;
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

	std::string to_string() override;
	void exec(std::map<std::string, ReturnType> &variables_type,
			  std::map<std::string, std::map<int, int>> &variables_value,
			  std::map<std::string, UP_SubprogramDeclNode> &functions) override;
};

class SubprogramDeclNode : public ASTNode
{
  public:
	SubprogramDeclNode(std::string id,
					   ArgumentDeclList argument_decl_list,
					   ReturnType return_type,
					   int index1,
					   int index2,
					   VariableDeclList variable_section,
					   StatementList statement_list) : id(id),
													   argument_decl_list(std::move(argument_decl_list)),
													   return_type(return_type),
													   index1(index1),
													   index2(index2),
													   variable_section(std::move(variable_section)),
													   statement_list(std::move(statement_list)) {}

	std::string id;
	ArgumentDeclList argument_decl_list;
	ReturnType return_type;
	int index1;
	int index2;

	VariableDeclList variable_section;

	StatementList statement_list;

	std::string to_string() override;
	void exec(std::map<std::string, ReturnType> &variables_type,
			  std::map<std::string, std::map<int, int>> &variables_value,
			  std::map<std::string, UP_SubprogramDeclNode> &functions) override;
};

class StatementNode : public ASTNode
{
  public:
	~StatementNode() {}
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

	std::string to_string() override;
	void exec(std::map<std::string, ReturnType> &variables_type,
			  std::map<std::string, std::map<int, int>> &variables_value,
			  std::map<std::string, UP_SubprogramDeclNode> &functions) override;
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

	std::string to_string() override;
};

class AssignNode : public StatementNode
{
  public:
	AssignNode(std::string id,
			   UP_ASTNode index,
			   UP_ASTNode expr) : id(id),
								  index(std::move(index)),
								  expr(std::move(expr)) {}
	std::string id;
	UP_ASTNode index;

	UP_ASTNode expr;

	std::string to_string() override;
	void exec(std::map<std::string, ReturnType> &variables_type,
			  std::map<std::string, std::map<int, int>> &variables_value,
			  std::map<std::string, UP_SubprogramDeclNode> &functions) override;
};

class ExprNode : public ASTNode
{
  public:
	~ExprNode() {}

	virtual std::string get_oper() = 0;
	virtual int eval(std::map<std::string, ReturnType> &variables_type,
					 std::map<std::string, std::map<int, int>> &variables_value,
					 std::map<std::string, UP_SubprogramDeclNode> &functions) = 0;
	virtual NodeKind get_kind()
	{
		return NodeKind::ExprNode;
	}
};

class SubprogramCallNode : public StatementNode
{
  public:
	SubprogramCallNode(std::string id,
					   ASTNodelList ast_node_list) : id(id),
													 ast_node_list(std::move(ast_node_list)) {}

	std::string id;

	ASTNodelList ast_node_list;

	std::string to_string() override;
	void exec(std::map<std::string, ReturnType> &variables_type,
			  std::map<std::string, std::map<int, int>> &variables_value,
			  std::map<std::string, UP_SubprogramDeclNode> &functions) override;
	NodeKind get_kind() override
	{
		return NodeKind::SubprogramCallNode;
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
	IfNode(UP_ASTNode expr,
		   StatementList block_true,
		   StatementList block_false) : expr(std::move(expr)),
										block_true(std::move(block_true)),
										block_false(std::move(block_false)) {}

	UP_ASTNode expr;
	StatementList block_true;
	StatementList block_false;

	std::string to_string() override;
	void exec(std::map<std::string, ReturnType> &variables_type,
			  std::map<std::string, std::map<int, int>> &variables_value,
			  std::map<std::string, UP_SubprogramDeclNode> &functions) override;
};

class WhileNode : public StatementNode
{
  public:
	WhileNode(UP_ASTNode expr,
			  StatementList block) : expr(std::move(expr)),
									 block(std::move(block)) {}

	UP_ASTNode expr;
	StatementList block;

	std::string to_string() override;
	void exec(std::map<std::string, ReturnType> &variables_type,
			  std::map<std::string, std::map<int, int>> &variables_value,
			  std::map<std::string, UP_SubprogramDeclNode> &functions) override;
};

class RepeatNode : public StatementNode
{
  public:
	RepeatNode(StatementList block,
			   UP_ASTNode expr) : block(std::move(block)),
								  expr(std::move(expr)) {}

	StatementList block;
	UP_ASTNode expr;

	std::string to_string() override;
	void exec(std::map<std::string, ReturnType> &variables_type,
			  std::map<std::string, std::map<int, int>> &variables_value,
			  std::map<std::string, UP_SubprogramDeclNode> &functions) override;
};

class ForNode : public StatementNode
{
  public:
	ForNode(UP_AssignNode assign,
			UP_ASTNode expr,
			StatementList block) : assign(std::move(assign)),
								   expr(std::move(expr)),
								   block(std::move(block)) {}

	UP_AssignNode assign;
	UP_ASTNode expr;
	StatementList block;

	std::string to_string() override;
	void exec(std::map<std::string, ReturnType> &variables_type,
			  std::map<std::string, std::map<int, int>> &variables_value,
			  std::map<std::string, UP_SubprogramDeclNode> &functions) override;
};

class BranchingStatementNode : public StatementNode
{
  public:
	BranchingStatementNode(BranchingStatement branching_statement)
		: branching_statement(branching_statement) {}

	BranchingStatement branching_statement;

	std::string to_string() override;
	NodeKind get_kind() override
	{
		return NodeKind::BranchingStatementNode;
	}
};

class NotExprNode : public ExprNode
{
  public:
	NotExprNode(UP_ASTNode expr1) : expr1(std::move(expr1)) {}
	UP_ASTNode expr1;

	int get_precedence()
	{
		return 3;
	}

	std::string get_oper()
	{
		return "not";
	}

	std::string to_string() override;

	int eval(std::map<std::string, ReturnType> &variables_type,
			 std::map<std::string, std::map<int, int>> &variables_value,
			 std::map<std::string, UP_SubprogramDeclNode> &functions) override
	{
		return ~((ExprNode *)expr1.get())->eval(variables_type, variables_value, functions);
	}
};

class UnaryExprNode : public ExprNode
{
  public:
	UnaryExprNode(UP_ASTNode expr1) : expr1(std::move(expr1)) {}
	UP_ASTNode expr1;

	int get_precedence()
	{
		return 3;
	}

	std::string get_oper()
	{
		return "-";
	}

	std::string to_string() override;
	int eval(std::map<std::string, ReturnType> &variables_type,
			 std::map<std::string, std::map<int, int>> &variables_value,
			 std::map<std::string, UP_SubprogramDeclNode> &functions) override
	{
		return -1 * ((ExprNode *)expr1.get())->eval(variables_type, variables_value, functions);
	}
};

class BinaryExprNode : public ExprNode
{
  public:
	BinaryExprNode(UP_ASTNode expr1,
				   UP_ASTNode expr2) : expr1(std::move(expr1)),
									   expr2(std::move(expr2)) {}
	UP_ASTNode expr1; // Should have index // in Expr?
	UP_ASTNode expr2; // Should have index

	std::string to_string() override;
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

class NumberNode : public ExprNode
{
  public:
	NumberNode(int val) : val(val) {}

	int val;

	std::string get_oper()
	{
		return "";
	}

	std::string to_string() override;

	int eval(std::map<std::string, ReturnType> &variables_type,
			 std::map<std::string, std::map<int, int>> &variables_value,
			 std::map<std::string, UP_SubprogramDeclNode> &functions) override
	{
		return val;
	}
};

class StringNode : public ExprNode
{
  public:
	StringNode(std::string val) : val(val) {}

	std::string val;

	std::string get_oper()
	{
		return "";
	}
	NodeKind get_kind()
	{
		return NodeKind::StringNode;
	}

	std::string to_string() override;
	int eval(std::map<std::string, ReturnType> &variables_type,
			 std::map<std::string, std::map<int, int>> &variables_value,
			 std::map<std::string, UP_SubprogramDeclNode> &functions) override
	{
		return 0;
	}
};

class BooleanNode : public ExprNode
{
  public:
	BooleanNode(std::string val) : val(val) {}

	std::string val;

	std::string get_oper()
	{
		return "";
	}

	std::string to_string() override;
	int eval(std::map<std::string, ReturnType> &variables_type,
			 std::map<std::string, std::map<int, int>> &variables_value,
			 std::map<std::string, UP_SubprogramDeclNode> &functions) override
	{
		if (val.compare("true") == 0)
		{
			return 1;
		}
		return 0;
	}
};

class IdNode : public StringNode
{
  public:
	IdNode(std::string id,
		   UP_ASTNode index) : StringNode(id),
							   index(std::move(index)) {}

	UP_ASTNode index;

	NodeKind get_kind()
	{
		return NodeKind::IdNode;
	}

	std::string to_string() override;
	int eval(std::map<std::string, ReturnType> &variables_type,
			 std::map<std::string, std::map<int, int>> &variables_value,
			 std::map<std::string, UP_SubprogramDeclNode> &functions) override
	{
		return variables_value[val][0];
	}
};

#endif
