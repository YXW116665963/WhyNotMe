#include "file_util.h"
#include "text_util.h"
#include "logger.h"

#include <fstream>
#include <filesystem>
#include <algorithm>
#include <vector>

namespace why
{
	std::filesystem::path UTF8ToPath(const std::string& strUTF8)
	{
#ifdef WIN32
		std::wstring	strFileName = UTF8Decode(strUTF8);

		return std::filesystem::path(strFileName);
#else
		return std::filesystem::path(strUTF8);
#endif
	}
	
	std::string PathToUTF8(const std::filesystem::path& filePath)
	{
#ifdef WIN32
		std::wstring	strFileName = filePath.wstring();

		return UTF8Encode(strFileName);
#else
		return filePath.u8string();
#endif
	}

	std::string UTF8ToLocal(const std::string& strUTF8)
	{
#ifdef WIN32
		return Unicode2Ascii(UTF8Decode(strUTF8));
#else
		return strUTF8;
#endif
	}

	std::string LocalToUTF8(const std::string& strLocal)
	{
#ifdef WIN32
		return UTF8Encode(Ascii2Unicode(strLocal));
#else
		return strLocal;
#endif
	}

	bool CreateDirectories(const std::string& dir)
	{
		std::filesystem::path			dirPath = UTF8ToPath(dir);
		std::error_code					errorCode;

		if (std::filesystem::create_directories(dirPath, errorCode) || errorCode.value() == 0)
			return true;


		LOG_INFO << "CreateDirectories failed! errorCode:" << errorCode.value();
		return false;
	}

	bool IsFolder(const std::string& dir)
	{
		std::filesystem::path			dirPath = UTF8ToPath(dir);

		return std::filesystem::is_directory(dirPath);
	}

	bool IsFile(const std::string& filePath)
	{
		std::filesystem::path			path = UTF8ToPath(filePath);

		return std::filesystem::is_regular_file(path);
	}

	bool LoadTextFile(const std::string& strFileName, std::vector<std::string>& lineList)
	{
		std::ifstream			textFile;
		std::wstring			localFileName = UTF8Decode(strFileName);

		textFile.open(localFileName);
		if (!textFile.is_open())
			return false;

		std::string					strLine;

		lineList.clear();
		while (!textFile.eof())
		{
			std::getline(textFile, strLine, '\n');
			if (!strLine.empty())
				lineList.push_back(strLine);
		}

		return true;
	}

	bool LoadTextFileAllLine(const std::string& strFileName, std::vector<std::string>& lineList)
	{
		std::ifstream			textFile;
		std::wstring			localFileName = UTF8Decode(strFileName);

		textFile.open(localFileName);
		if (!textFile.is_open())
			return false;

		std::string					strLine;

		lineList.clear();
		while (!textFile.eof())
		{
			std::getline(textFile, strLine, '\n');
			lineList.push_back(strLine);
		}

		return true;
	}

	bool SaveTextTile(const std::string& strFileName, const std::vector<std::string>& lineList)
	{
		std::wstring strLocalFileName = UTF8Decode(strFileName);
		std::wstring strFileNameBak = strLocalFileName + std::wstring(L".bak");
		
		if (std::filesystem::exists(strFileNameBak))
		{
			try
			{
				std::filesystem::remove(strFileNameBak);
			}
			catch (const std::filesystem::filesystem_error& /*e*/)
			{
				return false;
			}
		}
		try
		{
			std::filesystem::create_directories(std::filesystem::path(strFileNameBak).parent_path());
			std::ofstream txtFile(strFileNameBak);
			if (!txtFile.is_open()) {
				return false;
			}
			std::string									strBuffer;
	
			for (auto lineInfo : lineList)
			{
				strBuffer += lineInfo;
				strBuffer += "\n";
			}
			txtFile << strBuffer;
			txtFile.close();

			if (std::filesystem::exists(strLocalFileName))
				std::filesystem::remove(strLocalFileName);

			std::filesystem::rename(strFileNameBak, strLocalFileName);
		}
		catch (const std::filesystem::filesystem_error& /*e*/)
		{
			return false;
		}
		return true;
	}

