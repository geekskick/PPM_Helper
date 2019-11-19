#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "ppm_file.h"
#include <tclap/CmdLine.h>

int main(const int argc, const char **argv) {

    constexpr std::string_view whirly {"\\|/-"};
    auto cmd = TCLAP::CmdLine{ "Draws the song lyrics, or basically any words, as a map to show the structure of the words", '=', "0.2" };
    auto in_arg = TCLAP::ValueArg<std::string>{ "i", "in", "Input filename", true, "default", "string" }; 
    auto out_arg = TCLAP::ValueArg<std::string>{ "o", "out", "Output filename", false, "default", "string" }; 
    cmd.add(in_arg);
    cmd.add(out_arg);

    auto outfile = std::string{};
    auto infile = std::string{};
    try{
        cmd.parse(argc, argv);
        outfile = out_arg.getValue();
        infile = in_arg.getValue();
    }
    catch(TCLAP::ArgException& e){
        std::cerr << "Error: " << e.error() << " for arg " << e.argId() << '\n';
        return EXIT_FAILURE;
    }

	auto wordmap = std::unordered_map<std::string, std::vector<int>> {};
    auto file = std::fstream{infile, std::fstream::in};

	if(!file.is_open()){
		std::cerr << "Unable to open " << infile << '\n';
		return EXIT_FAILURE;
	}

	auto word = std::string{};
	auto word_num = 0;

	/*
	 * The maximum number of occurrences of any word will be used later of evenly distributing the colour values.
	 * So the word with the most occurences will be bluer. Must start at one to prevent a divide by 0 error if only one word.
	 */
	auto max_occurrences = std::vector<int>::size_type{1};

	while(file >> word){

		word.erase (std::remove_if (word.begin (), word.end (), ispunct), word.end ());
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);

		if(wordmap.find(word) != wordmap.end()){
			wordmap[word].push_back(word_num);
			if(wordmap[word].size() > max_occurrences){ max_occurrences = static_cast<int>(wordmap[word].size()); }
		}
		else{
			// if the first occurrence make a new vector
			wordmap[word].emplace_back(word_num);
		}
		word_num++;
	}
	file.close();

	auto p = ppm_image{};

	// Create background of image totally white
	for(int w = 0; w < word_num; w++){
		p << std::vector<rgb_pixel>(word_num, rgb_pixel::get_colour(rgb_pixel::colours::WHITE));
	}

	
	// k is the multiplier for the values, so that the word with the most occurrences is the bluest
	const auto k = std::numeric_limits<uint8_t>::max()/max_occurrences;

	// Create the image by changing values in the grid
	int i = 0;
	for(const auto& unique_word : wordmap){
		auto vect_idx_x = 0;

		// Each word has a number of indices, so loop over the indices and use each
		// as an x and as a y value to make the grid.
		for(const auto& x: unique_word.second){
			// Increment this before use to prevent having a multiplier of 0
			vect_idx_x++;
			auto vect_idx_y = 0;
			for(const auto& y: unique_word.second) {
				// Increment this before use to prevent having a multiplier of 0
				vect_idx_y++;

				// These colours values are fairly random:
				// make there more red when the x index increases
				// make there more blue when the y index increases
				// make the blue more intense based on how many occurrences of this word
                const auto r = vect_idx_x * k;
                const auto g = vect_idx_y * k;
                const auto b = unique_word.second.size() * k;
				const auto pix = rgb_pixel{static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};

				// Display something on the screen so the user knows something's happening.
				std::cout << "\r" << whirly[i++ % whirly.length() - 1];
				p[x][y] = pix;
			}

		}
	}

	std::cout << "\r";

	// Finally write the file out
	file.open(outfile + ".ppm", std::fstream::out);
	if(!file.is_open()){
		std::cerr << "Unable to open " << outfile << '\n';
		return EXIT_FAILURE;
	}
	file << p;
	file.close();

	std::cout << "Result written to " << outfile << ".ppm" << std::endl;

	return EXIT_SUCCESS;
}
