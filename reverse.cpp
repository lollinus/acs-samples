#include <algorithm>
#include <functional>

#include <iostream>
#include <iterator>
#include <array>
#include <bitset>
#include <limits>
#include <type_traits>

using namespace std;

// template <typename T>
// struct bit_iterator : public random_access_iterator_tag
// {
// };


// template <typename T, size_t BITS = std::numeric_limits<T>::size >
// struct bit_adapter
// {
//     explicit bit_iterator(T& val) : val_{val} {}

// private:
//     T& val_;
// }
namespace bitmask_adapter
{
/**
 * Sample adapter for given \tparam B to look like STL type container.
 */
template <typename B> struct BitmaskAdapter
{
    static_assert(std::is_integral<B>::value, "Only integer types can be adapted");

    /**
     * @brief Helper class to access individual bits in BitmaskAdapter
     */
    template <typename V>
    struct ValueAdapter
    {
        static_assert(std::is_integral<B>::value, "Only integer types can be adapted");
        typedef V type;

        type* _V_pointer;
        type _V_mask;

        operator bool() const noexcept
        {
            return !!(*_V_pointer & _V_mask);
        }

        ValueAdapter& operator=(bool x) noexcept
        {
            if (x)
                *_V_pointer |= _V_mask;
            else
                *_V_pointer &= ~_V_mask;
            return *this;
        }

        ValueAdapter& operator=(const ValueAdapter& x) noexcept
        {
            return *this = bool(x);
        }

        bool operator==(const ValueAdapter& x) const
        {
            return bool(*this) == bool(x);
        }

        bool operator<(const ValueAdapter& x) const
        {
            return !bool(*this) && bool(x);
        }

        void flip() noexcept
        {
            *_V_pointer ^= _V_mask;
        }
    };

    template <typename I>
    struct BitmaskIterator : public std::iterator<std::random_access_iterator_tag, bool>
    {
        static_assert(std::is_integral<B>::value, "Only integer types can be adapted");

    public:
        typedef typename ValueAdapter<I>::type value_type;
        typedef ValueAdapter<I> reference;
        typedef void pointer;
        typedef BitmaskIterator<I> iterator;

    public:
        value_type* _V_pointer;
        unsigned int _V_offset;

    public:

        /** Standard random iterator requirements */
        BitmaskIterator() : _V_pointer{0}, _V_offset{0} {};

        BitmaskIterator(value_type* v, unsigned int o) : _V_pointer{v}, _V_offset{o}
        {
        }

        reference operator*() const
        {
            return reference(_V_pointer, 1UL << _V_offset);
        }

        iterator& operator++()
        {
            _V_offset++;
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp = *this;
            _V_offset++;
            return tmp;
        }

        iterator& operator--()
        {
            _V_offset--;
            return *this;
        }

        iterator operator--(int)
        {
            iterator tmp = *this;
            _V_offset--;
            return tmp;
        }

        iterator& operator+=(difference_type i)
        {
            increment(i);
            return *this;
        }

        iterator& operator-=(difference_type i)
        {
            *this += -i;
            return *this;
        }

        reference operator[](difference_type i) const
        {
            return *(*this + i);
        }

        friend iterator operator+(const iterator& x, difference_type n)
        {
            iterator tmp = x;
            tmp += n;
            return tmp;
        }

        friend iterator operator+(difference_type n, const iterator& x)
        {
            return x + n;
        }

        friend iterator operator-(const iterator& x, difference_type n)
        {
            iterator tmp = x;
            tmp -= n;
            return tmp;
        }

        /** End Standard random iterator requirements */

    public:
        void increment(ptrdiff_t i)
        {
            difference_type n = i + _V_offset;
            _V_offset = static_cast<unsigned int>(n);
        }
    };

    typedef BitmaskAdapter<B> type;
    /**
     * We are iterating over all bits of a value so keep it internally as unsigned for simplify
     * operations.
     */
    typedef typename make_unsigned<B>::type value;

    typedef size_t size_type;

    typedef ValueAdapter<B> value_type;
    typedef ValueAdapter<B> reference;
    typedef const ValueAdapter<B> const_reference;

    typedef BitmaskIterator<type> iterator;
    typedef const BitmaskIterator<type> const_iterator;

    static constexpr size_type _S_size = numeric_limits<value>::digits;

    /*--------------------------------------------------------------------------------*/
    /* standard Container requirements */

    BitmaskAdapter() : _value{} {};

    BitmaskAdapter(value &v) : _value{v}
    {
    }

    BitmaskAdapter(type& other) : _value{other._value}
    {
    }

    BitmaskAdapter(type &&other) : _value{std::move(other._value)}
    {
    }

    type& operator=(const type& rhs)
    {
        _value = rhs._value;
        return *this;
    }

    type& operator=(type&& rhs)
    {
        _value = std::move(rhs._value);
        return *this;
    }

    ~BitmaskAdapter()
    {
    }

    iterator begin()
    {
        return iterator(*this);
    }

    iterator end()
    {
        return iterator();
    }

    const_iterator cbegin() const
    {
        return begin();
    }

    const_iterator cend() const
    {
        return begin();
    }

    bool operator==(const type& rhs)
    {
        return _value = rhs._value;
    }

    void swap(type& b) noexcept
    {
        swap(_value, b._value);
    }

    /** @brief Returns size of adapted value in bits  */
    size_type size() const noexcept
    {
        return _S_size;
    }

    size_type max_size() const noexcept
    {
        return _S_size;
    }

    constexpr bool empty() const noexcept
    {
        return size() == 0;
    }
    /* End of standard Container */
    /*--------------------------------------------------------------------------------*/

    const value& get() const
    {
        return _value;
    }

private:
    // @todo(barskik) use it as a wrapper for provided value
    // std::reference_wrapper<value> _value; ///< reference to adapter value
    // For now store copy of value internally
    value _value;
};

/*--------------------------------------------------------------------------------*/
/* standard Container requirements */

template<typename T>
inline bool operator!=(const BitmaskAdapter<T> &lhs, const BitmaskAdapter<T> &rhs)
{
    return !(lhs == rhs);
}

template<typename T>
void swap(BitmaskAdapter<T>& a, BitmaskAdapter<T>& b)
{
    a.swap(b);
}
/* End of standard Container */
/*--------------------------------------------------------------------------------*/

}  // namespace bitmask_adapter

