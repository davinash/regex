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
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "regex.h"
int main( int argc, char **argv ) {
	RegEx re;
	re.Compile("abcd");
	assert ( true == re.Match("abcd"));
	assert ( true == re.Match("abcde"));
	assert ( false == re.Match("acde"));

	re.Compile("ab*");
	assert ( true == re.Match("ac"));
	assert ( true == re.Match("abc"));
	assert ( true == re.Match("abbc"));
	assert ( true == re.Match("acbb"));
	assert ( false == re.Match("xcbb"));

	re.Compile("ab+");
	assert ( true == re.Match("abc"));
	assert ( true == re.Match("abbc"));
	assert ( false == re.Match ("ac"));

	re.Compile("ab?");
	assert ( true == re.Match("ac"));
	assert ( true == re.Match("abc"));
	assert ( true == re.Match ("abbc"));

	re.Compile("a|b*");
	assert ( true == re.Match("a"));
	assert ( true == re.Match("b"));
	assert ( true == re.Match ("bbb"));
	assert ( true == re.Match ("bb"));
		
	re.Compile("(a|b)*");
	assert ( true == re.Match("a"));
	assert ( true == re.Match("b"));
	assert ( true == re.Match ("aa"));
	assert ( true == re.Match ("ab"));
	assert ( true == re.Match ("ba"));
	assert ( true == re.Match ("bb"));
	assert ( true == re.Match ("aaa"));

	re.Compile("a*b");
	assert ( false == re.Match("a"));
	assert ( true == re.Match("ab"));
	assert ( true == re.Match("aaaabbbcccb"));
	assert ( false == re.Match("a12344b"));
	assert ( false == re.Match("a@#$@#$b"));
	assert ( true == re.Match("b"));

	re.Compile("(a|b)*abb");
	assert ( true == re.Match("abb"));
	assert ( true == re.Match("aabb"));
	assert ( true == re.Match("babb"));
	assert ( true == re.Match("aaabb"));
	assert ( true == re.Match("bbabb"));
	assert ( true == re.Match("ababb"));
	assert ( true == re.Match("aababb"));

	re.Compile("ab[0-9]");
	assert ( true == re.Match("ab0"));
	assert ( true == re.Match("ab1"));
	assert ( true == re.Match("ab2"));
	assert ( true == re.Match("ab3"));
	assert ( true == re.Match("ab4"));
	assert ( true == re.Match("ab5"));
	assert ( true == re.Match("ab6"));
	assert ( true == re.Match("ab7"));
	assert ( true == re.Match("ab8"));
	assert ( true == re.Match("ab9"));
	assert ( true == re.Match("ab199"));

	re.Compile("XYZ[aeiou]");
	assert ( true == re.Match("XYZa"));
	assert ( true == re.Match("XYZe"));
	assert ( true == re.Match("XYZi"));
	assert ( true == re.Match("XYZo"));
	assert ( true == re.Match("XYZu"));
	assert ( false == re.Match("XYZb"));

	return 0;
}
