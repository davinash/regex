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
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string.h>
#include "regex.h"


#define EPSILON       -1 
#define CLOSURE       '*'
#define PLUS_CLOSURE  '+'
#define OPTIONAL      '?'
#define OPEN_PAREN    '('
#define CLOSE_PAREN   ')'
#define OR            '|'
#define FALSE_CONCAT  '&'


RegEx::RegEx() {
}
RegEx::~RegEx(){
	CleanUp();
}
bool RegEx::IsMetaChar( char inputCh ) {
	return(( inputCh == CLOSURE )      || 
		   ( inputCh == OR )           || 
		   ( inputCh == OPEN_PAREN )   || 
		   ( inputCh == CLOSE_PAREN )  || 
		   ( inputCh == PLUS_CLOSURE ) ||
		   ( inputCh == FALSE_CONCAT ) ||
		   ( inputCh == OPTIONAL ));
}
bool RegEx::IsInput(char inputCh)      { return(!IsMetaChar(inputCh));   }
bool RegEx::IsLeftParan(char inputCh)  { return(inputCh == OPEN_PAREN);  }
bool RegEx::IsRightParan(char inputCh) { return(inputCh == CLOSE_PAREN); }

void RegEx::CleanUp() {
	for(int i = 0; i < (int)m_NFATable.size(); ++i) {
		delete m_NFATable[i];
	}
	m_NFATable.clear();

	for(int i = 0; i < (int)m_DFATable.size(); ++i)
		delete m_DFATable[i];
	m_DFATable.clear();

	m_nNextStateID = 0;

	while(!m_CharacterClassStack.empty())
		m_CharacterClassStack.pop();

	while(!m_ExpressionStack.empty())
		m_ExpressionStack.pop();

	m_InputSet.clear();

}

bool RegEx::PopTable(Table  &NFATable) {
	if(m_CharacterClassStack.size()>0) {
		NFATable = m_CharacterClassStack.top();
		m_CharacterClassStack.pop();
		return true;
	}
	return false;
}

bool RegEx::Concatenate() {

    Table  LeftTable, RightTable;
    if(!PopTable(RightTable) || !PopTable(LeftTable))
		return false;

	(*(LeftTable.rbegin()))->AddTransition(EPSILON, (*(RightTable.begin())));
	LeftTable.insert(LeftTable.end(), RightTable.begin(), RightTable.end());
	m_CharacterClassStack.push(LeftTable);

	return true;
}

void RegEx::PushOnCharacterStack(char chInput) {

	RegExState *s0 = new RegExState(m_nNextStateID++);
	RegExState *s1 = new RegExState(m_nNextStateID++);

	s0->AddTransition(chInput, s1);

	Table NewSubTable;
	NewSubTable.push_back(s0);
	NewSubTable.push_back(s1);
	m_CharacterClassStack.push(NewSubTable);

	m_InputSet.insert(chInput);
}

bool RegEx::ClosureOptional() {

	Table PrevTable;
	if(!PopTable(PrevTable))
		return false;

	RegExState *LeftTable  = new RegExState(m_nNextStateID++);
	RegExState *RightTable = new RegExState(m_nNextStateID++);

	LeftTable->AddTransition(EPSILON, RightTable);
	LeftTable->AddTransition(EPSILON, ((*(PrevTable.begin()))));
	((*(PrevTable.rbegin())))->AddTransition(EPSILON, RightTable);

	PrevTable.insert( PrevTable.begin(), LeftTable );
	PrevTable.push_back ( RightTable);

	m_CharacterClassStack.push(PrevTable);

	return true;
}

