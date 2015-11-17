#include <iostream>
#include <memory>
#include <string.h>
#include <string>
#include <time.h>
using namespace std;

class B1
{
	public:
		B1(){ cout << "construct B1\n";}
		virtual ~B1(){ cout << "destruct B1\n";}
};

class B2:public B1
{
	public:
		//B2(){ cout << "construct B2\n";}
		//virtual ~B2(){ cout << "destruct B2\n";}
};

class D:public B2
{
	public:
		D(){ cout << "construct D\n";}
		virtual ~D(){ cout << "destruct D\n";}
};

int main()
{
	//D* pD = new D();
	//delete pD;
	std::unique_ptr<D> up(nullptr);
	up = std::make_unique<D>();
	return 0;
	clock_t s = clock();
	for( int i = 1; i < 1000000; i++){
		std::string* p = new std::string("abc");
		delete p;
	}
	std::cout << "string cost:" << clock() - s << std::endl;
	s = clock();
	for( int i = 1; i < 1000000; i++){
		char* p = new char[10];
		strncpy(p, "abc", 10);
		delete [] p;
	}
	std::cout << "char* cost:" << clock() - s << std::endl;
	return 0;
}
