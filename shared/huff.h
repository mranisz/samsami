#ifndef HUFF_H
#define	HUFF_H

#include <vector>

using namespace std;

namespace samsami {

class Symbol {
public:
	unsigned int symbol;
	unsigned int code;
	unsigned int codeLen;

	Symbol(unsigned symbol) {
		this->symbol = symbol;
		this->code = 0;
		this->codeLen = 0;
	}
};

class HuffNode {
public:
	unsigned int freq;
	vector<Symbol> symbols;

	HuffNode() {
		this->freq = 0;
	};

	void add(HuffNode *hNode, unsigned int d, unsigned int order);

	HuffNode(unsigned int freq, unsigned int symbol) {
		this->freq = freq;
		Symbol *sym = new Symbol(symbol);
		this->symbols.push_back(*sym);
	};
	bool operator()(const HuffNode* lh, const HuffNode* rh) const { return lh->freq > rh->freq; }
};

void encodeHuff(unsigned int d, unsigned char *text, unsigned int textLen, unsigned long long *huffCode, unsigned int *huffCodeLen);

}

#endif	/* HUFF_H */

