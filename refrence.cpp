#include<vector>
#include <iostream>
using namespace std;
class T{
	int v;
	public:
		T(int x):v(x){
			cout << "construct T" << v << endl;
		}
		~T(){
			cout << "destruct T" << v << endl;
		}
		int GetValue()const{
			return v;
		}
};

const vector<T> GetVector(int n)
{
	vector<T> Ret;
	for( int i = 0; i < n; i++){
		Ret.push_back(T(i));
	}
	return Ret;
}

int main()
{
	const vector<T>& VT = GetVector(1);
	for( T obj : VT ){
		cout << "couting VT " << obj.GetValue() << endl;
	}
	return 0;
}
