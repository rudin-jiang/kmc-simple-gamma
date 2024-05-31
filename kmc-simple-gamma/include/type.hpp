#pragma once

#include <cstddef>
#include <vector>
#include <array>
#include "rdset.hpp"

#define OFF (std::size_t(-1))

// SizeType
using SizeType   = std::size_t;
using SizeArr2   = std::array<SizeType, 2>;
using SizeArr3   = std::array<SizeType, 3>;
using SizeVec    = std::vector<SizeType>;
using SizeMat    = std::vector<SizeVec>;
using SizeTen    = std::vector<SizeMat>;
using SizeSet    = RdSet<SizeType>;
using SizeSetVec = std::vector<SizeSet>;

// double
using RealVec = std::vector<double>;
using RealMat = std::vector<RealVec>;

