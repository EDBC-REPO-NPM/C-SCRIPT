#ifndef NODEPP_REGEX
#define NODEPP_REGEX

#include "string.h"
#include "array.h"
#include "ptr.h"

namespace REGEX_NODEPP {
    class NODE { public:

        char data = 0; bool f=0, r=0, n=0; uint idx = 0;
        NODE* alt = nullptr; array_t<NODE*> nxt;
        int rep[2] = { 0, 0 };

       ~NODE(){ for( auto x:nxt ){ 
            if( x!=nullptr ){ delete x; x = nullptr; }
        }   if( alt!=nullptr ){ delete alt; alt = nullptr; } }

        void pipe( function_t<NODE*,NODE*> func ){
            NODE* n = this; while( n!=nullptr ){
                idx = 0; n = func( n );
            }
        }
    };
}

/*────────────────────────────────────────────────────────────────────────────*/

class regex_t { 
    
    protected:

    bool icase=false, multi=false, dotl=false;
    ptr_t<REGEX_NODEPP::NODE> root;

    void add_new( REGEX_NODEPP::NODE*& act, REGEX_NODEPP::NODE*& prv, char data ){
        REGEX_NODEPP::NODE* nw = new REGEX_NODEPP::NODE; nw->data = data; 
        act->nxt.push( nw ); prv = act; act = nw;
    }

    void add_flag( char& flag, REGEX_NODEPP::NODE* act ){
         switch( tolower(flag) ){
            case 'b': act->f = 1; act->data = flag; break;
            case 'w': act->f = 1; act->data = flag; break;
            case 'd': act->f = 1; act->data = flag; break;
            case 's': act->f = 1; act->data = flag; break;
            default:              act->data = flag; break;
        }
    }

    REGEX_NODEPP::NODE* null(){ REGEX_NODEPP::NODE* nw = new REGEX_NODEPP::NODE; nw->data = 1; return nw; }

