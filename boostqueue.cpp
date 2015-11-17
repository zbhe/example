#include <iostream>
#include <memory>
#include <functional>
#include <boost/lockfree/queue.hpp>
//g++ -std=c++14 -o boostqueue boostqueue.cpp
using F = std::function<void(int)>;

int main()
{
	boost::lockfree::queue<F*> MyQueue(8);
	for( int i = 0; i < 10; i++){
		F* pf = new F([](int x){std::cout << "fuck..." << x << std::endl;});
		if( ! MyQueue.bounded_push(pf) ){
			std::cout << "MyQueue is full???" << std::endl;
			MyQueue.push(pf);
		}
	}
	F* ret = nullptr;
	int i = 0;
	while(true){
		if( ! MyQueue.pop(ret) ){
			break;
		}
		assert(ret);
		ret->operator () ( i++ );
		delete ret;
	};
	return 0;
}
