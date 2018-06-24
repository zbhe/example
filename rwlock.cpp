//g++ -Wall -O3 -std=c++14 -o rwlock.out rwlock.cpp
#include <thread>
#include <mutex>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <atomic>
#include <chrono>
#include <ctime>
using namespace std;
struct FlagLock{
	atomic_flag Flag = ATOMIC_FLAG_INIT;
	mutex Lock;
	FlagLock(){}
	void lock()
	{
		while( Flag.test_and_set(memory_order_acquire) );
		//Lock.lock();
	}
	void unlock()
	{
		Flag.clear(memory_order_release);
		//Lock.unlock();
	}
};
#define USE_RW_LOCK
struct RWLock{
	int ReaderCount;
	mutex WriteLock;
	FlagLock OpLock;
	//mutex OpLock;
	FlagLock PrefLock;
	void LockRead()
	{
#ifdef USE_RW_LOCK
		PrefLock.lock();
		PrefLock.unlock();
		OpLock.lock();
		ReaderCount++;
		if( ReaderCount == 1 ){
			WriteLock.lock();
		}
		OpLock.unlock();
#else
		WriteLock.lock();
#endif
	}
	void UnlockRead()
	{
#ifdef USE_RW_LOCK
		OpLock.lock();
		ReaderCount--;
		if( ReaderCount == 0 ){
			WriteLock.unlock();
		}
		OpLock.unlock();
#else
		WriteLock.unlock();
#endif
	}
	void LockWrite()
	{
#ifdef USE_RW_LOCK
		PrefLock.lock();
		PrefLock.unlock();
		WriteLock.lock();
#else
		WriteLock.lock();
#endif
	}
	void UnlockWrite()
	{
		WriteLock.unlock();
	}
};
using MAPTYPE = unordered_map<int, vector<int> >;
atomic<bool> Reading{false};
bool Insert(MAPTYPE& Map)
{
	assert( ! Reading.load() );
	int Key = rand() % 10000;
	vector<int> Value;
	for( int i = 0; i < 100; ++i){
		Value.push_back(20000 + rand()%10000);
	}
	const auto& Ret = Map.emplace(Key, std::move(Value));
	return Ret.second;
}

void Delete(MAPTYPE& Map)
{
	assert( ! Reading.load() );
	Map.erase(Map.begin());
}

bool Find(const MAPTYPE& Map)
{
	Reading = true;
	int Key = rand() % 10000;
	const auto& It = Map.find(Key);
	if( It != Map.end() ){
		Reading = false;
		return true;
	}
	Reading = false;
	return false;
}
void Init(MAPTYPE& Map)
{
	int Count = 0;
	do{
		if( Insert(Map) ){
			Count++;
		}
	}while(Count < 1000);
}
void Write(MAPTYPE& Map, int Count, RWLock& Lock, atomic<bool>& Ready)
{
	while( ! Ready ){
		this_thread::yield();
	}
	std::cout << "begin Write\n";
	for( int i = 0; i < Count; i++){
		Lock.LockWrite();
		if( Insert(Map) ){
			Delete(Map);
		}
		Lock.UnlockWrite();
	}
	std::cout << "write done\n";
}
void Read(MAPTYPE& Map, int Count, RWLock& Lock, atomic<bool>& Ready)
{
	while( ! Ready ){
		this_thread::yield();
	}
	std::cout << "begin read\n";
	for( int i = 0; i < Count; i++){
		Lock.LockRead();
		Find(Map);
		Lock.UnlockRead();
	}
	std::cout << "read done\n";
}
int main()
{
	MAPTYPE DataMap;
	RWLock DataLock;
	Init(DataMap);
	srand(time(0));
	atomic<bool> Ready{false};
	vector<thread> WThreads;
	for( int i = 0; i < 2; i++ ){
		WThreads.emplace_back(thread{Write, ref(DataMap), 10000, ref(DataLock), ref(Ready)});
	}
	vector<thread> RThreads;
	for( int i = 0; i < 5; i++ ){
		RThreads.emplace_back(thread{Read, ref(DataMap), 1000000, ref(DataLock), ref(Ready)});
	}
	std::cout << "begin:" << DataMap.size() << std::endl;
	auto StartTime = std::chrono::system_clock::now();
	Ready = true;
	for(auto& Thread : WThreads ){
		Thread.join();
	}
	for(auto& Thread : RThreads ){
		Thread.join();
	}
	auto EndTime = std::chrono::system_clock::now();
	std::chrono::duration<double> Cost = EndTime - StartTime;
	std::cout << "all done:" << DataMap.size() << ", cost:" << Cost.count() << std::endl;
	return 0;
}
