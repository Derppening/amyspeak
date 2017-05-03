# Amyspeak

A C++ console program which could concatenate or split strings by patterns. 

## Getting Started

These instructions will set the project up and run the application on your 
local machine. Note that currently only Windows platform is officially 
tested and supported.

Linux support will be added once I get to it.

### Prerequisites

* [CMake 3.7.0+](https://cmake.org/download/)
* Any C++14-supported compiler 
    * Windows: [MinGW-w64 6.3.0](https://sourceforge.net/projects/mingw-w64/)

### Compiling and Running

Run CMake in the project root.
```
<path_to_cmake>\cmake.exe -G "CodeBlocks - MinGW Makefiles" .
```

Then run Make in the project root
```
make
```

Run the program with `amyspeak.exe --version` and ensure it outputs something 
similar to as follows:
```
Amyspeak 2.1.1
Copyright (C) 2017 David Mak
Licensed under MIT.
```

### Distributing the Binary

It is strongly recommended to distribute the static version of the binary 
(`amyspeak_static.exe`), as it bundles all necessary libraries to run the 
program. 

It is also recommended to attach a working copy of `patterns` and `tokens`, or 
link the user to this page for instructions to create their own.

## How This Works

### On The Surface

* In `merge` mode, this program reads all phrases entered by the user, and 
concatenates them into a long string, separated by spaces. 
    * Similar to reading all words, then putting them together to form a 
    sentence.
* In `delimit` (normal) mode, this program reads a string, breaks it down into
words, and merges certain combinations of words into a longer phrase.
    * The "certain combinations" are dictated by a text file (`patterns` by 
    default).

### Under the Hood

#### `merge` mode

This program pushes back any input string into a vector, then when user enters 
`:p` command, outputs all the strings in the vector separated by a space.

#### `delimit` mode

1. Upon launching the program, it reads all the words (referred to as `tokens`)
from `./tokens` or specified by an argument. These tokens are constructed into
a key-value map.
2. Program then reads all the combinations of words which should be merged, 
from `./patterns` or specified by an argument. These patterns are constructed 
into a vector.
3. When the user enters a sentence (or string), the program will do the following:
    * Break the string down by spaces and putting them into a vector
    * Loop through the input tokens, and attempt to match the next tokens with
    any token patterns
    * If a match is found, merge the two tokens together
    * Repeat the process until the vector of input tokens is processed

#### Format of `tokens`

* Every token category is separated by its header
    * Headers are defined by any string followed by a colon, e.g. `pronoun:`
    * The header name will be the above string without the colon
    * Subsequent lines without `:` are interpreted as words falling under that
    category
* Special headers:
    * `verb`
        * The first token of any line falling under the category is considered
        to be the header
        * Subsequent words (delimited by space) are interpreted as different 
        form of the same word
        * Example: The category `to_be` has the forms `be`, `am`, `are`...
    * `single_match`
        * Any token falling under this header guaranteed to merge with the 
        next token
* Any line appended by `!` will be treated specially and thus will not be 
interpreted
    * Currently, only the switch `VERSION` will be interpreted (`!VERSION=<ver_string>`)
* Any line appended by `//` will be ignored

#### Format of `patterns`

* Patterns are defined by a combination of space-delimited categories, along 
with special characters.
    * Wildcard (`*`): Matches any word
    * Literals (`"<word>"`): Matches `<word>` literally
    * Verb Wildcard (`to_*`): Matches any verb or their different forms
* Every pattern is separated by a newline (`\n`)
* Any line appended by `!` will be treated specially and thus will not be 
interpreted. Those include:
    * `l=<n>`: Only concatenate wildcards if the token has less than `<n>` 
    characters
    * `VERSION=<ver_string>`: Version of the file. Must be placed on the first 
    line.
* Any line appended by `//` will be ignored

## Versioning

There used to be no specific versioning system. 

Beginning with release 2.1.2, 
[SemVer](http://semver.org/) will be used. For all releases, see 
[tags on this repository](https://github.com/Derppening/amyspeak/tags).

## Project Development

### Origins of Amyspeak

Among the many people I have met on the Internet over the years, I have one 
particular friend who, due to a condition, types in a 'broken manner'. Where
most people are able to type coherent sentences without much effort, she would
take extra effort to do so, and thus she breaks the sentences into smaller 
phrases.

However, there are people who found interpreting this manner of typing as 
rather annoying, and thus I wrote the token-merging mode of the program to 
help her overcome this problem. 

Eventually, this project expanded to do the opposite, which aims to break 
longer sentences down to assist in her reading. Originally, the delimiting 
mode only breaks the sentence into words, but I decided that would be 
unnatural to read, and this resulted in the ability to merge certain words 
into a more coherent phrase. No more singular a's lying around!

### What Next?

Although this project is largely complete, it would be interesting to see if 
I am able to implement a learning algorithm to extract patterns from existing 
speech.

## Authors

* **David Mak** - [Derppening](https://github.com/Derppening)

## License

This project is licensed under the MIT license - see 
[LICENSE](https://github.com/Derppening/amyspeak/blob/master/LICENSE) for 
details.

## Acknowledgements

* Amy, for inspiring me to make this project
* Anyone who has helped me or offered me suggestions
