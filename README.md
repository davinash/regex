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

Simplest Regular Expression is defined by following machine. 

