#include "huff.h"
#include <queue>
#include <cmath>

namespace samsami {

void HuffNode::add(HuffNode *hNode, unsigned int d, unsigned int order) {
	this->freq += hNode->freq;
	for(vector<Symbol>::iterator it = hNode->symbols.begin(); it != hNode->symbols.end(); ++it) {
		Symbol *s = new Symbol((*it).symbol);
		//s->code = (*it).code + order * (unsigned int)pow((double)d, (double)(*it).codeLen);
		s->code = (*it).code * d + order;
		s->codeLen = (*it).codeLen + 1;
		this->symbols.push_back(*s);
	}
}

void encodeHuff(unsigned int d, unsigned char *text, unsigned int textLen, unsigned long long *huffCode, unsigned int *huffCodeLen) {
	unsigned int freq[256];
	for (int i = 0; i < 256; ++i) {
		freq[i] = 0;
		huffCode[i] = 0;
		huffCodeLen[i] = 0;
	}
	for (unsigned int i = 0; i < textLen; ++i) freq[(unsigned int)text[i]]++;
	unsigned int n = 0;
	priority_queue<HuffNode *, vector<HuffNode *>, HuffNode> heap;
	for (int i = 0; i < 256; ++i) if (freq[i] > 0) {
		++n;
		heap.push(new HuffNode(freq[i], (unsigned int)i));
	}
	unsigned int q = (unsigned int)(ceil((double)(n - d) / (d - 1)));
	unsigned int N = d + q * (d - 1);
	for (unsigned int i = 0; i < (N - n); ++i) heap.push(new HuffNode(0, (unsigned int)(256 + i)));
	while (heap.size() > 1) {
		HuffNode *newHuffNode = new HuffNode();
		for (unsigned int i = 0; i < d; ++i) {
			HuffNode *hf = heap.top();
			heap.pop();
			newHuffNode->add(hf, d, i);
			delete hf;
		}
		heap.push(newHuffNode);
	}
	HuffNode *hf = heap.top();
	heap.pop();
	for(vector<Symbol>::iterator it = hf->symbols.begin(); it != hf->symbols.end(); ++it) {
		unsigned int s = (*it).symbol;
		if (s > 255) continue;
		huffCode[s] = (*it).code;
		huffCodeLen[s] = (*it).codeLen;
	}
}

}
