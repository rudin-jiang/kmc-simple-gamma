#pragma once

#include "type.hpp"
#include "input.hpp"
#include "specinfo.hpp"
#include "reaction.hpp"
#include "lattice.hpp"
#include "event-simple.hpp"
#include <iostream>
#include <fstream>
#include <string>

class KmcSys {
public:
    // random token
    std::string token;

    // some print file stream
    std::ofstream   specsCntOfs;
    std::ofstream   reactCntOfs;

    // input
    Input input;

    // reaction information
    SpecInfoVec specinfoList;
    ReactionVec reactionList;

    // core: lattice
    Lattice     lattice;

    // step and time
    SizeType    kmcStep;
    double      kmcTime;

    // reaction happened count
    SizeVec     rCntList;

    // kmc core utility
    EventMat    eventList;
    RealVec     rItemList;  // num of reactants
    RealVec     constList;  // reaction rate constant
    RealVec     rRateList;  // reactionRateList

    KmcSys();
    ~KmcSys();

    void kmc_run();
    void update_eventList_reactItem();  // eventList, reactItem
    void calculate_reactionRateList();  // rateList
    void take_reaction(SizeType reac);
    void take_diffusion_type_1();       // move to random position
    void take_diffusion_type_2();       // move to close position
    void print_kmc_information();
};