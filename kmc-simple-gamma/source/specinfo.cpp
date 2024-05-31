#include "specinfo.hpp"
#include "type.hpp"
#include "nhfstr.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <exception>

SpecInfo::SpecInfo(): siteType(OFF), fixCover(OFF), iniCover(0.0) {}

SpecInfo::SpecInfo(std::string line) {
    std::istringstream iss(line);
    assert(iss.good());
    iss >> chemSymb >> siteType >> fixCover >> iniCover;
}

std::string SpecInfo::to_string() const {
    char buff[64];
    std::sprintf(buff, "%-10s    %1zu    %1zu    %.5lf\n", 
                chemSymb.data(), siteType, fixCover, iniCover);
    return std::string(buff);
}


SpecInfoVec::SpecInfoVec() {}

SpecInfoVec::SpecInfoVec(std::string filename) {
    std::ifstream ifs(filename);
    assert(ifs.good());

    std::string line;
    while (std::getline(ifs, line)) {
        remove_comment(line);
        remove_dual_space(line);
        if (line.empty()) continue;

        specInfoList.push_back(SpecInfo(line));
    }
    ifs.close();
}

SizeType SpecInfoVec::size() const {
    return specInfoList.size();
}

const SpecInfo& SpecInfoVec::operator[](SizeType idx) const {
    assert(idx < specInfoList.size());
    return specInfoList[idx];
}


SizeType SpecInfoVec::specinfo_index(std::string chemSymb) const {
    for (SizeType idx = 0; idx < specInfoList.size(); ++idx) {
        if (specInfoList[idx].chemSymb == chemSymb) {
            return idx;
        }
    }

    // should not have arrived here
    assert(false);
    throw std::runtime_error("SpecInfoVec::specinfo_index: "
                                   + chemSymb + "not found.");

    return OFF;
}
