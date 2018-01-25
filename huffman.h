#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <map>
#include <string>
#include <vector>

/**
 * General exception class for exceptions that occur within this library.
 */
class HuffmanException : public std::runtime_error {
public:
    HuffmanException(const std::string &message)
    : runtime_error("huffman error"), message(message)
    { }

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
private:
    std::string message;
};

/**
 * Base class for all nodes that occur in the Huffman table.
 */
class HuffmanNode {
public:
	enum NodeType {
		BadType = 99,
		Branch = 0,
		End = 1,
		SingleChar = 2,
	};

	explicit HuffmanNode(int weight = 0, NodeType type = HuffmanNode::BadType)
		: _weight(weight), _type(type)
	{ }
	virtual ~HuffmanNode() {
	}

	int getWeight() const {
		return _weight;
	}
	void setWeight(int weight) {
		_weight = weight;
	}

	NodeType getType() const {
		return _type;
	}
	void setType(NodeType type) {
		_type = type;
	}

	virtual const std::string getName() const = 0;
	virtual void dump(std::string s) const = 0;
	virtual bool contains(int character) const = 0;
private:
	int _weight;
	NodeType _type;
};

/**
 * Leaf-node for Huffman table repersenting single characters.
 */
class HuffmanLeafChar : public HuffmanNode {
public:
	explicit HuffmanLeafChar(int character = 0, int weight = 0)
	: HuffmanNode(weight, HuffmanNode::SingleChar), _character(character)
	{ }

	int getCharacter() const {
		return _character;
	}
	void setCharacter(int character) {
		_character = character;
	}

	virtual const std::string getName() const {
		std::string s;
		s += _character;
		return s;
	}
	virtual void dump(std::string s) const {
//		std::cout << s << ": " << (char)_character << " (" << _character << ")\n";
	}
	virtual bool contains(int character) const {
		return (character == _character);
	}
private:
	int _character;
};

/**
 * Leaf-node for Huffman table repersenting the end of a string.
 */
class HuffmanLeafEnd : public HuffmanNode {
public:
	explicit HuffmanLeafEnd(int weight = 0)
	: HuffmanNode(weight, HuffmanNode::End)
	{ }

	virtual const std::string getName() const {
		return "\x1\0";
	}
	virtual void dump(std::string s) const {
//		std::cout << s << ": NUL\n";
	}
	virtual bool contains(int character) const {
		return character == 0 || character == 1;
	}
};

/**
 * General branch node for Huffman table.
 */
class HuffmanBranch : public HuffmanNode {
public:
	HuffmanBranch(HuffmanNode *left, HuffmanNode *right)
	 : HuffmanNode(0, HuffmanNode::Branch), _left(left), _right(right)
	{
		setWeight(_left->getWeight() + _right->getWeight());
		setName(_left->getName() + _right->getName());
	}

	HuffmanNode* nextNode(bool right) {
		if (right) {
			return _right;
		}
		return _left;
	}
	const HuffmanNode* nextNode(bool right) const {
		if (right) {
			return _right;
		}
		return _left;
	}
	const HuffmanNode* getRight() const {
		return _right;
	}
	HuffmanNode* getRight() {
		return _right;
	}
	const HuffmanNode* getLeft() const {
		return _left;
	}
	HuffmanNode* getLeft() {
		return _left;
	}

	virtual void dump(std::string s) const;

	void setName(const std::string &name) {
		_name = name;
	}
	virtual const std::string getName() const {
		return _name;
	}
	virtual bool contains(int character) const {
		return (_left && _left->contains(character)) || (_right && _right->contains(character));
	}
private:
	HuffmanNode *_left, *_right;
	std::string _name;
};

/**
 * Main class for the Huffman table. Handles building the table as well as
 * encoding and decoding strings.
 */
class HuffmanTable {
public:
    /**
     * Encode a string into a block of binary data.
     * @param text The text to encode.
     * @return The encoded version of the string.
     * @throw HuffmanException Thrown if an error occurs during the encoding
     *                         process.
     */
	std::vector<bool> encode(const char *text) const;

    /**
     * Decode an encoded string back into a block of text.
     * @param data The encoded string to decode.
     * @return The unencoded version of the string.
     * @throw HuffmanException Thrown if an error occurs during the decoding
     *                         process.
     */
	std::string decode(const std::vector<bool> &data) const;

    /**
     * Use the provided text to add to the frequencies data used to build the
     * Huffman table. This does not actually build the table; see buildTree()
     * for that.
     * @param  text  The text to add the frequencies of.
     */
	void addFrequencies(const char *text);

    /**
     * Makes sure every standard ascii character has a frequency of at least
     * one. This will make sure that the encoder can deal with any possible
     * string, even if the input data for the frequency table did not contain
     * some characters.
     */
	void addMinFrequencies();

    /**
     * Dumps a table of all current character frequencies to std::cout.
     */
	void dumpFrequencies() const;

    /**
     * Use previously gathered frequency data to build the Huffman
     * encoding/decoding tree.
     */
	void buildTree();

    /**
     * Dumps a Graphviz DOT file containing a graph of the Huffman encoding
     * tree to std::cout
     */
	void dumpTree() const;

private:
	HuffmanNode *root;
	std::map<int,int> charFrequency;
};

#endif