#include<iostream>
#include<string>
#include<initializer_list>
using namespace std;
template<typename T> class Type;
class B
{
	public:
		/*
		B(initializer_list<int> L)
		{
			cout << "call with initializer_list" << endl;
		}
		*/
		B(int i, bool b, string s)
		{
			cout << "call with i b s " << i << " " << b << " " << s << endl;
		}
		B(int i, int n)
		{
			cout << "call with i n " << i << " " << n << endl;
		}
		B(int i)
		{
			cout << "call with i " << i << endl;
		}
		virtual void F(int x){
			cout << "call B's virtaul F with " << x << endl;
		}
		virtual void F(string s){
			cout << "call B's virtaul F with " << s << endl;
		}
};
class D:public B
{
	public:
		template<typename... TArgs> D(TArgs... Args):B(Args...){}
		void F(int x)override{
			cout << "call D's virtaul F with " << x << endl;
		}
		void F(string s)override{
			cout << "call D's virtaul F with " << s << endl;
		}
};
B F()
{
	return {3};
}

int main()
{
	D{1, true, "abc"};
	D{2, 3};
	D c{2};
	B& R = c;
	R.F(3);
	R.F("ABCSTR");
	B* pb = new B{3};
	void* pv = new int(3);
	if( dynamic_cast<B*>(static_cast<B*>(pv)) ){
		cout << "dynamic_cast ok" << endl;
	}
	//decltype(F()) v = F();
	return 0;
}
