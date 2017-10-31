#include<iostream>
using namespace std;
constexpr int fib(unsigned int n)
{
	if( n == 0 || n == 1){
		return 1;
	}
	return fib(n-1) + fib(n-2);
}

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
