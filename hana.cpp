//g++ -std=c++14 -I /opt/local/include/ -o hana hana.cpp

#include <boost/hana.hpp>
#include <string>
#include <iostream>
namespace hana = boost::hana;
//using namespace std;
struct Fish
{
	std::string name;
};
struct Mac
{
	int size;
};
#define REFLECTION
class Person
{
	public:
#ifdef REFLECTION
		BOOST_HANA_DEFINE_STRUCT(Person,
			(std::string, name),
			(int, age),
			(std::string, address)
		);
#else
		std::string name;
		int age;
		std::string address;
#endif
	public:
		Person(const std::string& _name, int _age, const std::string& _address):name(_name), age(_age), address(_address){}
};

int main()
{
	auto Tup = hana::make_tuple(Fish{"Fish"}, Mac{13}, Fish{"Gold"}, Mac{15});
	auto HasName = hana::is_valid([](auto&& Obj)->decltype(Obj.name){});
	auto NamedList = hana::remove_if(Tup, [HasName](auto&& Obj){return !HasName(Obj);});
	hana::for_each(NamedList, [](auto&& Obj){
			std::cout << Obj.name << std::endl;
	});
	Person Bobo{"bobo", 35, "广州..."};
	std::cout << "size Person:" << sizeof(Person) << std::endl;
#ifdef REFLECTION
	hana::for_each(Bobo, [](auto pair){
		std::cout << hana::to<const char*>(hana::first(pair)) << ":" << hana::second(pair) << std::endl;
	});
#endif
	return 0;
}
