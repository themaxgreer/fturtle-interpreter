#ifndef _EXPR_HPP
#define _EXPR_HPP

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include "SymbolTable.hpp"
#include <iterator>

class Expr;
class BinaryExpr;
class UnaryExpr;
class IfExpr;
class LetExpr;
class CallExpr;
class VarExpr;
class NumExpr;
class BlockExpr;
class UserFuncExpr;
class BIFuncExpr;
class SymbolTable;
struct Symbol;

class Visitor {
	public:
		virtual double visit(Expr *e) = 0;
		virtual double visit(BinaryExpr *e) = 0;
		virtual double visit(UnaryExpr *e) = 0;
		virtual double visit(IfExpr *e) = 0;
		virtual double visit(LetExpr *e) = 0;
		virtual double visit(CallExpr *e) = 0;
		virtual double visit(VarExpr *e) = 0;
		virtual double visit(NumExpr *e) = 0;
		virtual double visit(BlockExpr *e) = 0;
		virtual double visit(UserFuncExpr *e) = 0;
		virtual double visit(BIFuncExpr *e) = 0;
};

class Expr {
	public:
		virtual double accept(Visitor *v) = 0;
};
class BinaryExpr : public Expr {
	public:
		Expr *left;
		Expr *right;
		enum BinaryOperation {
			OrOp, AndOp, EqOp, NeOp,LtOp, 
			LeOp, GtOp, GeOp, PlusOp,MinusOp, 
			MultOp, DivOp
		} op;
		BinaryExpr(BinaryOperation op, Expr *left, Expr *right): op(op), left(left), right(right) {};
		virtual double accept(Visitor *v) {
				return v->visit(this);
		}
};
class UnaryExpr : public Expr {
	public: 
		Expr *subExpr;
		enum UnaryOp {
		 NotOp, NegOp
		} op;
		UnaryExpr(UnaryOp op, Expr *subExpr): op(op), subExpr(subExpr) {};
		virtual double accept(Visitor *v) {
			return v->visit(this);
		}
};
class IfExpr : public Expr {
	public:
		Expr *condExpr;
		Expr *thenExpr;
		Expr *elseExpr;
		IfExpr(Expr *condExpr, Expr *thenExpr, Expr *elseExpr): condExpr(condExpr), thenExpr(thenExpr), elseExpr(elseExpr) {};
		virtual double accept(Visitor *v) {
			return v->visit(this);
		}
};
class LetExpr : public Expr {
	public:
		std::vector<Symbol *> lexicals;
		Expr *body;
		LetExpr(const std::list<Symbol *>&lexicals, Expr *body): lexicals(lexicals.begin(), lexicals.end()), body(body) {};
		virtual double accept(Visitor *v) {
				return v->visit(this);
		}
};
class CallExpr : public Expr {
	public:
		const std::string name;
		std::vector<Expr *> args;
		CallExpr(const std::string &name, const std::list<Expr *>&args): name(name), args(args.begin(), args.end()) {};
		virtual double accept(Visitor *v) {
			return v->visit(this);
		}
};
class VarExpr : public Expr {
	public:
		std::string name;
		VarExpr(const std::string &name) : name(name) {};
		virtual double accept(Visitor *v) {
			return v->visit(this);
		}
};
class NumExpr : public Expr {
	public:
		double num;
		NumExpr(double num) : num(num) {};
		virtual double accept(Visitor *v) {
			return v->visit(this);
		} 
};
class BlockExpr : public Expr {
	public:
		std::list<Expr*> exprs;
		BlockExpr(const std::list<Expr*>&exprs): exprs(exprs) {};
		virtual double accept(Visitor *v) {
			return v->visit(this);
		}
};
class UserFuncExpr : public Expr {
	public:
		std::vector<std::string> params;
		Expr *body;
		UserFuncExpr(const std::list<std::string>&params, Expr *body): params(params.begin(), params.end()), body(body) {};
		virtual double accept(Visitor *v) {
			return v->visit(this);
		}
};
class BIFuncExpr : public Expr {
	public:
		std::string name;
		BIFuncExpr(const std::string &name): name(name) {};
		virtual double accept(Visitor *v) {
			return v->visit(this);
		}
};
class EvalVisitor : public Visitor {
	public:
		SymbolTable *symtab;
		EvalVisitor(SymbolTable *symtab) : symtab(symtab) {};
		virtual double visit(Expr *e) {
			return 0;
		}
		virtual double visit(BinaryExpr *e){
			switch(e->op) {
			case BinaryExpr::AndOp:
				if(e->left->accept(this) == 0.0){
					return 0.0;
				} else {
					return e->right->accept(this);
				}
				break;
			case BinaryExpr::OrOp:
				if(e->left->accept(this) == 0.0){
					return e->right->accept(this);
				} else {
					return 1.0;
				}
				break;
			case BinaryExpr::LeOp:
				return (e->left->accept(this) <= e->right->accept(this));
				break;
			case BinaryExpr::LtOp:
				return (e->left->accept(this) < e->right->accept(this));
				break;
			case BinaryExpr::EqOp:
				return (e->left->accept(this) == e->right->accept(this));
				break;
			case BinaryExpr::GeOp:
				return (e->left->accept(this) >= e->right->accept(this));
				break;
			case BinaryExpr::GtOp:
				return (e->left->accept(this) > e->right->accept(this));
				break;
			case BinaryExpr::PlusOp:
				return (e->left->accept(this) + e->right->accept(this));
				break;
			case BinaryExpr::MinusOp:
				return (e->left->accept(this) - e->right->accept(this));
				break;
			case BinaryExpr::MultOp:
				return (e->left->accept(this) * e->right->accept(this));
				break;
			case BinaryExpr::DivOp:
				return (e->left->accept(this) / e->right->accept(this));
				break;
			case BinaryExpr::NeOp:
				return (e->left->accept(this) != e->right->accept(this));
				break;
			}
		}
		virtual double visit(UnaryExpr *e) {
			switch(e->op) {
			case UnaryExpr::NegOp:
				return ( -1.0 * e->subExpr->accept(this));
				break;
			case UnaryExpr::NotOp:
				return (e->subExpr->accept(this) == 0);
				break;
			}
		}
		virtual double visit(IfExpr *e) {
			if(e->condExpr->accept(this) != 0.0){
				return e->thenExpr->accept(this);
			} else {
				return e->elseExpr->accept(this);
			}
		}
		virtual double visit(LetExpr *e) {
			SymbolTable *parent = symtab; // (1) save parent
			symtab = new SymbolTable(symtab); // (2) new scope
			for (auto  it = e->lexicals.begin(); it != e->lexicals.end(); ++it) { 
				Symbol *s = *it;
			// (3) eval/store lex-var expr's
				NumExpr *n = new NumExpr(s->value->accept(this));
				symtab->put(s->name, n);
			}
			const double result = e->body->accept(this); // (4) eval body
			delete symtab; // (4) done with symtab
			symtab = parent; // (5) restore parent
			return result; // (6) result
		}
		virtual double visit(CallExpr *e) {
			// (1) Lookup function in global function table
			//std::cout << "New Call with: " << e->name << std::endl;
			int count = 0;
			UserFuncExpr *func = dynamic_cast<UserFuncExpr *>(symtab->get(e->name));
			if (func){ //is there a function?
				//save parent
				SymbolTable *current = symtab;
				symtab = new SymbolTable(symtab);
				auto iter_params = func->params.begin(); 
				auto iter_call = e->args.begin();
				for(; iter_params != func->params.end() && iter_call != e->args.end();++iter_params, ++iter_call){
					std::string param = *iter_params;
					//std::cout << count << std::endl;
					//std::cout << "Param: " << param << std::endl;
					Expr *currentExpr = *iter_call;
					NumExpr *n = new NumExpr(currentExpr->accept(this));
					symtab->put(*iter_params,n);
				}
				//Evaluate the body of our new expression with it local symtable
				const double result = func->body->accept(this);
				
				//restore order and clean up after ourselves
				delete symtab;
				symtab = current;
				return result;
			}
			//add builtin
			BIFuncExpr *bi = dynamic_cast<BIFuncExpr *>(symtab->get(e->name));
			if(bi){
				std::cout << bi->name;
				auto iter_args = e->args.begin();
				for(; iter_args != e->args.end(); ++iter_args){
					Expr *varE = *iter_args;
					std::cout << " " << varE->accept(this); 
				}
				std::cout << std::endl;
			}
			return 0;
		}
		virtual double visit(VarExpr *e) {
			return symtab->get(e->name)->accept(this);
		}
		virtual double visit(NumExpr *e) {
			return e->num;
		}
		virtual double visit(BlockExpr *e) {
			double result;
			//Evaluate all of the body expressions
			for(auto iter = e->exprs.begin(), iter2 = --e->exprs.end(); iter != iter2; ++iter){
				Expr *f = *iter; 
				if(f != e->exprs.back()) {
					result = f->accept(this);
				}
			}
			//return the last expression's result
			result = e->exprs.back()->accept(this);
			return result;
		}
		virtual double visit(UserFuncExpr *e) {
		//	std::cout << "Got Here Unexpectedly" << std::endl;
			return 0;
		}
		virtual double visit(BIFuncExpr *e) {
			return 0;
		}
};
#endif // !EXPR_HPP
