#ifndef ARRAY_CSCRIPT
#define ARRAY_CSCRIPT

using namespace std;

namespace std { string to_string( string input ){ return input; } }

namespace std {

    //───────────────────────────────────────────────────────────────────────//
    
    template< class T > T getFirst( T base ){ return base[ base.begin() ]; }

    template< class T > T getLast( T base ){ return base[ base.end() ]; }

    template< class T > T getFrom( T base, int i ){ return base[i]; }

    //───────────────────────────────────────────────────────────────────────//

    template< class V, class T, class U >
    V replace_if( V base, T func, U targ ){ V B( base.begin(), base.end() ); replace_copy_if( base.begin(), base.end(), B.begin(), func, targ ); return B; }

    template< class V, class T >
    V remove_if( V base, T func ){ V B( base.begin(), base.end() ); remove_copy_if( base.begin(), base.end(), B.begin(), func ); return B; }

    template< class V, class T  >
    V copy_if( V base, V out, T func ){ return copy_if( base.begin(), base.end(), out.begin(), func ); return out; }

    template< class V, class T >
    int count_if( V base, T func ){ return count_if( base.begin(), base.end(), func ); }

    //───────────────────────────────────────────────────────────────────────//

    template< class V, class T >
    V replace( V base, T A, T B ){ V C( base.begin(), base.end() ); replace_copy( base.begin(), base.end(), B.begin(), A, B ); return C; }

    template< class V, class T >
    V remove( V base, T A ){ V B( base.begin(), base.end() ); remove_copy( base.begin(), base.end(), B.begin(), A ); return B; }

    template< class V >
    V reverse( V base ){ V B( base.begin(), base.end() ); reverse_copy( base.begin(), base.end(), B.begin() ); return B; }

    template< class V >
    V copy( V base, V out ){ copy( base.begin(), base.end(), out.begin() ); return out; }

    template< class V, class T >
    int count( V base, T A ){ return count( base.begin(), base.end(), A ); }

    template< class V >
    V move( V A, V& B ){ move( A.begin(), A.end(), B.begin() ); return B; }

    //───────────────────────────────────────────────────────────────────────//

    template< class V, class T >
    bool some( V base, T func ){ return any_of( base.begin(), base.end(), func ); }

    template< class V, class T >
    bool none( V base, T func ){ return none_of( base.begin(), base.end(), func ); }

    template< class V, class T >
    bool every( V base, T func ){ return all_of( base.begin(), base.end(), func ); }

    //───────────────────────────────────────────────────────────────────────//

    template< class V, class T > 
    void unshift( V& base, T value ){ base.insert(base.begin(), value); }

    template< class V, class T > 
    void push( V& base, T value ){ base.push_back( value ); }

    template< class V > 
    void shift( V& base ){ base.erase(base.begin()); }

    template< class V > 
    void pop( V& base ){ base.pop_back(); }

    //───────────────────────────────────────────────────────────────────────//

    template< class V, class T >
    auto valueOf( V base, T func ){ return find_if( base.begin(), base.end(), func ); }

    template< class V, class T >
    auto fill( V& base, T val ){ fill( base.begin(), base.end(), val ); return base; }

    template< class V, class T >
    auto indexOf( V base, T func ){ return search( base.begin(), base.end(), func ); }

    template< class V, class T >
    auto sort( V base, T func ){ return sort( base.begin(), base.end(), func ); }

    template< class V, class T >
    void all( V base, T func ){ for_each( base.begin(), base.end(), func ); }

    //───────────────────────────────────────────────────────────────────────//

    template< class V >
    V slice( V base, int start=0, int end=0 ){

        if( start < 0 ) start = base.size() + start;   if( end < 0 ) end = base.size() + end;
        if( start > base.size() ) start = base.size(); if( end > base.size() ) end = base.size();
        if( end < start ) end = base.size();           V B( base.begin()+start, base.begin()+end ); 
        
        copy( base.begin()+start, min( base.begin()+end, base.end() ), B.begin() ); return B;
    }


    //───────────────────────────────────────────────────────────────────────//

    template< class V >
    V splice( V& base, int start=0, int del=0 ){
        
        if( start < 0 ) start = base.size() + start; if( start > base.size() ) start = base.size();
            del += start; if( del < 0 ) del = 0;
        if( del != 0 ) base.erase( base.begin()+start, min( base.begin()+del, base.end() ) );
        
        return base;
    
    }

    template< class V, class T, class... U >
    V splice( V& base, int start, int del, T argc, U... args ){
        
        if( start < 0 ) start = base.size() + start; if( start > base.size() ) start = base.size();
            del += start; if( del < 0 ) del = 0;
        if( del != 0 ) base.erase( base.begin()+start, min( base.begin()+del, base.end() ) );
        
        pack( [&]( T x ){ base.insert( base.begin()+start, x ); start++; },  argc, args... );
        
        return base;
    
    }

    //───────────────────────────────────────────────────────────────────────//

    template< class V >
    string join( V base, string c="," ){
        string B; for( auto i=base.begin(); i!=base.end(); i++ ){
            if( !is_same<string,decltype(B[0])>::value ) B += to_string(*i); 
            else                                         B += (*i);
            if( i+1 != base.end() )                      B += c;
        }   return B;
    }

}

#endif 