    REGEX_NODEPP::NODE* compile( string_t* _reg ){ 
        
        REGEX_NODEPP::NODE* root = new REGEX_NODEPP::NODE;
        REGEX_NODEPP::NODE* prv = nullptr;
        REGEX_NODEPP::NODE* act = root;

        for( uint i=0; i<_reg->size(); i++ ){ char x = (*_reg)[i];

            if( x == ']' || x == '}' || x == ')' ){
                throw err_t(("Error regex at character: "+string::to_string(i)).c_str());
            }
            
            if( x == '?' || x == '*' || x == '+' ){ 
                if( prv == nullptr ){ throw err_t(("Error regex at character: "+string::to_string(i)).c_str()); }
                REGEX_NODEPP::NODE* nw = new REGEX_NODEPP::NODE; switch(x){
                    case '?': nw->r = 1; nw->rep[0] = 0; nw->rep[1] = 1; break;
                    case '*': nw->r = 1; nw->rep[0] = 1; nw->rep[1] =-1; break;
                    case '+': nw->r = 1; nw->rep[0] = 2; nw->rep[1] =-1; break;
                }   nw->alt = new REGEX_NODEPP::NODE; nw->alt->nxt.push(act);
                    prv->nxt[ prv->nxt.size()-1 ] = nw; 
                    act->nxt.push( null() ); act  = nw;
                continue;
            }

            if( x == '^' || x == '$' ){ add_new( act, prv, 0 ); switch(x){
                    case '^': act->f = 1; act->data = '^'; break;
                    case '$': act->f = 1; act->data = '$'; break;
                }   continue;
            }

            /*────────────────────────────────────────────────────────────────────────────*/

            if( x == '\\' || x == '.' ){ add_new( act, prv, 0 );
                if( x == '\\' ){ add_flag( (*_reg)[i+1], act ); i++; }
                if( x == '.' ){ act->f = 1; act->data = '.'; }                                       
                continue;
            }

            /*────────────────────────────────────────────────────────────────────────────*/

            if( x == '{' ){ i++; string_t s; int j=0, k=0; REGEX_NODEPP::NODE* nw = new REGEX_NODEPP::NODE;
                if( prv == nullptr ){ throw err_t(("Error regex at character: "+string::to_string(i)).c_str()); }

                while( i<_reg->size() ){ int y = (*_reg)[i];
                    if( y == ',' ){ nw->rep[j] = string::to_int(s); s.clear(); i++; j++; continue; }
                    if( y == '}' ){ if( !s.empty() ){ nw->rep[j] = string::to_int(s); s.clear(); } k--; break; } 
                    if( !isdigit(y) ){ throw err_t(("Error regex at character: "+string::to_string(i)).c_str()); } s.push(y); i++;
                }   if( k!=-1 ){ throw err_t(("Error regex at character: "+string::to_string(i)).c_str()); }

                    nw->alt = new REGEX_NODEPP::NODE; nw->alt->nxt.push(act);
                    prv->nxt[ prv->nxt.size()-1 ] = nw; 
                    act->nxt.push( null() ); act  = nw;
                    act->r = 1;

                continue;
            }

            if( x == '[' ){ i++; string_t s; int j=0, k=0, n=0; add_new( act, prv, 0 );

                while( i<_reg->size() ){ int y = (*_reg)[i];
                    if( j == 0 && y == '^' ){ n = true; i++; j++; continue; }
                    if( y == ']' ){ k--; break; } s.push(y); i++; j++;
                }   if( k!=-1 ){ throw err_t(("Error regex at character: "+string::to_string(i)).c_str()); }
                
                act->alt = new REGEX_NODEPP::NODE; for( uint i=0; i<s.size(); i++ ){ 
                    if( s[i+1] == '-' && (i+2)<s.size() ){
                        int a = math::min( (int)s[i], (int)s[i+2] ); 
                        int b = math::max( (int)s[i], (int)s[i+2] );
                        for( int j=a; j<=b; j++ ){
                            REGEX_NODEPP::NODE* nw = new REGEX_NODEPP::NODE; nw->data = j; nw->n = n;
                            act->alt->nxt.push(nw); nw->nxt.push(null());
                        }   i+=2;
                    } else {
                        REGEX_NODEPP::NODE* nw = new REGEX_NODEPP::NODE; nw->n = n;
                        if( s[i] == '\\' ){ add_flag( s[i+1], nw ); i++; }
                        else              { nw->data = s[i]; }
                        act->alt->nxt.push(nw); nw->nxt.push(null());
                    }
                }   continue;
            }

            if( x == '(' ){ i++; string_t s; add_new( act, prv, 0 ); long int j=0;
                while( i<_reg->size() ){ int y = (*_reg)[i]; 
                    if( y == '(' ){ j++; } else if( y == ')' ){ j--; }
                    if( j<0 && y == ')' ){ break; } s.push(y); i++; 
                }
                if( j!=-1 ){ throw err_t(("Error regex at character: "+string::to_string(i)).c_str()); }
                if( !s.empty() ){ act->alt = compile(&s); } continue;
            }

            /*────────────────────────────────────────────────────────────────────────────*/

            if( x == '|' ){ add_new( act, prv, (char)1 ); 
                root->nxt.push( new REGEX_NODEPP::NODE );
                act = root->nxt[ root->nxt.size()-1 ];
                continue; 
            }

            add_new( act, prv, x ); 
        }   act->nxt.push(null()); 

    return root; }

    int can_repeat( REGEX_NODEPP::NODE* NODE ){ 
        if( !NODE->r ){ return -1; } NODE->idx++; 

             if( NODE->rep[1] == 0 ){ return ( NODE->idx < (uint)NODE->rep[0] ) ? 0 :-1; } 
        else if( NODE->rep[1] ==-1 ){ return ( NODE->idx < (uint)NODE->rep[0] ) ? 0 : 1; } 
        
        else {
            if( NODE->idx>=(uint)NODE->rep[0] && NODE->idx<=(uint)(NODE->rep[1]+1) ){ return  1; }
            else if( NODE->idx>=(uint)(NODE->rep[1]+1) )                            { return -1; }
            else                                                                    { return  0; }
        }
        
    }