bool RegEx::ClosurePlus() {
	Table PrevTable;
	if(!PopTable(PrevTable))
		return false;

	RegExState *LeftTable  = new RegExState(m_nNextStateID++);
	RegExState *RightTable = new RegExState(m_nNextStateID++);

	LeftTable->AddTransition(EPSILON, ((*(PrevTable.begin()))));
	((*(PrevTable.rbegin())))->AddTransition(EPSILON, RightTable);
	((*(PrevTable.rbegin())))->AddTransition(EPSILON, ((*(PrevTable.begin()))));

	PrevTable.insert( PrevTable.begin(), LeftTable );
	PrevTable.push_back ( RightTable);

	m_CharacterClassStack.push(PrevTable);

	return true;
}
bool RegEx::Closure() {

	Table PrevTable;
	if(!PopTable(PrevTable))
		return false;

	RegExState *LeftTable  = new RegExState(m_nNextStateID++);
	RegExState *RightTable = new RegExState(m_nNextStateID++);

	LeftTable->AddTransition(EPSILON, RightTable);
	LeftTable->AddTransition(EPSILON, ((*(PrevTable.begin()))));
	((*(PrevTable.rbegin())))->AddTransition(EPSILON, RightTable);
	((*(PrevTable.rbegin())))->AddTransition(EPSILON, ((*(PrevTable.begin()))));

	PrevTable.insert( PrevTable.begin(), LeftTable );
	PrevTable.push_back ( RightTable);

	m_CharacterClassStack.push(PrevTable);

	return true;
}

bool RegEx::Or() {

	Table  UpperTable, LowerTable;

	if(!PopTable(LowerTable) || !PopTable(UpperTable))
		return false;

	RegExState *LeftTable	= new RegExState(m_nNextStateID++);
	RegExState *RightTable	= new RegExState(m_nNextStateID++);
	
	LeftTable->AddTransition(EPSILON, ((*(UpperTable.begin()))));
	LeftTable->AddTransition(EPSILON, ((*(LowerTable.begin()))));
	((*(UpperTable.rbegin())))->AddTransition(EPSILON, RightTable);
	((*(LowerTable.rbegin())))->AddTransition(EPSILON, RightTable);

	LowerTable.push_back(RightTable);
	UpperTable.insert(UpperTable.begin(),LeftTable);
	UpperTable.insert(UpperTable.end(), LowerTable.begin(), LowerTable.end());

	m_CharacterClassStack.push(UpperTable);
	return true;
}
void RegEx::EpsilonClosure(std::set<RegExState*> startSet, std::set<RegExState*> &result) {
	std::stack<RegExState*> UnVisitedStates;	
	result.clear();	
	result = startSet; 
	StateIterator iter;
	for(iter = startSet.begin(); iter != startSet.end(); ++iter) {
		UnVisitedStates.push(*iter);
	}

	while(!UnVisitedStates.empty()) {
		RegExState* curState = UnVisitedStates.top();
		UnVisitedStates.pop();

		Table epsilonStates;
		curState->GetTransition(EPSILON, epsilonStates);

		TableIterator epsilonItr;

		for ( epsilonItr = epsilonStates.begin(); epsilonItr != epsilonStates.end(); ++epsilonItr ){
			if(result.find((*epsilonItr)) == result.end()) {
				result.insert((*epsilonItr));
				UnVisitedStates.push((*epsilonItr));
			}
		}
	}
}

void RegEx::Move(char chInput, std::set<RegExState*> NFAState, std::set<RegExState*> &Result) {

	Result.clear();
	StateIterator iter;
	for(iter = NFAState.begin(); iter != NFAState.end(); ++iter) {
		Table States;
		(*iter)->GetTransition(chInput, States);
		for(int index = 0; index < (int)States.size(); ++index){
			Result.insert(States[index]);
		}
	}
}

