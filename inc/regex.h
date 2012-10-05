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
#ifndef _REG_EX_H_
#define _REG_EX_H_

#include <stack>
#include <set>
#include <list>
#include <string>
#include <vector>
#include <map>

#include "regexstate.h"


class RegEx {
public:
	RegEx();
	~RegEx();

	bool Compile(std::string strRegEx);
	bool Match(std::string strText);
	typedef std::vector<RegExState*> Table;
	typedef Table::reverse_iterator TableReverseIterator;
	typedef Table::iterator TableIterator; 
	typedef std::set<RegExState*>::iterator StateIterator;



private:
	Table m_NFATable;
	Table m_DFATable;

	std::stack<Table >       m_CharacterClassStack;
	std::stack<char>         m_ExpressionStack;
	std::set<char>           m_InputSet;
	std::string              m_strText;
	std::string              m_PostStrRegEx;
	char*                    m_InfixRegEx;
	char                     m_CurPreProcChar;
	int                      m_nNextStateID;

	bool ConstructThompsonNFA();
	void PushOnCharacterStack(char chInput);
	bool PopTable(Table  &NFATable);
	bool Concatenate();
	bool Closure();
	bool ClosureOptional();
	bool ClosurePlus();
	bool Or();
	bool IsMetaChar(char inputCh); 
	bool IsInput(char inputCh);
	bool IsLeftParan(char inputCh); 
	bool IsRightParan(char inputCh); 
	void EpsilonClosure(std::set<RegExState*> T, std::set<RegExState*> &Res);
	void Move(char chInput, std::set<RegExState*> T, std::set<RegExState*> &Res);
	void ConvertNFAtoDFA();
	void ReduceDFA();
	void CleanUp();

	int CovertToPostfix();
	int PreProcessLiterals();
	int PreProcessClosure();
	int PrePreprocessConcatenation();
	int PreProcessOr();
	std::string PreProcessBracket( std::string strRegEx);
	void MinimizeDFA ();

	void PrintTable(Table &table);

protected:
};
#endif