	bool LoadTextFile(const std::string &strFileName, std::list<std::string>& lineList)
	{
		std::wstring			strLocalFileName = UTF8Decode(strFileName);
		std::ifstream			textFile;

		textFile.open(strLocalFileName);
		if (!textFile.is_open())
			return false;

		std::string					strLine;

		lineList.clear();
		while (!textFile.eof())
		{
			std::getline(textFile, strLine, '\n');
			if (!strLine.empty())
				lineList.push_back(strLine);
		}

		return true;
	}

	bool SaveTextTile(const std::string& strFileName, const std::list<std::string>& lineList)
	{
		std::wstring strLocalFileName = UTF8Decode(strFileName);
		std::wstring strFileNameBak = strLocalFileName + std::wstring(L".bak");

		if (std::filesystem::exists(strFileNameBak))
		{
			try
			{
				std::filesystem::remove(strFileNameBak);
			}
			catch (const std::filesystem::filesystem_error& /*e*/)
			{
				return false;
			}
		}
		try
		{
			std::filesystem::create_directories(std::filesystem::path(strFileNameBak).parent_path());
			std::ofstream txtFile(strFileNameBak);
			if (!txtFile.is_open()) {
				return false;
			}
			std::string									strBuffer;

			for (auto lineInfo : lineList)
			{
				strBuffer += lineInfo;
				strBuffer += "\n";
			}
			txtFile << strBuffer;
			txtFile.close();

			if (std::filesystem::exists(strLocalFileName))
				std::filesystem::remove(strLocalFileName);

			std::filesystem::rename(strFileNameBak, strLocalFileName);
		}
		catch (const std::filesystem::filesystem_error& /*e*/)
		{
			return false;
		}
		return true;
	}

	bool LoadCSVFile(const std::string& strFileName, std::vector<std::vector<std::string>>& dataList)
	{
		std::vector<std::string>	lineList;

		if (!LoadTextFile(strFileName, lineList))
			return false;

		std::for_each(lineList.begin(), lineList.end(), [&dataList](const std::string& strLine) {
			std::vector<std::string>		lineInfo;

			StringSplit(strLine, ',', lineInfo);
			dataList.push_back(std::move(lineInfo));
		});

		return true;
	}

	bool WriteCSVFile(std::vector<std::vector<std::string>>& dataList, const std::string& strFileName)
	{
		std::wstring					strLocalFileName = UTF8Decode(strFileName);
		std::wstring					strFileNameBak = strLocalFileName + std::wstring(L".bak");
		
		if (std::filesystem::exists(strFileNameBak))
		{
			try
			{
				std::filesystem::remove(strFileNameBak);
			}
			catch (const std::filesystem::filesystem_error& /*e*/)
			{
				return false;
			}
		}
		try
		{
			std::filesystem::create_directories(std::filesystem::path(strFileNameBak).parent_path());
			std::ofstream txtFile(strFileNameBak);
			if (!txtFile.is_open()) {
				return false;
			}
			std::string									strBuffer;
			uint32_t									uLineCount{ 0 };
			uint32_t									uItemCount{ 0 };
			uLineCount = (uint32_t)dataList.size();
			for (uint32_t i = 0; i < uLineCount; i++)
			{
				std::vector<std::string>&	lineInfo = dataList[i];
				uItemCount = (uint32_t)lineInfo.size();
				for (uint32_t j = 0; j < uItemCount; j++)
				{
					strBuffer.append(lineInfo[j].find(',') != std::string::npos ? "\"" + lineInfo[j] + "\"" : lineInfo[j]);
					strBuffer.append((j < uItemCount - 1 ? "," : "\n"));
				}
			}
			txtFile << strBuffer;
			txtFile.close();

			if (std::filesystem::exists(strLocalFileName))
				std::filesystem::remove(strLocalFileName);

			std::filesystem::rename(strFileNameBak, strLocalFileName);
		}
		catch (const std::filesystem::filesystem_error& /*e*/)
		{
			return false;
		}
		return true;
	}

