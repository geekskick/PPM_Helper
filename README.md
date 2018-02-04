# PPM_Helper

Makes for easy creation of PPM Images. You create the image you want using the `rgb_pixel` class and the `ppm_image`s `<<` operator. 

## `rgb_pixel`

The `rgb_pixel` class allows for management of the colours as RGB values. It also provides a Factory Method with allows you to get a few specific colours premade: `rgb_pixel::get_colour(colour);`.

## `ppm_image`

```c++
ppm_image ppm;
std::vector<rgb_pixel> row;
row.emplace_back(rgb_pixel::get_colour(rgb_pixel::colours::GREEN));
row.emplace_back(rgb_pixel::get_colour(rgb_pixel::colours::RED));

ppm << row;
// The image now has a row with two pixels in

ppm << row:
// The image now has two rows with two pixels in
```

The pixels are stored as a 2D grid (vector of vectors) and you can access the pixels using the `[]` operator to get a specific row out

```c++
ppm[0][0]; // Top left pixel
ppm[ppm.size()-1].back() // Bottom right pixel
```

If you decide to add a new row which is longer than previous rows, rather than having a bunch of jagged rows, the empty spaces are filled with white pixels.

The header for the `.ppm` output is automatically generated based on what you put into the `ppm_image` class. 
Stream out to the destination file using `<<` operator. The example provided is a simple rip off of [SongSim](https://colinmorris.github.io/SongSim/#/abc)
