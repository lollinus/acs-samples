#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <memory>
#include <type_traits>
#include <variant>

using namespace std;

enum class Zwierz : uint8_t
{
    KOZA = 0,
    KURA,
    KOT,
    LIS,

    _FIRST = KOZA,  ///< Keep it always assigned to first value
    _LAST = LIS,    ///< Keep it always assigned with last udpated value
};

constexpr underlying_type<Zwierz>::type ZwierzFirst{
    static_cast<underlying_type<Zwierz>::type>(Zwierz::_FIRST)};
constexpr underlying_type<Zwierz>::type ZwierzLast{
    static_cast<underlying_type<Zwierz>::type>(Zwierz::_LAST)};

constexpr static const array<string_view, ZwierzLast + 1> ZwierzStr{"Koza", "Kura", "Kot", "Lis"};

/**
 * @brief Generic function to get integral value out of Enumerated type value.
 *
 * @param[in] e enumerated value.
 *
 * @return \a e value converted to underlying integral type.
 */
template <typename E> constexpr auto toIntegral(E e) -> typename std::underlying_type<E>::type
{
    return static_cast<typename std::underlying_type<E>::type>(e);
}

struct Koza
{
    int nogi_ = 4;
    int rogi_ = 2;

    string glos() const { return "meee"; }
};

struct Kura
{
    int nogi = 2;
    int skrzydla = 2;

    string glos() const { return "koko"; }
};

struct Kot
{
    int nogi = 4;

    string glos() const { return "meow"; }
};

struct Lis
{
    int nogi = 4;

    string glos() const { return what_the_fox_says(); }
    string what_the_fox_says() const { return "Hatee-hatee-hatee-ho"; }
};

/** Sample message containing one of possible zwierz instances
 * @note this is to simulate Protobuf generated message with \c oneof field.
 *
 */
class zwierz_message
{
private:
    using Options = std::variant<Koza, Kura, Kot, Lis>;

private:
    Options _value;
    size_t _count;

public:
    zwierz_message() = default;

    zwierz_message(Koza k, size_t c) : _value{k}, _count{c} {}
    zwierz_message(Kura k, size_t c) : _value{k}, _count{c} {}
    zwierz_message(Kot k, size_t c) : _value{k}, _count{c} {}
    zwierz_message(Lis l, size_t c) : _value{l}, _count{c} {}

    size_t get_index() const noexcept { return _value.index(); }

    size_t get_count() const { return _count; }
    void set_count(size_t c) { _count = c; }

    void set_koza(Koza k) { _value = std::move(k); }
    void set_kura(Kura k) { _value = std::move(k); }
    void set_kot(Kot k) { _value = std::move(k); }
    void set_lis(Lis l) { _value = l; }
    bool has_koza() const noexcept { return holds_alternative<Koza>(_value); }
    bool has_kura() const noexcept { return holds_alternative<Kura>(_value); }
    bool has_kot() const noexcept { return holds_alternative<Kot>(_value); }
    bool has_lis() const noexcept { return holds_alternative<Lis>(_value); }

    const Koza& koza() const { return get<Koza>(_value); }
    const Kura& kura() const { return get<Kura>(_value); }
    const Kot& kot() const { return get<Kot>(_value); }
    const Lis& lis() const { return get<Lis>(_value); }
};

/**
 * @class zwierz_base
 *
 * @brief Public interface used as Type adapter for @a ZwierzTraits.
 */
struct zwierz_worker
{
public:
    virtual ~zwierz_worker() = default;

    virtual Zwierz getZwierz() const noexcept = 0;
    virtual size_t getIndex() const noexcept = 0;
    virtual std::string_view get_name() const noexcept = 0;

    virtual size_t count(const zwierz_message& msg) const noexcept = 0;
    virtual string glos(const zwierz_message& msg) const noexcept = 0;
};

/**
 * @tparam ZT Type of zwierz.
 *
 * @brief Generic class providing adapter functions to extract data from Zwierz object based on
 *        Zwierz @tparam ZT
 *
 *        For each supported Zwier should be provided specialization.
 *        The aforementioned specialization provide definitions of all static members related to
 *        Zwierz provided as \p ZT.
 */
template <Zwierz ZT> struct ZwierzTraits;

template <> struct ZwierzTraits<Zwierz::KURA>
{
    using MessageType = zwierz_message;
    using DataType = Kura;

    typedef const DataType& (MessageType::*getDataFnT)() const;
    typedef bool (MessageType::*hasDataFnT)() const;

    using base_type = underlying_type<Zwierz>::type;
    using type = ZwierzTraits<Zwierz::KURA>;

    constexpr static Zwierz zwierz = Zwierz::KURA;
    constexpr static base_type value = static_cast<base_type>(zwierz);
    constexpr static std::string_view name = ZwierzStr.at(toIntegral(zwierz));

    constexpr static const hasDataFnT hasDataFn = &MessageType::has_kura;
    constexpr static const getDataFnT getDataFn = &MessageType::kura;
};

template <> struct ZwierzTraits<Zwierz::KOZA>
{
    using MessageType = zwierz_message;
    using DataType = Koza;

    typedef const DataType& (MessageType::*getDataFnT)() const;
    typedef bool (MessageType::*hasDataFnT)() const;

    using base_type = underlying_type<Zwierz>::type;
    using type = ZwierzTraits<Zwierz::KOZA>;

    constexpr static Zwierz zwierz = Zwierz::KOZA;
    constexpr static base_type value = static_cast<base_type>(zwierz);
    constexpr static std::string_view name = ZwierzStr.at(toIntegral(zwierz));

    constexpr static const hasDataFnT hasDataFn = &MessageType::has_koza;
    constexpr static const getDataFnT getDataFn = &MessageType::koza;
};

