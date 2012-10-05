# regex

## How to build

For `Windows` Visual Studio Solution files are available in `win` directory. For `Unix` run `makefile` from the root directory. 

## Usage

```
  RegEx re;
  re.Compile("(a|b)*");
  bool match = re.Match("a");
  match  = re.Match("b"));
  match = re.Match ("aa");
  match = re.Match ("ab");
```

This library support basic regular expression operator ``*`, |, +, (, ), ?.`

## Theory

`regex` library construct `NFA` from `Regular Expression` using system called `Thompson's Construction`. It works as follows

`Simplest Regular Expression` is defined by following machine. 

![1](https://raw.github.com/davinash/regex/master/doc/images/1.png)

`Concatenation` is defined by following machine. 

![2](https://raw.github.com/davinash/regex/master/doc/images/2.png)

`OR` is defined by following machine. 

![3](https://raw.github.com/davinash/regex/master/doc/images/3.png)

`*` closure operation is defined by following machine. 

![4_star](https://raw.github.com/davinash/regex/master/doc/images/4_star.png)

`+` closure operation is defined by following machine.

![5_plus](https://raw.github.com/davinash/regex/master/doc/images/5_plus.png)

`?` closure operation is defined by following machine. 

![6_question](https://raw.github.com/davinash/regex/master/doc/images/6_question.png)

Once the `NFA` is created, we can apply greedy method to find the matching pattern. But it is quite heavy. Convert this `NFA` into `DFA` using subset construction mechanism and remove the dead state.

Constructed `DFA` may not be always optimal and minimum one, New `DFA` can be optimize using `Hopcroft Minimization Algorithm` or `Brzozowski's algorithm`. 

## Example of Regular Expression to DFA

Let us convert `(a|b)*abb` into `DFA`

`NFA` for simple regular expressions will be as follows. 

![s1](https://raw.github.com/davinash/regex/master/doc/images/s1.png)

![s2](https://raw.github.com/davinash/regex/master/doc/images/s2.png)

![s3](https://raw.github.com/davinash/regex/master/doc/images/s3.png)

`NFA` for `(a|b)*` will be as follows 

![s4](https://raw.github.com/davinash/regex/master/doc/images/s4.png)

`NFA` after concatenating with `abb` will be as follows 

![s5](https://raw.github.com/davinash/regex/master/doc/images/s5.png)


