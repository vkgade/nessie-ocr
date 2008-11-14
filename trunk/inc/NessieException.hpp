///
/// @file
/// @brief Declaration of NessieException class
///

#if !defined(_NESSIE_EXCEPTION_H)
#define _NESSIE_EXCEPTION_H


#include <string>
#include <exception>


///
/// Exception raised by a Nessie OCR object.
/// 
/// This class derives from std::exception class, so that all the exceptions either from this library or the STL itself
/// can be caught using a reference to an 'exception' object.
/// 
/// @author	Eliezer Talón (elitalon@gmail.com)
/// @date 2008-10-03
///
class NessieException : public std::exception
{
public:

	///
	/// Constructor.
	///
	/// @param what	A message that explains the exception raised.
	///
	/// @author Eliezer Talón (elitalon@gmail.com)
	/// @date 2008-10-03
	/// 
	NessieException (const std::string& what);


	///
	/// Destructor.
	///
	/// @author Eliezer Talón (elitalon@gmail.com)
	/// @date 2008-10-03
	///
	virtual ~NessieException () throw ();


	///
	/// Returns a message that explains the exception raised.
	/// 
	/// @return Message that explains the situation that caused the exception.
	///
	/// @author Eliezer Talón (elitalon@gmail.com)
	/// @date 2008-10-03
	///
	virtual const char* what () const throw();

private:

	std::string	what_;	///< Message that explains the exception raised
	
	
	///
	/// Private constructor.
	/// 
	/// @details This declaration is intended to avoid creating an empty NessieException object.
	/// 
	/// @warning Do not define this method.
	/// 
	/// @author Eliezer Talón (elitalon@gmail.com)
	/// @date 2008-11-14
	///
	NessieException ();
};

#endif  //_NESSIE_EXCEPTION_H
