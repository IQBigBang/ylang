#include <iostream>

extern "C" double _Wavg_n_n(double, double);

int main()
{
	std::cout << _Wavg_n_n(2, 3) << std::endl;
	return 1;
}
