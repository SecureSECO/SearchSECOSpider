#pragma once
#include <map>
#include <string>
enum class ErrorType
{
	GitError,
	GitCloneError,
	PipeOpenFailed,
	BlameIncorrectFormat,
	FileExtensionsNotFound,
};

class Error
{
private:
	inline static std::map<ErrorType, const char*> errorToString =
	{
			{ErrorType::GitError, "A git error occured"},
			{ErrorType::GitCloneError, "Git cloning failed"},
			{ErrorType::PipeOpenFailed, "popen() failed."},
			{ErrorType::BlameIncorrectFormat, "Blame data has incorrect format."},
		{ErrorType::FileExtensionsNotFound, "File extensions file not found. Please re-add this file to your project. No files will be cloned."},
	};
public:
	static const char* getErrorMessage(ErrorType e);

};