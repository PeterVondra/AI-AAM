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

bool breakLearning = false;
bool showHighest = false;

void tf()
{
	while (1)
	{
		std::string str;
		std::cin >> str;
		if (str == "q" || str == "Q")
			breakLearning = true;
		else if (str == "r" || str == "R")
			showHighest = true;
		else
			showHighest = false;
	}
}

int main()
{
	const int batchsize = 1;
	int imagecount = 0;

	std::vector<MatrixN> images;
	std::vector<int> labels;
	std::vector<MatrixN> testimages;
	std::vector<int> testlabels;

	xnn::MNISTDataLoader::loadImages("TrainingData/train-images.idx3-ubyte", images);
	xnn::MNISTDataLoader::loadLabels("TrainingData/train-labels.idx1-ubyte", labels);
	xnn::MNISTDataLoader::loadImages("TrainingData/t10k-images.idx3-ubyte", testimages);
	xnn::MNISTDataLoader::loadLabels("TrainingData/t10k-labels.idx1-ubyte", testlabels);

	std::vector<std::vector<MatrixN>> imagesBatched;
	imagesBatched.resize(images.size() / batchsize);
	for (int i = 0; i < imagesBatched.size(); i++)
	{
		imagesBatched[i].resize(batchsize);
		for (int b = 0; b < batchsize; b++)
		{
			imagesBatched[i][b] = images[(i + 1) * batchsize - (batchsize - b)];
		}
	}

	std::vector<std::vector<MatrixN>> correct;

	correct.resize(labels.size() / batchsize);

	std::cout << correct.size() << std::endl;

	for (int i = 0; i < correct.size(); i++)
	{
		correct[i].resize(batchsize);
		for (int j = 0; j < batchsize; j++)
		{
			correct[i][j] = MatrixN(10, 1, (double)0);
			correct[i][j].setRow(labels[(i + 1) * batchsize - (batchsize - j)], 0, 1);
		}
	}

	xnn::NeuralNetwork nn(784, { 28 }, 10, 3.2);
	//nn.loadFromFile("nn.xnn");

	xnn::NetworkTrainer::setNetwork(nn);

	float start = Utils::Logger::getTimePoint();

	float best = 0;

	std::thread thr(tf);

	int k = 0;
	while(1)
	{
		std::cout << "Epoch: " << k + 1 << std::endl;
		int count = 1;
		for (int i = 0; i < correct.size(); i++)
		{
			xnn::NetworkTrainer::feedForwardMBGD(imagesBatched[i], correct[i]);
			//xnn::NetworkTrainer::backpropogate(correct[i]);

			if (i*batchsize == count * images.size() / 6)
			{
				std::cout << count * images.size() / 6 << "/" << images.size() << "time: " << Utils::Logger::getTimePoint() - start << "s\r" << std::endl;
				count++;
			}
		}

		//auto rng = std::default_random_engine{};
		//std::shuffle(imagesBatched.begin(), imagesBatched.end(), rng);
		//std::shuffle(correct.begin(), correct.end(), rng);

		int counter = 0;
		for (int i = 0; i < testlabels.size(); i++)
		{
			xnn::NetworkTrainer::feedForwardSGD(testimages[i]);

			int max = -INFINITY;
			int index = 0;
			for (int j = 0; j < nn.getOutput().getRawData().size(); j++)
			{
				if (nn.getOutput()[j] > max)
				{
					max = nn.getOutput()[j];
					index = j;
				}
				//sum += nn.getOutput()[j];
				
				//if(k == 0)
				//	std::cout << nn.getOutput()[j] << ", " << "[" << j << ", " << labels[i] << "]" << std::endl;
			}

			if (index == testlabels[i])
				counter++;

			//if (sum < 1.1 && sum > 0.9 && f)
			//	counter++;
		}

		//std::cout << counter << "/" << testimages.size() << std::endl;
		//std::cout << "Accuracy: " << (float)counter * 100 / testimages.size() << "%" << std::endl;
		Utils::Logger::logMSG("[Epoch " + std::to_string(k + 1) + "]" + std::to_string(counter) + "/" + std::to_string(testimages.size())
			+ " Accuracy: " + std::to_string((float)counter * 100 / testimages.size()) + "%", "MNIST DIGITS", Utils::Severity::Debug);

		Utils::Logger::setLogFile("log.txt");

		if ((float)counter * 100 / testimages.size() > best)
			best = (float)counter * 100 / testimages.size();

		//nn.save("", "nn");
		k++;

		if(showHighest)
			Utils::Logger::logMSG("Highest accuracy: " + std::to_string(best) + "%", "", Utils::Severity::Info);
		if (breakLearning)
			break;
	}

	//std::cout << "Highest accuracy: " << best << "%" << std::endl;
	Utils::Logger::logMSG("Highest accuracy: " + std::to_string(best) + "%", "", Utils::Severity::Info);

	Utils::Logger::setLogFile("log.txt");

	std::cout << Utils::Logger::getTimePoint() - start << "s" << std::endl;

	thr.join();
	/*
	//int counter = 0;
	//
	//for (int i = 0; i < testlabels.size(); i++)
	//{
	//	xnn::NetworkTrainer::feedForward(testimages[i]);
	//
	//	int sum = 0;
	//	bool f = false;
	//	for (int k = 0; k < nn.getOutput().getRawData().size(); k++)
	//	{
	//		if (nn.getOutput()[k] > 0.9 && k == testlabels[i])
	//			f = true;
	//		sum += nn.getOutput()[k];
	//
	//		//std::cout << nn.getOutput()[k] << ", " << "[" << k << ", " << labels[i] << "]" << std::endl;
	//	}
	//
	//	if (sum < 1.1 && sum > 0.9 && f)
	//		counter++;
	//}
	//
	//std::cout << counter << "/" << testimages.size() << std::endl;
	//std::cout << "Accuracy: " << (float)counter * 100 / testimages.size() << "%" << std::endl;

	//float learningRate = 0.05;
	//
	//
	//MatrixN weight0(3, 2, true);
	//MatrixN weight1(1, 3, true);
	//MatrixN bias0(3, 1, true);
	//MatrixN bias1(1, 1, true);
	//
	//MatrixN a0;
	//
	//MatrixN output(1, 1, (double)0);
	//std::vector<MatrixN> dOutput(3);
	//dOutput[0] = MatrixN(1, 1, (double)1);
	//dOutput[1] = MatrixN(1, 1, (double)0);
	//dOutput[2] = MatrixN(1, 1, (double)0);
	//
	//MatrixN err1;
	//MatrixN err0;
	//
	//for(int k = 0; k < 10000; k++)
	//{
	//	for (int i = 0; i < input.size(); i++)
	//	{
	//		MatrixN tz0 = weight0 * input[i] + bias0;
	//		a0 = xnn::NetworkTrainer::getLeakyReLU(tz0);
	//		MatrixN tz1 = weight1 * a0 + bias1;
	//		output = xnn::NetworkTrainer::getLeakyReLU(tz1);
	//		
	//		err1 = (output - dOutput[i]).hadamard(xnn::NetworkTrainer::getLeakyReLUDyDx(tz1));
	//		weight1 = weight1 - (err1 * a0.transposed()) * learningRate;
	//		bias1 = bias1 - err1 * learningRate;
	//
	//		err0 = (weight1.transposed() * err1) * xnn::NetworkTrainer::getLeakyReLUDyDx(tz1);
	//		weight0 = weight0 - (err0 * input[i].transposed()) * learningRate;
	//		bias0 = bias0 - err0 * learningRate;
	//	}
	//}
	//
	//for (int i = 0; i < input.size(); i++)
	//{
	//	MatrixN tz0 = weight0 * input[i] + bias0;
	//	a0 = xnn::NetworkTrainer::getLeakyReLU(tz0);
	//	MatrixN tz1 = weight1 * a0 + bias1;
	//	output = xnn::NetworkTrainer::getLeakyReLU(tz1);
	//
	//	std::cout << "input: " << input[i] << std::endl;
	//	std::cout << "output from network: " << output << "correct output: " << dOutput[i] << std::endl;
	//}
	*/
}