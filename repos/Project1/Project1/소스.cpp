#include <iostream>

#include "List.h"

int main()
{
	CList<float> list;
	

	for (int i = 0; i < 4; ++i)
	{
		list.push_back(i);
	}

	// C printf
	std::cout << "¾È³ç" << 10 << std::endl;

	// C scanf
	int Input = 0;
	std::cin >> Input;

	return 0;
}