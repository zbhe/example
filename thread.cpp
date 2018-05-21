#include<vector>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include<thread>
#include <atomic>

int Count = 0;
std::atomic<int> ACount{0};

template<typename T> void Inc(T& C)
{
	for(int i =0; i < 1000; i++){
		C++;
	}
}
int main(){
	std::vector<std::thread> VThread;
	VThread.push_back( std::thread{Inc<int>, &Count} );
	/*VThread.emplace_back( std::thread{Inc<int>, &Count} );

	VThread.emplace_back( std::thread{Inc<decltype(ACount)>, &ACount} );
	VThread.emplace_back( std::thread{Inc<decltype(ACount)>, &ACount} );
	*/
	for( auto& T : VThread){
		T.join();
	}
	std::cout << "Count:" << Count << std::endl;
	std::cout << "ACount:" << ACount << std::endl;
	return 0;
}
