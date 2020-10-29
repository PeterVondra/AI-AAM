#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include "Logger.h"

namespace exz
{
	namespace Utils
	{
		class FileGui
		{
		public:

			static char* getFileDialog(const char* p_FileType);
			static char* getFileDialog(std::initializer_list<std::string> p_FileTypes);
			static std::string getFileDialog(std::initializer_list<std::string> p_FileTypes, std::string& p_Path);

		protected:
		private:

		};
	}
}

