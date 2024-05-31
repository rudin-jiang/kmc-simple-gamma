#include "utility.hpp"
#include "type.hpp"
#include "random.hpp"
#include <cmath>
#include <cassert>

SizeType reg_event_selector(const RealVec &rateList) {
    RealVec  cumsum = rateList;
    for (SizeType i = 1; i < cumsum.size(); i++) {
        cumsum[i] += cumsum[i-1];
    }

    double rateSum = cumsum.back();

    if (rateSum == 0.0) {
        return OFF;
    }

    double cmpNum = rateSum * uniform_random_real();
    SizeType event = 0;
    while (cmpNum > cumsum[event]) event++;

    return event;
}

SizeType log_event_selector(const RealVec &rateList) {
    const double epsilon = 1.0;
    RealVec logRate(rateList.size(), 0.0);
    for (size_t i = 0; i < logRate.size(); i++) {
        logRate[i] = std::log(rateList[i] + epsilon);
    }
    return reg_event_selector(logRate);
}


SizeType sqrt2_event_selector(const RealVec &rateList) {
    RealVec sqrt2Rate(rateList.size(), 0.0);
    for (size_t i = 0; i < sqrt2Rate.size(); i++) {
        assert(rateList[i] >= 0.0);
        sqrt2Rate[i] = std::sqrt(rateList[i]);
    }
    return reg_event_selector(sqrt2Rate);
}

static SizeType sqrt_n_event_selector(const RealVec &rateList, double n) {
    RealVec sqrt_n_Rate(rateList.size(), 0.0);
    for (size_t i = 0; i < sqrt_n_Rate.size(); i++) {
        assert(rateList[i] >= 0.0);
        sqrt_n_Rate[i] = std::pow(rateList[i], 1.0 / n);
    }
    return reg_event_selector(sqrt_n_Rate);
}

SizeType sqrt3_event_selector(const RealVec &rateList) {
    return sqrt_n_event_selector(rateList, 3.0);
}

SizeType sqrt4_event_selector(const RealVec &rateList) {
    return sqrt_n_event_selector(rateList, 4.0);
}