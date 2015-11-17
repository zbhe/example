#include <vector>
#include <iostream>
using namespace std;
class B{
	public:
		virtual void print(){
			cout << "this is B" << endl;
		}
};
class T:public B{
	private:
		int v;
	public:
		T* p;
		/*
		void print(){
			cout << "this is T" << endl;
		}
		*/
};
class D:public T{
	public:
	D(){}
	/*
	void print(){
		cout << "this is D"<< endl;
	}
	*/
};
int main()
{
	B* pd = new D();
	pd->print();
	return 0;
	vector<int> v;
	for( int i = 1; i < 100; i++){
		v.push_back(i*i);
	}
	int* p = &v[10];
	cout << "v[10] = " << *p << endl;
	cout << "sizeof(v[0]) = " << sizeof(v[0]) <<endl;
	cout << " p - begin = " << p - &v[0] << endl;
	return 0;
}
