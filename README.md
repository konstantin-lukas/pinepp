# PINE++
PINE++ stands for "Programming Interface for New and Easy C++" and is a general 
utility library written in modern C++ providing functions and data structures that are
missing from the standard library (like a trie), as well as utilities (like base64) for 
implementing simple programs quickly.

## Feature List
- base64: for encoding and decoding base64
- bit_pattern: for doing bitwise operations on long bit patterns
- timer: an easy-to-use interface for measuring time with clock_gettime
- trie: a data structure for storing strings without duplicates allowing for constant time lookup
- static_trie: a slightly optimized version of a trie with a fixed string length and alphabet
- fetch: an interface for making HTTP requests
- print_iterable: easily print an iterable container to an ostream or directly to stdout
- is_class: a utility for testing if a type is primitive or not

See documentation for more information.

Tests are written for Google test. For compilation instructions refer to CMakeLists.txt