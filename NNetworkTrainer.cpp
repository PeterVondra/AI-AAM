#include "NNetworkTrainer.h"

namespace xnn
{
	NeuralNetwork* NetworkTrainer::m_Network = nullptr;
	double NetworkTrainer::lambda = 0.0001;

	void NetworkTrainer::backpropogateSGD(MatrixN& p_Desired)
	{
		std::vector<MatrixN>& deltas = m_Network->m_Deltas;
		std::vector<MatrixN>& activations = m_Network->m_Activations;
		std::vector<MatrixN>& z = m_Network->z;
		std::vector<MatrixN>& weights = m_Network->m_Weights;
		std::vector<MatrixN>& bias = m_Network->m_Bias;

		deltas[deltas.size() - 1] = (activations[activations.size() - 1] - p_Desired).hadamard(getSigmoidDyDx(z[z.size() - 1]));

		weights[weights.size() - 1] = weights[weights.size() - 1] - (deltas[deltas.size() - 1] * activations[activations.size() - 2].transposed()) * m_Network->m_LearningRate
			;// +weights[weights.size() - 1].getSign() * lambda;

		bias[bias.size() - 1] = bias[bias.size() - 1] - deltas[deltas.size() - 1] * m_Network->m_LearningRate;

		for (int i = 2; i < m_Network->m_Weights.size(); i++)
		{
			deltas[deltas.size() - i] = (weights[weights.size() - i + 1].transposed() * deltas[deltas.size() - i + 1]).hadamard(getSigmoidDyDx(z[z.size() - i]));

			weights[weights.size() - i] = weights[weights.size() - i] - (deltas[deltas.size() - i] * activations[activations.size() - i - 1].transposed()) * m_Network->m_LearningRate
				;// +weights[weights.size() - i].getSign() * lambda;

			bias[bias.size() - i] = bias[bias.size() - i] - deltas[deltas.size() - i] * m_Network->m_LearningRate;
		}

		deltas[0] = (weights[1].transposed() * deltas[1]).hadamard(getSigmoidDyDx(z[0]));

		weights[0] = weights[0] - (deltas[0] * m_Network->m_Input.transposed()) * m_Network->m_LearningRate;// +weights[0].getSign() * lambda;

		bias[0] = bias[0] - deltas[0] * m_Network->m_LearningRate;
	}
	void NetworkTrainer::feedForwardSGD(MatrixN& p_Input)
	{
		m_Network->m_Input = p_Input;

		m_Network->z[0] = (m_Network->m_Weights[0] * m_Network->m_Input + m_Network->m_Bias[0]);// +(lambda * m_Network->m_Weights[0].tcnorm());
		m_Network->m_Activations[0] = getSigmoid(m_Network->z[0]);

		for (int i = 1; i < m_Network->m_Activations.size(); i++)
		{
			m_Network->z[i] = (m_Network->m_Weights[i] * m_Network->m_Activations[i - 1] + m_Network->m_Bias[i]);// +(lambda * m_Network->m_Weights[i].tcnorm());
			m_Network->m_Activations[i] = getSigmoid(m_Network->z[i]);
		}

		m_Network->m_Output = getSigmoid(m_Network->z[m_Network->z.size() - 1]);
	}
	void NetworkTrainer::backpropogateMBGD(std::vector<MatrixN>& p_DesiredBatch)
	{
		//std::vector<MatrixN>& deltas = m_Network->m_Deltas;
		//std::vector<MatrixN>& activations = m_Network->m_Activations;
		//std::vector<MatrixN>& z = m_Network->z;
		//std::vector<MatrixN>& weights = m_Network->m_Weights;
		//std::vector<MatrixN>& bias = m_Network->m_Bias;
		//
		//deltas[deltas.size() - 1] = (activations[activations.size() - 1] - p_Desired).hadamard(getSigmoidDyDx(z[z.size() - 1]));
		//
		//weights[weights.size() - 1] = weights[weights.size() - 1] - (deltas[deltas.size() - 1] * activations[activations.size() - 2].transposed()) * m_Network->m_LearningRate
		//	;// +weights[weights.size() - 1].getSign() * lambda;
		//
		//bias[bias.size() - 1] = bias[bias.size() - 1] - deltas[deltas.size() - 1] * m_Network->m_LearningRate;
		//
		//for (int i = 2; i < m_Network->m_Weights.size(); i++)
		//{
		//	deltas[deltas.size() - i] = (weights[weights.size() - i + 1].transposed() * deltas[deltas.size() - i + 1]).hadamard(getSigmoidDyDx(z[z.size() - i]));
		//
		//	weights[weights.size() - i] = weights[weights.size() - i] - (deltas[deltas.size() - i] * activations[activations.size() - i - 1].transposed()) * m_Network->m_LearningRate
		//		;// +weights[weights.size() - i].getSign() * lambda;
		//
		//	bias[bias.size() - i] = bias[bias.size() - i] - deltas[deltas.size() - i] * m_Network->m_LearningRate;
		//}
		//
		//deltas[0] = (weights[1].transposed() * deltas[1]).hadamard(getSigmoidDyDx(z[0]));
		//
		//weights[0] = weights[0] - (deltas[0] * m_Network->m_Input.transposed()) * m_Network->m_LearningRate;// +weights[0].getSign() * lambda;
		//
		//bias[0] = bias[0] - deltas[0] * m_Network->m_LearningRate;
	}
	void NetworkTrainer::feedForwardMBGD(std::vector<MatrixN>& p_InputBatch, std::vector<MatrixN>& p_DesiredBatch)
	{
		std::vector<MatrixN> gradientWSum(m_Network->m_Weights.size());
		for (int i = 0; i < gradientWSum.size(); i++)
			gradientWSum[i] = MatrixN(m_Network->m_Weights[i].getRowSize(), m_Network->m_Weights[i].getColumnSize(), (double)0);

		std::vector<MatrixN> gradientBSum(m_Network->m_Bias.size());
		for (int i = 0; i < gradientBSum.size(); i++)
			gradientBSum[i] = MatrixN(m_Network->m_Bias[i].getRowSize(), m_Network->m_Bias[i].getColumnSize(), (double)0);

		std::vector<MatrixN>& weights = m_Network->m_Weights;
		std::vector<MatrixN>& bias = m_Network->m_Bias;

		for (int b = 0; b < p_InputBatch.size(); b++)
		{
			m_Network->m_Input = p_InputBatch[b];

			m_Network->z[0] = (m_Network->m_Weights[0] * m_Network->m_Input + m_Network->m_Bias[0]);// +(m_Network->m_Weights[0].tcnorm() * lambda);
			m_Network->m_Activations[0] = getSigmoid(m_Network->z[0]);

			for (int i = 1; i < m_Network->m_Activations.size(); i++)
			{
				m_Network->z[i] = (m_Network->m_Weights[i] * m_Network->m_Activations[i - 1] + m_Network->m_Bias[i]);// +(m_Network->m_Weights[i].tcnorm() * lambda);
				m_Network->m_Activations[i] = getSigmoid(m_Network->z[i]);
			}

			std::vector<MatrixN>& deltas = m_Network->m_Deltas;
			std::vector<MatrixN>& activations = m_Network->m_Activations;
			std::vector<MatrixN>& z = m_Network->z;

			int L = deltas.size() - 1;

			deltas[L] = (activations[activations.size() - 1] - p_DesiredBatch[b]).hadamard(getSigmoidDyDx(z[z.size() - 1]));

			gradientWSum[L] = gradientWSum[L] + (deltas[L] * activations[activations.size() - 2].transposed());// +weights[L].getSign() * lambda;

			gradientBSum[L] = gradientBSum[L] + deltas[L];

			for (int i = 2; i < m_Network->m_Weights.size(); i++)
			{
				deltas[deltas.size() - i] = (weights[weights.size() - i + 1].transposed() * deltas[deltas.size() - i + 1]).hadamard(getSigmoidDyDx(z[z.size() - i]));

				gradientWSum[deltas.size() - i] = gradientWSum[deltas.size() - i] + (deltas[deltas.size() - i] * activations[activations.size() - i - 1].transposed());// + weights[weights.size() - i].getSign() * lambda;

				gradientBSum[deltas.size() - i] = gradientBSum[deltas.size() - i] + deltas[deltas.size() - i];
			}

			deltas[0] = (weights[1].transposed() * deltas[1]).hadamard(getSigmoidDyDx(z[0]));
			
			gradientWSum[0] = gradientWSum[0] + (deltas[0] * m_Network->m_Input.transposed());// +weights[0].getSign() * lambda;
			
			gradientBSum[0] = gradientBSum[0] + deltas[0];
		}

		for (int i = 0; i < m_Network->m_Weights.size(); i++)
		{
			weights[i] = weights[i] - gradientWSum[i] * (m_Network->m_LearningRate / p_InputBatch.size());// +weights[i].getSign() * lambda;
		
			bias[i] = bias[i] - gradientBSum[i] * (m_Network->m_LearningRate / p_InputBatch.size());
		}
	}

