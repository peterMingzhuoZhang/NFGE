#pragma once

#include <vector>

namespace AI
{
	class Neuron;
	using Layer = std::vector<Neuron>;

	struct Connection
	{
		double weight;
		double deltaWeight;
	};

	class Neuron
	{
	public:
		Neuron(size_t numOutputs, size_t myIndex);

		void SetOutputValue(double value) { mOutputValue = value; }
		double GetOutputValue() const { return mOutputValue; }

		void FeedForward(const Layer& previousLayer);
		void CalculateOutputGradients(double targetValue);
		void CalculateHiddenGradients(const Layer& nextLayer);
		void UpdateInputWeights(Layer& previousLayer);

	private:
		std::vector<Connection> mOutputWeights;
		size_t mIndex;
		double mOutputValue;
		double mGradient;
	};

	class NeuralNetwork
	{
	public:
		NeuralNetwork(const std::vector<size_t>& topology);

		void FeedFoward(const std::vector<double>& inputValues);
		void BackwardPropagate(const std::vector<double>& targetValues);
		std::vector<double> GetResults() const;

	private:
		std::vector<Layer> mLayers;
	};
}