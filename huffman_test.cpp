#include <fstream>
#include <iostream>

#include "huffman.h"

int main() {
	HuffmanTable ht;
    const char *inputStrings[] = {
        "The manor-house of the Prelate's Servant Randolph was a massive edifice of stone and wood with towers that, well, towered over the surrounding countryside. Within were three above-ground levels and even more below. Despite the \"name\", it was not where the Prelate's Servant Randolph lived, but rather was home to the bureaucracy that lay behind the servant's power.\n",
        "In addition to the countless offices and records-rooms, it was also home to much of the artistic wealth that the Prelate's Servant had acquired. There was so much of this that it was crammed in almost carelessly wherever space could be found. Public viewing was allowed during restricted hours, but few wanted to dare the volatile moods of one of the prelate's governors.\n",
        // Japanese text from http://generator.lorem-ipsum.info/_japanese
        "引阜ハモ展勝ヒヨユト奪手き人年ヱレオル毎東フぐか迎作69治全ょっ載提ほべ問窓ヲヱ現可さづ意免話執折トはラ。新トハリル観他タソホメ光20感ょどひれ渡情1全べ勲体囲ト軽迫ゃ面長ネウ間広ゅんがと題定ま一公のょざ重64県乗歌規4提ノミサ和風リ税就僚弱招て。爆購テノ正本ゆる果政ナミフネ職読ス断確無経ゅと針激ばずめ戦天柔コシソ東考どほびル内初いリト概最ぱぜ調的ハフ区助エ転拘肌陶トを。",
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
    std::ofstream dumpFile("huffman_dump.txt");
	ht.dumpFrequencies(dumpFile);
    dumpFile << '\n';
	ht.dumpTree(dumpFile);


    std::vector<bool> encodedString;
    /* ***********************************************************************
     * Test Encoding String
     */
	const char *toEncode = "towered over フぐ the prelate";
    std::cout << "Encoding test string: \"" << toEncode << "\"\n\n";
    try {
    	encodedString = ht.encode(toEncode);
    } catch (HuffmanException &e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }

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
    try {
    	std::string out = ht.decode(encodedString);
    	std::cout << "Decoded string text: " << out << "\n\n";
    } catch (HuffmanException &e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }

	return 0;
}