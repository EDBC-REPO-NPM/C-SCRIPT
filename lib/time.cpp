#ifndef TIME_CSCRIPT
#define TIME_CSCRIPT

#include <ctime>

using namespace std;

namespace std {

    int now(){ return (int) time( nullptr ); }

    //───────────────────────────────────────────────────────────────────────//

    int year( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_year; 
    }

    //───────────────────────────────────────────────────────────────────────//

    int month( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_mon; 
    }

    //───────────────────────────────────────────────────────────────────────//

    int day( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_mday; 
    }

    //───────────────────────────────────────────────────────────────────────//
    
    int hour( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_hour; 
    }

    //───────────────────────────────────────────────────────────────────────//

    int minute( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_min; 
    }

    //───────────────────────────────────────────────────────────────────────//

    int second( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_sec; 
    }

    //───────────────────────────────────────────────────────────────────────//

    int wday( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_wday; 
    }

    //───────────────────────────────────────────────────────────────────────//

    int yday( bool UTC=false ){
        time_t _time = time( nullptr ); struct tm *info;
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
        return info->tm_yday; 
    }

    //───────────────────────────────────────────────────────────────────────//

    string fullTime(){
        time_t _time = time( nullptr ); 
        return (string)ctime(_time); 
    }

}

class Time {

    private:
        time_t _time;
        struct tm* info;

    public:

    //───────────────────────────────────────────────────────────────────────//

    Time( bool UTC=false ){  _time = time(nullptr); 
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
    }

    Time( time_t stamp, bool UTC=false ){  _time = time(stamp); 
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
    }

    Time( int year, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; _time = mktime(info);
    }

    Time( int year, int month, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        _time = mktime(info);
    }

    Time( int year, int month, int day, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; _time = mktime(info);
    }

    Time( int year, int month, int day, int hour, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        _time = mktime(info);
    }

    Time( int year, int month, int day, int hour, int min, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        info->tm_min = min; _time = mktime(info);
    }

    Time( int year, int month, int day, int hour, int min, int sec, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        info->tm_min = min; info->tm_sec = sec;
        _time = mktime(info);
    }

    //───────────────────────────────────────────────────────────────────────//

    void set_time( bool UTC=false ){  _time = time(nullptr); 
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
    }

    void set_time( time_t& stamp, bool UTC=false ){ _time = time(stamp); 
        if( UTC ) info = gmtime( &_time ); 
        else info = localtime( &_time );
    }

    void set_time( int year, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; _time = mktime(info);
    }

    void set_time( int year, int month, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        _time = mktime(info);
    }

    void set_time( int year, int month, int day, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; _time = mktime(info);
    }

    void set_time( int year, int month, int day, int hour, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        _time = mktime(info);
    }

    void set_time( int year, int month, int day, int hour, int min, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        info->tm_min = min; _time = mktime(info);
    }

    void set_time( int year, int month, int day, int hour, int min, int sec, bool UTC=false ) { time_t _t_time = time( nullptr );
        if( UTC ) info = gmtime( &_time ); else info = localtime( &_time );
        info->tm_year = year; info->tm_mon = month; 
        info->tm_mday = day; info->tm_hour = hour; 
        info->tm_min = min; info->tm_sec = sec;
        _time = mktime(info);
    }

    //───────────────────────────────────────────────────────────────────────//

    void set_month( int month ); void set_year( int year );
    void set_hour( int hour );   void set_day( int day );
    void set_min( int min );     void set_sec( int sec );

    string get_fullDate(); 
    int get_stamp(); int get_month();
    int get_year();  int get_day();
    int get_hour();  int get_sec();
    int get_min();

};

void Time::set_month( int month ){ info->tm_mon = month; _time = mktime(info); }
void Time::set_year( int year ){ info->tm_year = year; _time = mktime(info); }
void Time::set_hour( int hour ){ info->tm_hour = hour; _time = mktime(info); }
void Time::set_day( int day ){ info->tm_mday = day; _time = mktime(info); }
void Time::set_min( int min ){ info->tm_min = min; _time = mktime(info); }
void Time::set_sec( int sec ){ info->tm_sec = sec; _time = mktime(info); }

string Time::get_fullDate(){ return (string)ctime(&_time); }

int Time::get_month(){ return (int)info->tm_mon; }
int Time::get_year(){ return (int)info->tm_year; }
int Time::get_hour(){ return (int)info->tm_hour; }
int Time::get_day(){ return (int)info->tm_mday; }
int Time::get_min(){ return (int)info->tm_min; }
int Time::get_sec(){ return (int)info->tm_sec; }
int Time::get_stamp(){ return (int)_time; }

#endif