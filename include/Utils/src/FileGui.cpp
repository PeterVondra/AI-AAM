#include "../FileGui.h"

namespace exz
{
	namespace Utils
	{
		char* FileGui::getFileDialog(const char* p_FileType)
		{
			#ifdef _WIN32
			static char filename[MAX_PATH];

			std::string title = "Select " + std::string(p_FileType) + " file";

			std::string fileTypeName;
			fileTypeName.append(p_FileType);
			fileTypeName.append(" files");
			fileTypeName.push_back('\0');
			fileTypeName.append("*.");
			fileTypeName.append(p_FileType);
			fileTypeName.push_back('\0');

			OPENFILENAME ofn;
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = fileTypeName.c_str();
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = title.c_str();
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileName(&ofn))
				return filename;
			#endif
		}
		char* FileGui::getFileDialog(std::initializer_list<std::string> p_FileTypes)
		{
			#ifdef _WIN32
			static char filename[MAX_PATH];

			std::string title = "Select ";
			std::string fileTypeName;

			for (auto& str : p_FileTypes)
			{
				title += str;
			}

			title += " file";

			for (auto& str : p_FileTypes)
			{
				fileTypeName.append(str);
				fileTypeName.push_back('\0');
				fileTypeName.append("*.");
				fileTypeName.append(str);
				fileTypeName.push_back('\0');
			}

			OPENFILENAME ofn;
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = fileTypeName.c_str();
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = title.c_str();
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileName(&ofn))
				return filename;
			#endif
		}
		std::string FileGui::getFileDialog(std::initializer_list<std::string> p_FileTypes, std::string& p_Path)
		{
			#ifdef _WIN32
			static char filename[MAX_PATH];

			std::string title = "Select ";
			std::string fileTypeName;

			for (auto& str : p_FileTypes)
			{
				title += str;
			}

			title += " file";

			for (auto& str : p_FileTypes)
			{
				fileTypeName.append(str);
				fileTypeName.push_back('\0');
				fileTypeName.append("*.");
				fileTypeName.append(str);
				fileTypeName.push_back('\0');
			}

			OPENFILENAME ofn;
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = fileTypeName.c_str();
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = title.c_str();
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			
			std::string path;
			std::string file;
			int index = 0;

			if (GetOpenFileName(&ofn))
			{
				path = filename;
				index = path.find_last_of("\\");

				file = path.substr(index + 1, path.length() - 1);
				path = path.substr(0, index + 1);
				p_Path = path;
			}

			return file;
			#endif
		}
	}
}
