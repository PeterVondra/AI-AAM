#pragma once

#include<iostream>
#include <thread>
#include "include/Utils/Logger.h"
#include "include/Math/Math.h"
#include "include/Math/Vector.h"
#include "include/Math/Matrix.h"
#include "NNetwork.h"
#include "NNetworkTrainer.h"
#include "MNISTDataLoader.h"
#include "Regression.h"

namespace xnn
{
	struct State
	{
		MatrixN sequence;
		MatrixN transform;
		State* next;
		State* prev;
	};

	class HopField
	{
		public:
			HopField(const uint64_t p_StateExtent)
			{
				m_Weights = MatrixN(p_StateExtent, p_StateExtent, 0.0);
			}
			~HopField()
			{

			}

			// Add to memory, optimally two states per network
			void addStateToMemory(MatrixN& p_State)
			{
				for (uint64_t c = 0; c < m_Weights.getColumnSize(); c++)
				{
					for (uint64_t r = 0; r < m_Weights.getRowSize(); r++)
					{
						if (c != r)
							m_Weights.getColumn(c, r) += (2 * p_State.getColumn(c, 0) - 1) * (2 * p_State.getColumn(r, 0) - 1);
					}
				}

				m_MemoryStates.push_back(p_State);
			}

			// Get output given a sequence
			MatrixN& getOutput(MatrixN& p_StateInput)
			{
				m_Output = p_StateInput;
				
				uint64_t streak = 0;
				uint64_t count = 0;
				while(true)
				{
					uint64_t c = Math::getRandomNumber(0, p_StateInput.getColumnSize());
					int64_t r = (m_Weights.getRow(c).dot(m_Output.getRow(0)))[0];
					uint64_t prev = m_Output[c];
					m_Output[c] = r >= 0 ? 1 : 0;
					streak += r >= 0 ? 1 : 0;

					if (m_Output[c] != prev)
						streak = 0;
					else if (streak >= m_Weights.getColumnSize()*2)
						break;

					count++;
				}

				return m_Output;
			}
			
			// Get "input" given a sequence
			MatrixN& getInverseOutput(MatrixN& p_StateInput)
			{
				m_Output = p_StateInput;

				uint64_t streak = 0;
				uint64_t count = 0;
				while (true)
				{
					uint64_t c = Math::getRandomNumber(0, p_StateInput.getColumnSize());
					int64_t r = (m_Weights.getRow(c).dot(m_Output.getRow(0)))[0];
					uint64_t prev = m_Output[c];
					m_Output[c] = r < 0 ? 1 : 0;
					streak += r >= 0 ? 1 : 0;

					if (m_Output[c] != prev)
						streak = 0;
					else if (streak >= m_Weights.getColumnSize() * 2)
						break;

					count++;
				}

				return m_Output;
			}

		protected:
		private:
			MatrixN binary(MatrixN& p_Matrix)
			{
				MatrixN rhs = p_Matrix;
				for (auto& m : rhs.getRawData())
					m = m > 0.0 ? 1.0 : 0.0;
				return rhs;
			}
			MatrixN polar(MatrixN& p_Matrix)
			{
				MatrixN rhs = p_Matrix;
				for (auto& m : rhs.getRawData())
					m = m > 0.0 ? 1.0 : -1.0;
				return rhs;
			}

			std::vector<MatrixN> m_MemoryStates;
			MatrixN m_Weights;
			MatrixN m_Output;
			float m_Capacity;

			// std::vector<uint64_t> m_MemoryID;
	};
}