void RegEx::ConvertNFAtoDFA() {
	for(int i = 0; i < (int)m_DFATable.size(); ++i)
		delete m_DFATable[i];
	m_DFATable.clear();

	if(m_NFATable.size() == 0)
		return;
	
	std::set<RegExState*> NFAStartStateSet;
	NFAStartStateSet.insert(m_NFATable[0]);

	std::set<RegExState*> DFAStartStateSet;
	EpsilonClosure(NFAStartStateSet, DFAStartStateSet);

	m_nNextStateID = 0;
	RegExState *DFAStartState = new RegExState(DFAStartStateSet, m_nNextStateID++);

	m_DFATable.push_back(DFAStartState);

	Table UnVisitedStates;
	UnVisitedStates.push_back(DFAStartState);
	while(!UnVisitedStates.empty()) {

		RegExState* CurDFAState = UnVisitedStates[UnVisitedStates.size()-1];
		UnVisitedStates.pop_back();

		std::set<char>::iterator iter;
		for(iter = m_InputSet.begin(); iter != m_InputSet.end(); ++iter) {
			std::set<RegExState*> MoveRes, EpsilonClosureRes;

			Move(*iter, CurDFAState->GetNFAState(), MoveRes);
			EpsilonClosure(MoveRes, EpsilonClosureRes);

			StateIterator MoveResItr;
			StateIterator EpsilonClosureResItr;

			bool bFound = false;
			RegExState *s   = NULL;
			for(int i = 0; i < (int)m_DFATable.size(); ++i) {
				s = m_DFATable[i];
				if(s->GetNFAState() == EpsilonClosureRes) {
					bFound = true;
					break;
				}
			}
			if(!bFound) {
				RegExState* U = new RegExState(EpsilonClosureRes, m_nNextStateID++);
				UnVisitedStates.push_back(U);
				m_DFATable.push_back(U);                                
				CurDFAState->AddTransition(*iter, U);
			} else {
				CurDFAState->AddTransition(*iter, s);
			}
		}
	}	
	ReduceDFA();
}
void RegEx::ReduceDFA() {
	std::set<RegExState*> DeadEndSet;
	for(int i = 0; i < (int)m_DFATable.size(); ++i) {
		if(m_DFATable[i]->IsDeadEnd()) {
			DeadEndSet.insert(m_DFATable[i]);
		}
	}
	if(DeadEndSet.empty())
		return;
	StateIterator iter;
	for(iter = DeadEndSet.begin(); iter != DeadEndSet.end(); ++iter) {
		for(int i = 0; i < (int)m_DFATable.size(); ++i)
			m_DFATable[i]->RemoveTransition(*iter);

		TableIterator pos;
		for(pos = m_DFATable.begin(); pos != m_DFATable.end(); ++pos) {
			if(*pos == *iter) {
				break;
			}
		}
		m_DFATable.erase(pos);
		delete *iter;
	}
}

void RegEx::MinimizeDFA () {
}

std::string RegEx::PreProcessBracket( std::string strRegEx) {
	std::string::size_type startPos, endPos, separatorPos;
	std::string ReplacedStrRegEx;

	startPos = strRegEx.find_first_of("[");
	endPos   = strRegEx.find_first_of("]");
	separatorPos = strRegEx.find_first_of("-");

	if ( startPos == std::string::npos || endPos == std::string::npos )
		return strRegEx;

	ReplacedStrRegEx += strRegEx.substr( 0, startPos );
	ReplacedStrRegEx.push_back('(');
	std::string result = strRegEx.substr( startPos + 1, endPos - startPos - 1);
	char first = result[0];
	char last  = result[result.size() - 1 ];

	if ( separatorPos != std::string::npos ) {		
		while ( first != last ) {
			ReplacedStrRegEx.push_back(first);
			ReplacedStrRegEx += "|";
			first++;
		}
		ReplacedStrRegEx.push_back(first);				
	} else {
		startPos++;
		while ( startPos != endPos - 1) {
			ReplacedStrRegEx.push_back(strRegEx[startPos]);
			ReplacedStrRegEx += "|";
			startPos++;
		}
		ReplacedStrRegEx.push_back(strRegEx[endPos - 1]);
	}
	ReplacedStrRegEx += strRegEx.substr( endPos + 1, strRegEx.size() - endPos );
	ReplacedStrRegEx += ")";

	return ReplacedStrRegEx;
}

bool RegEx::Compile(std::string strRegEx) {

	while ( strRegEx.find( "[" ) != std::string::npos ) {
		strRegEx = PreProcessBracket(strRegEx);
	}

	m_InfixRegEx = const_cast<char*>(strRegEx.c_str());
	CleanUp();
	
	if(!ConstructThompsonNFA())
		return false;	

	PopTable( m_NFATable);
	m_NFATable[m_NFATable.size() - 1 ]->m_bAcceptingState = true;
	
	ConvertNFAtoDFA();
	MinimizeDFA();
	return true;
}

bool RegEx::Match(std::string strText) {
	m_strText = strText;
	RegExState *pState = m_DFATable[0];
	std::vector<RegExState*>  Transition;
	for(int i = 0; i < (int)m_strText.size(); ++i) {
		char CurrChar = m_strText[i];		
		pState->GetTransition(CurrChar, Transition);
		if ( Transition.empty())
			return false;
		pState = Transition[0];
		if ( pState->m_bAcceptingState )
			return true;
		
	}
	return false;
}

