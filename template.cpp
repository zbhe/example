#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <functional>

template <class CBT, class FUNCT, class... Arg> void Invok(CBT CB, FUNCT Func, Arg... Paras )
{
	std::cout << "ONE~\n";
	CB( Func(Paras...) );
}

template <class R, class... Arg> void Invok( void(CB)(R), R(Func)(Arg...), Arg... Paras )
{
	std::cout << "TWO~\n";
	CB( Func(Paras...) );
}

template <class FUNCT, class... Args> void Call(FUNCT Func, Args... Paras)
{
	(void)std::initializer_list<int>{(Func(Paras), 0)...};
}

template < template<typename T, typename A = std::allocator<T> > class C, typename T > struct Container {
	C<T> c;
};

int Sum(int x, int y)
{
	return x + y;
}
void print( int v )
{
	std::cout << "print:" << v << std::endl;
}

int main()
{
	Invok(print, Sum, 3, 5);
	Invok(print, [](int x)->int{return x+99;}, 1);
	Invok(
		[](const std::string& s){
			std::cout << s << std::endl;
		},
		[]()->std::string{
			return "abc";
		}
	);
	Call(print, 32,343,25,345,65,7,6887,98,5425,214,9889,54,42);
	auto Vector = Container<std::vector, int>();
	auto List = Container<std::list, std::string>();
	return 0;
}
