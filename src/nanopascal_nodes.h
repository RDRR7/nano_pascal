#ifndef _NANOPASCAL_NODES_H
#define _NANOPASCAL_NODES_H

#include <memory>
#include <string>
#include <list>

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
	Array = 4,
};

enum class BranchingStatement : unsigned int
{
	Continue = 0,
	Break = 1,
};

class ASTNode
{
  public:
	virtual ~ASTNode() {}

	virtual std::string to_string() = 0;
};

class ProgramNode : public ASTNode
{
  public:
	ProgramNode(VariableDeclList variable_section,
				SubprogramDeclList subprogram_decl,
				StatementList statement_list) : variable_section(std::move(variable_section)),
												subprogram_decl(std::move(subprogram_decl)),
												statement_list(std::move(statement_list)) {}

	VariableDeclList variable_section;
	SubprogramDeclList subprogram_decl;
	StatementList statement_list;
};

class SubprogramDeclNode : public ASTNode
{
  public:
	SubprogramDeclNode(std::string id,
					   ArgumentDeclList argument_decl_list,
					   ReturnType return_type,
					   VariableDeclList variable_section,
					   StatementList statement_list) : id(id),
													   argument_decl_list(argument_decl_list),
													   return_type(return_type),
													   variable_section(variable_section),
													   statement_list(statement_list) {}

	std::string id;
	ArgumentDeclList argument_decl_list;
	ReturnType return_type;

	VariableDeclList variable_section;

	StatementList statement_list;
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
			   ExprNode index,
			   ExprNode expr) : id(id),
								index(index),
								expr(expr) {}
	std::string id;
	ExprNode index;

	ExprNode expr;
};

class SubprogramCallNode : public StatementNode
{
  public:
	SubprogramCallNode(std::string id,
					   ExprList expr_list,
					   ArgumentList argument_list) : id(id),
													 expr_list(expr_list),
													 argument_list(argument_list) {}

	std::string id;

	ExprList expr_list;
	ArgumentList argument_list;
};

class IfNode : public StatementNode
{
  public:
	IfNode(ExprNode expr,
		   StatementList block_true,
		   StatementList block_false) : expr(expr),
										block_true(block_true),
										block_false(block_false) {}

	ExprNode expr;
	StatementList block_true;
	StatementList block_false;
};

class WhileNode : public StatementNode
{
  public:
	WhileNode(ExprNode expr,
			  StatementList block) : expr(expr),
									 block(block) {}

	ExprNode expr;
	StatementList block;
};

class RepeatNode : public StatementNode
{
  public:
	RepeatNode(StatementList block,
			   ExprNode expr) : block(block),
								expr(expr) {}

	StatementList block;
	ExprNode expr;
};

class ForNode : public StatementNode
{
  public:
	ForNode(AssignNode assign,
			ExprNode expr,
			StatementList block) : assign(assign),
								   expr(expr),
								   block(block) {}

	AssignNode assign;
	ExprNode expr;
	StatementList block;
};

class BranchingStatementNode : public StatementNode
{
  public:
	BranchingStatementNode(BranchingStatement branching_statement) : branching_statement(branching_statement) {}

	BranchingStatement branching_statement;
};

class ExprNode : public ASTNode
{
  public:
	~ExprNode() {}
};

class BinaryExprNode : public ExprNode
{
  public:
};

// Change to UP

#endif
