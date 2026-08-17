

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

