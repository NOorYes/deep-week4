#include <iostream>
#include <string>

// 이것을 템플릿화합니다.

template<typename T>
void print(const T num1)
{
	std::cout << num1 << std::endl;
}

/*
void print(const int& i)
{
	std::cout << i << std::endl;
}

void print(const float& i)
{
	std::cout << i << std::endl;
}

void print(const std::string& i)
{
	std::cout << i << std::endl;
}
*/

int main()
{
	print<int>(1);
	print<float>(2.345f);
	print<std::string>("Hello World");

	return 0;
}


