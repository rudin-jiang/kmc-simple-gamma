#include "input.hpp"
#include "nhfstr.hpp"
#include <fstream>
#include <sstream>
#include <cassert>
#include <exception>

Input::Input():
temp(0), rows(0), cols(0), maxStep(0), selectr(0),
diffType(0), diffStep(0), diffPerc(0), printStep(0) {}

Input::Input(std::string filename) {
    // input file
    std::ifstream inpFile(filename);
    assert(inpFile.good());
    if (! inpFile.good()) {
        throw std::runtime_error("KmcSys cfgFile not good!");
    }

    std::string inputInfo, line;
    while (std::getline(inpFile, line)) {
        remove_comment(line);
        remove_dual_space(line);
        inputInfo += " " + line;
    }

    std::istringstream input(inputInfo);

    input >> temp;
    input >> rows;
    input >> cols;

    input >> maxStep;
    input >> selectr;
    
    input >> diffType;
    input >> diffStep;
    input >> diffPerc;

    input >> printStep;
}
