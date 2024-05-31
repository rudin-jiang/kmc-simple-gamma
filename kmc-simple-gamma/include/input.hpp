#pragma once 

#include "type.hpp"
#include <string>

class Input {
public:
    double      temp;
    SizeType    rows;
    SizeType    cols;

    SizeType    maxStep;
    SizeType    selectr;    // 0: reg   1: log

    // diffusion control
    SizeType    diffType;   // 0: off  1: skip  2: close move
    SizeType    diffStep;
    SizeType    diffPerc;

    // some print control
    SizeType    printStep;

    Input();
    Input(std::string filename);
};