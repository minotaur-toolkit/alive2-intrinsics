#include <iostream>
#include <random>

int main()
{	
	std::cout << "1..." << std::flush;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::cout << "2..." << std::flush;
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::cout << "3..." << std::flush;
	std::uniform_int_distribution<> distrib(1, 6);
	std::cout << "4..." << std::endl;
	std::cout << distrib(gen) << std::endl;
	std::cout << "Reached End" << std::endl;

	return 0;
}
