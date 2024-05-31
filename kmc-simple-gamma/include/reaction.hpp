#pragma once

#include "type.hpp"
#include "specinfo.hpp"
#include <string>
#include <vector>

class Reaction {
public:
    SizeType    reactCase;   // 0: ads  1: des  2: lattice
    SizeVec     reacTypes;
    SizeVec     prodTypes;

    double      radius;     // reaction radius
    double      factor;     // pre-exponential factor
    double      activa;     // activation energy

    double      area;       // m^2
    double      mass;       // amu
    double      theta;      // K
    double      sigma;      // 1
    double      stick;      // 1
    double      enads;      // eV
    double      press;      // pa

    Reaction();

    SizeType reac_num() const;
    SizeType prod_num() const;

    double rate_constant(double temperature) const;
    double rate_constant_0(double temperature) const;
    double rate_constant_1(double temperature) const;
    double rate_constant_2(double temperature) const;

    std::string rate_expression() const;
    std::string rate_expression_0() const;
    std::string rate_expression_1() const;
    std::string rate_expression_2() const;
};


class ReactionVec {
public:
    std::vector<Reaction> reactionList;

    ReactionVec();
    ReactionVec(std::string filename, const SpecInfoVec &specInfoList);

    SizeType size() const;
    const Reaction& operator[](SizeType idx) const;

    void print_rate_constant(double temperature) const;
};