template <> struct ZwierzTraits<Zwierz::KOT>
{
    using MessageType = zwierz_message;
    using DataType = Kot;

    typedef bool (MessageType::*hasDataFnT)() const;
    typedef const DataType& (MessageType::*getDataFnT)() const;

    using base_type = underlying_type<Zwierz>::type;
    using type = ZwierzTraits<Zwierz::KOT>;

    constexpr static Zwierz zwierz = Zwierz::KOT;
    constexpr static base_type value = static_cast<base_type>(zwierz);
    constexpr static std::string_view name = ZwierzStr.at(toIntegral(zwierz));

    constexpr static const hasDataFnT hasDataFn = &MessageType::has_kot;
    constexpr static const getDataFnT getDataFn = &MessageType::kot;
};

template <> struct ZwierzTraits<Zwierz::LIS>
{
    using MessageType = zwierz_message;
    using DataType = Lis;

    // typedef const DataType& (MessageType::*getDataFnT)() const;
    // typedef bool (MessageType::*hasDataFnT)() const;

    using base_type = underlying_type<Zwierz>::type;
    using type = ZwierzTraits<Zwierz::LIS>;

    constexpr static Zwierz zwierz = Zwierz::LIS;
    constexpr static base_type value = static_cast<base_type>(zwierz);
    constexpr static std::string_view name = ZwierzStr.at(toIntegral(zwierz));

    constexpr static bool (MessageType::*hasDataFn)() const = &MessageType::has_lis;
    constexpr static const DataType& (MessageType::*getDataFn)() const = &MessageType::lis;
};

template <Zwierz ZT> struct ZwierzAdapter : public ZwierzTraits<ZT>
{
    using traits = ZwierzTraits<ZT>;
    using MessageType = typename traits::MessageType;
    using DataType = typename traits::DataType;

    static size_t count(const typename traits::MessageType& m) noexcept { return m.get_count(); }
    static size_t set_count(const typename traits::MessageType& m, size_t v) noexcept
    {
        return m.set_count(v);
    }

    static bool hasZwierz(const MessageType& msg) noexcept { return (msg.*traits::hasDataFn)(); }

    static string getGlos(const MessageType& msg) noexcept
    {
        return (msg.*traits::getDataFn)().glos();
    }
};

template <Zwierz ZT, typename ZA = ZwierzAdapter<ZT>> struct zwierz_worker_t : public zwierz_worker
{
public:
    typedef ZA adapter;

public:
    constexpr static Zwierz zwierz_value = ZT;

public:
    /* zwierz_worker public interface */

    Zwierz getZwierz() const noexcept override { return adapter::zwierz; }
    size_t getIndex() const noexcept override { return adapter::value; }
    std::string_view get_name() const noexcept override { return adapter::name; }

    size_t count(const typename adapter::MessageType& msg) const noexcept override
    {
        return adapter::count(msg);
    }

    string glos(const typename adapter::MessageType& msg) const noexcept override
    {
        return adapter::getGlos(msg);
    }
};

/**
 * @var unique_ptr<ZwierzB> ZwierzPtr
 *
 * @brief Handle used to get generic handler for zwierz types.
 */
using ZwierzPtr = unique_ptr<zwierz_worker>;

ZwierzPtr make_zwierz(int z)
{
    switch (Zwierz(z))
    {
    case Zwierz::KOZA:
        return make_unique<zwierz_worker_t<Zwierz::KOZA>>();
    case Zwierz::KURA:
        return make_unique<zwierz_worker_t<Zwierz::KURA>>();
    case Zwierz::KOT:
        return make_unique<zwierz_worker_t<Zwierz::KOT>>();
    case Zwierz::LIS:
        return make_unique<zwierz_worker_t<Zwierz::LIS>>();
    }

    return nullptr;
}

int main(int argc, char** argv)
{
    cout << "Koza: " << static_cast<int>(ZwierzTraits<Zwierz::KOZA>::value) << " -> "
         << ZwierzTraits<Zwierz::KOZA>::name << endl;

    cout << "Kura: " << static_cast<int>(ZwierzTraits<Zwierz::KURA>::value) << " -> "
         << ZwierzTraits<Zwierz::KURA>::name << endl;

    cout << "Lis: " << static_cast<int>(ZwierzTraits<Zwierz::LIS>::value) << " -> "
         << ZwierzTraits<Zwierz::LIS>::name << endl;

    cout << "Kot: " << static_cast<int>(ZwierzTraits<Zwierz::KOT>::value) << " -> "
         << ZwierzTraits<Zwierz::KOT>::name << endl;

    cout << "----------------------------------------" << endl;

    std::vector<zwierz_message> messages{
        {Lis{}, 1}, {Kura{}, 20}, {Koza{}, 40}, {Lis{}, 22}, {Kot{}, 100}};

    for (const auto& msg : messages)
    {
        auto z = make_zwierz(msg.get_index());

        if (!z)
        {
            cerr << "Error getting adapter for : " << msg.get_index() << endl;
        }
        else
        {
            cout << setw(5) << z->get_name() << " idx: " << setw(3) << z->getIndex()
                 << " count: " << setw(5) << z->count(msg) << " says: \"" << z->glos(msg) << "\""
                 << endl;
        }
    }
    return 0;
}
