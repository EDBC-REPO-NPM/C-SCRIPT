#ifndef NODEPP_PTR
#define NODEPP_PTR

template<class T>
class ptr_t { public:

   ~ptr_t(){ reset(); }

    ptr_t() noexcept = default;

    ptr_t(T* value) {
        if (value == nullptr){ return; }
        count_ = new uint(1);
        value_ = value;
    }

    ptr_t(const ptr_t& other){ copy(other); }

    ptr_t(ptr_t&& other) noexcept { copy(other); }

    /*────────────────────────────────────────────────────────────────────────────*/

    void reset() noexcept {
        if (count_ == nullptr){ return; }
        if (--(*count_) == 0) {
            delete count_;
            delete value_;
        }
        count_ = nullptr;
        value_ = nullptr;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    T& operator[]( int i ) const noexcept { return value_[i]; }

    ptr_t& operator=(const ptr_t& other) {
        if( this->value_ != &other ){ reset(); copy(other); }
        return *this;
    }

    ptr_t& operator=(ptr_t&& other) noexcept {
        if( this->value_ != &other ){ reset(); copy(other); }
        return *this;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool operator==( ptr_t<T> B ) const noexcept { return value_ == B.value_; }
    bool operator!=( ptr_t<T> B ) const noexcept { return value_ != B.value_; }

    bool operator==( T* value ) const noexcept { return value_ == value; }
    bool operator!=( T* value ) const noexcept { return value_ != value; }

    /*────────────────────────────────────────────────────────────────────────────*/

    explicit operator bool() const noexcept { return value_ != nullptr; }

    T* operator->() const noexcept { return value_; }

    T& operator*() const noexcept { return *value_; }

    T* operator&() const noexcept { return value_; }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint count() const noexcept { return ( count_ != nullptr) ? (uint)(*count_) : 0; }

    T* get() const noexcept { return value_; }

protected:
    uint* count_ = nullptr;
       T* value_ = nullptr;

    void copy(const ptr_t& other) {
        if (other.count_ == nullptr){ return; }
        count_ = other.count_;
        value_ = other.value_;
        ++(*count_);
    }

    void move(ptr_t&& other) noexcept {
        count_ = other.count_;
        value_ = other.value_;
        other.count_ = nullptr;
        other.value_ = nullptr;
    }

};

#endif