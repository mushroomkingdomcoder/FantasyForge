#pragma once
#include "BaseException.h"
#include <ShObjIdl.h>
#include <string>
#include <vector>

class FileDialog
{
public:
	class Exception : public BaseException
	{
	private:
		HRESULT hr;
	public:
		Exception() = delete;
		Exception(int line, std::string file, HRESULT hr, std::string note = "") noexcept
			:
			BaseException(line, file, note),
			hr(hr)
		{}
		const char* what() const noexcept override
		{
			std::ostringstream wht;
			wht << "Exception Type: " << GetType() << std::endl
				<< "Error Code: " << hr << std::endl
				<< "Error Description: " << GetErrorCodeDesc(hr) << std::endl
				<< "File Name: " << GetFile() << std::endl
				<< "Line Number: " << GetLine() << std::endl
				<< "Additional Info: " << GetNote() << std::endl;
			whatBuffer = wht.str();
			return whatBuffer.c_str();
		}
		const char* GetType() const noexcept override
		{
			return "FantasyForge FileDialog Exception";
		}
	};
	struct FileType
	{
		std::wstring displayName;
		std::wstring filterPattern;
	};
public:
	static std::vector<std::wstring> OpenFiles(
		std::vector<FileType> allowed_files = { { L"All Files",L"*.*" } },
		bool multi_file = false,
		std::wstring title = L"Open File",
		std::wstring button = L"Open",
		std::wstring label = L"File name:");

	static std::wstring SaveFile(
		std::vector<FileType> allowed_files = { { L"All Files",L"*.*" } },
		std::wstring title = L"Save File", 
		std::wstring button = L"Save",
		std::wstring label = L"File name:");

	static std::wstring GetFolder(
		std::wstring title = L"Select Folder",
		std::wstring button = L"Select",
		std::wstring label = L"Folder name:");

	static std::string ConvertWString(const std::wstring& wstring);
};

