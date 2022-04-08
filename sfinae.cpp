#include <iostream>
#include <type_traits>

using namespace std;

struct Iface
{
public:
    virtual ~Iface() = default;

    virtual int get_c() const = 0;
};

struct DerivedA : public Iface
{
public:
    DerivedA() : c_{-1} {};
    explicit DerivedA(int c) : c_{c}
    {
    }

    virtual ~DerivedA() = default;

    void set_c(int c)
    {
        c_ = c;
    }

    int get_c() const override
    {
        return c_;
    }

private:
    int c_;
};

struct DerivedB : public Iface
{
public:
    DerivedB() : c_{-2} {}
    explicit DerivedB(int c) : c_(c)
    {
    }

    virtual ~DerivedB() = default;

    int get_c() const override
    {
        return c_;
    }

public:

    void setC(int c)
    {
        c_ = c;
    }

private:
    int c_;
};

struct Unrelated {
    int c;
};

template <typename T,
          enable_if_t<is_member_function_pointer<decltype(&T::set_c)>::value, bool> = true>
void set_c(T &t, int c)
{
    t.set_c(c);
}

template <typename T,
          enable_if_t<is_member_function_pointer<decltype(&T::setC)>::value, bool> = true>
void set_c(T &t, int c)
{
    t.setC(c);
}

template <typename T, enable_if_t<is_member_object_pointer<decltype(&T::c)>::value, bool> = true>
void set_c(T &t, int c)
{
    t.c = c;
}

int main(int argc, char **argv)
{

    DerivedA da{};
    DerivedB db{};
    Unrelated u{};

    cout << "da: " << da.get_c() << endl;
    // cout << "da::set_c: is: " << is_member_function_pointer<decltype(&DerivedA::set_c)>::value
    //      << endl;

    set_c(da, 2);
    cout << "da: " << da.get_c() << endl;

    cout << "db: " << db.get_c() << endl;
    // cout << "db::setC: is: " << is_member_function_pointer<decltype(&DerivedB::setC)>::value
    //      << endl;

    set_c(db, 40);
    cout << "da: " << da.get_c() << endl;

    cout << "u: " << u.c << endl;

    set_c(u, 33);
    cout << "u: " << u.c << endl;

    return 0;
}
