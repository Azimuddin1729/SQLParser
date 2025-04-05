// cfg for sql (we are following):

//bnf form:

// <SQL> -> <STMT> |<STMT> <SQL>
// <STMT> -> <SELECT_STMT> | <INSERT_STMT> | <DELETE_STMT>

// <SELECT_STMT> -> SELECT <LIST> FROM <TABLE> <WHERE_OPT> <ORDER_BY_OPT> <GROUP_BY_OPT>;

// <LIST> -> * | <ATTR_LIST>

// <TABLE> -> IDENTIFIER

// <ATTR_LIST> -> IDENTIFIER| <ATTR_LIST>,IDENTIFIER



// <WHERE_OPT> -> WHERE <CONDITION> | ε

//<CONDITION> -> <CONDITION> OR <AND_CONDITION> | <AND_CONDITION>

// <AND_CONDITION> -> <AND_CONDITION> AND <NOT_CONDITION> | <NOT_CONDITION>

// <NOT_CONDITION> -> NOT <NOT_CONDITION> | (<CONDITION>) | IDENTIFIER <OP> <LITERAL>


// <OP> -> = | > | <
// <LITERAL> -> NUMBER | STRING

// <ORDER_BY_OPT> -> ORDER BY <LIST> | ε
// <GROUP_BY_OPT> -> GROUP BY <LIST> | ε

//<INSERT_STMT> -> INSERT INTO <TABLE> (LIST) VALUES (<VALUE_LIST>) ;

// <INSERT_STMT> -> INSERT INTO <TABLE> VALUES (<VALUE_LIST>) ;

// <VALUE_LIST> -> <LITERAL> | <VALUE_LIST>,<LITERAL>


// <DELETE_STMT> -> DELETE FROM <TABLE> WHERE IDENTIFIER <OP> <LITERAL>;



//LL(1) compatible:


//<SQL> -> <STMT> <SQL_TAIL>
//<SQL_TAIL> -> <STMT><SQL_TAIL> | ε

//<STMT> -> <SELECT_STMT> | <INSERT_STMT> | <DELETE_STMT>

//<SELECT_STMT> -> SELECT <LIST> FROM <TABLE> <WHERE_OPT> <ORDER_BY_OPT> <GROUP_BY_OPT>;

// <LIST> -> * | <ATTR_LIST>

// <TABLE> -> IDENTIFIER

// <ATTR_LIST> → IDENTIFIER <ATTR_LIST_TAIL>
// <ATTR_LIST_TAIL> → , IDENTIFIER <ATTR_LIST_TAIL> | ε

// <WHERE_OPT> -> WHERE <CONDITION> | ε

// <CONDITION> → <AND_CONDITION> <CONDITION_TAIL>
// <CONDITION_TAIL> → OR <AND_CONDITION> <CONDITION_TAIL> | ε

// <AND_CONDITION> → <NOT_CONDITION> <AND_CONDITION_TAIL>
// <AND_CONDITION_TAIL> → AND <NOT_CONDITION> <AND_CONDITION_TAIL> | ε

// <NOT_CONDITION> → NOT <NOT_CONDITION> | (<CONDITION>) | IDENTIFIER <OP> <LITERAL>

// <OP> → = | > | <

// <LITERAL> → NUMBER | STRING

// <ORDER_BY_OPT> → ORDER BY <LIST> | ε
// <GROUP_BY_OPT> → GROUP BY <LIST> | ε

//<INSERT_STMT> -> INSERT INTO <TABLE> <INSERT_TAIL>
//<INSERT_TAIL> -> (LIST) VALUES (<VALUE_LIST>) ; | VALUES (<VALUE_LIST>) ;

//<VALUE_LIST> → <LITERAL> <VALUE_LIST_TAIL>
//<VALUE_LIST_TAIL> → , <LITERAL> <VALUE_LIST_TAIL> | ε

