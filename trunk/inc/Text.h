#if !defined(_TEXT_H)
#define _TEXT_H

#include "WordRate.h"
#include "FontProportionality.h"
#include "Style.h"

#include <string>
#include <vector>
#include <algorithm>
using namespace std;


///
/// Text extracted from the clip by the recognizer
/// 
/// This class stores the text que has been extracted from the press-clip
/// during the recognition process. Besides the text itself, it also keeps
/// some extra information: appearance rate of every word, font style of every
/// character and the font proportionality, among others.
/// 
/// @author	Eliezer Talón (elitalon@gmail.com)
/// @date 2008-09-22
///
class Text
{
	public:
		
		///
		/// Constructor
		///
		Text ();
		
		///
		/// Constructor
		///
		Text (string content_);
		
		///
		/// Constructor
		///
		Text (string content_, vector<Style> styles_);
		
		///
		/// Destructor
		///
		~Text ();
		
		///
		/// Adds a character to the text
		///
		void addCharacter (char character);
		
		///
		/// Adds a character to the text
		///
		void addCharacter (char character, Style style_);
		
		///
		/// Adds a character to the text
		///
		void addCharacter (char character, unsigned int position_);
		
		///
		/// Adds a character to the text
		///
		void addCharacter (char character, Style style_, unsigned int position_);

		///
		/// Removes a character from the text
		///
		void removeCharacter (unsigned int position_);
		
		///
		/// Returns the style of a single character
		///
		Style getCharacterStyle (unsigned int position_) const;

		///
		/// Sets the style of a single character
		///
		void setCharacterStyle (unsigned int position_, Style style_);
				
		///
		/// Returns the text itself
		///
		string getContent () const;

		///
		/// Sets the text
		///
		void setContent (string content_);
		
		///
		/// Returns the list of text styles
		///
		vector<Style> getStyles () const;

		///
		/// Sets the list of text styles
		///
		void setStyles (vector<Style> styles_);
		
		///
		/// Returns the text length
		///
		unsigned int getLength () const;
		
		///
		/// Returns the appearance rates of every single word in text
		///
		vector<WordRate> getWordRates () const;
		
		///
		/// Returns the font proportionality in text
		///
		FontProportionality getProportionality () const;

		///
		/// Sets the font proportionality in text
		///
		void setProportionality (FontProportionality proportionality_);


	private:
		
		///
		/// The text itself
		///		
		string content;
		
		///
		/// A list of every character style
		///
		vector<Style> styles;
		
		///
		/// The text proportionality
		///
		FontProportionality proportionality;
		
		///
		/// A list of appearance rates of every single word in text
		///
		vector<WordRate> wordRates;
		
		///
		/// Builds the vector with every word appearance rate
		///
		void computeWordRates ();
		
		///
		/// Increases by one the number of appearances of a word
		///
		void updateWordRate (string word_);
		
		///
		/// Extracts the words surrounded by default delimiters from text
		///
		void tokenize(vector<string>& tokens_, const string& delimiters_ = " ,.\n\t:;!¡¿?&/()=") const;
};

#endif  //_TEXT_H