/// @file
/// @brief Declaration of Region class

#if !defined(_REGION_H)
#define _REGION_H


#include <vector>
#include <utility>


/// @typedef	PixelCoordinates.
/// @brief		Internal representation of pixel coordinates in a 2D coordinates system.
/// @author		Eliezer Talón (elitalon@gmail.com)
/// @date		2009-01-19
typedef std::pair<unsigned int, unsigned int> PixelCoordinates;



///	A set of ink pixel in a press clip that defines a region to be further defined as a pattern for recognition.
/// 
/// This class stores a set of ink pixels to identify a region that will be used in a further preprocessing stage to build a pattern
/// for feature extraction and classification. Every region has four representative pixels of ink, located on each extreme: one pixel at the top,
///	one pixel at the bottom, one pixel on the left and one pixel on the right.
/// 
/// @author Eliezer Talón (elitalon@gmail.com)
/// @date 2009-01-19
///
class Region
{
public:
	
	/// Constructor.
	Region();
	
	
	/// Allows read-and-write access to a pixel in the region.
	///
	/// @param index	The position of a pair of coordinates within the region.
	/// 
	/// @return A reference to a PixelCoordinates object that holds the coordinates of a pixel at given index.
	PixelCoordinates& operator() (const unsigned int& index);
	
	
	/// Allows read-only access to a pixel in the region.
	///
	/// @param index	The position of a pair of coordinates within the region.
	/// 
	/// @return A PixelCoordinates object that holds the coordinates of a pixel at given index.
	PixelCoordinates operator() (const unsigned int& index) const;
	
	
	/// Appends a region to a current one.
	/// 
	/// @param region	Region to be appended.
	/// 
	/// @return A Region object as a result of appending the pixels of both regions.
	Region operator+ (const Region& region) const;
	
	
	/// Evaluates if a region's position is less than another, according to their position in text when reading from up to down and left to right.
	/// 
	/// @param region	Region of reference.
	/// 
	/// @return True if the current region is placed before than <em>region</em> in a press clip, false otherwise.
	bool operator< (const Region& region) const;
	
	
	/// Returns the height of the region.
	/// 
	/// @return Height of the region in pixels.
	unsigned int height () const;
	
	
	/// Returns the width of the region.
	/// 
	/// @return Width of the region in pixels.
	unsigned int width () const;
	
	
	/// Returns the number of pixels in the region.
	/// 
	/// @return Number of pixels in the region.
	const unsigned int& size () const;
	
	
	/// Adds a pair of pixel coordinates to the region.
	/// 
	/// @param coordinates		Coordinates of the pixel.
	/// 
	/// @see PixelCoordinates
	void addCoordinates(const PixelCoordinates& coordinates);
	
	
	/// Returns the coordinates of the top leftmost pixel.
	/// 
	/// @return A PixelCoordinates object with the coordinates of the top leftmost pixel.
	PixelCoordinates topLeftmostPixelCoordinates () const;
	
	
	/// Normalize the coordinates of every pixel so that the top leftmost pixel is located at (0,0).
	void normalizeCoordinates ();
	
private:

	std::vector<PixelCoordinates>	coordinates_;	///< Set of pixel coordinates that defines the whole region
	
	unsigned int					size_;			///< Number of pixels in the region
};


inline const unsigned int& Region::size () const
{
	return size_;
};


inline PixelCoordinates& Region::operator() (const unsigned int& index)
{
	return coordinates_.at(index);
};


inline PixelCoordinates Region::operator() (const unsigned int& index) const
{
	return coordinates_.at(index);
};


inline Region Region::operator+ (const Region& region) const
{
	Region temp;
	
	// Copy the pixels of this region
	for ( std::vector<PixelCoordinates>::const_iterator i = this->coordinates_.begin(); i not_eq coordinates_.end(); ++i )
		temp.addCoordinates( (*i) );
	
	// Copy the pixels of the second region
	for ( std::vector<PixelCoordinates>::const_iterator i = region.coordinates_.begin(); i not_eq region.coordinates_.end(); ++i )
		temp.addCoordinates( (*i) );
	
	return temp;
};

#endif  //_REGION_H