//<DELETE_STMT> -> DELETE FROM <TABLE> WHERE IDENTIFIER <OP> <LITERAL>;



#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <string>
#include "lexer.h"  
using namespace std;

struct Production {
    string lhs;
    vector<string> rhs;
};

vector<Production> productions = {
    // <SQL> → <STMT> <SQL_TAIL>
    {"<SQL>", {"<STMT>", "<SQL_TAIL>"}},
    
    // <SQL_TAIL> → <STMT> <SQL_TAIL>
    {"<SQL_TAIL>", {"<STMT>", "<SQL_TAIL>"}},
    
    // <SQL_TAIL> → ε
    {"<SQL_TAIL>", {"ε"}},

    //case of sqltail with eof
    // {"<SQL_TAIL>", {"$"}},
    
    // <STMT> → <SELECT_STMT>
    {"<STMT>", {"<SELECT_STMT>"}},
    
    // <STMT> → <INSERT_STMT>
    {"<STMT>", {"<INSERT_STMT>"}},
    
    // <STMT> → <DELETE_STMT>
    {"<STMT>", {"<DELETE_STMT>"}},
    
    // <SELECT_STMT> → SELECT <LIST> FROM <TABLE> <WHERE_OPT> <ORDER_BY_OPT> <GROUP_BY_OPT> ;
    {"<SELECT_STMT>", {"SELECT", "<LIST>", "FROM", "<TABLE>", "<WHERE_OPT>", "<ORDER_BY_OPT>", "<GROUP_BY_OPT>", ";"}},
    
    // <LIST> → *
    {"<LIST>", {"*"}},
    
    // <LIST> → <ATTR_LIST>
    {"<LIST>", {"<ATTR_LIST>"}},
    
    // <TABLE> → IDENTIFIER
    {"<TABLE>", {"IDENTIFIER"}},
    
    // <ATTR_LIST> → IDENTIFIER <ATTR_LIST_TAIL>
    {"<ATTR_LIST>", {"IDENTIFIER", "<ATTR_LIST_TAIL>"}},
    
    // <ATTR_LIST_TAIL> → , IDENTIFIER <ATTR_LIST_TAIL>
    {"<ATTR_LIST_TAIL>", {",", "IDENTIFIER", "<ATTR_LIST_TAIL>"}},
    
    // <ATTR_LIST_TAIL> → ε
    {"<ATTR_LIST_TAIL>", {"ε"}},
    
    // <WHERE_OPT> → WHERE <CONDITION>
    {"<WHERE_OPT>", {"WHERE", "<CONDITION>"}},
    
    // <WHERE_OPT> → ε
    {"<WHERE_OPT>", {"ε"}},
    
    // <CONDITION> → <AND_CONDITION> <CONDITION_TAIL>
    {"<CONDITION>", {"<AND_CONDITION>", "<CONDITION_TAIL>"}},
    
    // <CONDITION_TAIL> → OR <AND_CONDITION> <CONDITION_TAIL>
    {"<CONDITION_TAIL>", {"OR", "<AND_CONDITION>", "<CONDITION_TAIL>"}},
    
    // <CONDITION_TAIL> → ε
    {"<CONDITION_TAIL>", {"ε"}},
    
    // <AND_CONDITION> → <NOT_CONDITION> <AND_CONDITION_TAIL>
    {"<AND_CONDITION>", {"<NOT_CONDITION>", "<AND_CONDITION_TAIL>"}},
    
    // <AND_CONDITION_TAIL> → AND <NOT_CONDITION> <AND_CONDITION_TAIL>
    {"<AND_CONDITION_TAIL>", {"AND", "<NOT_CONDITION>", "<AND_CONDITION_TAIL>"}},
    
    // <AND_CONDITION_TAIL> → ε
    {"<AND_CONDITION_TAIL>", {"ε"}},
    
    // <NOT_CONDITION> → NOT <NOT_CONDITION>
    {"<NOT_CONDITION>", {"NOT", "<NOT_CONDITION>"}},
    
    // <NOT_CONDITION> → ( <CONDITION> )
    {"<NOT_CONDITION>", {"(", "<CONDITION>", ")"}},
    
    // <NOT_CONDITION> → IDENTIFIER <OP> <LITERAL>
    {"<NOT_CONDITION>", {"IDENTIFIER", "<OP>", "<LITERAL>"}},
    
    // <OP> → =
    {"<OP>", {"="}},
    
    // <OP> → >
    {"<OP>", {">"}},
    
    // <OP> → <
    {"<OP>", {"<"}},
    
    // <LITERAL> → NUMBER
    {"<LITERAL>", {"NUMBER"}},
    
    // <LITERAL> → STRING
    {"<LITERAL>", {"STRING"}},
    
    // <ORDER_BY_OPT> → ORDER BY <LIST>
    {"<ORDER_BY_OPT>", {"ORDER", "BY", "<LIST>"}},
    
    // <ORDER_BY_OPT> → ε
    {"<ORDER_BY_OPT>", {"ε"}},
    
    // <GROUP_BY_OPT> → GROUP BY <LIST>
    {"<GROUP_BY_OPT>", {"GROUP", "BY", "<LIST>"}},
    
    // <GROUP_BY_OPT> → ε
    {"<GROUP_BY_OPT>", {"ε"}},
    
    // <INSERT_STMT> → INSERT INTO <TABLE> <INSERT_TAIL>
    {"<INSERT_STMT>", {"INSERT", "INTO", "<TABLE>", "<INSERT_TAIL>"}},
    
    // <INSERT_TAIL> → ( <ATTR_LIST> ) VALUES ( <VALUE_LIST> ) ;
    {"<INSERT_TAIL>", {"(", "<ATTR_LIST>", ")", "VALUES", "(", "<VALUE_LIST>", ")", ";"}},
    
    // <INSERT_TAIL> → VALUES ( <VALUE_LIST> ) ;
    {"<INSERT_TAIL>", {"VALUES", "(", "<VALUE_LIST>", ")", ";"}},
    
    // <VALUE_LIST> → <LITERAL> <VALUE_LIST_TAIL>
    {"<VALUE_LIST>", {"<LITERAL>", "<VALUE_LIST_TAIL>"}},
    
    // <VALUE_LIST_TAIL> → , <LITERAL> <VALUE_LIST_TAIL>
    {"<VALUE_LIST_TAIL>", {",", "<LITERAL>", "<VALUE_LIST_TAIL>"}},
    
    // <VALUE_LIST_TAIL> → ε
    {"<VALUE_LIST_TAIL>", {"ε"}},
    
    // <DELETE_STMT> → DELETE FROM <TABLE> WHERE IDENTIFIER <OP> <LITERAL> ;
    {"<DELETE_STMT>", {"DELETE", "FROM", "<TABLE>", "WHERE", "IDENTIFIER", "<OP>", "<LITERAL>", ";"}}
};


