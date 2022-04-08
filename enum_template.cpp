#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <type_traits>


using std;

enum class Zwierz : uint8_t {
  Koza,
  Kura,
  Krowa,
  Pies,
  Kot,
  Lis,

  // helper values used for bounds checks/iteration
  First = Koza,
  Last = Lis, ///< Keep it always assigne with last udpated value
};

constexpr const char kKoza[] = "Koza";
constexpr const char kKura[] = "Kura";

constexpr static const array<const char *const, static_cast<uint8_t>(Zwierz::Last) + 1>
    ZwierzStr{"Koza", "Kura", "Krowa", "Pies", "Kot"};

struct Koza
{
    int nogi_ = 4;

    string glos()
    {
        return "meee";
    }
};

struct Kura
{
    int nogi_ = 2;
    int skrzydla = 2;

    std::string glos()
    {
        return "koko";
    }
};

struct Krowa
{
    int nogi_ = 4;
    int rogi_ = 2;

    string glos()
    {
        return "muu";
    }
};

struct Pies
{
    int nogi_ = 4;

    string glos()
    {
        return "woof";
    }
};

struct Kot
{
    int nogi_ = 4;

    string glos()
    {
        return "meow";
    }
};

struct Lis
{
    int nogi_ = 4;

    string glos()
    {
        return "Hatee-hatee-hatee-ho";
    }
};

/**
 * @class ZwierzB
 *
 * @brief Public interface used as Type adapter for @a ZwierzT.
 */
struct zwierz_base
{
protected:
    zwierz_base(Zwierz p_zwierz, size_t p_index, const char *const p_name)
        : zwierz{p_zwierz}, index{p_index}, name{p_name}
    {
    }

public:
    const Zwierz zwierz;
    const size_t index;
    const char *const name;

    virtual ~zwierz_base() = default;
    virtual Zwierz getZwierz() const noexcept = 0;
    virtual size_t getIndex() const noexcept = 0;
    virtual const char *getName() const noexcept = 0;


    std::function<std::string()> glos_fn;

    template <
        typename T,
        std::enable_if_t<std::is_member_function_pointer<decltype(&T::glos)>::value, bool> = true>
    std::string glos(std::add_const<T> &t) const
    {
        return t.glos();
    }
};

template <Zwierz Z> struct ZwierzT : public zwierz_base {
  using base_type = underlying_type<Zwierz>::type;

  constexpr static Zwierz zwierz = Z;
  constexpr static base_type value = static_cast<base_type>(zwierz);
  constexpr static size_t index = value;
  constexpr static const char *const name = ZwierzStr[value];

  ZwierzT() : zwierz_base(zwierz, index, name) {}

  Zwierz getZwierz() const noexcept override { return zwierz; }
  size_t getIndex() const noexcept override { return index; }
  const char *getName() const noexcept override { return name; }
};

/**
 * @var unique_ptr<ZwierzB> ZwierzPtr
 *
 * @brief Handle used to get generic handler for zwierz types.
 */
using ZwierzPtr = unique_ptr<zwierz_base>;

ZwierzPtr make_zwierz(Zwierz z)
{
    switch (Zwierz(index))
    {
    case Zwierz::Koza:
        z = make_unique<ZwierzT<Zwierz::Koza>>();
        break;
    case Zwierz::Pies:
        z = make_unique<ZwierzT<Zwierz::Pies>>();
        break;
    }

    return nullptr;
}

int main(int argc, char **argv)
{
    cout << "Koza: " << static_cast<int>(ZwierzT<Zwierz::Koza>::value) << " -> "
              << ZwierzT<Zwierz::Koza>::name << endl;

    cout << "Kura: " << static_cast<int>(ZwierzT<Zwierz::Kura>::value) << " -> "
              << ZwierzT<Zwierz::Kura>::name << endl;

    cout << "Pies: " << static_cast<int>(ZwierzT<Zwierz::Pies>::value) << " -> "
              << ZwierzT<Zwierz::Pies>::name << endl;

    cout << "Kot: " << static_cast<int>(ZwierzT<Zwierz::Kot>::value) << " -> "
              << ZwierzT<Zwierz::Kot>::name << endl;

    cout << "----------------------------------------" << endl;

    const char *search = "Pies";
    auto found = find(ZwierzStr.cbegin(), ZwierzStr.cend(), search);
    const size_t index = distance(ZwierzStr.cbegin(), found);

    auto z{make_zwierz(Zwierz::Lis)};

    cout << R"("Koza")"
              << " -> " << z->index << " - " << ZwierzT<Zwierz::Koza>::name << endl;
    cout << R"("Kura")"
              << " -> " << ZwierzT<Zwierz::Kura>::index << " - " << ZwierzT<Zwierz::Kura>::name
              << endl;
    return 0;
}
