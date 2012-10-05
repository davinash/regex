/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
 *  This file is part of regex library
 *  Copyright (C) 2011 Avinash Dongre ( dongre.avinash@gmail.com )
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/
#ifndef _REGEXSTATE_H_
#define _REGEXSTATE_H_

#include <map>
#include <set>
#include <vector>
#include <string>

class RegExState{
private:	
	std::set<RegExState*> m_NFAStates;	
public:
	typedef std::vector<RegExState*> Table;
	typedef std::set<RegExState*>::iterator StateIterator;
	std::multimap<char, RegExState*> m_Transition;	
	int m_nStateID;
	bool m_bAcceptingState;
	bool m_Marked;
	int  m_GroupID;

	RegExState() : m_nStateID(-1), m_bAcceptingState(false) {};
	RegExState(int nID) : m_nStateID(nID), m_bAcceptingState(false), m_GroupID(0) {};
	RegExState(std::set<RegExState*> NFAState, int nID);
	RegExState(const RegExState &other);

	virtual ~RegExState();

	void AddTransition(char inputCh, RegExState *pState);
	void RemoveTransition(RegExState* pState);
	void GetTransition(char inputCh, Table &States);
	std::set<RegExState*>& GetNFAState();

	bool IsDeadEnd();
	RegExState& operator=(const RegExState& other);
	bool operator==(const RegExState& other);

	std::string getStringID();
protected:
};
#endif
