import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.io.*;

class SQLTokenizer {
    private final String input;
    private int currentPosition = 0;

    public SQLTokenizer(String input) {
        this.input = input;
    }

    public List<String> tokenize() {
        List<String> tokens = new ArrayList<>();

        while (currentPosition < input.length()) {
            char currentChar = input.charAt(currentPosition);


            if (Character.isWhitespace(currentChar)) {
                // skip whitespace
                currentPosition++;
                continue;
            } 
            else if((currentChar == '<' ||currentChar == '>')&&(currentPosition+1<input.length() && input.charAt(currentPosition+1)=='=')){
                currentPosition+=2;
                tokens.add(currentChar+"=");
            }
            else if (currentChar == '(' || currentChar == ')' || currentChar == ',' || currentChar == ';'
                    || currentChar == '=' || currentChar == '*'||currentChar=='<'||currentChar=='>') {
                tokens.add(Character.toString(currentChar).trim());
                currentPosition++;
                continue;
            } else if (currentChar == '"') {
                // string literals
                int endPosition = input.indexOf('"', currentPosition + 1);
                if (endPosition == -1) {
                    throw new IllegalArgumentException("Unclosed string literal at position " + currentPosition);
                }
                String literal = input.substring(currentPosition + 1, endPosition);
                tokens.add("'" + literal + "'");
                currentPosition = endPosition + 1;
                continue;
            } else {

                // identifiers
                int endPosition = currentPosition;
                while (endPosition < input.length() && isValidIdentifierChar(input.charAt(endPosition))) {
                    endPosition++;
                }
                String identifier = input.substring(currentPosition, endPosition);
                if (identifier.equalsIgnoreCase("create") || identifier.equalsIgnoreCase("insert")
                        || identifier.equalsIgnoreCase("into") || identifier.equalsIgnoreCase("table")
                        || identifier.equalsIgnoreCase("between")
                        || identifier.equalsIgnoreCase("in") || identifier.equalsIgnoreCase("like")
                        || identifier.equalsIgnoreCase("select")
                        || identifier.equalsIgnoreCase("where") || identifier.equalsIgnoreCase("is")) {
                    tokens.add(identifier.toLowerCase().trim());
                } else {
                    tokens.add(identifier.trim());
                }
                currentPosition = endPosition;
            }
        }

        return tokens;
    }

    private boolean isValidIdentifierChar(char c) {
        return Character.isLetterOrDigit(c) || c == '_' || c == '$' || c == '.';
    }
}

class SQLLexer {

    private HashSet<String> Keywords = new HashSet<>();
    private HashSet<String> Operator = new HashSet<>();

    public SQLLexer() {

        // Initizilsing keywords set

        Collections.addAll(Keywords, "SELECT", "FROM", "WHERE", "AND", "OR", "NOT",
                "ORDER", "BY", "AS", "DISTINCT", "UNION", "ALL", "VARCHAR", "CHAR", "INT", "DECIMAL", "FLOAT", "DOUBLE",
                "INSERT", "INTO", "VALUES", "UPDATE", "SET", "DELETE", "CREATE", "TABLE",
                "ALTER", "TABLE", "DROP", "TABLE", "PRIMARY", "KEY", "FOREIGN",
                "LIKE", "BETWEEN", "IN", "IS", "NULL", "NOT",
                "CASE", "WHEN", "THEN", "ELSE", "END", ";", ",", "(", ")", "*");

        Collections.addAll(Operator, "/", "%", "=", "<", ">", "!=", "|", "&", "^", "<=", ">=");
    }

    private String getCommand() throws IOException {
        File file = new File("lexerinput.txt");

        BufferedReader br = new BufferedReader(new FileReader(file));

        // Declaring a string variable
        String st;
        StringBuilder str = new StringBuilder();
        while ((st = br.readLine()) != null) {

            str.append(st+" ");
        }
        String input = str.toString();

        br.close();
        return input;
    }

    public boolean isInteger(String s) {
        try {
            Integer.parseInt(s);
        } catch (NumberFormatException e) {
            return false;
        } catch (NullPointerException e) {
            return false;
        }
        return true;
    }

    public boolean isDouble(String s) {
        try {
            Double.parseDouble(s);
        } catch (NumberFormatException e) {
            return false;
        } catch (NullPointerException e) {
            return false;
        }
        return true;
    }

    public List<String> lex() throws IOException {

        String command = getCommand();
        List<String> lexiomString = new ArrayList<>();
        SQLTokenizer tokenizer = new SQLTokenizer(command);
        List<String> tokens = tokenizer.tokenize();
        int i=1;
        for (String s : tokens) {

            // System.out.println(s);
            if (Keywords.contains(s.toUpperCase())) {
                lexiomString.add(s.toLowerCase() + " " + s.toLowerCase()+" "+i);
            } else if (Operator.contains(s)) {
                lexiomString.add("operator" + " " + s+" "+i);
            } else if (s.charAt(0) == '\'') {
                lexiomString.add("stringwithquotes" + " " + s+" "+i);
            } else if (isInteger(s)) {
                lexiomString.add("inttype" + " " + s+" "+i);
            } else if (isDouble(s)) {
                lexiomString.add("floattype" + " " + s+" "+i);
            } else {
                lexiomString.add("stringwithoutquotes" + " " + s+" "+i);
            }
            // System.out.println(lexiomString);
            i++;
        }

        return lexiomString;
    }
}

public class Lexer {

    public static void main(String[] args) throws IOException {

        SQLLexer lexer = new SQLLexer();
        List<String> lexioms = lexer.lex();
        FileWriter writer = new FileWriter("lexeroutput.txt"); 
        for(String str: lexioms ) {
            writer.write(str + System.lineSeparator());
        }
        writer.close();


    }
}
