This is a Lexer+Parser built for parsing MySQL files and verifying their correctness.
The Lexer.java creates an exe that tokenizes all the strings in the .sql file
Then the parser.cpp takes in the tokens, production rules, and a list of terminals to parse the whole tokenized output.
This uses the LL1 parsing table and in the process, creates various tables including the "firstpos", "followpos", "nullables", etc.
Using that the text is then parsed.

To run the whole pipelne, use the compile.bat.
Usage: compile <sql_filename>
