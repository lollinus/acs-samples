#include <algorithm>
#include <array>
#include <iostream>
#include <memory>

enum class Zwierz : uint8_t {
  Koza,
  Kura,
  Krowa,
  Pies,
  Kot,

  First = Koza,
  Last = Kot,
};

constexpr const char kKoza[] = "Koza";
constexpr const char kKura[] = "Kura";

constexpr static const std::array<const char *const, static_cast<uint8_t>(Zwierz::Last) + 1>
    ZwierzStr{"Koza", "Kura", "Krowa", "Pies", "Kot"};

struct ZwierzB {
  protected:
  ZwierzB(Zwierz p_zwierz, size_t p_index, const char *const p_name)
      : zwierz{p_zwierz}, index{p_index}, name{p_name} {}

  public:
  const Zwierz zwierz;
  const size_t index;
  const char *const name;

  virtual ~ZwierzB() = default;
  virtual Zwierz getZwierz() const noexcept = 0;
  virtual size_t getIndex() const noexcept = 0;
  virtual const char *getName() const noexcept = 0;
};

template <Zwierz Z> struct ZwierzT : public ZwierzB {
  using base_type = std::underlying_type<Zwierz>::type;

  constexpr static Zwierz zwierz = Z;
  constexpr static base_type value = static_cast<base_type>(zwierz);
  constexpr static size_t index = value;
  constexpr static const char *const name = ZwierzStr[value];

  ZwierzT() : ZwierzB(zwierz, index, name) {}

  Zwierz getZwierz() const noexcept override { return zwierz; }
  size_t getIndex() const noexcept override { return index; }
  const char *getName() const noexcept override { return name; }
};

int main(int argc, char **argv) {
  std::cout << "Koza: " << static_cast<int>(ZwierzT<Zwierz::Koza>::value) << " -> "
            << ZwierzT<Zwierz::Koza>::name << std::endl;

  std::cout << "Kura: " << static_cast<int>(ZwierzT<Zwierz::Kura>::value) << " -> "
            << ZwierzT<Zwierz::Kura>::name << std::endl;

  std::cout << "Pies: " << static_cast<int>(ZwierzT<Zwierz::Pies>::value) << " -> "
            << ZwierzT<Zwierz::Pies>::name << std::endl;

  std::cout << "Kot: " << static_cast<int>(ZwierzT<Zwierz::Kot>::value) << " -> "
            << ZwierzT<Zwierz::Kot>::name << std::endl;

  std::cout << "----------------------------------------" << std::endl;

  const char *search = "Pies";
  auto found = std::find(ZwierzStr.cbegin(), ZwierzStr.cend(), search);
  const size_t index = std::distance(ZwierzStr.cbegin(), found);

  std::unique_ptr<ZwierzB> z;
  switch (Zwierz(index)) {
  case Zwierz::Koza:
    z = std::make_unique<ZwierzT<Zwierz::Koza>>();
    break;
  case Zwierz::Pies:
    z = std::make_unique<ZwierzT<Zwierz::Pies>>();
    break;
  }

  std::cout << R"("Koza")"
            << " -> " << z->index << " - " << ZwierzT<Zwierz::Koza>::name << std::endl;
  std::cout << R"("Kura")"
            << " -> " << ZwierzT<Zwierz::Kura>::index << " - " << ZwierzT<Zwierz::Kura>::name
            << std::endl;
  return 0;
}
