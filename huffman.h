#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <map>
#include <string>
#include <vector>

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
class HuffmanNodeCompareWeights {
public:
	bool operator()(const HuffmanNode *lhs, const HuffmanNode *rhs) {
		// we want this sorted in reverse order, so we're using > rather than <
		return (lhs->getWeight() > rhs->getWeight());
	}
};

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
		return (_name.find_first_of(character) != std::string::npos);
	}
private:
	HuffmanNode *_left, *_right;
	std::string _name;
};


class HuffmanTable {
public:

	std::vector<bool> encode(const char *text) const;
	std::string decode(const std::vector<bool> &data) const;

	void addFrequencies(const char *text);
	void addMinFrequencies();
	void dumpFrequencies() const;
	void buildTree();
	void dumpTree() const;

private:
	HuffmanNode *root;
	std::map<int,int> charFrequency;
};

#endif