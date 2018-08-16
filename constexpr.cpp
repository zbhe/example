#include<iostream>
#include<vector>
#include<boost/variant.hpp>
#include<array>

using namespace std;
constexpr int fib(unsigned int n)
{
	if( n == 0 || n == 1){
		return 1;
	}
	return fib(n-1) + fib(n-2);
}

template<typename T, int size> struct Container
{
	boost::variant<shared_ptr<array<T, size>>, shared_ptr<vector<T>> > Ptr;
	Container()
	{
		if(size < 100){
			Ptr = make_shared<array<T, size>>();
		}else{
			Ptr = make_shared<vector<T>>();
		}
	}
};

int main()
{
	for(unsigned int i = 0; i < 10; i++){
		cout << fib(i) << " ";
	}
	cout << endl;
	constexpr unsigned int v = fib(20);
	cout << v << endl;
	return 0;
}
