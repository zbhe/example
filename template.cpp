#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <functional>
#include <type_traits>

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
	void OK(){}
};

int Sum(int x, int y)
{
	return x + y;
}
void print( int v )
{
	std::cout << "print:" << v << std::endl;
}

template<typename... > using void_t = void;
template<typename... T> using is_same_t = typename std::is_same<T...>::type;
template<typename T, typename=void> struct has_c:std::false_type{};
template<typename T> struct has_c<T, is_same_t<int, decltype(T::c)>>:std::true_type{};

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
	std::cout << "has_copy:" << has_c<decltype(List)>::value << std::endl;
	return 0;
}
