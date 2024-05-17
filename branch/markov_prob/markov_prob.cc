#include <array>
#include <cstdint>
#include <map>
#include <random>

#include "ooo_cpu.h"

namespace
{
constexpr std::size_t BIMODAL_TABLE_SIZE = 16384;
constexpr std::size_t BIMODAL_PRIME = 16381;
constexpr std::size_t MARKOV_ORDER = 2;

class MarkovPredictor final
{
public:
  bool predict() const { return table_[lastPattern].predict(); }

  void update(bool taken)
  {
    table_[lastPattern].update(taken);
    lastPattern <<= 1;
    lastPattern |= taken;
    lastPattern %= table_.size();
  }

private:
  class Entry
  {
    std::array<std::size_t, 2> freqs{};

  public:
    bool predict() const
    {
      const auto sum = freqs[0] + freqs[1];
      std::mt19937 gen{};
      std::bernoulli_distribution pred{static_cast<double>(freqs[1]) / sum};

      return pred(gen);
    }
    void update(bool taken) { freqs[taken]++; }
  };

  std::array<Entry, MARKOV_ORDER << 1> table_{};
  std::uint8_t lastPattern{};
};

std::map<O3_CPU*, std::array<MarkovPredictor, BIMODAL_TABLE_SIZE>> freq_table;
} // namespace

void O3_CPU::initialize_branch_predictor() {}

uint8_t O3_CPU::predict_branch(uint64_t ip)
{
  auto hash = ip % ::BIMODAL_PRIME;
  const auto& pred = ::freq_table[this][hash];

  return pred.predict();
}

void O3_CPU::last_branch_result(uint64_t ip, uint64_t branch_target, uint8_t taken, uint8_t branch_type)
{
  auto hash = ip % ::BIMODAL_PRIME;
  ::freq_table[this][hash].update(taken);
}
