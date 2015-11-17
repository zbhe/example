#include <iostream>
#include <vector>
#include <stdlib.h>

class Fenwick_Tree_Sum
{
	std::vector<int> tree;
	public:
    Fenwick_Tree_Sum(const std::vector<int>& Arg)//Arg is our array on which we are going to work
    {
        tree.resize(Arg.size());
 
        for(int i = 0 ; i<tree.size(); i++)
            increase(i, Arg[i]);
 
    }
 
    // Increases value of i-th element by ''delta''.
    void increase(int i, int delta)
    {
        for (; i < (int)tree.size(); i |= i + 1)
            tree[i] += delta;
    }
 
    // Returns sum of elements with indexes left..right, inclusive; (zero-based);
    int getsum(int left, int right)
    {
        return sum(right) - sum(left-1); //when left equals 0 the function hopefully returns 0
    }
 
    int sum(int ind)
    {
        int sum = 0;
        while (ind>=0)
        {
            sum += tree[ind];
            ind &= ind + 1;
            ind--;
        }
        return sum;
    }
};

int main()
{
	srandom(clock());
	std::vector<int> Data = {100, 300, 200, 1000, 600, 500, 0};

	Fenwick_Tree_Sum Tree(Data);

	int Index = 0;
	while(1){
		for( int v : Data ){
			std::cout << v << " ";
		}
		std::cout << "\ninput index:" <<std::endl;
		std::cin >> Index;
		std::cout << Tree.sum(Index) << std::endl;
		int R = random()%100;
		Data[Index] += R;
		Tree.increase(Index, R);
	}
	return 0;
}
