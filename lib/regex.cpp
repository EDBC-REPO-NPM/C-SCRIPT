#ifndef REGEX_CSCRIPT
#define REGEX_CSCRIPT

#include <regex>

using namespace std;

namespace std {

    bool same( string A, string B ){
        return regex_match ( A.begin(), A.end(), regex(B) );
    }

    bool test( string base, regex reg ){
        return regex_search( base, reg );
    }

    string replace( string base, regex reg,string sub ){
        return regex_replace( base, reg, sub, regex_constants::format_first_only );
    }

    string replaceAll( string base, regex reg,string sub ){
        return regex_replace( base, reg, sub );
    }

    string match( string base, regex reg ){
        vector<string> result; smatch m;
        regex_search( base, m, reg); 
        return (string)m[0];
    }

    vector<string> matchAll( string base, regex reg ){
        vector<string> result; string tmp = base; smatch m;
        while (regex_search( tmp, m, reg)) {
            for(auto x:m) result.push_back((string)x);
            tmp = m.suffix().str();
        }   return result;
    }

    vector<string> split( string base, regex reg ) {
        vector<string> result; string tmp = base; smatch m;
        while (regex_search( tmp, m, reg)) {
            for(auto x:m) result.push_back( m.prefix().str() );
            tmp = m.suffix().str();
        }   result.push_back( m.suffix().str() ); return result;        
    }

}

#endif 