	double NetworkTrainer::getSigmoid(double& x) {
		return 1 / (1 + std::expf(-x));
	}
	double NetworkTrainer::getSigmoidDyDx(double& x) {
		return NetworkTrainer::getSigmoid(x) * (1 - getSigmoid(x));
	}
	double NetworkTrainer::getLeakyReLU(double& x)
	{
		return x > 0 ? x : 0.01 * x;
	}
	double NetworkTrainer::getLeakyReLUDyDx(double& x)
	{
		return x > 0 ? 1 : 0.01;
	}
	double NetworkTrainer::getParametricReLU(double& x) {
		if (m_Network->m_ReLUDyDx <= 1)
			return std::max(x, m_Network->m_ReLUDyDx * x);
		return x > 0 ? x : m_Network->m_ReLUDyDx * x;
	}
	double NetworkTrainer::getParametricReLUDyDx(double& x) {
		double slope = x > 0 ? 1 : m_Network->m_ReLUDyDx;
		slope = m_Network->m_ReLUDyDx <= 1 ? (std::max(m_Network->m_ReLUDyDx * x, x) == x ? 1 : m_Network->m_ReLUDyDx) : slope;

		return slope;
	}
	double NetworkTrainer::getELU(double& x) {
		return x > 0 ? x : m_Network->m_ReLUDyDx * (std::expf(x) - 1);
	}
	double NetworkTrainer::getELUDyDx(double& x) {
		return x > 0 ? 1 : m_Network->m_ReLUDyDx * std::expf(x);
	}