	bool WriteCSVFile(std::list<std::vector<std::string>>& dataList, const std::string& strFileName)
	{
		std::wstring					strLocalFileName = UTF8Decode(strFileName);
		std::wstring					strFileNameBak = strLocalFileName + std::wstring(L".bak");
		
		if (std::filesystem::exists(strFileNameBak))
		{
			try
			{
				std::filesystem::remove(strFileNameBak);
			}
			catch (const std::filesystem::filesystem_error& /*e*/)
			{
				return false;
			}
		}
		try
		{
			std::filesystem::create_directories(std::filesystem::path(strFileNameBak).parent_path());
			std::ofstream txtFile(strFileNameBak);
			if (!txtFile.is_open()) {
				return false;
			}
			std::string									strBuffer;
			uint32_t									uItemCount{ 0 };

			for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
			{
				std::vector<std::string>&	lineInfo = *iter;
				uItemCount = (uint32_t)lineInfo.size();
				for (uint32_t j = 0; j < uItemCount; j++)
				{
					strBuffer.append(lineInfo[j].find(',') != std::string::npos ? "\"" + lineInfo[j] + "\"" : lineInfo[j]);
					strBuffer.append((j < uItemCount - 1 ? "," : "\n"));
				}
			}

			txtFile << strBuffer;
			txtFile.close();

			if (std::filesystem::exists(strLocalFileName))
				std::filesystem::remove(strLocalFileName);

			std::filesystem::rename(strFileNameBak, strLocalFileName);
		}
		catch (const std::filesystem::filesystem_error& /*e*/)
		{
			return false;
		}
		return true;
	}

	bool WriteCSVFile(std::list<std::string>& dataList, const std::string& strFileName)
	{
		std::wstring					strLocalFileName = UTF8Decode(strFileName);
		std::wstring					strFileNameBak = strLocalFileName + std::wstring(L".bak");
		
		if (std::filesystem::exists(strFileNameBak))
		{
			try
			{
				std::filesystem::remove(strFileNameBak);
			}
			catch (const std::filesystem::filesystem_error& /*e*/)
			{
				return false;
			}
		}
		try
		{
			std::filesystem::create_directories(std::filesystem::path(strFileNameBak).parent_path());
			std::ofstream txtFile(strFileNameBak);
			if (!txtFile.is_open()) {
				return false;
			}
			std::string									strBuffer;

			for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
			{
				strBuffer.append(*iter + "\n");
			}

			txtFile << strBuffer;
			txtFile.close();

			if (std::filesystem::exists(strLocalFileName))
				std::filesystem::remove(strLocalFileName);

			std::filesystem::rename(strFileNameBak, strLocalFileName);
		}
		catch (const std::filesystem::filesystem_error& /*e*/)
		{
			return false;
		}
		return true;
	}

	bool GetSubFolders(const std::string &dir, std::map<std::string, std::list<std::pair<std::string, std::string>>> &subs)
	{
		if (!IsFolder(dir))
		{
			LOG_ERROR << "dir:" << dir << ", is not folder";
			return false;
		}

		std::list<std::pair<std::string, std::string>> list;
		std::filesystem::path dirPath = UTF8ToPath(dir);
		for (const auto& entry : std::filesystem::directory_iterator(dirPath))
		{
			if (entry.is_directory())
			{
				std::string name = PathToUTF8(entry.path().filename().string());
				std::string path = PathToUTF8(entry.path().string()) + "\\";
				list.push_back(std::pair(name, path));
				GetSubFolders(path, subs);
			}
		}

		if (!list.empty())
		{
			subs.emplace(dir, std::move(list));
		}

		return true;
	}

	bool GetOneSubFolders(const std::string &dir, std::list<std::pair<std::string, std::string>> &subs)
	{
		subs.clear();

		if (!IsFolder(dir))
		{
			LOG_ERROR << "dir:" << dir << ", is not folder";
			return false;
		}

		std::filesystem::path dirPath = UTF8ToPath(dir);
		for (const auto& entry : std::filesystem::directory_iterator(dirPath))
		{
			if (entry.is_directory())
			{
				std::string name = entry.path().filename().string();
				std::string path = entry.path().string() + "\\";
				subs.push_back(std::pair(name, path));
			}
		}

		return true;
	}

