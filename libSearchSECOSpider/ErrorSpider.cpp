/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "ErrorSpider.h"

const char* Error::getErrorMessage(ErrorType e)
{
	return errorToString[e];
}