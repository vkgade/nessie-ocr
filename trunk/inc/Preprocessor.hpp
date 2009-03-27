/// @file
/// @brief Declaration of Preprocessor class

#if !defined(_PREPROCESSOR_H)
#define _PREPROCESSOR_H

#include "Clip.hpp"
#include "PreprocessorStatistics.hpp"
#include "Region.hpp"
#include <vector>
#include <list>
#include <utility>


/// @brief		Preprocessor of the OCR process.
///
/// @details	This class encapsulates all the algorithms related to the preprocessing stage of the OCR process. Its task is to extract the regions
///	of interest from a press clip, enhance them and clean the result by smoothing and noise removal. In the very end of the process, a list of
///	regions is available through the Preprocessor::regions() method.
///
///	@details	For an optimal preprocessing, it is strongly recommended that the following algorithms will be executed:
///	
///		-# Smoothing and noise removal by averaging filters
///		-# Global gray level thresholding
///		-# Smoothing and noise removal by template filters
///		-# Patterns extraction and isolation
///		-# Slant detection and correction
///
/// @see	Clip, PreprocessorStatistics, Region
///
/// @author Eliezer Talón (elitalon@gmail.com)
/// @date 2009-01-08
class Preprocessor
{
	public:

		/// @brief	Constructor.
		/// 
		/// @param	pressClip A press clip over which apply all the preprocessing algorithms.
		explicit Preprocessor (const Clip& pressClip);

		/// @brief	Get the regions found, if any, in the last segmentation process
		///
		/// @return A list of Region objects
		const std::list<Region>& regions () const;

		///	@brief	Get the statistics about the preprocessing stage.
		/// 
		/// @return A PreprocessorStatistics object with all the statistics gathered.
		const PreprocessorStatistics& statistics () const;

		/// @brief	Apply an averaging filter mask (linear filtering) over the press clip to remove noise and smooth character borders.
		///
		///	@pre	The press clip must be in grayscale mode.
		///
		///	@post	The noise in the image is removed and the borders of characters are blurred.
		void applyAveragingFilters ();
	
		/// @brief		Apply a global thresholding algorithm over the press clip.
		/// @details	The algorithm uses the Otsu's method to find automatically the optimal threshold for the press clip. Then, it compares each pixel
		/// gray level with that threshold and transforms the source clip into a binary image. As a result, the final histogram is bimodal. The algorithm
		/// also assumes that gray levels in the press clip above the threshold belong to the background, while gray levels below belong to the ink.
		///
		/// @post		Every pixel in the press clip belonging to the background has a gray level value of 0, while every pixel of ink has a gray level of 1.
		void applyGlobalThresholding ();

		///	@brief	Apply four 3x3 matching templates over the press clip to remove noise and smooth character borders.
		///
		///	@pre	The press clip must have been converted to binary mode, i.e. 0 for background pixels and 1 for ink pixels.
		///
		///	@post	The noise in the image is removed and the borders of characters are smoothed.
		void applyTemplateFilters ();

		/// @brief		Apply a segmentation process over the press clip to isolate every region of ink pixels.
		/// @details	This method isolates every region of ink pixels in a press clip following a region flooding algorithm. Starting from an arbitrary pixel of ink,
		///	it connects its neighbouring ink pixels and builds a set of regions. This set is then post-processed to join accents and other punctuation signs to their
		/// final region. A final list is generated by sorting the set of regions by lines and columns, so that traversing the list forward is equivalent to reading
		/// the text from left to right and from up to down.
		///
		///	@pre		The press clip must have been thresholded.
		///
		///@post		The internal members are set, so that a list of regions becomes available through Preprocessor::regions() method.
		void extractRegions ();

		///	@brief	Correct the slanting of every region extracted from press clip.
		///
		///	@pre	The internal list of regions must have been initialized by calling Preprocessor::extractRegions().
		///
		///	@post	The regions that present slanting are rotated as much as possible to be aligned with the Y-axis.
		void correctSlanting ();

		/// @brief	Scan the list of regions detecting the spaces between words.
		///
		/// @return An array of numbers, each one represents the position where a blank space must be inserted when building the text in further post-processing.
		std::vector<unsigned int> findSpacesBetweenWords ();

		/// @brief	Computes the average character height.
		/// 
		/// @return	The average character height in pixels.
		const double& averageCharacterHeight () const;

	private:

		/// @typedef	LineDelimiter.
		/// @brief		Internal representation of a pair of x-axis coordinates that delimits a row of regions as if they were characters in a text.
		/// @author		Eliezer Talón (elitalon@gmail.com)
		/// @date		2009-01-19
		typedef std::pair<unsigned int, unsigned int> LineDelimiter;

		Clip					clip_;						///< The press clip over which the preprocessing algorithms are applied.

		PreprocessorStatistics	statistics_;				///< Statistics about the execution of algorithms.

		std::list<Region>		regions_;					///< A list of regions that represents every set of ink pixels found in the press clip.
		
		double					averageCharacterHeight_;	///< Average character height.

		/// @brief		Find every pair of rows that delimits a line of regions as if they were characters in a text.
		///
		///	@param		visited		A vector of boolean values that tell which pixels have been visited during the regions extraction process.
		/// @param[out] delimiters	A list of LineDelimiter objects to store the result.
		void findLineDelimiters (const std::vector<bool>& visited, std::list<LineDelimiter>& delimiters) const;

		/// @brief	Merge every pair of regions that are vertically overlapped, assuming they are accents isolated from their characters.
		/// 
		/// @param	delimiters A list of text line delimiters.
		void mergeVerticallyOverlappedRegions (const std::list<LineDelimiter>& delimiters);

		// Do not implement these methods, as they are only declared here to prevent objects to be copied. 
		Preprocessor (const Preprocessor&);
		Preprocessor& operator= (const Preprocessor&);
};


inline const PreprocessorStatistics& Preprocessor::statistics () const
{
	return statistics_;
};

inline const std::list<Region>& Preprocessor::regions() const
{
	return regions_;
};

inline const double& Preprocessor::averageCharacterHeight () const
{
	return averageCharacterHeight_;
};

#endif