//now we need to compute the first and follow sets for the grammar

map<string, set<string>> firstSet;

map<string, set<string>> followSet;//these are cached for efficiency

bool isNonTerminal(const string &symbol) {
    return symbol[0] == '<' && symbol[symbol.size() - 1] == '>';
}


set<string> computeFirst(const string &Non_Terminal) {
    // if the first set is already computed, return it
    if (firstSet.find(Non_Terminal) != firstSet.end()) {
        return firstSet[Non_Terminal];
    }

    set<string> first;

    for(const auto &prod: productions){
        //checking if we directly get epsilon in any rule
        if(prod.lhs == Non_Terminal && prod.rhs[0] == "ε"){
            first.insert("ε");
        }

        else if(prod.lhs == Non_Terminal){
            //indirectly we may get epsilon overall too for that check we use allNullable bool

            bool allNullable = true;
            for(const auto &symbol: prod.rhs){
                if(isNonTerminal(symbol)){

                    set<string> firstofSymbol = computeFirst(symbol);

                    //dont insert epsilon in first set if we dont get all nullable in the terminals 

                    if(firstofSymbol.find("ε") != firstofSymbol.end()){

                        firstofSymbol.erase("ε");

                        first.insert(firstofSymbol.begin(), firstofSymbol.end());

                    }
                    else{
                        first.insert(firstofSymbol.begin(), firstofSymbol.end());

                        allNullable = false;
                        break;
                    }
                }
                else{

                    first.insert(symbol);
                    allNullable = false;
                    break;
                }
            }

            //if all symbols in the production are nullable, add epsilon to the first set
            if(allNullable){
                first.insert("ε");
            }

        }
    }
    // Cache the computed first set
    firstSet[Non_Terminal] = first;
    return first;
}