int RegEx::PreProcessLiterals() {
  if (m_CurPreProcChar == '\0') 
	  return -1;
  if (m_CurPreProcChar == '\\') {
    m_CurPreProcChar = *(m_InfixRegEx++);
    if (m_CurPreProcChar == '\0') 
		return -1;
    m_PostStrRegEx += m_CurPreProcChar;
    m_CurPreProcChar = *(m_InfixRegEx++);
  } else if (strchr("()|*+?", m_CurPreProcChar) == NULL) { 
    if (m_CurPreProcChar == '&')
		m_PostStrRegEx += "\\&";
    else 
		m_PostStrRegEx += m_CurPreProcChar;

    m_CurPreProcChar = *(m_InfixRegEx++);
  } else if (m_CurPreProcChar == '(') {
    m_CurPreProcChar = *(m_InfixRegEx++);
    PreProcessOr();
    if (m_CurPreProcChar != ')') 
		return -1;
    m_CurPreProcChar = *(m_InfixRegEx++);
  } else
		return -1;
  return 0;
}
int RegEx::PreProcessClosure() {
	PreProcessLiterals();
	while (m_CurPreProcChar != '\0' && strchr("*+?",m_CurPreProcChar) != NULL) {
		m_PostStrRegEx += m_CurPreProcChar;
		m_CurPreProcChar = *(m_InfixRegEx++);
	}
	return 0;
}
int RegEx::PrePreprocessConcatenation(void) {
	PreProcessClosure();
	while (strchr(")|*+?", m_CurPreProcChar) == NULL) { 
		PreProcessClosure();
		m_PostStrRegEx += "&";
	}
	return 0;
}
int RegEx::PreProcessOr() {
	PrePreprocessConcatenation();
	while (m_CurPreProcChar == '|') {
		m_CurPreProcChar = *(m_InfixRegEx++);
		PrePreprocessConcatenation();
		m_PostStrRegEx += "|";
	}
	return 0;
}

int RegEx::CovertToPostfix() {
	m_CurPreProcChar = *(m_InfixRegEx++);
	PreProcessOr();
	return 0;
}
bool RegEx::ConstructThompsonNFA() {
	CovertToPostfix();
	for(int i = 0; i < (int)m_PostStrRegEx.size(); ++i) {
		char curChar = m_PostStrRegEx[i];
		if ( IsInput( curChar )) {
			PushOnCharacterStack( curChar );
		} else if ( IsMetaChar( curChar ) ) {
			switch ( curChar ) {
				case CLOSURE:
					Closure();
					break;
				case PLUS_CLOSURE:
					ClosurePlus();
					break;
				case OPTIONAL:
					ClosureOptional();
					break;
				case OR:
					Or();
					break;
				case FALSE_CONCAT:
					Concatenate();
					break;
			}
		}		
	}
	return true;
}


void RegEx::PrintTable(Table &table) {
	std::string TableString;
	for(int i = 0; i < (int)table.size(); ++i) {
		RegExState *pState = table[i];
		if(pState->m_bAcceptingState) {
			TableString += "\t" + pState->getStringID() + "\t\n";
		}
	}
	TableString += "\n";
	for( int i = 0; i < (int)table.size(); ++i) {
		RegExState *pState = table[i];
		std::vector<RegExState*> State;
		pState->GetTransition(EPSILON, State);
		for(int j = 0; j < (int)State.size(); ++j) {
			TableString += "\t" + pState->getStringID() + " -> " + State[j]->getStringID();
			TableString += "\t[label = \"epsilon \"]\n";
		}
		std::set<char>::iterator iter;
		for(iter = m_InputSet.begin(); iter != m_InputSet.end(); ++iter) {
			pState->GetTransition(*iter, State);
			for(int j = 0; j < (int)State.size(); ++j) {
				TableString += "\t" + pState->getStringID() + " -> " + State[j]->getStringID();
				std::stringstream out;
				out << *iter;
				TableString += "\t[label = \"" + out.str() + " \"]\n";
			}
		}
	}
	std::cout << TableString;
}
      
