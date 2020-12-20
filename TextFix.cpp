#include<iostream>
#include <thread>
#include <bitset>
#include "include/Utils/Logger.h"
#include "include/Math/Math.h"
#include "include/Math/Vector.h"
#include "include/Math/Matrix.h"
#include "NNetwork.h"
#include "NNetworkTrainer.h"
#include "MNISTDataLoader.h"
#include "Regression.h"
#include "HopField.h"


#define XNNBASE 8

std::string BinaryStringToText(std::string binaryString) {
	std::string text = "";
	std::stringstream sstream(binaryString);
	while (sstream.good())
	{
		std::bitset<8> bits;
		sstream >> bits;
		text += char(bits.to_ulong());
	}
	return text;
}

// Base is 8
MatrixN getMatrixFromText(std::string p_Text, uint32_t p_Characters)
{
	MatrixN text(p_Characters, XNNBASE, 0.0);
	for (uint32_t i = 0; i < p_Text.length(); i++)
		for (int j = 0; j < XNNBASE; j++)
			text.getRow(i, j) = (std::bitset<XNNBASE>(p_Text[i]).to_ulong() & (1 << j)) >> j;
	text.m_columns = XNNBASE * p_Characters;
	text.m_rows = 1;
	return text;
}
//
std::string getTextFromMatrix(MatrixN& p_Matrix, uint32_t p_Characters)
{
	p_Matrix.m_columns = XNNBASE;
	p_Matrix.m_rows = p_Characters;

	std::string text;
	for (uint32_t i = 0; i < p_Characters; i++) {
		int ascii = 0;
		for (int j = 0; j < XNNBASE; j++)
			ascii += p_Matrix.getRow(i)[j] * std::pow(2, j);
		text.push_back((char)(ascii));
	}

	p_Matrix.m_columns = XNNBASE * p_Characters;
	p_Matrix.m_rows = 1;

	return text;
}

std::vector<std::string> words = {
	"Hopfield"
};

#define MAX_WORD_LENGTH 100

int main()
{
	xnn::HopField hf(XNNBASE * MAX_WORD_LENGTH);
	
	for(auto& str : words)
		hf.addStateToMemory(getMatrixFromText(str, str.length()));

	std::string strIn = "HopsR  ";
	MatrixN stateIn = getMatrixFromText(strIn, MAX_WORD_LENGTH);
	std::cout << "txt(" << strIn << ") corrupted" << std::endl;

	std::cout << "txt(" << getTextFromMatrix(hf.getOutput(stateIn), MAX_WORD_LENGTH) << ") fixed" << std::endl;

	return 0;
}