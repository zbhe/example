//g++ -std=c++14 -I /opt/local/include/ -o hana hana.cpp

#include <boost/hana.hpp>
#include <string>
#include <iostream>
namespace hana = boost::hana;
using namespace std;
struct Fish{
	string name;
};
struct Mac{
	int size;
};

int main()
{
	auto Tup = hana::make_tuple(Fish{"Fish"}, Mac{13}, Fish{"Gold"}, Mac{15});
	auto HasName = hana::is_valid([](auto&& Obj)->decltype(Obj.name){});
	auto NamedList = hana::remove_if(Tup, [HasName](auto&& Obj){return !HasName(Obj);});
	hana::for_each(NamedList, [](auto&& Obj){
		cout << Obj.name << endl;
	});
	return 0;
}
