#include <iostream>
#include "NeuralNetwork.h"

int main(int argc, char* argv[])
{
	const std::vector<std::vector<double>> xorInputValues = 
	{
		{0,0},
		{ 0,1 },
		{ 1,0 },
		{ 1,1 }
	};

	const std::vector<std::vector<double>> xorOutputValues =
	{
		{ 0 },
		{ 1 },
		{ 1 },
		{ 0 }
	};

	AI::NeuralNetwork ann({ 2u, 2u,1u });

	size_t example = 0;
	for (size_t i = 0; i < 3000; i++)
	{

		ann.FeedFoward(xorInputValues[example]);
		ann.BackwardPropagate(xorOutputValues[example]);
		example = (example == 3) ? 0 : example + 1;
	}

	return 0;
	system("pause");
}