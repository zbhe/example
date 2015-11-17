#include <stdio.h>
using namespace std;
class Print{
	public:
		Print(...){
			printf(__VA_ARGS__);
		}
};

int main(){
	Print p("number %d, %d\n", 1, 2);
	return 0;
}