set<string> computeFollow(const string &Non_Terminal) {
    // if the follow set is already computed, return it
    if (followSet.find(Non_Terminal) != followSet.end()) {
        return followSet[Non_Terminal];
    }

    set<string> follow;

    // if the non-terminal is the start symbol, add $ to its follow set
    if (Non_Terminal == "<SQL>") {
        follow.insert("$");
    }

    //look for the non-terminal in the productions
    for (const auto &prod : productions) {
        for (int i = 0; i < prod.rhs.size(); ++i) {
            if (prod.rhs[i] == Non_Terminal) {

                bool Nullable = true; //checking whether the current non-terminal is nullable in this production in the rhs side 

                //we dont need to deal with epilon accuring in the rule since it would never accur if some termina is there in the rhs 


                    // if the non-terminal is followed by another symbol
                    for (int j = i + 1; j < prod.rhs.size(); ++j) {
                        if (isNonTerminal(prod.rhs[j])) {
                            set<string> firstOfNext = computeFirst(prod.rhs[j]);
                             
                            for(const auto &symbol : firstOfNext){
                                if(symbol != "ε"){
                                    follow.insert(symbol);
                                }
                            }
                            // if the first set of the next symbol contains epsilon, continue to the next symbol
                            if(firstOfNext.find("ε") == firstOfNext.end()){
                                Nullable = false;
                                break;
                            }

                        } else {
                            // if the next symbol is a terminal, add it to the follow set and break
                            follow.insert(prod.rhs[j]);
                            Nullable = false;
                            break;
                        }
                    }
                
                    // if all symbols after the non-Terminal are nullable, add follow of the current non-Terminal to the follow set
                    if (Nullable) {
                    
                        if(prod.lhs != Non_Terminal){
                            set<string> followOfCurrent = computeFollow(prod.lhs);
                            follow.insert(followOfCurrent.begin(), followOfCurrent.end());
                        }
                    }
            }
        }
    }

    // caching the computed follow set
    followSet[Non_Terminal] = follow;
    return follow;
}

//parser table for the grammar

// compute first for a sequence of symbols (a production's rhs)
set<string> firstforSeq(const vector<string> &symbols) {
    set<string> result;
    bool allNullable = true;
    for (const auto &symbol : symbols) {
        if (isNonTerminal(symbol)) {
            set<string> firstOfSymbol = computeFirst(symbol);
            // Insert all symbols except epsilon.
            for (const auto &s : firstOfSymbol) {
                if (s != "ε") {
                    result.insert(s);
                }
            }
            // If this symbol's first set doesn't contain ε, then stop.
            if (firstOfSymbol.find("ε") == firstOfSymbol.end()) {
                allNullable = false;
                break;
            }
        } else {  // terminal symbol
            result.insert(symbol);
            allNullable = false;
            break;
        }
    }
    if (allNullable) {
        result.insert("ε");
    }
    return result;
}

map<pair<string,string>, int> parseTable;

