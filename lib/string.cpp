#ifndef STRING_CSCRIPT
#define STRING_CSCRIPT

#include <cctype>

using namespace std;
namespace std {

    char toLowerCase( char base ){  
        return tolower( (int)base );
    }

    char toUpperCase( char base ){  
        return toupper( (int)base );
    }

    //───────────────────────────────────────────────────────────────────────//

    string toLowerCase( string base ){ 
        string result; for( auto i:base )
            result += tolower( (int)i );
            return result;
    }

    string toUpperCase( string base ){ 
        string result; for( auto i:base )
            result += toupper( (int)i );
            return result;
    }

    string toCapitalCase( string text ){
        regex reg("\\w+");
        for( auto x:matchAll(text,reg) ) {
            regex nreg(x); x[0] = toupper(x[0]);
            text = replaceAll( text, nreg, x );
        }   return text;
    }

    //───────────────────────────────────────────────────────────────────────//
    
    vector<string> split( string base, char ch='\0'  ){
        vector<string> result; for( auto i:base ){ string empty;
            if( i == ch ){ result.push_back(empty); continue; }
            else if( ch == '\0')     result.push_back(empty);
            if( result.size() == 0 ) result.push_back(empty);
            int length = result.size()-1; result[length] += i; 
        }   return result;
    }

    vector<string> split( string base, int ch ){
        vector<string> result; for( auto i:base ){ string empty;
            if( result.size() == 0 ) result.push_back(empty); 
            if( result[result.size()-1].size() == ch ) 
                result.push_back(empty);
                result[result.size()-1] += i; 
        }   return result;
    }

}

#endif 