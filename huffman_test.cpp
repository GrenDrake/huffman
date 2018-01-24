#include <iostream>

#include "huffman.h"

int main() {
	HuffmanTable ht;
    const char *inputStrings[] = {
        "The manor-house of the Prelate's Servant Randolph was a massive edifice of stone and wood with towers that, well, towered over the surrounding countryside. Within were three above-ground levels and even more below. Despite the \"name\", it was not where the Prelate's Servant Randolph lived, but rather was home to the bureaucracy that lay behind the servant's power.\n",
        "In addition to the countless offices and records-rooms, it was also home to much of the artistic wealth that the Prelate's Servant had acquired. There was so much of this that it was crammed in almost carelessly wherever space could be found. Public viewing was allowed during restricted hours, but few wanted to dare the volatile moods of one of the prelate's governors.\n",
        nullptr
    };

    /* ***********************************************************************
     * Build (and optionally dump) Huffman Tables
     */
    for (int i = 0; inputStrings[i] != nullptr; ++i) {
	    ht.addFrequencies(inputStrings[i]);
    }
//	ht.addMinFrequencies();
	ht.buildTree();

    /* ***********************************************************************
     * Dump Huffman Table Data
     */
	// ht.dumpFrequencies();
	// ht.dumpTree();

    /* ***********************************************************************
     * Test Encoding String
     */
	const char *toEncode = "towered over the prelate";
    std::cout << "Encoding test string: \"" << toEncode << "\"\n\n";
	encodedString = ht.encode(toEncode);

    std::cout << "Encoded string data:\n";
    int counter = 0;
	for (auto i : encodedString) {
		if (i) {
			std::cout << '1';
		}	else {
			std::cout << '0';
		}

        ++counter;
        if (counter % 8 == 0) {
            if (counter >= 72) {
                counter = 0;
                std::cout << '\n';
            } else {
                std::cout << ' ';
            }
        }
	}
    int initialSize = strlen(toEncode) * 8;
	std::cout << "\nSize (bits): " << initialSize << " => " << encodedString.size();
    std::cout << "   " << 100 - (encodedString.size() * 100 / initialSize) << "% compression\n\n";

    /* ***********************************************************************
     * Test Decoding String
     */
	std::string out = ht.decode(encodedString);
	std::cout << "Decoded string text: " << out << "\n\n";

	// encodedString.clear();
	// const char *bitStr = "1110 101 01100 01100 0101 1001100 10011111";
	// for (size_t i = 0; bitStr[i] != 0; ++i) {
	// 	if (bitStr[i] == '1')
	// 		encodedString.push_back(1);
	// 	if (bitStr[i] == '0')
	// 		encodedString.push_back(0);
	// }
	// out = ht.decode(encodedString);
	// std::cout << "Result: " << out << "\n\n";

	return 0;
}