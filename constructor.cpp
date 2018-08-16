#include <iostream>
#include <type_traits>
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

template<typename T, typename...TArgs> struct can_construct
{
	template<typename...> static std::false_type check(...);
	template<typename... Params> static std::true_type check(decltype(T(std::declval<Params>()...)));
	using RetType = decltype(check(std::declval<TArgs>()...));
	using type = typename RetType::type;
	static constexpr bool value = RetType::value;
};

template<typename T, typename... TP>struct if_construct
{
	template<typename U> struct true_t : true_type{};
	static std::false_type check(...);
	template<typename... SP > static true_t<decltype(T(std::declval<SP>()...))> check(SP&&...);
	using RetType = decltype(check(std::declval<TP>()...));
	using type = typename RetType::type;
	constexpr static bool value = RetType::value;
};

template<typename T, typename... TP> struct is_construct
{
	template<typename, typename U, typename... UP> struct helper : false_type{};
	template<typename U, typename... UP> struct helper<decltype(U(std::declval<UP>()...)), U, UP...> : true_type{};
	using type = helper<T, T, TP...>;
	constexpr static bool value = type::value;
};

int main()
{
	std::cout << can_construct<std::string, char*>::value << std::endl;
	std::cout << can_construct<std::string, int>::value << std::endl;
	std::cout << if_construct<std::string, char*>::value << std::endl;
	std::cout << if_construct<std::string, int>::value << std::endl;
	std::cout << is_construct<std::string, char*>::value << std::endl;
	std::cout << is_construct<std::string, int>::value << std::endl;
	std::unique_ptr<D> up(nullptr);
	up = std::make_unique<D>();
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
