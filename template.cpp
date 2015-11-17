#include <iostream>
#include <functional>
template< typename R, typename ... Args > using TFunc = std::function<R(Args...)>;
template< typename R > using TCB = std::function<void(R)>;

template <class R, class... Arg> void Invok( std::function<auto(Arg...)> Func, Arg... Paras, TCB<auto> CB)
{
	CB( Func(Paras...) );
}
/*
template <class R> void Invok( TFunc<R> Func, TCB<R> CB)
{
	CB( Func() );
}
*/

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
	Invok<int, int, int>(Sum, 3, 5, print);
	Invok<int, int>([](int x)->int{return x+99;}, 1, print);
	Invok<std::string>([]()->std::string{
					return "abc";
				},
			[](const std::string& v){
				std::cout << "string:" << v << std::endl;
			}
		);
	return 0;
}
