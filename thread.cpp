#include<vector>
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
#include<thread>
#include<atomic>
#include<sstream>

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

std::atomic<unsigned int> UCount{0};
template<typename T> void Inc(T& C)
{
	for(int i =0; i < 1000; i++){
		C++;
	}
}

void test()
{
	for(int i =0; i < 1000; i++){
		unsigned int Current = UCount.load();
		assert(UCount.fetch_add(1) == Current);
		static_assert( std::is_same<decltype(UCount = Current), unsigned int>::value, "error");
		assert((Current + 1) == UCount);
	}
}

int main(){
	std::vector<std::thread> VThread;
	VThread.emplace_back( std::thread{Inc<int>, std::ref(Count)} );
	VThread.emplace_back( std::thread{Inc<int>, std::ref(Count)} );

	VThread.emplace_back( std::thread{Inc<decltype(ACount)>, std::ref(ACount)} );
	VThread.emplace_back( std::thread{Inc<decltype(ACount)>, std::ref(ACount)} );

	VThread.emplace_back( std::thread{test} );
	for( auto& T : VThread){
		T.join();
	}
	std::cout << "Count:" << Count << std::endl;
	std::cout << "ACount:" << ACount << std::endl;
	return 0;
}
