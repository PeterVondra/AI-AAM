#pragma once

namespace xnn
{
	class MNISTDataLoader
	{
		public:
			template<typename T> static void loadImages(std::string p_Path, int& p_ImageCount, Vector2i& p_ImageSize, std::vector<std::vector<T>>& p_Images)
			{
				std::ifstream file(p_Path, std::ios::binary);

				if (file.is_open())
				{
					int p_MagicNumber = 0;

					std::cout << "found file: " << p_Path << std::endl;

					file.read((char*)&p_MagicNumber, sizeof(p_MagicNumber));
					file.read((char*)&p_ImageCount, sizeof(p_ImageCount));
					file.read((char*)&p_ImageSize, sizeof(p_ImageSize));

					p_MagicNumber = reverseInt(p_MagicNumber);
					p_ImageCount = reverseInt(p_ImageCount);
					p_ImageSize = reverseInt(p_ImageSize);

					p_Images.resize(p_ImageCount);

					std::cout << "Magic number: " << p_MagicNumber << std::endl;
					std::cout << "Image count: " << p_ImageCount << std::endl;
					std::cout << "Image size: " << p_ImageSize << std::endl;

					float start = Utils::Logger::getTimePoint();

					for (int k = 0; k < p_ImageCount; k++)
					{
						for (int i = 0; i < p_ImageSize.x; i++)
						{
							for (int j = 0; j < p_ImageSize.y; j++)
							{
								unsigned char t = 0;
								file.read((char*)&t, sizeof(t));
								p_Images[k].push_back((int)t);
							}
						}
					}

					std::cout << "Execution time: " << Utils::Logger::getTimePoint() - start << std::endl;
				}
			}
			static void loadImages(std::string p_Path, std::vector<MatrixN>& p_Images)
			{
				std::ifstream file(p_Path, std::ios::binary);

				if (file.is_open())
				{
					int p_MagicNumber = 0;

					std::cout << "found file: " << p_Path << std::endl;

					int p_ImageCount;
					Vector2i p_ImageSize;

					file.read((char*)&p_MagicNumber, sizeof(p_MagicNumber));
					file.read((char*)&p_ImageCount, sizeof(p_ImageCount));
					file.read((char*)&p_ImageSize, sizeof(p_ImageSize));

					p_MagicNumber = reverseInt(p_MagicNumber);
					p_ImageCount = reverseInt(p_ImageCount);
					p_ImageSize = reverseInt(p_ImageSize);

					p_Images.resize(p_ImageCount);

					std::cout << "Magic number: " << p_MagicNumber << std::endl;
					std::cout << "Image count: " << p_ImageCount << std::endl;
					std::cout << "Image size: " << p_ImageSize << std::endl;

					float start = Utils::Logger::getTimePoint();

					for (int k = 0; k < p_ImageCount; k++)
					{
						p_Images[k] = MatrixN(p_ImageSize.x * p_ImageSize.y, 1, (double)0);

						for (int i = 0; i < p_ImageSize.x; i++)
						{
							for (int j = 0; j < p_ImageSize.y; j++)
							{
								unsigned char t = 0;
								file.read((char*)&t, sizeof(t));
								p_Images[k][(i + 1) * p_ImageSize.x - (p_ImageSize.x - j)] = (float)t / 256;
							}
						}
					}

					std::cout << "Execution time: " << Utils::Logger::getTimePoint() - start << std::endl;
				}
			}
			static void loadImages(std::string p_Path, int& p_ImageCount, Vector2i& p_ImageSize)
			{
				std::ifstream file(p_Path, std::ios::binary);

				if (file.is_open())
				{
					int p_MagicNumber = 0;

					std::cout << "found file: " << p_Path << std::endl;

					file.read((char*)&p_MagicNumber, sizeof(p_MagicNumber));
					file.read((char*)&p_ImageCount, sizeof(p_ImageCount));
					file.read((char*)&p_ImageSize, sizeof(p_ImageSize));

					p_MagicNumber = reverseInt(p_MagicNumber);
					p_ImageCount = reverseInt(p_ImageCount);
					p_ImageSize = reverseInt(p_ImageSize);

				}
			}
			template<typename T> static void loadLabels(std::string p_Path, std::vector<T>& p_Labels)
			{
				std::ifstream file(p_Path, std::ios::binary);

				if (file.is_open())
				{
					int p_MagicNumber = 0;
					int p_LabelCount = 0;

					std::cout << "found file: " << p_Path << std::endl;

					file.read((char*)&p_MagicNumber, sizeof(p_MagicNumber));
					file.read((char*)&p_LabelCount, sizeof(p_LabelCount));

					p_MagicNumber = reverseInt(p_MagicNumber);
					p_LabelCount = reverseInt(p_LabelCount);

					std::cout << "Magic number: " << p_MagicNumber << std::endl;
					std::cout << "Label count: " << p_LabelCount << std::endl;

					float start = Utils::Logger::getTimePoint();

					p_Labels.resize(p_LabelCount);

					for (int k = 0; k < p_LabelCount; k++)
					{
						unsigned char t = 0;
						file.read((char*)&t, 1);
						p_Labels[k] = (int)t;
					}

					std::cout << "Execution time: " << Utils::Logger::getTimePoint() - start << std::endl;
				}
				else
					std::cout << "file not found: " << p_Path << std::endl;
			}
		private:
			static int reverseInt(int i)
			{
				unsigned char ch1, ch2, ch3, ch4;
				ch1 = i & 255;
				ch2 = (i >> 8) & 255;
				ch3 = (i >> 16) & 255;
				ch4 = (i >> 24) & 255;
				return((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
			}
			static Vector2i reverseInt(Vector2i i)
			{
				return Vector2i(reverseInt(i.x), reverseInt(i.y));
			}
	};
}