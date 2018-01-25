#include <map>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdint>

#include "utf8/utf8.h"
#include "huffman.h"


void HuffmanTable::dumpTree() const {
	if (!root) {
		throw HuffmanException("Tried to dump non-existant tree");
	}
	std::cout << "HUFFMAN TREE DATA DUMP\n    BIT SEQUENCE  CHARACTER\n";
	root->dump("");
}

void HuffmanBranch::dump(std::string s) const {
	if (_left) {
		_left->dump(s + "0");
	}
	if (_right) {
		_right->dump(s + "1");
	}
}

void HuffmanLeafChar::dump(std::string s) const {
	std::cout << std::setw(16) << s << ": ";
	if (_character >= 0x20 && _character != 0x1F) {
		std::cout << '\'' << (char)_character << "' (0x" << std::hex << std::uppercase << _character << std::dec << ')';
	} else {
		std::cout << "0x" << std::hex << _character << std::dec;
	}
	std::cout << '\n';
}

void HuffmanLeafEnd::dump(std::string s) const {
		std::cout << std::setw(16) << s << ": NUL\n";
}

void HuffmanTable::dumpFrequencies() const {
	std::multimap<int, int> reverseMap;
	for (const auto &i : charFrequency) {
		reverseMap.insert(std::make_pair(i.second, i.first));
	}

	std::cout << std::left << std::setw(8) << "CHAR" << " FREQUENCY ASCII\n";
	for (const auto &i : reverseMap) {
		std::cout << std::setw(8) << i.second << " " << std::setw(9) << i.first;
		if (i.second >= 0x20 && i.second != 0x7F) {
			std::cout << " '" << i.second << '\'';
		}
		std::cout << "\n";
	}
}

void HuffmanTable::addFrequencies(const char *text) {
	size_t i = 0;
	do {
		int codePoint = text[i];
		if (codePoint == 0) {
			codePoint = 1;
		}
		++charFrequency[codePoint];
	} while (text[i++]);
}

void HuffmanTable::addMinFrequencies() {
	for (size_t i = 32; i < 127; ++i) {
		if (charFrequency[i] == 0) {
			charFrequency[i] = 1;
		}
	}
	if (charFrequency[10] == 0) {
		charFrequency[10] = 1;
	}
}

class HuffmanNodeCompareWeights {
public:
	bool operator()(const HuffmanNode *lhs, const HuffmanNode *rhs) {
		// we want this sorted in reverse order, so we're using > rather than <
		return (lhs->getWeight() > rhs->getWeight());
	}
};
void HuffmanTable::buildTree() {
	std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, HuffmanNodeCompareWeights> q;
	for (const auto &i : charFrequency) {
		if (i.first == 0 || i.first == 1) {
			q.push(new HuffmanLeafEnd(i.second));
		} else {
			q.push(new HuffmanLeafChar(i.first,i.second));
		}
	}

	while(q.size() > 1) {
		HuffmanNode *right = q.top();
		q.pop();
		HuffmanNode *left = q.top();
		q.pop();

		HuffmanBranch *newNode = new HuffmanBranch(left, right);
		q.push(newNode);
	}
	root = q.top();
}


std::vector<bool> HuffmanTable::encode(const char *text) const {
	std::vector<bool> result;
	size_t pos = 0;
	size_t length = strlen(text) + 1;

	while (pos < length) {
		int c = text[pos];
		if (c == 0) c = 1;

		HuffmanNode *node = root;
		while (node->getType() == HuffmanNode::Branch) {
			HuffmanBranch *branch = dynamic_cast<HuffmanBranch*>(node);
			if (!branch) {
				return result;
			}

			if (branch->getLeft()->contains(c)) {
				node = branch->getLeft();
				result.push_back(false);
			} else if (branch->getRight()->contains(c)) {
				node = branch->getRight();
				result.push_back(true);
			} else {
				std::stringstream ss;
				ss << "Character ";
				if (c >= 0x20 && c != 0x7F) {
					ss << '\'' << static_cast<char>(c) << "' (" << std::hex << "0x" << c << ") ";
				} else {
					ss << std::hex << "0x" << c << ' ';
				}
				ss << "Not in Huffman Table";
				throw HuffmanException(ss.str());
			}
		}

		++pos;
	}
	return result;
}

std::string HuffmanTable::decode(const std::vector<bool> &data) const {
	std::string result;
	size_t pos = 0;

	while (pos < data.size()) {
		HuffmanNode *node = root;
		while (node->getType() == HuffmanNode::Branch) {
			HuffmanBranch *branch = dynamic_cast<HuffmanBranch*>(node);
			if (!branch) {
				throw HuffmanException("Malformed Tree in Decoding");
			}

			bool bit = data[pos++];
			node = branch->nextNode(bit);
		}

		switch(node->getType()) {
			case HuffmanNode::Branch:
			case HuffmanNode::BadType:
				throw HuffmanException("Bad Decode Path");
			case HuffmanNode::SingleChar: {
				HuffmanLeafChar *leaf = dynamic_cast<HuffmanLeafChar*>(node);
				if (!leaf) {
					throw HuffmanException("Malformed Tree in Decoding");
				}
				result.append(1, leaf->getCharacter());
				break; }
			case HuffmanNode::End:
				return result;
		}
	}

	throw HuffmanException("Unexpected End of Data");
}