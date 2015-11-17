#include<iostream>
#include<memory>
#include<vector>
#include <algorithm>

int main(int argc, char** argv)
{
	std::unique_ptr<std::vector<int>> pv = std::make_unique<std::vector<int>>();

	pv->push_back(9);
	std::vector<int> vt;
	vt.push_back(1);
	vt.push_back(2);
	vt.push_back(3);

	std::for_each(vt.begin(), vt.end(), [pv = std::move(pv)](auto& vi){
		for(auto& i: *pv)
		std::cout << i << "\n";
	}); 
}
