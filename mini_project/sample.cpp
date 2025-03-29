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
    {"<SQL>", {"<STMT>", "<SQL_TAIL>"}},

    {"<SQL_TAIL>", {"<STMT>", "<SQL_TAIL>"}},

    {"<SQL_TAIL>", {"ε"}},

    {"<STMT>", {"<SELECT_STMT>"}},
  
    {"<STMT>", {"<INSERT_STMT>"}},
 
    {"<STMT>", {"<DELETE_STMT>"}},
    
    {"<SELECT_STMT>", {"SELECT", "<LIST>", "FROM", "<TABLE>", "<WHERE_OPT>", "<ORDER_BY_OPT>", "<GROUP_BY_OPT>", ";"}},

    {"<LIST>", {"*"}},
    
    {"<LIST>", {"<ATTR_LIST>"}},
 
    {"<TABLE>", {"IDENTIFIER"}},
    
    {"<ATTR_LIST>", {"IDENTIFIER", "<ATTR_LIST_TAIL>"}},
     
    {"<ATTR_LIST_TAIL>", {",", "IDENTIFIER", "<ATTR_LIST_TAIL>"}},
    
    {"<ATTR_LIST_TAIL>", {"ε"}},
    
    {"<WHERE_OPT>", {"WHERE", "<CONDITION>"}},
    
    {"<WHERE_OPT>", {"ε"}},
    
    {"<CONDITION>", {"<AND_CONDITION>", "<CONDITION_TAIL>"}},
    
    {"<CONDITION_TAIL>", {"OR", "<AND_CONDITION>", "<CONDITION_TAIL>"}},
    
    {"<CONDITION_TAIL>", {"ε"}},
    
    {"<AND_CONDITION>", {"<NOT_CONDITION>", "<AND_CONDITION_TAIL>"}},

    {"<AND_CONDITION_TAIL>", {"AND", "<NOT_CONDITION>", "<AND_CONDITION_TAIL>"}},
    
    {"<AND_CONDITION_TAIL>", {"ε"}},
    
    {"<NOT_CONDITION>", {"NOT IDENTIFIER", "<OP>", "<LITERAL>"}},
 
    {"<NOT_CONDITION>", {"IDENTIFIER", "<OP>", "<LITERAL>"}},
    
    {"<OP>", {"="}},
 
    {"<OP>", {">"}},

    {"<OP>", {"<"}},
  
    {"<LITERAL>", {"NUMBER"}},
  
    {"<LITERAL>", {"STRING"}},

    {"<ORDER_BY_OPT>", {"ORDER", "BY", "<LIST>"}},
  
    {"<ORDER_BY_OPT>", {"ε"}},
    
    {"<GROUP_BY_OPT>", {"GROUP", "BY", "<LIST>"}},
  
    {"<GROUP_BY_OPT>", {"ε"}},

    {"<INSERT_STMT>", {"INSERT", "INTO", "<TABLE>", "<INSERT_TAIL>"}},
  
    {"<INSERT_TAIL>", {"(", "<ATTR_LIST>", ")", "VALUES", "(", "<VALUE_LIST>", ")", ";"}},
   
    {"<INSERT_TAIL>", {"VALUES", "(", "<VALUE_LIST>", ")", ";"}},

    {"<VALUE_LIST>", {"<LITERAL>", "<VALUE_LIST_TAIL>"}},
    
    {"<VALUE_LIST_TAIL>", {",", "<LITERAL>", "<VALUE_LIST_TAIL>"}},
   
    {"<VALUE_LIST_TAIL>", {"ε"}},

    {"<DELETE_STMT>", {"DELETE", "FROM", "<TABLE>", "WHERE", "IDENTIFIER", "<OP>", "<LITERAL>", ";"}}
};


//now we need to compute the first and follow sets for the grammar

map<string, set<string>> firstSet;

map<string, set<string>> followSet;//these are cached for efficiency

bool isNonTerminal(const string &symbol) {
    return symbol[0] == '<' && symbol[symbol.size() - 1] == '>';
}


set<string> computeFirst(const string &Non_Terminal) {
    if (firstSet.find(Non_Terminal) != firstSet.end()) {
        return firstSet[Non_Terminal];
    }
    
    set<string> first;
    cout<<"hello"<<endl;
    for(const auto &prod: productions){
        if(prod.lhs == Non_Terminal && prod.rhs[0] == "ε"){
            first.insert("ε");
        }

        else if(prod.lhs == Non_Terminal){
            bool allNullable = true;
            for(const auto &symbol: prod.rhs){
                if(isNonTerminal(symbol)){

                    set<string> firstofSymbol = computeFirst(symbol);

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

            if(allNullable){
                first.insert("ε");
            }

        }
    }
 
    firstSet[Non_Terminal] = first;
    return first;
}



//testing

int main(){
    cout<<"First and Follow Sets for SQL Grammar"<<endl;
    string input = "<SQL>";
    set<string> first = computeFirst(input);
    cout << "First(" << input << ") = { ";
    for (const auto &symbol : first) {
        cout << symbol << " ";
    }
    cout << "}" << endl;

    return 0;
}