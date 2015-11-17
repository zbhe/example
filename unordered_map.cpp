#include <iostream>
#include <string>
#include <unordered_map>
int main()
{
	std::unordered_map<std::string, int> Map;
	Map["a"] = 'a';
	Map["b"] = 'b';
	Map["c"] = 'c';
	auto It = Map.find("b");
	if( It != Map.end() ){
		Map.erase(It);
	}
	std::cout << "find:" << It->first << ", " << It->second <<std::endl;
	return 0;
}
