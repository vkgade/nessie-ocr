///
/// @file
/// @brief Implementation of the class NessieException
///

#include "NessieException.h"


///
/// @details Initializes an NessieException object with a message
/// 
NessieException::NessieException (const std::string &what): what_(what)
{
	
};
	
		
///
/// @details Destroys a NessieException object
///
NessieException::~NessieException () throw()
{

};


///
/// @details This method overrides the one in class std::exception
/// 
const char* NessieException::what () const throw()
{
	return what_.data();
};
