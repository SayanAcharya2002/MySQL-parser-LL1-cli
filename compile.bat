@echo off
set file_name=%1
type %file_name% > lexerinput.txt
javac Lexer.java
java Lexer
g++ parser.cpp -o parser.exe
parser.exe productions_new.txt terminals_new.txt lexeroutput.txt