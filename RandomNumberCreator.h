#ifndef __RANDOMNUMBERCREATOR_H__
#define __RANDOMNUMBERCREATOR_H__

#include <random>

template<typename T = int,typename P = int>
int RandomNumberCreator(T&& first,P&& last){
	std::random_device r;
	std::seed_seq seed{r()};
	std::mt19937 rng(seed);

	std::uniform_int_distribution<int> dist(first,last);

	return dist(rng);
}	

#endif
