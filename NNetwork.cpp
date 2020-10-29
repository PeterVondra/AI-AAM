#include "NNetwork.h"

namespace xnn
{
	NeuralNetwork::NeuralNetwork()
	{

	}
	NeuralNetwork::NeuralNetwork(const unsigned int p_InputNeuronCount, std::initializer_list<unsigned int> p_HiddenLayersNeuronCount, const unsigned int p_OutputNeuronCount, const float p_LearningRate)
		: m_Input(p_InputNeuronCount, 1, (double)0), m_Output(p_OutputNeuronCount, 1, (double)0), m_LearningRate(p_LearningRate)
	{
		std::vector<unsigned int> hiddenLayers(p_HiddenLayersNeuronCount.begin(), p_HiddenLayersNeuronCount.end());
		m_Weights.resize(hiddenLayers.size() + 1);
		m_Bias.resize(hiddenLayers.size() + 1);

		m_Weights[0] = MatrixN(hiddenLayers[0], p_InputNeuronCount, true);
		m_Bias[0] = MatrixN(hiddenLayers[0], 1, true);

		for (int i = 1; i < hiddenLayers.size(); i++)
		{
			m_Weights[i] = MatrixN(hiddenLayers[i], hiddenLayers[i - 1], true);
			m_Bias[i] = MatrixN(hiddenLayers[i], 1, true);
		}

		m_Weights[hiddenLayers.size()] = MatrixN(p_OutputNeuronCount, m_Weights[hiddenLayers.size() - 1].getRowSize(), true);
		m_Bias[hiddenLayers.size()] = MatrixN(p_OutputNeuronCount, 1, true);
		m_Activations.resize(hiddenLayers.size() + 1);
		m_Deltas.resize(hiddenLayers.size() + 1);
		z.resize(hiddenLayers.size() + 1);
	}
	NeuralNetwork::NeuralNetwork(const unsigned int p_InputNeuronCount, std::initializer_list<unsigned int> p_HiddenLayersNeuronCount, const unsigned int p_OutputNeuronCount)
		: m_Input(p_InputNeuronCount, 1, (double)0), m_Output(p_OutputNeuronCount, 1, (double)0)
	{
		std::vector<unsigned int> hiddenLayers(p_HiddenLayersNeuronCount.begin(), p_HiddenLayersNeuronCount.end());
		m_Weights.resize(hiddenLayers.size() + 1);
		m_Bias.resize(hiddenLayers.size() + 1);

		m_Weights[0] = MatrixN(hiddenLayers[0], p_InputNeuronCount, true);
		m_Bias[0] = MatrixN(hiddenLayers[0], 1, true);

		for (int i = 1; i < hiddenLayers.size(); i++)
		{
			m_Weights[i] = MatrixN(hiddenLayers[i], hiddenLayers[i - 1], true);
			m_Bias[i] = MatrixN(hiddenLayers[i], 1, true);
		}

		m_Weights[hiddenLayers.size()] = MatrixN(p_OutputNeuronCount, m_Weights[hiddenLayers.size() - 1].getRowSize(), true);
		m_Bias[hiddenLayers.size()] = MatrixN(p_OutputNeuronCount, 1, true);
		m_Activations.resize(hiddenLayers.size() + 1);
		m_Deltas.resize(hiddenLayers.size() + 1);
		z.resize(hiddenLayers.size() + 1);
	}
	NeuralNetwork::~NeuralNetwork()
	{

	}
	void NeuralNetwork::flush()
	{

	}
	void NeuralNetwork::clampOutput()
	{

	}
}