void buildParsingTable() {
    // for each production in our grammar:
    for (auto &prod : productions) {
        // compute first for the production's RHS.
        set<string> firstSeq = firstforSeq(prod.rhs);
        // for every terminal in first(α) (except ε), add the production to the table.
        for ( const auto &terminal : firstSeq) {
            if (terminal != "ε") {
                pair<string, string> key(prod.lhs, terminal);
                // Check for conflicts:
                if (parseTable.find(key) != parseTable.end()) {
                    cerr << "Conflict in parsing table for " << prod.lhs << ", " << terminal << endl;
                }
                parseTable[key] = &prod - &productions[0];  // Store the index of the production
            }
        }
        // if first(α) contains ε, then for every terminal f in follow(prod.lhs),
        // add the production to the table entry (A, f).
        if (firstSeq.find("ε") != firstSeq.end()) {
            set<string> followOfLHS = followSet[prod.lhs];  // assuming followSet is computed
            for (const auto &terminal : followOfLHS) {
                pair<string, string> key(prod.lhs, terminal);
                if (parseTable.find(key) != parseTable.end()) {
                    cerr << "Conflict in parsing table for " << prod.lhs << ", " << terminal << endl;
                }
                parseTable[key] = &prod - &productions[0];  // Store the index of the production
            }
        }
    }
}


bool parseTokens(const vector<Token>& tokens) {
    stack<string> parseStack;
    parseStack.push("$");
    parseStack.push("<SQL>");
    
    int index = 0;
    
    while (!parseStack.empty()) {
        string top = parseStack.top();
        Token currentToken = tokens[index];
        string tokenStr = tokenToString(currentToken.type);
        
        // Map token names to symbols for matching
        if (tokenStr == "COMMA") {
            tokenStr = ",";
        } else if (tokenStr == "SEMICOLON") {
            tokenStr = ";";
        } else if (tokenStr == "LPAREN") {
            tokenStr = "(";
        } else if (tokenStr == "RPAREN") {
            tokenStr = ")";
        } else if (tokenStr == "EQ") {
            tokenStr = "=";
        } else if (tokenStr == "GT") {
            tokenStr = ">";
        } else if (tokenStr == "LT") {
            tokenStr = "<";
        } else if (tokenStr == "EOF") {
            tokenStr = "$";
        }
        else if (tokenStr == "ASTERISK") {
            tokenStr = "*";
        }
        
        // When both the parse stack and the input are at the end.
        if (top == "$" && tokenStr == "$") {
            parseStack.pop();
            break;
        }
        
        // If top of stack is a terminal.
        if (!isNonTerminal(top)) {
            if (top == tokenStr) {
                parseStack.pop();
                index++;
            } else {
                cerr << "Syntax Error at line " << currentToken.line << ", column " << currentToken.column 
                     << ": expected \"" << top << "\", found \"" << tokenStr << "\" (" 
                     << currentToken.lexeme << ")" << endl;
                return false;
            }
        } else { // top is a nonterminal.
            pair<string, string> key(top, tokenStr);
            
            if (parseTable.find(key) == parseTable.end()) {
                // Check for specific error cases:
                if (top == "<ATTR_LIST_TAIL>" && tokenStr == "IDENTIFIER") {
                    // Case: missing comma between attribute names.
                    cerr << "Syntax Error at line " << currentToken.line << ", column " << currentToken.column 
                         << ": missing comma between attribute names before \"" << currentToken.lexeme << "\"" << endl;
                }
                else if (top == "<INSERT_TAIL>" && tokenStr == "(") {
                    // A possible case: missing VALUES keyword.
                    cerr << "Syntax Error at line " << currentToken.line << ", column " << currentToken.column 
                         << ": missing 'VALUES' keyword before '(' in INSERT statement." << endl;
                }
                else if (top == "<SQL_TAIL>" && tokenStr == "$") {
                    // When reaching the end, it might be that a semicolon is missing.
                    cout << "As we have reached the end and got '$', there might be a missing semicolon (';')." << endl;
                }
                else if (top == "<TABLE>" && tokenStr == "WHERE") {
                    // Case: missing table name before WHERE clause.
                    cerr << "Syntax Error at line " << currentToken.line << ", column " << currentToken.column 
                         << ": missing table name before WHERE clause." << endl;

                }
                else if( top == "<STMT>" && tokenStr == "SELECT"){
                    //case : missing select or insert or delete before the statement
                    cerr << "Syntax Error at line " << currentToken.line << ", column " << currentToken.column 
                         << ": missing SELECT, INSERT, or DELETE before the statement." << endl;
                }
                else if (tokenStr == "$") {
                    // Generic handling when end-of-input is reached unexpectedly.
                    cout << "Syntax Error: reached end of input unexpectedly. Possibly a missing semicolon (';')." << endl;
                }

               
                else {
                    // generic error message for unhandled cases.
                    cerr << "Syntax Error at line " << currentToken.line << ", column " << currentToken.column 
                         << ": no rule for nonterminal " << top << " with lookahead \"" << tokenStr 
                         << "\" (" << currentToken.lexeme << ")" << endl;
                }
                return false;
            }
            
            int prodIndex = parseTable[key];
            Production prod = productions[prodIndex];
            parseStack.pop();
            // Push the production's RHS in reverse order (unless it is epsilon).
            if (!(prod.rhs.size() == 1 && prod.rhs[0] == "ε")) {
                for (int i = prod.rhs.size() - 1; i >= 0; i--) {
                    parseStack.push(prod.rhs[i]);
                }
            }
        }
    }
    
    // After parsing, ensure no extra tokens remain.
    if (tokens[index].type != TOKEN_EOF) {
        cerr << "Syntax Error: Extra tokens remain after parsing." << endl;
        return false;
    }
    
    return true;
}

