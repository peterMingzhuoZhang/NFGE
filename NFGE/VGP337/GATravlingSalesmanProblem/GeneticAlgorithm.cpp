#include "GeneticAlgorithm.h"

#include <random>
#include <assert.h>

using namespace AI;

namespace
{
	std::random_device myRandomDevice{};
	std::mt19937 myRandomEngine{ myRandomDevice() };

	inline int RandomInt(int min, int max)
	{
		return std::uniform_int_distribution<>{min, max}(myRandomEngine);
	}

	inline float RandomFloat()
	{
		return std::uniform_real_distribution<float>{0.0f, 1.0f}(myRandomEngine);
	}
}

void GeneticAlgorithm::Initialize(int populationSize, int chromoLength, int maxGeneValue, int crossoverRate, float mutationRate, ComputeFitness computeFitness)
{
	mComputeFitness = std::move(computeFitness);

	// Reset
	mGeneration = 0;
	mMaxGeneValue= maxGeneValue;
	mMutationRate = mutationRate;
	mFound = false;

	// Create initial population
	mPopulation.clear();
	mPopulation.resize(populationSize);
	for (auto& genome : mPopulation)
	{
		genome.chromosome.reserve(chromoLength);
		for (int i = 0; i < chromoLength; i++)
		{
			genome.chromosome.push_back(RandomInt(0, maxGeneValue));
		}
	}

	SortPopulation();
	
}

void GeneticAlgorithm::Advance()
{
	if (mFound)
		return;

	++mGeneration;

	std::vector<Genome> newGeneration;
	newGeneration.reserve(mPopulation.size());

	// Perform Elitism: keep top 10%, mate the rest using top 50%
	const int eliteCount = ((int)mPopulation.size() * 10) / 100;
	for (int i = 0; i < eliteCount; i++)
	{
		newGeneration.push_back(mPopulation[i]);
	}

	const int mateCount = (int)mPopulation.size() - eliteCount;
	const int cutoff = (int)mPopulation.size() / 2;
	for (int i = 0; i < mateCount; i++)
	{
		const auto& parent1 = mPopulation[RandomInt(0, cutoff)];
		const auto& parent2 = mPopulation[RandomInt(0, cutoff)];
		newGeneration.push_back(Mate(parent1, parent2));
	}

	mPopulation = std::move(newGeneration);
	SortPopulation();

	if (mPopulation[0].fitness <= 0.0f)
	{
		mFound = true;
	}
}


GeneticAlgorithm::Genome GeneticAlgorithm::Mate(const Genome& parent1, const Genome& parent2)
{
	Genome offspring;
	offspring.chromosome.reserve(parent1.chromosome.size());

	for (size_t i = 0; i < parent1.chromosome.size(); i++)
	{
		if (RandomFloat() < mMutationRate)
			offspring.chromosome.push_back(RandomInt(0, mMaxGeneValue));
		else if (RandomFloat() < mCrossoverRate)
			offspring.chromosome.push_back(parent1.chromosome[i]);
		else
			offspring.chromosome.push_back(parent2.chromosome[i]);
	}

	return offspring;
}

void AI::GeneticAlgorithm::SortPopulation()
{
	for (auto& genome : mPopulation)
	{
		mComputeFitness(genome);
	}
	std::sort(mPopulation.begin(), mPopulation.end(), [](const auto& a, const auto& b) {return a.fitness < b.fitness; });
}
