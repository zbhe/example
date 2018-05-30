#include<vector>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include<thread>
#include <atomic>

int Count = 0;
std::atomic<int> ACount{0};
class T
{
	public:
		int value;
		T(int x):value(x){
			std::cout << "construct..." << x << std::endl;
		}
		T(const T& t) = delete;
		/*
		T(const T& t){
			value = t.value;
			std::cout << "copy construct..." << t.value << std::endl;
		}
		*/
		T(T&& t){
			value = t.value;
			std::cout << "move construct..." << t.value << std::endl;
		}
};

T t = 1;

template<typename T> void Inc(T& C)
{
	for(int i =0; i < 1000; i++){
		C++;
	}
}
int main(){
	std::vector<std::thread> VThread;
	VThread.emplace_back( std::thread{Inc<int>, std::ref(Count)} );
	VThread.emplace_back( std::thread{Inc<int>, std::ref(Count)} );
	VThread.emplace_back( std::thread{Inc<int>, std::ref(Count)} );
	VThread.emplace_back( std::thread{Inc<int>, std::ref(Count)} );
	VThread.emplace_back( std::thread{Inc<int>, std::ref(Count)} );

	VThread.emplace_back( std::thread{Inc<decltype(ACount)>, std::ref(ACount)} );
	VThread.emplace_back( std::thread{Inc<decltype(ACount)>, std::ref(ACount)} );
	VThread.emplace_back( std::thread{Inc<decltype(ACount)>, std::ref(ACount)} );
	VThread.emplace_back( std::thread{Inc<decltype(ACount)>, std::ref(ACount)} );
	VThread.emplace_back( std::thread{Inc<decltype(ACount)>, std::ref(ACount)} );
	for( auto& T : VThread){
		T.join();
	}
	std::cout << "Count:" << Count << std::endl;
	std::cout << "ACount:" << ACount << std::endl;
	return 0;
}
