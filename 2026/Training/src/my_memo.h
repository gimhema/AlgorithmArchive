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

typedef int RC_SIZE;

struct my_ctrl_block {
    RC_SIZE strong = 1;
    RC_SIZE weak = 0;
};



template<class T>
class my_weak_ptr;

template<class T>
class my_shared_ptr
{
private:
    T* Val;
    my_ctrl_block* cb;

    my_shared_ptr(T* p, my_ctrl_block* c) : Val(p), cb(c) {
        if (cb) ++cb->strong;
    }

    friend class my_weak_ptr<T>;

public:
    explicit my_shared_ptr(T* p = nullptr) : Val(p), cb(p ? new my_ctrl_block() : nullptr) {}
    ~my_shared_ptr() 
    {
        if (cb) {
            if (--cb->strong == 0) {
                delete Val;
                if (cb->weak == 0) delete cb;
            }
        }
    }

    my_shared_ptr(const my_shared_ptr& other) : Val(other.Val), cb(other.cb) {
        if (cb) ++cb->strong;
    }

    my_shared_ptr& operator=(const my_shared_ptr& other) {
        if (this != &other) {
            if (cb && --cb->strong == 0) {
                delete Val;
                if (cb->weak == 0) delete cb;
            }

            Val = other.Val;
            cb = other.cb;
            if (cb) ++cb->strong;
        }
        return *this;
    }

    my_shared_ptr& operator=(my_shared_ptr&& other) noexcept {
        if (this != &other) {

            if (cb && --cb->strong == 0) {
                delete Val;
                if (cb->weak == 0) delete cb;
            }

            Val = other.Val;
            cb = other.cb;

            other.Val = nullptr;
            other.cb = nullptr;
        }
        return *this;
    }

    T* operator->() const {return Val;}
    T& operator*() const {return *Val;}
    T* get() const {return Val;}


    void reset(T* p = nullptr) {
        if (cb && --cb->strong == 0) {
            delete Val;
            if (cb->weak == 0) delete cb;
        }
        Val = p;
        cb = p ? new my_ctrl_block() : nullptr;
    }

public:
    my_ctrl_block* get_cb() const {return cb;}

};

template<class T>
using WEAK_REF_PTR = my_shared_ptr<T>;

template<class T>
class my_weak_ptr
{
private:
    T* Val;
    my_ctrl_block* cb;

public:
    my_weak_ptr() : Val(nullptr), cb(nullptr) {}
    
    my_weak_ptr(const my_shared_ptr<T>& sp) : Val(sp.get()), cb(sp.get_cb()) {
        if(cb) ++cb->weak;
    }

    my_weak_ptr(const my_weak_ptr& other) : Val(other.Val), cb(other.cb) {
        if(cb) ++cb->weak;
    }

    my_weak_ptr& operator=(const my_weak_ptr& other) {
        if(this != &other) {
            if(cb && --cb->weak == 0 && cb->strong == 0) delete cb;
            Val = other.Val;
            cb = other.cb;
            if (cb) ++cb->weak;
        }

        return *this;
    }

    ~my_weak_ptr() {
        if (cb) {
            if (--cb->weak == 0 && cb->strong == 0) {
                delete cb;
            }
        }
    }

    bool expired() const {return !cb || cb->strong == 0;}

    my_shared_ptr<T> lock() const {
        if(expired()) return my_shared_ptr<T>();
        return my_shared_ptr<T>(Val, cb);
    }
};