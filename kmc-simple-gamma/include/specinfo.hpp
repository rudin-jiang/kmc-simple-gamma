#pragma once

#include "type.hpp"
#include <cstddef>
#include <vector>
#include <string>

class SpecInfo {
public:
    std::string     chemSymb;
    SizeType        siteType;   // occupied which kind of site
    SizeType        fixCover;   // whether fix coverage
    double          iniCover;   // init coverage

    SpecInfo();
    SpecInfo(std::string line);

    std::string to_string() const;
};


class SpecInfoVec {
public:
    std::vector<SpecInfo> specInfoList;

    SpecInfoVec();
    SpecInfoVec(std::string filename);

    SizeType size() const;
    const SpecInfo& operator[](SizeType idx) const;

    SizeType specinfo_index(std::string chemSymb) const;
};