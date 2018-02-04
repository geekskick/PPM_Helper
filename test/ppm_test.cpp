#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ppm_file.h"

TEST_CASE("Pixels", "[pixels]"){
	rgb_pixel r = rgb_pixel::get_colour(rgb_pixel::colours::RED);
	rgb_pixel g = rgb_pixel::get_colour(rgb_pixel::colours::GREEN);
	rgb_pixel blu = rgb_pixel::get_colour(rgb_pixel::colours::BLUE);
	rgb_pixel blk = rgb_pixel::get_colour(rgb_pixel::colours::BLACK);
	rgb_pixel w = rgb_pixel::get_colour(rgb_pixel::colours::WHITE);
	rgb_pixel t(100,99,98);

	REQUIRE(r.red() == 255);
	REQUIRE(r.green() == 0);
	REQUIRE(r.blue() == 0);

	REQUIRE(g.red() == 0);
	REQUIRE(g.green() == 255);
	REQUIRE(g.blue() == 0);

	REQUIRE(blu.red() == 0);
	REQUIRE(blu.green() == 0);
	REQUIRE(blu.blue() == 255);

	REQUIRE(blk.red() == 0);
	REQUIRE(blk.green() == 0);
	REQUIRE(blk.blue() == 0);

	REQUIRE(w.red() == 255);
	REQUIRE(w.green() == 255);
	REQUIRE(w.blue() == 255);

	REQUIRE(t.red() == 100);
	REQUIRE(t.green() == 99);
	REQUIRE(t.blue() == 98);

	r.red() = 7;
	REQUIRE(r.red() == 7);
	REQUIRE(r.green() == 0);
	REQUIRE(r.blue() == 0);

	r.green() = 8;
	REQUIRE(r.red() == 7);
	REQUIRE(r.green() == 8);
	REQUIRE(r.blue() == 0);

	r.blue() = 9;
	REQUIRE(r.red() == 7);
	REQUIRE(r.green() == 8);
	REQUIRE(r.blue() == 9);
}

TEST_CASE("Size", "[size]"){
	image_size s;
	REQUIRE(s.width() == 0);
	REQUIRE(s.height() == 0);

	s.width() = 70;
	REQUIRE(s.width() == 70);
	REQUIRE(s.height() == 0);

	s.height() = 79;
	REQUIRE(s.width() == 70);
	REQUIRE(s.height() == 79);

	image_size p(79, 54);
	REQUIRE(p.width() == 79);
	REQUIRE(p.height() == 54);
}

TEST_CASE("PPM Building", "[ppm_build]"){
	ppm_image ppm;
	REQUIRE(ppm.size() == image_size());
	REQUIRE(ppm.max_colour() == 0);

	rgb_pixel r = rgb_pixel::get_colour(rgb_pixel::RED);
	ppm << r;
	REQUIRE(ppm.size() == image_size(1,1));
	REQUIRE(ppm.max_colour() == 255);
	ppm << r;
	REQUIRE(ppm.size() == image_size(2,1));

	std::vector<rgb_pixel> row;
	row.emplace_back(0,0,0);
	row.emplace_back(255,255,255);
	ppm << row;
	REQUIRE(ppm.size() == image_size(2,2));

	row.emplace_back(44,44,44);
	ppm << row;
	REQUIRE(ppm.size() == image_size(3,3));
	REQUIRE(ppm[0].size() == 3);
	REQUIRE(ppm[0].back() == rgb_pixel::get_colour(rgb_pixel::colours::WHITE));

	row.pop_back();
	ppm<<row;
	REQUIRE(ppm.size() == image_size(3,4));

	ppm.new_line();
	REQUIRE(ppm.size() == image_size(3, 5));

	ppm.append_last_line(r);
	REQUIRE(ppm[ppm.size().height()-1 ].back() == r);
}

TEST_CASE("PPM Streaming", "[ppm_stream]"){
	ppm_image ppm;

	std::vector<rgb_pixel> row;
	row.emplace_back(rgb_pixel::get_colour(rgb_pixel::colours::GREEN));
	row.emplace_back(rgb_pixel::get_colour(rgb_pixel::colours::RED));

	ppm << row;
	std::iter_swap(row.begin(), row.begin() + 1);
	ppm << row;

	std::stringstream result;
	result << ppm;

	std::string EXPECTED_IMAGE = "P3\n2 2\n255\n0 255 0 255 0 0 \n255 0 0 0 255 0 \n";
	REQUIRE(EXPECTED_IMAGE == result.str());

}
