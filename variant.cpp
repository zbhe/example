#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <chrono>
#include "boost/variant.hpp"

struct Print : public boost::static_visitor<>
{
	void operator () (int& i) const {
		std::cout << "int:" << i << std::endl;
	}
	void operator () (std::string& s) const {
		std::cout << "string:" << s << std::endl;
	}
	void operator () (std::vector<int>& v) const{
		std::cout << "vector:[";
		for( auto x : v ){
			std::cout << x << ",";
		}
		std::cout << "]\n";
	}
};

struct SizeVisitor : public boost::static_visitor<size_t>
{
	size_t operator () (const int& i) const{
		return sizeof(i);
	}
	size_t operator () (const std::string& s) const{
		return s.size();
	}
	size_t operator () (const std::vector<int>& v) const{
		return v.size();
	}
};

using VTYPE = boost::variant<int, std::string, std::vector<int>>;
size_t SizeFunc(const VTYPE& V)
{
	switch( V.which() ){
		case 0:
			return sizeof(int);
		case 1:
			return boost::get<std::string>(V).size();
		case 2:
			return boost::get<std::vector<int>>(V).size();
	}
	return 0;
}

void Performance(const VTYPE& V, int x)
{
	auto Begin = std::chrono::high_resolution_clock::now();
	for( int i = 0; i < x; i++){
		boost::apply_visitor(SizeVisitor(), V);
	}
	auto End1 = std::chrono::high_resolution_clock::now();
	auto Cost1 = End1 - Begin;
	for( int i = 0; i < x; i++){
		SizeFunc(V);
	}
	auto End2 = std::chrono::high_resolution_clock::now();
	auto Cost2 = End2 - End1;
	std::cout << "visitor cost:" << std::chrono::duration_cast<std::chrono::nanoseconds>(Cost1).count() << std::endl;
	std::cout << "which cost:" << std::chrono::duration_cast<std::chrono::nanoseconds>(Cost2).count() << std::endl;
}

int main()
{
	Print Printer;
	//using VTYPE = boost::variant<int, boost::recursive_wrapper<std::string>, boost::recursive_wrapper<std::vector<int>>>;
	std::cout << "size:" << sizeof(VTYPE) << std::endl;
	std::vector<VTYPE> VList;
	VList.push_back(3);
	VList.push_back(std::string("abc"));
	VList.push_back(std::vector<int>(3, 1.0));
	std::for_each(VList.begin(), VList.end(), boost::apply_visitor(Printer));
	std::cout << "Performance:" << std::endl;
	Performance(VList[2], 10000000);
	return 0;
}
