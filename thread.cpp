#include<thread>
#include<vector>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
class T{
	int* p;
	public:
		T(){
			p = new int[100];
		}
		~T(){
			delete [] p;
			p = NULL;
			usleep(1000);
		}
		void Execute(){
			for(int i = 0; i < 100; i++ )
				p[i] = random();
		}
};

void func(T* pT){
	while(true){
		pT->Execute();
	}
}
T Obj5;
int main(){
	T Obj4;
	std::thread t1(func, &Obj4);
	usleep(10);
	exit(0);
	return 0;
}
