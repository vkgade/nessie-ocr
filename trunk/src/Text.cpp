/// @file
/// @brief Definition of Text class

#include "Text.hpp"
#include "NessieException.hpp"
#include <boost/tokenizer.hpp>


Text::Text ()
:	data_(""),
	size_(0),
	averageCharacterHeight_(0)
{};


void Text::assign (const std::string& data)
{
	data_ = data;
	size_ = 0;
	
	for ( std::string::iterator i = data_.begin(); i != data_.end(); ++i )
	{
		if ( static_cast<int>(*i) < 0 )	// Wide character
			++i;
			
		++size_;
	}
};


const std::string Text::at(const unsigned int& n) const
{
	std::string tmp("");

	if( !(n > size_) )
	{
		unsigned int i = 0;
		for ( unsigned int j = 0; j < n; ++j )
		{
			if ( static_cast<int>(data_.at(i)) < 0 )	// Wide character
				i += 2;
			else
				++i;
		}

		if ( static_cast<int>(data_.at(i)) < 0 )	// Wide character
			tmp = data_.substr (i, 2);
		else
			tmp = data_.at(i);
	}
	
	return tmp;
};


void Text::append (const std::string& character)
{
	if ( character.size() > 2 || (character.size() == 2 && character.at(0) > 0) )
		throw NessieException("Text::addCharacter() : The string passed cannot contain more than one character.");
	
	data_.append(character);
	size_++;
}


void Text::insert (const std::string& character, const unsigned int& n)
{
	if ( character.size() > 2 || (character.size() == 2 && character.at(0) >= 0) )
		throw NessieException("Text::addCharacter() : The string passed cannot contain more than one character.");

	if( !(n > size_) )
	{
		if ( n == 0 )
			data_.insert(0, character);
		else
		{
			unsigned int i = 0;
			for ( unsigned int j = 0; j < n; ++j )
			{
				if ( static_cast<int>(data_.at(i)) < 0 )	// Wide character
					i += 2;
				else
					++i;
			}

			std::string tmp;
			tmp.append(data_.begin(), data_.begin() + i);
			tmp.append(character);
			tmp.append(data_.begin() + i, data_.end());

			data_ = tmp;
		}
		size_++;
	}
};


void Text::erase (const unsigned int& n)
{
	if ( !(n > size_) )
	{
		unsigned int i = 0;
		for ( unsigned int j = 0; j < n; ++j )
		{
			if ( static_cast<int>(data_.at(i)) < 0 )	// Wide character
				i += 2;
			else
				++i;
		}
		
		std::string tmp;
		tmp.append(data_.begin(), data_.begin() + i);

		if ( *(data_.begin() + i + 1) < 0 )	// Wide character
			tmp.append(data_.begin() + i + 2, data_.end());
		else
			tmp.append(data_.begin() + i + 1, data_.end());

		data_ = tmp;
		size_--;
	}
};


unsigned int Text::nWords () const
{
	unsigned int n = 0;

	boost::char_separator<char> delimiters("+*/= ,:¡!.;()¿?\"'[]{}<>\\|");
	boost::tokenizer< boost::char_separator<char> > tokenizer(data_, delimiters);

	for( boost::tokenizer< boost::char_separator<char> >::iterator i = tokenizer.begin(); i != tokenizer.end(); ++i)
		++n;

	return n;
};


double Text::averageWordSize () const
{
	double wordSize	= 0.0;
	unsigned int n	= 0;

	boost::char_separator<char> delimiters("+*/= ,:¡!.;()¿?\"'[]{}<>\\|");
	boost::tokenizer< boost::char_separator<char> > tokenizer(data_, delimiters);

	for( boost::tokenizer< boost::char_separator<char> >::iterator i = tokenizer.begin(); i != tokenizer.end(); ++i)
	{
		++n;
		wordSize += i->size();
	}

	if ( wordSize > 0.0 )
		return wordSize / n;
	else
		return wordSize;
};

