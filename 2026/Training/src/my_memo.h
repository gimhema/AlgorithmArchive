#pragma once

template<class T>
class my_unique_ptr
{
private:
    T* Val;

public:
    explicit my_unique_ptr(T* p = nullptr) : Val(p) {}
    ~my_unique_ptr() {delete Val;}

    my_unique_ptr(const my_unique_ptr&) = delete;
    my_unique_ptr& operator=(const my_unique_ptr&) = delete;

    my_unique_ptr(my_unique_ptr&& other) noexcept : Val(other.Val) {
        other.Val = nullptr;
    }
    my_unique_ptr& operator=(my_unique_ptr&&) noexcept {
        if (this != &other) {
            delete Val;
            Val = other.Val;
            other.Val = nullptr;
        }

        return *this;
    }

    T* operator->() const {return Val;}
    T& operator*() const {return *Val;}
    T* get() const {return Val;}

    T* release() {
        T* tmp = Val;
        Val = nullptr;
        return tmp;
    }

    void reset(T* p = nullptr) {
        delete Val;
        Val = p;
    }
};

struct my_ctrl_block {
    RC_SIZE strong = 1;
    RC_SIZE weak = 0;
};

typedef int RC_SIZE;

template<class T>
class my_shared_ptr
{
private:
    T* Val;
    my_ctrl_block* cb;
    RC_SIZE _rc;

public:
    explicit my_shared_ptr(T* p = nullptr) : Val(p), _rc(1) {}
    ~my_shared_ptr() 
    {
        if (cb) {
            if (--cb->strong == 0) {
                delete Val;
                if (cb->weak == 0) delete cb;
            }
        }

        if (_rc && --(*_rc) == 0) {
            delete Val;
            delete _rc;
        }
    }

    my_shared_ptr(const my_shared_ptr& other) : Val(other.Val), _rc(other._rc) {
    if (_rc) ++(*_rc);
    }

    my_shared_ptr& operator=(my_shared_ptr&& other) noexcept {
        if (this != &other) {

            if (_rc && --(*_rc) == 0) {
                delete Val;
                delete _rc;
            }

            Val = other.Val;
            _rc = other._rc;

            other.Val = nullptr;
            other._rc = nullptr;
        }
        return *this;
    }

    T* operator->() const {return Val;}
    T& operator*() const {return *Val;}
    T* get() const {return Val;}


    void reset(T* p = nullptr) {
        if (_rc && --(*_rc) == 0) {
            delete Val;
            delete _rc;
        }
        Val = p;
        _rc = p ? new RC_SIZE(1) : nullptr;
    }

public:
    RC_SIZE get_rc() const {return _rc;}

};

template<class T>
using WEAK_REF_PTR = my_shared_ptr<T>;

template<class T>
class my_weak_ptr
{
private:
    T* Val;
    my_ctrl_block* cb;
    RC_SIZE* _rc;

public:
    ~my_weak_ptr() {
    if (cb) {
        if (--cb->weak == 0 && cb->strong == 0) {
            delete cb;
        }
    }
}
};