#include "random.hpp"
#include <cassert>
#include <cstddef>
#include <random>
#include <sstream>

static std::random_device rd;
static std::mt19937 gen(rd());


static std::uniform_int_distribution<std::size_t> disSizeT(0);

std::size_t uniform_random_sizet(void) {
    return disSizeT(gen);
}

std::size_t generate_uuid(void) {
    static std::size_t uuid = 0;
    return uuid++;
}


static std::uniform_real_distribution<double> disReal01(0.0, 1.0);

double uniform_random_real(void) {
    return disReal01(gen);
}

double uniform_random_real(double a, double b) {
    std::uniform_real_distribution<double> disReal(a, b);
    return disReal(gen);
}


static std::uniform_int_distribution<> disHex(0, 15);

std::string generate_token(std::size_t len) {
    std::stringstream ss;
    ss << std::hex;
    for (std::size_t i = 0; i < len; ++i) {
        ss << disHex(gen);
    }

    return ss.str();
}