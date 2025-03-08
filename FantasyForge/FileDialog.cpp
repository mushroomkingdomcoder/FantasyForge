#include "FileDialog.h"
#include <wrl.h>

#define FLDLGEXCPT(hr) FileDialog::Exception(__LINE__, __FILE__, hr)
#define FLDLGEXCPT_NOTE(hr, note) FileDialog::Exception(__LINE__, __FILE__, hr, note)
#define FLDLG_CHECK(hr) if (FAILED(hr)) { throw FLDLGEXCPT(hr); }

std::vector<std::wstring> FileDialog::OpenFiles(std::vector<FileType> allowed_files, bool multi_file, std::wstring title, std::wstring button, std::wstring label)
{
	using namespace Microsoft::WRL;
	std::vector<std::wstring> files = {};
	ComPtr<IFileOpenDialog> pDialog = nullptr;
	FLDLG_CHECK(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pDialog)));
	FILEOPENDIALOGOPTIONS options = 0;
	FLDLG_CHECK(pDialog->GetOptions(&options));
	options |= FOS_FILEMUSTEXIST;
	if (multi_file)
	{
		options |= FOS_ALLOWMULTISELECT;
	}
	FLDLG_CHECK(pDialog->SetOptions(options));
	FLDLG_CHECK(pDialog->SetTitle(title.c_str()));
	FLDLG_CHECK(pDialog->SetOkButtonLabel(button.c_str()));
	FLDLG_CHECK(pDialog->SetFileNameLabel(label.c_str()));
	std::vector<COMDLG_FILTERSPEC> filters;
	filters.resize(allowed_files.size());
	for (int i = 0; i < allowed_files.size(); ++i)
	{
		filters[i].pszName = allowed_files[i].displayName.c_str();
		filters[i].pszSpec = allowed_files[i].filterPattern.c_str();
	}
	FLDLG_CHECK(pDialog->SetFileTypes((UINT)allowed_files.size(), filters.data()));
	HRESULT hr = pDialog->Show(nullptr);
	if (hr != HRESULT_FROM_WIN32(ERROR_CANCELLED))
	{
		FLDLG_CHECK(hr);
		ComPtr<IShellItemArray> pResults = nullptr;
		FLDLG_CHECK(pDialog->GetResults(&pResults));
		DWORD nFiles = 0;
		pResults->GetCount(&nFiles);
		for (unsigned int i = 0; i < nFiles; ++i)
		{
			ComPtr<IShellItem> pResult = nullptr;
			FLDLG_CHECK(pResults->GetItemAt(i, &pResult));
			LPWSTR file = nullptr;
			FLDLG_CHECK(pResult->GetDisplayName(SIGDN_FILESYSPATH, &file));
			files.emplace_back(file);
			CoTaskMemFree(file);
		}
	}
	return files;
}

std::wstring FileDialog::SaveFile(std::vector<FileType> allowed_files, std::wstring title, std::wstring button, std::wstring label)
{
	using namespace Microsoft::WRL;
	LPWSTR file = nullptr;
	ComPtr<IFileSaveDialog> pDialog = nullptr;
	FLDLG_CHECK(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pDialog)));
	FLDLG_CHECK(pDialog->SetTitle(title.c_str()));
	FLDLG_CHECK(pDialog->SetOkButtonLabel(button.c_str()));
	FLDLG_CHECK(pDialog->SetFileNameLabel(label.c_str()));
	std::vector<COMDLG_FILTERSPEC> filters;
	filters.resize(allowed_files.size());
	for (int i = 0; i < allowed_files.size(); ++i)
	{
		filters[i].pszName = allowed_files[i].displayName.c_str();
		filters[i].pszSpec = allowed_files[i].filterPattern.c_str();
	}
	FLDLG_CHECK(pDialog->SetFileTypes((UINT)allowed_files.size(), filters.data()));
	HRESULT hr = pDialog->Show(nullptr);
	if (hr != HRESULT_FROM_WIN32(ERROR_CANCELLED))
	{
		FLDLG_CHECK(hr);
		ComPtr<IShellItem> pResult = nullptr;
		FLDLG_CHECK(pDialog->GetResult(&pResult));
		pResult->GetDisplayName(SIGDN_FILESYSPATH, &file);
	}
	return file;
}

std::wstring FileDialog::GetFolder(std::wstring title, std::wstring button, std::wstring label)
{
	using namespace Microsoft::WRL;
	LPWSTR folder = nullptr;
	ComPtr<IFileOpenDialog> pDialog;
	FLDLG_CHECK(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pDialog)));
	FILEOPENDIALOGOPTIONS options = 0;
	FLDLG_CHECK(pDialog->GetOptions(&options));
	options |= FOS_PICKFOLDERS;
	FLDLG_CHECK(pDialog->SetOptions(options));
	FLDLG_CHECK(pDialog->SetTitle(title.c_str()));
	FLDLG_CHECK(pDialog->SetOkButtonLabel(button.c_str()));
	FLDLG_CHECK(pDialog->SetFileNameLabel(label.c_str()));
	HRESULT hr = pDialog->Show(nullptr);
	if (hr != HRESULT_FROM_WIN32(ERROR_CANCELLED))
	{
		FLDLG_CHECK(hr);
		ComPtr<IShellItem> pResult = nullptr;
		FLDLG_CHECK(pDialog->GetResult(&pResult));
		pResult->GetDisplayName(SIGDN_FILESYSPATH, &folder);
	}
	return folder;
}

std::string FileDialog::ConvertWString(const std::wstring& wstring)
{
	std::string result = {};
	result.resize(wstring.size(), ' ');
	for (int i = 0; i < wstring.size(); ++i)
	{
		result[i] = (char)wstring[i];
	}
	return result;
}