    ptr_t<uint> match( string_t _str, uint _off, REGEX_NODEPP::NODE* NODE ){
        if( icase ){ _str.to_lower_case(); }
        
        ptr_t<uint> _res(2); _res[0]=0; _res[1]=0;
        if( _str.empty() ) return _res;

        uint i=_off; NODE->pipe([&]( REGEX_NODEPP::NODE* NODE ){ 
            
            char data = (i>=_str.size()) ? 1 : _str[i];
            function_t<REGEX_NODEPP::NODE*,bool> end ([&]( bool e ){ 
                i++; if(e){ return NODE->nxt[0]; }
                else { _off=i; return (REGEX_NODEPP::NODE*) nullptr; }
            });

            if( NODE->data == 0 ){
                if( NODE->alt != nullptr ){ ptr_t<uint> idx; NODE->idx=0;
                    for( auto x:NODE->alt->nxt ){ int c=1; uint k=0; while(1){

                    if( i > _str.size() ){ return end(1); }

                    if( x->n ){

                        if( NODE->alt->nxt.size() <= k ){ i++; return NODE->nxt[0]; }
                            
                        REGEX_NODEPP::NODE* y = NODE->alt->nxt[k]; idx = match( _str, i, y );
                        bool d = ( idx[0] != idx[1] ) ? 1 : 0;

                        if( d ) return (REGEX_NODEPP::NODE*) nullptr; else { k++; continue; }

                    } else {

                        idx = match( _str, i, x ); c = can_repeat(NODE);
                        bool d = ( idx[0] != idx[1] ) ? 1 : 0;
                             i = (!d) ? i : idx[1];

                        if( c == 1 && !d ){ return NODE->nxt[0]; }
                        if( c == 0 && !d ){ break; }

                        if( c ==-1 ){
                            if(d) { return NODE->nxt[0]; }
                            else  { break; }
                        }
                    }

                    }}  return (REGEX_NODEPP::NODE*) nullptr;
                }

                else for( auto x:NODE->nxt ){ 
                    ptr_t<uint> idx = match( _str, i, x );
                    if( idx[0] != idx[1] ){ i = idx[1]; break; }
                }

                if( i != _off ){ _res[0] = _off; _res[1] = i; 
                    return (REGEX_NODEPP::NODE*) nullptr; 
                }   return end(0); 
            }

            if( NODE->f ){
                if( NODE->data == '$' && data == 1 ){ return NODE->nxt[0]; }
                if( NODE->data == '^' && i == 0 )   { return NODE->nxt[0]; }
                switch( NODE->data ){

                    case 'b': if( data == 1 || i == 0 ){ return NODE->nxt[0]; } return end(0); break;
                    case '.': if( data == 0 || data == 1 ) { return end(0); }   break;
                    case 'B': if( data == 1 || i == 0 )    { return end(0); }   break;
                    case 's': if( !string::is_space(data) ){ return end(0); }   break;
                    case 'S': if(  string::is_space(data) ){ return end(0); }   break;
                    case 'w': if( !string::is_alnum(data) ){ return end(0); }   break;
                    case 'W': if(  string::is_alnum(data) ){ return end(0); }   break;
                    case 'd': if( !string::is_digit(data) ){ return end(0); }   break;
                    case 'D': if(  string::is_digit(data) ){ return end(0); }   break;
                    default:                                 return end(0);     break;
                    
                }   return end(1);
            }

            if( NODE->data == 1 ){ if( _off != i ){ _res[0] = _off; _res[1] = i; } return (REGEX_NODEPP::NODE*) nullptr; }
            if( data != NODE->data ){ return end(0); } return end(1);

        }); return _res;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/
    public:

    regex_t( string_t _reg, string_t _flags="" ){
        for( auto x:_flags ){
            switch(x){
                case 'i': icase = true; break;
                case 'm': multi = true; break; //not available
                case 's': dotl  = true; break; //not available
            }
        }   root = compile( &_reg );
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    ptr_t<uint> search( string_t _str, uint s=0 ) {
        for( uint i=s; i<_str.size(); i++ ){
            auto idx = this->match( _str, i, &root );
            if( idx[0] != idx[1] ) return idx;
        }   return (uint*) nullptr;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    bool test( string_t _str ) {
        auto idx = search( _str );
        if( idx == nullptr )   return 0;
        if( idx[0] == idx[1] ) return 0;
                               return 1;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t match( string_t _str ) { 
        auto idx = search( _str );
        if( idx == nullptr )   return string::to_string("");
        if( idx[0] == idx[1] ) return string::to_string("");
            return _str.slice( idx[0], idx[1] );
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t replace( string_t _str, string_t _rep ){
        auto idx = search( _str );
        if( idx == nullptr )     return _str;
        if( idx[0] == idx[1] )   return _str;
            _str.splice( idx[0], idx[1] - idx[0], _rep ); return _str;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    array_t<ptr_t<uint>> search_all( string_t _str ) {
        array_t<ptr_t<uint>> result; uint s=0; while(1){
            auto idx = search( _str, s );
            if( idx == nullptr )     return result;
            if( idx[0] == idx[1] )   return result; s=idx[1];
                ptr_t<uint> mem(2); mem[0]=idx[0], mem[1]=idx[1]; result.push(mem);
        }
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    array_t<string_t> match_all( string_t _str ){
        auto idx = search_all( _str ); array_t<string_t> result;
        for( auto x : idx )
            result.push(_str.slice( x[0], x[1] ));
            return result;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t replace_all( string_t _str, string_t _rep ) {
        auto idx = search_all( _str ).reverse();
        for( auto x : idx ){
            _str.splice( x[0], x[1] - x[0], _rep );
        }   return _str;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    array_t<string_t> split( string_t _str ){ uint n = 0, m = 0;
        auto idx = search_all( _str ); array_t<string_t> result;
        for( auto x : idx )
            { result.push(_str.slice( m, x[0] )); m=x[1]; n++; }
        if( m<_str.size() ) result.push(_str.slice( m, _str.size() )); 
        return result;
    }

};

namespace regex {

    bool test( string_t _str, string_t _reg, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.test( _str );
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    ptr_t<uint> search( string_t _str, string_t _reg, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.search( _str );
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t match( string_t _str, string_t _reg, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.match( _str );
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    array_t<string_t> match_all( string_t _str, string_t _reg, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.match_all( _str );
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t replace( string_t _str, string_t _reg, string_t _rep, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.replace( _str, _rep );
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t replace_all( string_t _str, string_t _reg, string_t _rep, string_t _flg="" ){
        regex_t reg( _reg, _flg ); return reg.replace_all( _str, _rep );
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    array_t<string_t> split( string_t _str, char ch=0 ){
        array_t<string_t> result; for( auto i:_str ){ string_t empty;
            if( i == ch ){ result.push(empty); continue; }
            else if( ch==0)       { result.push(empty); }
            if( result.size()==0 ){ result.push(empty); }
            int length = result.size()-1; result[length] += i; 
        }   return result;
    }

    array_t<string_t> split( string_t _str, int ch ){
        array_t<string_t> result; for( auto i:_str ){ string_t empty;
            if( result.size() == 0 ){ result.push(empty); }
            if( result[result.size()-1].size() == (uint)ch ){
                result.push(empty); 
            }   result[result.size()-1] += i; 
        }   return result;
    }

    array_t<string_t> split( string_t _str, string_t _reg ){ 
        if( _reg.size() == 1 ){  return split( _str, _reg[0] ); }
        else if( _reg.empty() ){ return split( _str, 0 ); }
        regex_t reg(_reg);       return reg.split( _str ); 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class T >
    string_t join( array_t<T> A, string_t c="," ){ 
        uint n=A.size(); string_t B; for( auto x : A ){
            if( n-->1 ) B += x + c; else B += x;
        }   return B;
    }

    template< class... V >
    string_t join( string_t c, V... args ){
        uint n=0; string::map([&]( string_t x ){ n++; },args...);
        string_t B; string::map([&]( string_t x ){
            if( n-->1 ) B += x + c; else B += x; 
        },args...); return B;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class V, class... T >
    string_t format( V val, T... args ){
        string_t result = string::to_string(val); uint n=0; string::map([&]( string_t arg ){
            string_t reg = "\\$\\{" + string::to_string(n) + "\\}"; 
            result = replace_all(result,reg,arg); n++;
        },  args... ); return result;
    }
    
}

#endif