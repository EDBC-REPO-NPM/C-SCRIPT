#ifndef NODEPP_DATE
#define NODEPP_DATE

#include "algorithm.h"
#include <ctime>

namespace date {

    uint stamp(){ return (uint)( time( nullptr ) * 1000 ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint year( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return (info->tm_year+1900); 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint month( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_mon; 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint day( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_mday; 
    }

    /*────────────────────────────────────────────────────────────────────────────*/
    
    uint hour( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_hour; 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint minute( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_min; 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint second( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_sec; 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint weekday( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_wday; 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint monthday( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_mday; 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint yearday( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_yday; 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t fulltime(){
        time_t _time = time( nullptr ); 
        string_t data(ctime(&_time));
        return data; 
    }

}

class date_t {

    protected:

        struct tm* info = nullptr;
        time_t _time;

    public:

    /*────────────────────────────────────────────────────────────────────────────*/

    date_t( bool UTC=false ){  _time = time(nullptr); 
        if( UTC ){ info = gmtime( &_time ); }
        else { info = localtime( &_time ); }
    }

    date_t( uint year, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; _time = mktime(info);
    }

    date_t( uint year, uint month, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        _time = mktime(info);
    }

    date_t( uint year, uint month, uint day, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; _time = mktime(info);
    }

    date_t( uint year, uint month, uint day, uint hour, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        _time = mktime(info);
    }

    date_t( uint year, uint month, uint day, uint hour, uint min, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        info->tm_min = min; _time = mktime(info);
    }

    date_t( uint year, uint month, uint day, uint hour, uint min, uint sec, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        info->tm_min = min; info->tm_sec = sec;
        _time = mktime(info);
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    void set_time( bool UTC=false ){  _time = time(nullptr); 
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
    }

    void set_time( uint year, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; _time = mktime(info);
    }

    void set_time( uint year, uint month, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        _time = mktime(info);
    }

    void set_time( uint year, uint month, uint day, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; _time = mktime(info);
    }

    void set_time( uint year, uint month, uint day, uint hour, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        _time = mktime(info);
    }

    void set_time( uint year, uint month, uint day, uint hour, uint min, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        info->tm_min = min; _time = mktime(info);
    }

    void set_time( uint year, uint month, uint day, uint hour, uint min, uint sec, bool UTC=false ) { 
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        info->tm_min = min; info->tm_sec = sec;
        _time = mktime(info);
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    void set_minute( uint min );     void set_second( uint sec );
    void set_month( uint month ); void set_year( uint year );
    void set_hour( uint hour );   void set_day( uint day );

    string_t get_fulltime(); 
    uint get_monthday();   
    uint get_weekday();
    uint get_yearday();

    uint get_minute(); uint get_second(); 
    uint get_stamp();  uint get_month();
    uint get_year();   uint get_day();
    uint get_hour();    

};

void date_t::set_month( uint month ){ info->tm_mon = month; _time = mktime(info); }
void date_t::set_second( uint sec ){  info->tm_sec = sec;   _time = mktime(info); }
void date_t::set_minute( uint min ){  info->tm_min = min;   _time = mktime(info); }
void date_t::set_year( uint year ){   info->tm_year = year; _time = mktime(info); }
void date_t::set_hour( uint hour ){   info->tm_hour = hour; _time = mktime(info); }
void date_t::set_day( uint day ){     info->tm_mday = day;  _time = mktime(info); }

string_t date_t::get_fulltime(){       return (string_t)ctime(&_time);            }
uint     date_t::get_monthday(){       return (uint)info->tm_mday;                }
uint     date_t::get_weekday(){        return (uint)info->tm_wday;                }
uint     date_t::get_yearday(){        return (uint)info->tm_yday;                }
uint     date_t::get_year(){           return (uint)(info->tm_year+1900);         }
uint     date_t::get_hour(){           return (uint)info->tm_hour;                }
uint     date_t::get_day(){            return (uint)info->tm_mday;                }
uint     date_t::get_month(){          return (uint)info->tm_mon;                 }
uint     date_t::get_minute(){         return (uint)info->tm_min;                 }
uint     date_t::get_second(){         return (uint)info->tm_sec;                 }
uint     date_t::get_stamp(){          return (uint)_time * 1000;                 }

#endif