	bool GetOneSubFiles(const std::string &dir, const std::string &ext, std::list<std::pair<std::string, std::string>> &subs)
	{
		subs.clear();

		if (!IsFolder(dir))
		{
			LOG_ERROR << "dir:" << dir << ", is not folder";
			return false;
		}

		std::filesystem::path dirPath = UTF8ToPath(dir);
		for (const auto& entry : std::filesystem::directory_iterator(dirPath))
		{
			if (entry.is_regular_file())
			{
				std::string name = PathToUTF8(entry.path().filename().string());
				std::string path = PathToUTF8(entry.path().string());
				std::string extension = PathToUTF8(entry.path().extension().string());
				extension = Lowercase(extension);

				auto ftime = entry.last_write_time();
				auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
					ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
				std::time_t tt = std::chrono::system_clock::to_time_t(sctp);


				if (ext.empty() || ext == ".*" || ext == extension)
				{
					if (entry.path().has_extension())
					{
						subs.push_back(std::pair(name.substr(0, name.find_last_of(".")), std::to_string(tt)));
					}
					else
					{
						subs.push_back(std::pair(name, std::to_string(tt)));
					}
				}
			}
		}

		return true;
	}

	std::string GetFolderName(const std::string &dir)
	{
		std::string		strFolderName = dir;
		size_t			index = strFolderName.find_last_of(std::filesystem::path::preferred_separator);
		
		if (index == std::string::npos) 
			return strFolderName;

		if (index == (strFolderName.size() - 1))
		{
			strFolderName.erase(index);
			index = strFolderName.find_last_of(std::filesystem::path::preferred_separator);
			if (index == std::string::npos)
				return strFolderName;
		}

		return strFolderName.substr(index + 1);
	}

	std::string GetFileName(const std::string &path)
	{
		std::string name = GetFolderName(path);
		size_t index = name.find_last_of('.');
		if (index == std::string::npos) 
		{
			return name;
		}
		return name.substr(0, index);
	}

	std::string GetDirectory(const std::string &path)
	{
		std::filesystem::path filePath = UTF8ToPath(path);
		return PathToUTF8(filePath.parent_path());
	}

	std::string GetFileNameWithoutExtension(const std::string & path)
	{
		std::string		name = path;
		size_t			dotPos = name.find_last_of('.');
		if (dotPos != std::string::npos)
		{
			name.erase(dotPos);
		}
		return name;
	}

	int32_t DeleteFileEx(const std::wstring& path)
	{
		BOOL res = ::DeleteFileW(path.c_str());
		if (res > 0)
		{
			return NO_ERROR;
		}
		else 
		{
			return GetLastError();
		}
	}

	bool CopyFile(const std::string &src, const std::string &dst)
	{
		std::ifstream src_source(UTF8Decode(src), std::ios::binary);
		std::ofstream dst_source(UTF8Decode(dst), std::ios::binary);

		if (!src_source.is_open() || !dst_source.is_open())
		{
			LOG_ERROR << "can not open src:" << src << ", dst:" << dst;
			return false;
		}

		dst_source << src_source.rdbuf();
		return true;
	}

	bool IsExistFileInDir(const std::string &dir, const std::string &name)
	{
		std::string name_lower = name;
		std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), [](char c) { return (char)tolower(c); });

		for (const auto &entry : std::filesystem::directory_iterator(UTF8ToPath(dir)))
		{
			if (!entry.is_regular_file()) {
				continue;
			}

			std::string entry_name = PathToUTF8(entry.path().filename());
			std::transform(entry_name.begin(), entry_name.end(), entry_name.begin(), [](char c) { return (char)tolower(c); });

			if (entry_name == name_lower) {
				return true;
			}
		}

		return false;
	}
}