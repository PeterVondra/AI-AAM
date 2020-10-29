#pragma once

#include <iostream>
#include "NNetwork.h"

#undef max
#undef min

namespace xnn
{
	class NetworkTrainer
	{
		public:
			static void setNetwork(NeuralNetwork& p_Network) { m_Network = &p_Network; }

			static void backpropogateSGD(MatrixN& p_Desired);
			static void feedForwardSGD(MatrixN& p_Input);

			static void backpropogateMBGD(std::vector<MatrixN>& p_DesiredBatch);
			static void feedForwardMBGD(std::vector<MatrixN>& p_InputBatch, std::vector<MatrixN>& p_DesiredBatch);
		private:
			static NeuralNetwork* m_Network;
			static double lambda;

			static double getSigmoid(double& x);
			static double getSigmoidDyDx(double& x);
			static double getLeakyReLU(double& x);
			static double getLeakyReLUDyDx(double& x);
			static double getParametricReLU(double& x);
			static double getParametricReLUDyDx(double& x);
			static double getELU(double& x);
			static double getELUDyDx(double& x);

			static float getSigmoid(float& x);
			static float getSigmoidDyDx(float& x);
			static float getLeakyReLU(float& x);
			static float getLeakyReLUDyDx(float& x);
			static float getParametricReLU(float& x);
			static float getParametricReLUDyDx(float& x);
			static float getELU(float& x);
			static float getELUDyDx(float& x);

			static MatrixN getSigmoid(MatrixN& x);
			static MatrixN getSigmoidDyDx(MatrixN& x);
			static MatrixN getLeakyReLU(MatrixN& x);
			static MatrixN getLeakyReLUDyDx(MatrixN& x);
			static MatrixN getParametricReLU(MatrixN& x);
			static MatrixN getParametricReLUDyDx(MatrixN& x);
			static MatrixN getELU(MatrixN& x);
			static MatrixN getELUDyDx(MatrixN& x);
	};
}