//
// Created by Patrick Mintram on 14/10/2017.
//

#include "ppm_file.h"
#include <cassert>

 const rgb_pixel rgb_pixel::_examples[] = {
			// R		, G			, B
			{ 0			, 0			, 0 		},	// Black
			{ UINT8_MAX	, UINT8_MAX	, UINT8_MAX },	// White
			{ UINT8_MAX	, 0			, 0 		},	// Red
			{ 0			, UINT8_MAX	, 0 		},	// Green
			{ 0			, 0			, UINT8_MAX }	// Blue
 };

void ppm_image::_width_check( const std::vector<rgb_pixel> &last_line ) {
	if(last_line.size() > _size.width()){ _size.width() = (int)last_line.size(); }
}

void ppm_image::_colour_check( const rgb_pixel& new_val ) {
	if(new_val.red() > _max_colour_value){ _max_colour_value = new_val.red(); }
}

void ppm_image::operator<<( const std::vector<rgb_pixel> &line ) {
	new_line();
	for(rgb_pixel n: line){
		*this << n;
	}
}

void ppm_image::new_line( void ){
	_data.emplace_back(std::vector<rgb_pixel>());
	_size.height()++;
}

void ppm_image::operator<<( const rgb_pixel& n ) {
	if( _data.size() == 0 ) { new_line(); }
	std::vector<rgb_pixel>& last_line = _data[_data.size()-1];
	last_line.push_back(n);
	_width_check(last_line);
	_colour_check(n);

}

void ppm_image::append_last_line( const rgb_pixel& n ) {
	*this << n;
}

std::ostream &operator<<( std::ostream &os, const ppm_image& ppm ) {
	os << "P3" << std::endl << ppm._size << std::endl << std::to_string(ppm._max_colour_value) << std::endl;
	/*! Iterate over the 2d datastructure and output it to the stream */
	for(std::vector<rgb_pixel> line: ppm._data){
		for(rgb_pixel n: line){
			os << n << " ";
		}
		os << "\n";
	}
	return os;
}

std::vector<rgb_pixel> &ppm_image::operator[]( const int n ) {
	return _data[n];
}

std::ostream &operator<<( std::ostream &os, const rgb_pixel &p ) {
	os << std::to_string(p._r) << " " << std::to_string(p._g) << " " << std::to_string(p._b);
	return os;
}

rgb_pixel rgb_pixel::get_colour( const rgb_pixel::colours c ) {
	assert(c < COUNT);
	return _examples[c];
}

std::ostream &operator<<( std::ostream &os, const image_size &s ) {
	os << s._w << " " << s._h;
	return os;
}