int main(){
    
    //tokens for the parser first phase

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

    // cout<<"First and Follow Sets for SQL Grammar"<<endl;
    // cout<<"First Sets:"<<endl;
    set<string> firstSet;
    
    for(const auto &prod: productions){
        if(firstSet.find(prod.lhs) == firstSet.end()){
            set<string> first = computeFirst(prod.lhs);
            // cout<<prod.lhs<<": ";
            // for(const auto &symbol: first){
            //     cout<<symbol<<" ";
            // }
            // cout<<endl;
        }
        firstSet.insert(prod.lhs);
    }
    // cout<<endl;
 
    // cout<<"Follow Sets:"<<endl;
    set<string> followSet;
    for(const auto &prod: productions){
        if(followSet.find(prod.lhs) == followSet.end()){
            set<string> follow = computeFollow(prod.lhs);
            // cout<<prod.lhs<<": ";
            // for(const auto &symbol: follow){
            //     cout<<symbol<<" ";
            // }
            // cout<<endl;
        }
        followSet.insert(prod.lhs);
    }
    // cout<<endl;

    

    // cout<<"First and Follow Sets Computed Successfully!"<<endl;


    //making the parsing table for the grammar

    cout<<"Building Parsing Table..."<<endl;
    buildParsingTable();
    cout<<"Parsing Table Built Successfully!"<<endl;


    // cout<<"Parsing Table:"<<endl;
    // for (const auto &entry : parseTable) {
    //     cout << "[" << entry.first.first << ", " << entry.first.second << "] Rule follow: " << productions[entry.second].lhs << " -> ";
    //     for (const auto &symbol : productions[entry.second].rhs) {
    //         cout << symbol << " ";
    //     }
    //     cout << endl;
    // }
    // cout<<endl;


       cout<<"Parsing the SQL query..."<<endl;

    if (parseTokens(tokens)) {
        cout << "Parsing completed successfully!" << endl;
    } else {
        cout << "Parsing failed!" << endl;
    }
    cout<<endl;

    cout<<"Exiting..."<<endl;
    cout<<"Thank you!"<<endl;
    cout<<"Have a nice day!"<<endl;

    return 0;
}

