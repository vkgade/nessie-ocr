/// @file
/// @brief Definition of Clip class

#include "Clip.hpp"
#include "NessieException.hpp"
#include <cmath>



Clip::Clip (const Magick::Image& image, const unsigned int& row, const unsigned int& column, const unsigned int& height, const unsigned int& width)
	:	pixels_(std::deque<unsigned char>(0)),
		row_(row),
		column_(column),
		width_(width),
		height_(height),
		size_(0)
{
	if ( (height == 0) and (width == 0) )
		throw NessieException ("Clip::Clip() : Constructor has 0 size.");

	if ( width > image.columns() )
		throw NessieException ("Clip::Clip() : The press clip's width cannot be wider than the underlying image's.");

	if ( height > image.rows() )
		throw NessieException ("Clip::Clip() : The press clip's height cannot be higher than the underlying image's.");

	if ( row >= image.rows() or column >= image.columns() )
		throw NessieException ("Clip::Clip() : The press clip's top leftmost pixel falls outside the image.");

	if( (row + height) > image.rows() or (column + width) > image.columns() )
		throw NessieException ("Clip::Clip() : The clip does not fall completely within the underlying image.");

	size_ = width_ * height_;
	pixels_.resize(size_);

	// Allocate a frame over the image to access its pixels.
	Magick::Pixels frame(const_cast<Magick::Image&>(image));
	Magick::PixelPacket *pixels = frame.get(row, column, width_, height_);

	// Copy the image pixels into the deque
	for ( unsigned int i = 0; i < height_; ++i )
	{
		for ( unsigned int j = 0; j < width_; ++j )
		{
			Magick::ColorGray grayLevel(*pixels++);

			pixels_.at(i * width_ + j) = static_cast<unsigned char>( round(grayLevel.shade() * 255.0) );
		}
	}
};


void Clip::writeToOutputImage (const std::string& outputFile) const
{
	Magick::Image outputImage = Magick::Image(Magick::Geometry(width_, height_), "white");
	outputImage.type( Magick::GrayscaleType );

	// Allocate pixel view
	Magick::Pixels view(outputImage);
	Magick::PixelPacket *originPixel = view.get(0, 0, width_, height_);
	Magick::PixelPacket *pixel;

	// Copy the current data into the external image
	for ( unsigned int i = 0; i < view.rows(); ++i )
	{
		for ( unsigned int j = 0; j < view.columns(); ++j )
		{
			pixel = originPixel + (i * view.columns()) + j;

			*pixel = Magick::ColorGray ( static_cast<double>(pixels_.at(i * width_ + j) / 255.0) );
		}
	}

	// Synchronize changes
	view.sync();
	outputImage.syncPixels();
	outputImage.write(outputFile);
};
