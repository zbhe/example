#include <utility>
#include <type_traits>
#include <iostream>
using namespace std;
class B
{};
class D:public B
{};
class C:private D
{};

template<typename First, typename... Last>void DoPrint(First V, Last... Args)
{
	cout << V <<", ";
	DoPrint(Args...);
}
template<typename Type>void DoPrint(Type V)
{
	cout << V << endl;
}
template<bool>struct Print
{
	template<typename... Args> void operator () (Args... Values)const{
		DoPrint(Values...);
	}
};
template<>struct Print<false>
{
	template<typename... Args> void operator() (Args... Values)const{
		//pass
	}
};
int main()
{
	Print<std::is_base_of<B, D&>::value> ()("B is base of D&");
	Print<std::is_base_of<B, const D>::value> ()("B is base of const D");
	Print<std::is_base_of<B, C>::value> ()("B is base of C");
	Print<std::is_base_of<const D, C>::value> ()("const D is base of C");
	Print<std::is_base_of<D, const C>::value> ()("D is base of const C");
	Print<std::is_base_of<D, std::remove_reference<const C&>::type>::value> ()("D is base of const C&");
	return 0;
}
