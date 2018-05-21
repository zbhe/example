#include <cmath>
#include <limits>
#include <iostream>
bool IsInteger(float Value)
{
	return (Value - std::trunc(Value)) < std::numeric_limits<float>::epsilon();
}

int main()
{
	std::cout << 0.2 << "\t" << 1/std::sqrt(5)/std::sqrt(5) << "\t" << IsInteger(2.0000001) << std::endl;
	std::cout << "type\t\tlowest\t\thighest\t\tspecialized\n";
	std::cout << "int\t\t"
		<< std::numeric_limits<int>::lowest() << '\t'
		<< std::numeric_limits<int>::max() << '\t'
		<< std::numeric_limits<int>::is_specialized << '\n';
	std::cout << "char" << sizeof(char) << "\t\t"
		<< (int)std::numeric_limits<char>::lowest() << '\t'
		<< (int)std::numeric_limits<char>::max() << '\t'
		<< std::numeric_limits<char>::is_specialized << '\n';
	std::cout << "int16_t\t\t"
		<< std::numeric_limits<int16_t>::lowest() << '\t'
		<< std::numeric_limits<int16_t>::max() << '\t'
		<< std::numeric_limits<int16_t>::is_specialized << '\n';
	std::cout << "uint16_t" << sizeof(uint16_t) << "\t\t"
		<< std::numeric_limits<uint16_t>::lowest() << '\t'
		<< std::numeric_limits<uint16_t>::max() << '\t'
		<< std::numeric_limits<uint16_t>::is_specialized << '\n';
	std::cout << "float\t\t"
		<< std::numeric_limits<float>::lowest() << '\t'
		<< std::numeric_limits<float>::max() << '\t'
		<< std::numeric_limits<float>::is_specialized << '\n';
	return 0;
}
