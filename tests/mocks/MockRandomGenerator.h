#ifndef MOCK_RANDOM_GENERATOR_H
#define MOCK_RANDOM_GENERATOR_H

#include "core/interfaces/IRandomGenerator.h"
#include <vector>
#include <stdexcept>

namespace password_generator {
namespace tests {

class MockRandomGenerator : public core::interfaces::IRandomGenerator {
private:
    std::vector<int> sequence;
    mutable size_t index = 0;

public:
    explicit MockRandomGenerator(std::vector<int> seq) : sequence(std::move(seq)) {}
    
    int generate(int min, int max) override {
        if (index >= sequence.size()) {
            throw std::runtime_error("MockRandomGenerator: sequence exhausted");
        }
        
        int value = sequence[index++];
        
        // Clamp to range
        if (value < min) value = min;
        if (value > max) value = max;
        
        return value;
    }
    
    void reset() {
        index = 0;
    }
    
    void setSequence(const std::vector<int>& seq) {
        sequence = seq;
        index = 0;
    }
};

} // namespace tests
} // namespace password_generator

#endif // MOCK_RANDOM_GENERATOR_H