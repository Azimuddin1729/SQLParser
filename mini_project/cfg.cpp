// cfg for sql we are following:

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

//testing

int main(){
    cout<<"First and Follow Sets for SQL Grammar"<<endl;
    string input = "<WHERE_OPT>";
    set<string> first = computeFirst(input);
    cout << "First(" << input << ") = { ";
    for (const auto &symbol : first) {
        cout << symbol << " ";
    }
    cout << "}" << endl;

    string nonTerminal = "<STMT>";
    set<string> follow = computeFollow(nonTerminal);
    cout << "Follow(" << nonTerminal << ") = { ";
    for (const auto &symbol : follow) {
        cout << symbol << " ";
    }
    cout << "}" << endl;

    return 0;
}