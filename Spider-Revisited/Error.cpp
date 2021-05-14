#include "Error.h"
const char* Error::getErrorMessage(ErrorType e)
{
	return errorToString[e];
}