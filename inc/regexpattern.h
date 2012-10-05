#ifndef _REGEX_PATTERN_H_
#define _REGEX_PATTERN_H_

#include "regexstate.h"

class RegExPattern {
public:
	RegExPattern() : m_pState(NULL), m_nStartIndex(-1) {};
	RegExPattern(const RegExPattern &other){ 
		*this = other; 
	}
	virtual ~RegExPattern() {};
	RegExPattern& operator=(const RegExPattern& other){
		m_pState		= other.m_pState;
		m_nStartIndex	= other.m_nStartIndex;
		return *this;
	}
	RegExState* m_pState;
	int m_nStartIndex;
private:
protected:
};

#endif