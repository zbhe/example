#include <iostream>

using namespace std;
template<typename T> void Print(T&& t)
{
	cout << t << endl;
}

template<typename H, typename... T> void Print(H&& h, T... t)
{
	cout << h << ", ";
	Print(t...);
}

#define print(...) Print("__VA_ARGS__ DEMO", ##__VA_ARGS__)
int main(){
	print("number", 1, 2);
	print();
	return 0;
}
