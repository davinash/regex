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
#include <sstream>
#include "regexstate.h"

RegExState::~RegExState() {
	m_NFAStates.clear();
	m_Transition.clear();
}

RegExState::RegExState(std::set<RegExState*> NFAState, int nID) {
	m_NFAStates = NFAState;
	m_nStateID = nID;
	m_bAcceptingState = false;
	m_GroupID = 0;
	StateIterator iter;
	for(iter = NFAState.begin(); iter != NFAState.end(); ++iter) {
		if((*iter)->m_bAcceptingState) {
			m_bAcceptingState = true;
		}
	}
}
RegExState::RegExState(const RegExState &other){ 
	*this = other;
}

void RegExState::AddTransition(char inputCh, RegExState *pState){
	m_Transition.insert(std::make_pair(inputCh, pState));
}

void RegExState::RemoveTransition(RegExState* pState) {
	std::multimap<char, RegExState*>::iterator iter;
	for(iter = m_Transition.begin(); iter != m_Transition.end();) {
		RegExState *toState = iter->second;
		if(toState == pState)
			m_Transition.erase(iter++);
		else 
			++iter;
	}
}

void RegExState::GetTransition(char inputCh, Table &States) {
	States.clear();
	std::multimap<char, RegExState*>::iterator iter;
	for(iter = m_Transition.lower_bound(inputCh); iter != m_Transition.upper_bound(inputCh); ++iter) {
		RegExState *pState = iter->second;
		States.push_back(pState);
	}
}

std::set<RegExState*>& RegExState::GetNFAState() { 
	return m_NFAStates; 
}

bool RegExState::IsDeadEnd() {
	if(m_bAcceptingState)
		return false;
	if(m_Transition.empty())
		return true;
	std::multimap<char, RegExState*>::iterator iter;
	for(iter=m_Transition.begin(); iter!=m_Transition.end(); ++iter){
		RegExState *toState = iter->second;
		if(toState != this)
			return false;
	}
	return true;
}	

RegExState& RegExState::operator=(const RegExState& other) { 
	this->m_Transition	= other.m_Transition; 
	this->m_nStateID	= other.m_nStateID;
	this->m_NFAStates	= other.m_NFAStates;
	return *this;
}

bool RegExState::operator==(const RegExState& other) {
	if(m_NFAStates.empty())
		return(m_nStateID == other.m_nStateID);
	else 
		return(m_NFAStates == other.m_NFAStates);
}
std::string RegExState::getStringID(){
	std::string result;
	std::stringstream out;
	//if(m_bAcceptingState){
	//	out << "{" << m_nStateID << "}";
	//} else {
		out << m_nStateID;
	//}
	return out.str();
}



