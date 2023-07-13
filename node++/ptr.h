#ifndef NODEPP_PTR
#define NODEPP_PTR
#include <cstring>

template<class T>
class ptr_t { public:

   ~ptr_t(){ reset(); }

    ptr_t() noexcept = default;

    ptr_t( T* value ) noexcept {
        if( value == nullptr ){ return; }
        length_= new uint(0);
        count_ = new uint(1);
        value_ = value;
    }

    ptr_t( T* value, uint n ) noexcept {
        if( value == nullptr ){ return; }
        length_= new uint(n);
        count_ = new uint(1); 
        value_ = value;
    }

    ptr_t( const T& val, uint n ) noexcept { 
        count_ = new uint(1); value_ = new T[n];
        length_= new uint(n);
        while( n-->0 ){ value_[n] = val; }
    }

    ptr_t( const uint& n ) noexcept {
        length_= new uint(n);
        count_ = new uint(1);
        value_ = new T[n];
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    ptr_t( ptr_t&& other ) noexcept { move(std::move(other)); }

    ptr_t( const ptr_t& other ) noexcept { copy(other); }

    /*────────────────────────────────────────────────────────────────────────────*/

    void reset() noexcept {
        if( count() == 0 ){ return; }
        if( --(*count_) == 0 ) {
            if( size( ) == 0 )
                 delete    value_;
            else delete [] value_; 
                 delete    count_;
                 delete   length_;
        }
        length_= nullptr;
        count_ = nullptr;
        value_ = nullptr;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    bool operator> ( const ptr_t& oth ) const noexcept { return this->value_> oth.value_; }
    bool operator>=( const ptr_t& oth ) const noexcept { return this->value_>=oth.value_; }
    bool operator< ( const ptr_t& oth ) const noexcept { return this->value_< oth.value_; }
    bool operator<=( const ptr_t& oth ) const noexcept { return this->value_<=oth.value_; }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool operator==( ptr_t<T> B ) const noexcept { return value_ == B.value_; }
    bool operator!=( ptr_t<T> B ) const noexcept { return value_ != B.value_; }

    bool operator==( T* value ) const noexcept { return value_ == value; }
    bool operator!=( T* value ) const noexcept { return value_ != value; }

    T& operator[]( int i ) const noexcept { return value_[i]; }
        
    /*────────────────────────────────────────────────────────────────────────────*/

    T* end() const noexcept { return value_ + size(); }

    T* begin() const noexcept { return value_; }

    /*────────────────────────────────────────────────────────────────────────────*/

    ptr_t& operator=( const ptr_t& other ) noexcept {
        if( this->value_ != &other ){ reset(); copy(other); }
        return *this;
    }

    ptr_t& operator=( ptr_t&& other ) noexcept {
        if( this->value_ != &other ){ reset(); move(std::move(other)); }
        return *this;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint  size() const noexcept { return length_ == nullptr ? 0 : *length_; }
    uint count() const noexcept { return count_  == nullptr ? 0 : *count_; }
    bool  null() const noexcept { return value_  == nullptr ? 1 : 0; }
    bool empty() const noexcept { return null() || size( ) <= 0; }
    T*     get() const noexcept { return value_; }

    /*────────────────────────────────────────────────────────────────────────────*/

    explicit operator bool(void) const { return null(); }
    explicit operator T*(void) const { return value_; }

    T* operator->() const noexcept { return value_; }
    T& operator*() const noexcept { return *value_; }
    T* operator&() const noexcept { return  value_; }

    /*────────────────────────────────────────────────────────────────────────────*/

protected:
    uint* length_= nullptr;
    uint* count_ = nullptr;
       T* value_ = nullptr;

    void copy( const ptr_t& other ) noexcept {
        if( other.count() == 0 ){ return; }
        length_= other.length_;
        count_ = other.count_;
        value_ = other.value_;
        ++(*count_);
    }

    void move( ptr_t&& other ) noexcept {
        count_ = other.count_;
        value_ = other.value_;
        length_= other.length_;
        other.count_ = nullptr;
        other.value_ = nullptr;
        other.length_= nullptr;
    }

};

#endif