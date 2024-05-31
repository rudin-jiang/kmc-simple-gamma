#include "reaction.hpp"
#include "specinfo.hpp"
#include "nhfstr.hpp"
#include "type.hpp"

#include <cassert>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

static const double ev2jmol = 96485.0;
static const double amu = 1.66054e-27;
static const double kb = 1.380649e-23;
static const double hh = 6.626070e-34;
static const double RR = 8.3144626182;
static const double pi = 3.1415926536;

Reaction::Reaction(): reactCase(OFF), radius(0.0), factor(0.0), activa(0.0),
area(0.0), mass(0.0), theta(0.0), sigma(0.0), stick(0.0),enads(0.0), press(0.0) {}

SizeType Reaction::reac_num() const { return reacTypes.size(); }
SizeType Reaction::prod_num() const { return prodTypes.size(); }

double Reaction::rate_constant(double temperature) const {
    assert(reactCase < 3);
    if (reactCase == 0) return rate_constant_0(temperature);
    if (reactCase == 1) return rate_constant_1(temperature);
    if (reactCase == 2) return rate_constant_2(temperature);

    assert(false);
    throw std::runtime_error("Shouldn't be here.");
    return 0.0;
}

double Reaction::rate_constant_0(double temperature) const {
    assert(reactCase == 0);
    double inSqrt = 2.0 * pi * mass * amu * kb * temperature;
    return press * area * stick / std::sqrt(inSqrt);
}

double Reaction::rate_constant_1(double temperature) const {
    assert(reactCase == 1);
    double inExp = - enads * ev2jmol / (RR * temperature);
    return kb * std::pow(temperature / hh, 3.0) * area 
            * 2.0 * pi * mass * amu * kb
            / theta / sigma * std::exp(inExp);
}

double Reaction::rate_constant_2(double temperature) const {
    assert(reactCase == 2);
    const double RT = RR * temperature;
    return factor * std::exp(- activa * ev2jmol / RT);
}


std::string Reaction::rate_expression() const {
    assert(reactCase < 3);

    if (reactCase == 0) return rate_expression_0();
    if (reactCase == 1) return rate_expression_1();
    if (reactCase == 2) return rate_expression_2();

    assert(false);
    throw std::runtime_error("Shouldn't be here.");
    return std::string();
}

std::string Reaction::rate_expression_0() const {
    char buff[128];

    double inSqrt = 2.0 * pi * mass * amu * kb;
    double pre = press * area * stick / std::sqrt(inSqrt);
    std::sprintf(buff, "%.10e / T^0.5", pre);
    return std::string(buff); 
}

std::string Reaction::rate_expression_1() const {
    char buff[128];

    double pre = kb * std::pow(1.0 / hh, 3.0) * area
                 * 2.0 * pi * mass * amu * kb / theta / sigma;
    double inExp = - enads * ev2jmol;

    std::sprintf(buff, "%.10e * T^3.0 * exp(%.10e / RT)", pre, inExp);
    return std::string(buff);
}

std::string Reaction::rate_expression_2() const {
    char buff[128];
    std::sprintf(buff, "%.10e * exp(%.10e / RT)", factor, - activa * ev2jmol);
    return std::string(buff);
}

ReactionVec::ReactionVec() {}

ReactionVec::ReactionVec(std::string filename, const SpecInfoVec &specInfoList) {
    std::ifstream ifs(filename);
    assert(ifs.good());

    std::string line;
    while (std::getline(ifs, line)) {
        remove_comment(line);
        remove_dual_space(line);
        if (line.empty()) continue;

        // AR/HK  reaction  data
        std::vector<std::string> record = split_string(line, ";");
        for (std::string &str : record) remove_dual_space(str);

        assert(record.size() == 3);
        assert(record[0] == "AR" || record[0] == "HK");

        if (record[0] == "AR") {
            std::vector<std::string> reac_and_prod = split_string(record[1], "=>");
            assert(reac_and_prod.size() == 2);
            
            std::vector<std::string> reacSymbList = split_string(reac_and_prod[0], "+");
            std::vector<std::string> prodSymbList = split_string(reac_and_prod[1], "+");

            for (std::string &str : reacSymbList) remove_dual_space(str);
            for (std::string &str : prodSymbList) remove_dual_space(str);

            SizeVec reacTypes(reacSymbList.size());
            SizeVec prodTypes(prodSymbList.size());

            for (std::size_t i = 0; i < reacSymbList.size(); ++i) {
                std::string chemSymb = reacSymbList[i];
                SizeType specIdx = specInfoList.specinfo_index(chemSymb);
                assert(specIdx < specInfoList.size());
                assert(specInfoList[specIdx].chemSymb == chemSymb);
                reacTypes[i] = specIdx;
            }

            for (std::size_t i = 0; i < prodSymbList.size(); ++i) {
                std::string chemSymb = prodSymbList[i];
                SizeType specIdx = specInfoList.specinfo_index(chemSymb);
                assert(specIdx < specInfoList.size());
                assert(specInfoList[specIdx].chemSymb == chemSymb);
                prodTypes[i] = specIdx;
            }

            // parameters
            RealVec paramList;
            double tmp = 0;

            std::istringstream iss(record[2]);
            while (iss >> tmp) paramList.push_back(tmp);
            assert(paramList.size() == 6);

            Reaction rf, rb;

            // forward
            rf.reactCase = 2;
            rf.reacTypes = reacTypes;
            rf.prodTypes = prodTypes;
            rf.factor = paramList[0];
            rf.activa = paramList[2];
            rf.radius = paramList[4];

            // backward
            rb.reactCase = 2;
            rb.reacTypes = prodTypes;
            rb.prodTypes = reacTypes;
            rb.factor = paramList[1];
            rb.activa = paramList[3];
            rb.radius = paramList[5];

            reactionList.push_back(rf);
            reactionList.push_back(rb);
        }

        if (record[0] == "HK") {
            std::string chemSymb = record[1];
            remove_dual_space(chemSymb);
            SizeType specIdx = specInfoList.specinfo_index(chemSymb);
            assert(specIdx < specInfoList.size());

            Reaction r;
            std::istringstream iss(record[2]);

            iss >> r.area >> r.mass >> r.theta 
                >> r.sigma >> r.stick >> r.enads >> r.press;
            r.reacTypes.push_back(specIdx);

            r.reactCase = 0; reactionList.push_back(r);
            r.reactCase = 1; reactionList.push_back(r);
        }
    }
    ifs.close();
}


SizeType ReactionVec::size() const {
    return reactionList.size();
}

const Reaction& ReactionVec::operator[](SizeType idx) const {
    assert(idx < reactionList.size());
    return reactionList[idx];
}

void ReactionVec::print_rate_constant(double temperature) const {
    for (std::size_t i = 0; i < reactionList.size() - 1; i += 2) {
        double forward = reactionList[i    ].rate_constant(temperature);
        double bakward = reactionList[i + 1].rate_constant(temperature);
        std::printf("%15.2e  %15.2e\n", forward, bakward);
    }
}