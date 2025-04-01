#include<bits/stdc++.h>
#include<iomanip>
using namespace std;

//this is the tokens list using enums for better readability
enum TOKEN {
    // keywords
    TOKEN_SELECT,
    TOKEN_FROM,
    TOKEN_WHERE,
    TOKEN_INSERT,
    TOKEN_INTO,
    TOKEN_VALUES,
    TOKEN_ORDER,
    TOKEN_BY,
    TOKEN_GROUP,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_DELETE,
    // identifiers, numbers and strings
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    // operators and punctuation
    TOKEN_EQ,       // =
    TOKEN_GT,       // >
    TOKEN_LT,       // <
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_COMMA,    // ,
    TOKEN_SEMICOLON,// ;
    // end-of-file and error
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

//convert token types to string for display.
string tokenToString(TOKEN type) {
    switch(type) {
        case TOKEN_SELECT: return "SELECT";
        case TOKEN_FROM: return "FROM";
        case TOKEN_WHERE: return "WHERE";
        case TOKEN_INSERT: return "INSERT";
        case TOKEN_INTO: return "INTO";
        case TOKEN_VALUES: return "VALUES";
        case TOKEN_ORDER: return "ORDER";
        case TOKEN_BY: return "BY";
        case TOKEN_GROUP: return "GROUP";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";
        case TOKEN_DELETE: return "DELETE";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EQ: return "EQ";
        case TOKEN_GT: return "GT";
        case TOKEN_LT: return "LT";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}


// ------------------------------------------------------------------
// Token Structure
// ------------------------------------------------------------------
struct Token {
    TOKEN type;     // the type of the token
    string lexeme;  // the text of the token
    int line;     // line number in the input
    int column;   // column number in the input
};

// ------------------------------------------------------------------
// scans the SQL query input(file fully taken as string input) and generates tokens
// ------------------------------------------------------------------
class Lexer {
public:
    Lexer(const string &input){
        this->input = input;
        pos = 0;
        // initialize the line and column numbers.
        line = 1;
        column = 1;
    }
    // the next token from the input.
    Token getNextToken() {
        rWhiteSpace();

        int tokenLine = line;
        int tokenColumn = column;

        if (pos >= input.size())
         return {TOKEN_EOF, "", tokenLine, tokenColumn}; // end of input

        char current = input[pos];

        // handle punctuation and single-character tokens.
        if (current == '(') { advance(); return {TOKEN_LPAREN, "(",tokenLine,tokenColumn}; }
        if (current == ')') { advance(); return {TOKEN_RPAREN, ")",tokenLine,tokenColumn}; }
        if (current == ',') { advance(); return {TOKEN_COMMA, ",",tokenLine,tokenColumn}; }
        if (current == ';') { advance(); return {TOKEN_SEMICOLON, ";",tokenLine,tokenColumn}; }
        if (current == '=') { advance(); return {TOKEN_EQ, "=",tokenLine,tokenColumn}; }
        if (current == '>') { advance(); return {TOKEN_GT, ">",tokenLine,tokenColumn}; }
        if (current == '<') { advance(); return {TOKEN_LT, "<",tokenLine,tokenColumn}; }

        // check of string literals: assuming SQL strings use single quotes.
        if (current == '\'') {
            advance();// skip the opening quote
            string temp;
            while (pos < input.size() && input[pos] != '\'') {
                temp.push_back(input[pos]);
                advance();
            }
            if (pos < input.size() && input[pos] == '\'') {
                advance();// skip the closing quote
                return {TOKEN_STRING, temp, tokenLine, tokenColumn}; // return the string literal
            } else {
                // error: unmatched quote.
                return {TOKEN_UNKNOWN, temp, tokenLine, tokenColumn};
            }
        }

        // handle numeric literals.
        if (isDigit(current)) {
            string num;
            while (pos < input.size() && isDigit(input[pos])) {
                num.push_back(input[pos]);
                advance();
            }
            return {TOKEN_NUMBER, num,tokenLine,tokenColumn};
        }// floats,double handling later
        //also comments 

        // handle identifiers and keywords.
        if (isAlphabet(current)|| current == '_') {
            string ident;
            while (pos < input.size() && (isAlphaNumeric(input[pos]) || input[pos] == '_')) {
                ident.push_back(input[pos]);
                advance();
            }
            // Convert to uppercase for case-insensitive keyword matching.
            string upperIdent = toUpper(ident);
            if (keywords.find(upperIdent) != keywords.end()) {
                return {keywords[upperIdent], ident,tokenLine, tokenColumn}; // return the keyword token
            }
            return {TOKEN_IDENTIFIER, ident,tokenLine, tokenColumn}; // return the identifier token
        }

        //could have also done these checks using regex but i am trying to avoid regex as much as possible

        // if no known pattern matches, return an unknown token.
        advance(); // move to the next character
        string temp;
        temp.push_back(current);
        return {TOKEN_UNKNOWN, temp, tokenLine, tokenColumn};  // convert char to string
    }

private:
    string input;
    size_t pos;
    int line;   // current line number
    int column; // current column number

    // skips whitespace (spaces, tabs, newlines).
    
    void advance(){
        if (pos < input.size()) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
    }



    void rWhiteSpace() {
        while (pos < input.size() && isSpace(input[pos])) {
           advance(); // skip whitespace
        }
    }
    bool isSpace(char ch) {
        // check for space, horizontal tab, newline,
        return (ch == ' '  || ch == '\t' || ch == '\n'|| ch == '\r');
    }
    
    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }
    bool isAlphabet(char c) {
        return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
    }
    bool isAlphaNumeric(char c) {
        return isAlphabet(c) || isDigit(c);
    }

    // converts a string to uppercase for flexible keyword matching.
    string toUpper(const string &s) {
        string result = s;
        for (auto &ch : result){
            if(ch>='a'&&ch<='z')
                ch=ch-'a'+'A';
        }
        return result;
    }

    // Mapping of SQL keywords to token types.
    map<string, TOKEN> keywords = {
        {"SELECT", TOKEN_SELECT},
        {"FROM",   TOKEN_FROM},
        {"WHERE",  TOKEN_WHERE},
        {"INSERT", TOKEN_INSERT},
        {"INTO",   TOKEN_INTO},
        {"VALUES", TOKEN_VALUES},
        {"ORDER",  TOKEN_ORDER},
        {"BY",     TOKEN_BY},
        {"GROUP",  TOKEN_GROUP},
        {"AND",    TOKEN_AND},
        {"OR",     TOKEN_OR},
        {"NOT",    TOKEN_NOT},
        {"DELETE", TOKEN_DELETE},
    };
};


// reads input from a file and prints tokens.
int main() {
    string filename = "sample.sql";
    ifstream file(filename);
    if (!file) {
        cerr << "Cannot open file: " << filename << endl;
        return 1;
    }
    string input;
    char ch;
    while (file.get(ch)) {
        input.push_back(ch);
    }
    file.close();

    Lexer lexer(input);
    vector<Token> tokens;
    Token token;
    do {
        token = lexer.getNextToken();
        tokens.push_back(token);
    } while (token.type != TOKEN_EOF);

    // print the tokens.
    for (const auto &t : tokens) {
        cout << "Token: " << tokenToString(t.type) << " [ Lexeme: " << t.lexeme << " ]"<< endl;
           cout << "[ Line: " << t.line << ", Column: " << t.column 
             << " ]" << endl;
        // cout<<t.column<<endl;
    }

    return 0;
}
