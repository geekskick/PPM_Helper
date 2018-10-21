//
// Created by Patrick Mintram on 14/10/2017.
//

#ifndef SONGSIM_PPM_FILE_H
#define SONGSIM_PPM_FILE_H

#include <string>
#include <vector>
#include <iostream>


/*!
 * @brief A PPM File requires a bunch of pixels as r g b values
 */
class rgb_pixel{
public:
	/*!
	 * @brief For accessing the rgb_pixel::_examples array to get premade pixel objects
	 */
	enum class colours { BLACK = 0, WHITE, RED, GREEN, BLUE, COUNT };

	/*!
	 * @brief Get a copy of a premade pixel object
	 * @param c The colour to get
	 * @return a copy of a premade object in the chosen colour
	 * @warning Asserts that the colour is in range
	 */
	static rgb_pixel get_colour(const colours c);

	rgb_pixel() = default;

	rgb_pixel(const uint8_t r, const uint8_t g, const uint8_t b)
			: _r(r), _g(g), _b(b)
	{ /*! Intentionally blank */ }

	/*!
	 * @brief Output the pixel to the stream as "r g b"
	 * @param os The stream
	 * @param p The pixel
	 * @return The stream, with "r g b" values added
	 */
	friend std::ostream& operator<<(std::ostream& os, const rgb_pixel& p);

	// Mutable accessors

	/*!
	 * @brief Accessor
	 * @return The red value
	 */
	uint8_t& red() 		{ return _r; }
	/*!
	 * @brief Accessor
	 * @return The green value
	 */
	uint8_t& green() 	{ return _g; }
	/*!
	 * @brief Accessor
	 * @return The blue value
	 */
	uint8_t& blue()		{ return _b; }

	// Const accessors
	/*!
	 * @brief Const accessor
	 * @return The red value
	 */
	const uint8_t& red() const 		{ return _r; }
	/*!
	 * @brief Const accessor
	 * @return The green value
	 */
	const uint8_t& green() const 	{ return _g; }
	/*!
	 * @brief Const accessor
	 * @return The blue value
	 */
	const uint8_t& blue() const 	{ return _b; }

	/**
	 * @brief      Equality Test
	 * @param[in]  p     other pixel
	 * @return     True if it's rgb are the same
	 */
	bool operator==(const rgb_pixel& p) const;

	/**
	 * @brief      Inequality Test
	 * @param[in]  p     other pixel
	 * @return     False if it's rgb are the same
	 */
	bool operator!=(const rgb_pixel& p) const;

private:
	uint8_t _r{0};	/*! The pixels' R value */
	uint8_t _g{0}; /*! The pixels' G value */
	uint8_t _b{0}; /*! The pixels' B value */

	/*!
	 * @brief Example colours, must be in the same order as the rgb_pixel::colours enum
	 */
	static const rgb_pixel _examples[static_cast<int>(colours::COUNT)];
};

//------------------------------------------
/*!
 * @brief The size of an object in terms of width and height
 */
class image_size{
public:
	image_size() = default;

	image_size(const int w, const int h)
			: _w(w), _h(h)
	{ /*! Intentionally Blank */ }

	/*!
	 * @brief Accessor
	 * @return The width
	 */
	int& 		width() 		{ return _w; }
	/*!
	 * @brief Accessor
	 * @return The height
	 */
	int& 		height()		{ return _h; }

	/*!
	 * @brief Const accessor
	 * @return The width
	 */
	const int& 	width() const	{ return _w; }
	/*!
	 * @brief const accessor
	 * @return The height
	 */
	const int& 	height() const	{ return _h; }

	/**
	 * @brief      Equality operator
	 * @param[in]  rhs   The right hand side
	 * @return     true if the dimensions are equal
	 */
	bool 		operator== (const image_size& rhs) const ;

	/**
	 * @brief      Equality operator
	 * @param[in]  rhs   The right hand side
	 * @return     false if the dimensions are equal
	 */
	bool 		operator!= (const image_size& rhs) const;

	/*!
	 * @brief Describe the size as "w h"
	 * @param os The stream to write to
	 * @param s The size
	 * @return The stream with the size "w h" added
	 */
	friend std::ostream& operator<<(std::ostream& os, const image_size& s);

private:
	int _w{0}; /*! The image's width 	*/
	int _h{0}; /*! The image's height 	*/
};

/*!
 * @brief A PPM file
 */
class ppm_image
{
public:
	/// Ctors
	ppm_image() = default;

	ppm_image(const uint8_t max_colour)
			: _max_colour_value(max_colour)
	{ /*! Intentionally Blank */ }

	//----------------
	/*!
	 * @brief Accessor
	 * @return The max colour value used
	 */
	uint8_t& 				max_colour()		{ return _max_colour_value; }
	/*!
	 * @brief Accessor
	 * @return The size of the image
	 */
	image_size&				size() 				{ return _size; 			}

	/*!
	 * @brief const accessor
	 * @return The max colour value used
	 */
	const uint8_t& 			max_colour() const	{ return _max_colour_value; }
	/*!
	 * @brief Const accessor
	 * @return The size of the image
	 */
	const image_size&		size() const		{ return _size; 			}

	/*!
	 * @brief Add a value to the last line
	 * @param n The value to add
	 */
	void operator << (const rgb_pixel& n);

	/*!
	 * @brief Add a line
	 * @param line The line to add
	 */
	void operator << (const std::vector<rgb_pixel> &line);

	/*!
	 * @brief Add a new line of pixels to the image
	 */
	void new_line();

	/*!
	 * @brief Add to the last line of the image
	 * @param n The number to add
	 */
	void append_last_line(const rgb_pixel& n);

	/*!
	 * @brief Stream out the image data, including header.
	 * @param os The stream destination
	 * @param ppm The ppm object
	 * @return The stream with added image in
	 */
	friend std::ostream& operator<<(std::ostream& os, const ppm_image& ppm);

	/*!
	 * @brief Allows access to the image line by line
	 * @param n The line to get
	 * @return The line
	 */
	std::vector<rgb_pixel>&operator[](const int n);

private:
	uint8_t 							_max_colour_value{0};
	image_size							_size;
	std::vector<std::vector<rgb_pixel>> _data;

	/*!
	 * @brief Check that the width of the line doesn't overrun the current width, if it does then
	 * update the curreent width
	 * @param last_line The line to check
	 */
	void _width_check(const std::vector<rgb_pixel> &last_line);

	/*!
	 * @brief Check that the new colour value entered is in the range, if not - increase the range
	 * @param new_val The new colour
	 */
	void _colour_check(const rgb_pixel &new_val);

	/**
	 * @brief      If there are jagged dimensions then fill the gaps with white pixels
	 */
	void _fill();

};




#endif //SONGSIM_PPM_FILE_H
