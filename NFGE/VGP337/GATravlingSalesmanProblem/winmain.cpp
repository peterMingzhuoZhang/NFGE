#include "GameState.h"
#include "GeneticAlgorithm.h"

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//NFGEApp::AddState<GameState>("GameState_0");
	//NFGEApp::Run({});

	const std::string kTarget = "I Love Genetic Algorithm!";

	const int validGenevalue = 126 - 32;

	AI::GeneticAlgorithm ga;
	ga.Initialize(100, (int)kTarget.size(), validGenevalue, 0.45f, 0.1f, [&kTarget](auto& genome)
	{
		genome.fitness = 0.0f;
		for (size_t i = 0; i < kTarget.size(); i++)
		{
			if (kTarget[i] != genome.chromosome[i] + 32)
			{
				genome.fitness += 1.0f; // Penalize any characters that don't match the target
			}
		}
	});

	auto print = [](const AI::GeneticAlgorithm& ga)
	{
		int gen = ga.GetGeneration();
		auto& fittest = ga.BestGenome();
		std::string result;
		for (auto i : fittest.chromosome)
		{
			result += (char)(i + 32);
		}
		LOG("Generation: %d - Result: %s - Fitness: %f", gen, result.c_str(), fittest.fitness);
	};

	while (true)
	{
		ga.Advance();
		if (ga.Found())
		{
			break;
		}
		print(ga);
	}
	print(ga);

	return 0;
}