namespace bit_iterator_ns
{
template <typename IterableType> struct basic_iterator
{
};
}

int main(int argc, char *argv[])
{

    array<int, 30> a{1, 2, 3};

    array<int, 30>::iterator i;

    vector<bool> bv(32);
    bv.begin();

    long val = 0b1100'1110'1111'1001'0110;

    cout << hex << "val: " << val << dec << endl;

    unsigned int iv{10};
    bitmask_adapter::BitmaskAdapter<unsigned int> ba_i(iv);
    cout << "ba_i: " << ba_i.get() << " : "
         << std::bitset<numeric_limits<unsigned int>::digits>(ba_i.get()) << endl;

    unsigned short is{20};
    bitmask_adapter::BitmaskAdapter<unsigned short> ba_s(is);
    cout << "ba_s: " << ba_s.get() << " : "
         << std::bitset<numeric_limits<unsigned short>::digits>(ba_s.get()) << endl;

    unsigned long il{30};
    bitmask_adapter::BitmaskAdapter<unsigned long> ba_l(il);
    cout << "ba_l: " << ba_l.get() << " : "
         << std::bitset<numeric_limits<unsigned long>::digits>(ba_l.get()) << endl;

    // Won't compile because not met is_integral requirements
    // float f{40};
    // bitmask_adapter::BitmaskAdapter<float> ba_f(f);
    // cout << "ba_f: " << ba_f.get() << endl;

    return 0;
}
