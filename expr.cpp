#include <iostream>
#include <sstream>

class Expr;

class Node{
	public:
		virtual std::string print() = 0;
};
class NodeInt:public Node{
	private:
		int num;
	public:
		NodeInt(int _num):num(_num){}
		virtual std::string print(){
			std::stringstream s;
			s << num ;
			return s.str();
		}
};

class Node1:public Node{
	private:
		std::string op;
		Node* operand;
	public:
		Node1(std::string _op, Expr expr);
		virtual std::string print(){
			std::stringstream s;
			s << "(" << op << operand->print() << ")";
			return s.str();
		}
};

class Node2:public Node{
	private:
		std::string op;
		Node* operand1;
		Node* operand2;
	public:
		Node2(std::string _op, Expr Expr1, Expr Expr2);
		virtual std::string print(){
			std::stringstream s;
			s << "(" << operand1->print()<< op << operand2->print()<< ")";
			return s.str();
		}
};


class Expr{
	public:
		Node* pNode;
		Expr(int num){
			pNode = new NodeInt(num);
		}
		Expr(std::string op, Expr expr){
			pNode = new Node1(op, expr);
		}
		Expr(std::string op, Expr Expr1, Expr Expr2){
			pNode = new Node2(op, Expr1, Expr2);
		}
		std::string print()const{
			return pNode->print();
		}
		Expr(Expr&& e){
			pNode = e.pNode;
			e.pNode = NULL;
			std::cout << "call move constructor:"<< __LINE__<<std::endl;
		}
		~Expr(){
			delete pNode;
			pNode = NULL;
		}
};

Node1::Node1(std::string _op, Expr expr):op(_op), operand(expr.pNode){}
Node2::Node2(std::string _op, Expr Expr1, Expr Expr2):op(_op), operand1(Expr1.pNode), operand2(Expr2.pNode){}

Expr GenExpr(std::string&& op, int lsh, int rsh){
	return Expr(op, lsh, rsh);
}

int main()
{
	std::cout << Expr("*", Expr("+", 3, 4), Expr("-", 2, 1)).print() << std::endl;
	std::cout << Expr("*", Expr("+", 2, 3), GenExpr("-", 5, 4)).print() << std::endl;
	return 0;
}
