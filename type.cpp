#include <memory>
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

template<typename Type>void DoPrint(Type V)
{
	cout << V << endl;
}
template<typename First, typename... Last>void DoPrint(First V, Last... Args)
{
	cout << V <<", ";
	DoPrint(Args...);
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

template<typename T>class Contain
{
	public:
	T& Value;
	Contain(T& Obj):Value(Obj){}
	template<typename T1, typename=enable_if_t<is_base_of<T1, T>::value> > operator Contain<T1>*()
	{
		cout << "call operator Contain<T1>\n";
		return (Contain<T1>*)this;
	}
	template<typename T1, typename=enable_if_t<is_base_of<T1, T>::value> > operator Contain<T1>&()
	{
		if( is_base_of<T1, T>::value ){
			cout << "ref convertion true\n";
		}else{
			cout << "ref convertion false\n";
		}
		cout << "call operator Contain<T1>\n";
		return *(Contain<T1>*)this;
	}
};
template<typename...> using void_t = void;
template<typename F, typename = void> struct callable : false_type{};
template<typename F> struct callable<F, void_t<decltype(declval<F>()())>>:true_type{};
struct Function
{
	template<typename F, typename... TArgs> enable_if_t<callable<F>::value, void> Call(F&& Fn, TArgs&&... Args)
	{
		std::forward<F>(Fn)(std::forward<TArgs>(Args)...);
	}
	template<typename F, typename... TArgs> void Call(F&& Fn, TArgs&&... Args)
	{
		cout << "nothing to call " << Fn << endl;
	}
	int operator() (char*);
};

int main(int, char**)
{
	Print<std::is_base_of<B, D&>::value> ()("B is base of D&");
	Print<std::is_base_of<B, const D>::value> ()("B is base of const D");
	Print<std::is_base_of<B, C>::value> ()("B is base of C");
	Print<std::is_base_of<const D, C>::value> ()("const D is base of C");
	Print<std::is_base_of<D, const C>::value> ()("D is base of const C");
	Print<std::is_base_of<D, std::remove_reference<const C&>::type>::value> ()("D is base of const C&");
	DoPrint("callable main:", callable<decltype(main)>::value);
	DoPrint("callable Function:", callable<Function>::value);
	return 0;
}
