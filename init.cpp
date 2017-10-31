#include<iostream>
#include<string>
#include<initializer_list>
using namespace std;
template<typename T> class Type;
class C
{
	public:
		/*
		C(initializer_list<int> L)
		{
			cout << "call with initializer_list" << endl;
		}
		*/
		C(int i, bool b, string s)
		{
			cout << "call with i b s " << i << " " << b << " " << s << endl;
		}
		C(int i, int n)
		{
			cout << "call with i n " << i << " " << n << endl;
		}
		C(int i)
		{
			cout << "call with i " << i << endl;
		}

		//C(const C&) == delete;
};
C F()
{
	return {3};
}

int main()
{
	C{1, true, "abc"};
	C{2, 3};
	C c{2};
	decltype(F()) v = F();
	//static_assert(is_same<decltype(v), C&>::value, "type==");
	Type<decltype(F())> t;
	return 0;
}
