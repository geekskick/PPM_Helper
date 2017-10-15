#include <iostream>
#include <unordered_map>
#include <fstream>
#include "ppm_file.h"

enum ARGS{ APP_NAME = 1, IN_FILENAME, OUT_FILENAME };

void print_usage(const std::string& app_name){
	std::cout << app_name << " usage: ./" << app_name << " <input filename> <output filename (optional)>" << std::endl;
}
int main(const int argc, const char **argv) {

	// Do some simple checking of the user input
	if(argc < IN_FILENAME){
		print_usage(std::string(argv[APP_NAME-1]));
		exit(EXIT_FAILURE);
	}

	/*!
	 * @brief The output filename
	 */
	std::string outname = "default";

	if(argc >= OUT_FILENAME){
		outname = argv[OUT_FILENAME-1];
	}

	/*!
	 * @brief The words will be put into a hashmap of unique word occurrences, and which indices of the
	 * input those occurrences happened
	 */
	std::unordered_map<std::string, std::vector<int>> wordmap;

	/*!
	 * @brief The input
	 */
	std::fstream file;
	file.open(argv[IN_FILENAME-1], std::fstream::in);

	/*!
	 * @brief The word currently being processed
	 */
	std::string word;

	/*!
	 * @brief The index of this occurrence of the word
	 */
	int word_num = 0;

	/*!
	 * @brief The maximum number of occurrences of any word will be used later of evenly distributing the colour values.
	 * So the word with the most occurences will be bluer
	 */
	int max_occurrences = 0;
	while(file >> word){

		/* Remove punctuation https://stackoverflow.com/questions/19138983/c-remove-punctuation-from-string */
		word.erase (std::remove_if (word.begin (), word.end (), ispunct), word.end ());
		/* Make lowercase https://notfaq.wordpress.com/2007/08/04/cc-convert-string-to-upperlower-case/ */
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);

		// if the word is in the map already then add to the map value
		if(wordmap.find(word) != wordmap.end()){
			wordmap[word].push_back(word_num);
			if(wordmap[word].size() > max_occurrences){ max_occurrences = (int)wordmap[word].size(); }
		}
		else{
			// if the first occurrence make a new vector
			wordmap[word].emplace_back(word_num);
		}
		word_num++;
	}
	file.close();

	ppm_image p;

	// Create background of image totally white
	for(int w = 0; w < word_num; w++){
		p << std::vector<rgb_pixel>(word_num, rgb_pixel::get_colour(rgb_pixel::colours::WHITE));
	}

	/*!
	 * @brief k is the multiplier for the values, so that the word with the most occurrences is the bluest
	 */
	const int k = UINT8_MAX/max_occurrences;

	// Create the image by changing values in the grid
	for(std::pair<std::string, std::vector<int>> unique_word : wordmap){
		uint8_t vect_idx_x = 0;

		// Each word has a number of indices, so loop over the indices and use each
		// as an x and as a y value to make the grid.
		for(int x: unique_word.second){
			// Increment this before use to prevent having a multiplier of 0
			vect_idx_x++;
			uint8_t vect_idx_y = 0;
			for(int y: unique_word.second) {
				// Increment this before use to prevent having a multiplier of 0
				vect_idx_y++;

				// These colours values are farily random:
				// make there more red when the x index increases
				// make there more blue when the y index increases
				// make the blue more intense based on how many occurrences of this word
				p[x][y] = rgb_pixel( (uint8_t)(vect_idx_x * k), (uint8_t)(vect_idx_y * k), unique_word.second.size() * k);
			}

		}
	}

	// Finally write the file out
	file.open(outname + ".ppm", std::fstream::out);
	file << p;
	file.close();

	return 0;
}