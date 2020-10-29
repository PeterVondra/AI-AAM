#pragma once

#include <iostream>
#include <numeric>
#include "include/Math/Matrix.h"
#include <fstream>
#include "include/Utils/Logger.h"

#undef max
#undef min

namespace xnn
{
	enum class ActivationFunction
	{
		Sigmoid,
		ParametricReLU,
		ELU
	};

	class NeuralNetwork
	{
		public:
			NeuralNetwork();
			NeuralNetwork(const unsigned int p_InputNeuronCount, std::initializer_list<unsigned int> p_HiddenLayersNeuronCount, const unsigned int p_OutputNeuronCount, const float p_LearningRate);
			NeuralNetwork(const unsigned int p_InputNeuronCount, std::initializer_list<unsigned int> p_HiddenLayersNeuronCount, const unsigned int p_OutputNeuronCount);
			~NeuralNetwork();

			void feedforward(MatrixN& p_Input);
			void flush();

			void clampOutput();

			MatrixN getOutput() { return m_Output; }

		private:
			friend class NetworkTrainer;
			
			double m_LearningRate = 0.05;
			double m_ReLUDyDx; // = 0

			MatrixN m_Input;
			std::vector<MatrixN> m_Weights; // for each layer - input layer
			std::vector<MatrixN> m_Bias; // for each layer - input layer
			std::vector<MatrixN> m_Deltas; //  for each layer - input layer
			std::vector<MatrixN> m_Activations; // for each layer - input layer
			std::vector<MatrixN> z; // for each layer - input layer
			MatrixN m_Output;


	};
}