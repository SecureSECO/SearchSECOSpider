/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <map>
#include <string>
enum class ErrorType
{
	GitCloneError,
	PipeOpenFailed,
	FileExtensionsNotFound,
};

class Error
{
private:
	inline static std::map<ErrorType, const char*> errorToString =
	{
			{ErrorType::GitCloneError, "Git cloning failed"},
			{ErrorType::PipeOpenFailed, "popen() failed."},
		{ErrorType::FileExtensionsNotFound, "File extensions file not found. Please re-add this file to your project. No files will be cloned."},
	};
public:
	/// <summary>
	/// Gets the error message for a given error.
	/// </summary>
	/// <param name="e">The error.</param>
	/// <returns>A specific message for the error.</returns>
	static const char* getErrorMessage(ErrorType e);

};