	float NetworkTrainer::getSigmoid(float& x) {
		return 1 / (1 + std::expf(-x));
	}
	float NetworkTrainer::getSigmoidDyDx(float& x) {
		return getSigmoid(x) * (1 - getSigmoid(x));
	}
	float NetworkTrainer::getLeakyReLU(float& x)
	{
		return x > 0 ? x : 0.01 * x;
	}
	float NetworkTrainer::getLeakyReLUDyDx(float& x)
	{
		return x > 0 ? 1 : 0.01;
	}
	float NetworkTrainer::getParametricReLU(float& x) {
		if (m_Network->m_ReLUDyDx <= 1)
			return std::max(x, (float)m_Network->m_ReLUDyDx * x);
		return x > 0 ? x : m_Network->m_ReLUDyDx * x;
	}
	float NetworkTrainer::getParametricReLUDyDx(float& x) {
		double slope = x > 0 ? 1 : m_Network->m_ReLUDyDx;
		slope = m_Network->m_ReLUDyDx <= 1 ? (std::max((float)m_Network->m_ReLUDyDx * x, x) == x ? 1 : m_Network->m_ReLUDyDx) : slope;

		return slope;
	}
	float NetworkTrainer::getELU(float& x) {
		return x > 0 ? x : m_Network->m_ReLUDyDx * (std::expf(x) - 1);
	}
	float NetworkTrainer::getELUDyDx(float& x) {
		return x > 0 ? 1 : m_Network->m_ReLUDyDx * std::expf(x);
	}

	MatrixN NetworkTrainer::getSigmoid(MatrixN& x) {
		MatrixN m = x;
		for (auto& mi : m.getRawData())
			mi = getSigmoid(mi);
		return m;
	}
	MatrixN NetworkTrainer::getSigmoidDyDx(MatrixN& x) {
		MatrixN m = x;
		for (auto& mi : m.getRawData())
			mi = getSigmoidDyDx(mi);
		return m;
	}
	MatrixN NetworkTrainer::getLeakyReLU(MatrixN& x)
	{
		MatrixN m = x;
		for (auto& mi : m.getRawData())
			mi = getLeakyReLU(mi);
		return m;
	}
	MatrixN NetworkTrainer::getLeakyReLUDyDx(MatrixN& x)
	{
		MatrixN m = x;
		for (auto& mi : m.getRawData())
			mi = getLeakyReLUDyDx(mi);
		return m;
	}
	MatrixN NetworkTrainer::getParametricReLU(MatrixN& x) {
		MatrixN m = x;
		for (auto& mi : m.getRawData())
			mi = getParametricReLU(mi);
		return m;
	}
	MatrixN NetworkTrainer::getParametricReLUDyDx(MatrixN& x) {
		MatrixN m = x;
		for (auto& mi : m.getRawData())
			mi = getParametricReLUDyDx(mi);
		return m;
	}
	MatrixN NetworkTrainer::getELU(MatrixN& x) {
		MatrixN m = x;
		for (auto& mi : m.getRawData())
			mi = getELU(mi);
		return m;
	}
	MatrixN NetworkTrainer::getELUDyDx(MatrixN& x) {
		MatrixN m = x;
		for (auto& mi : m.getRawData())
			mi = getELUDyDx(mi);
		return m;
	}
}