#include "kmcsys.hpp"
#include "event-simple.hpp"
#include "input.hpp"
#include "nhfstr.hpp"
#include "random.hpp"
#include "type.hpp"
#include "utility.hpp"
#include "scan-simple.hpp"

#include <cassert>
#include <vector>
#include <string>
#include <vector>
#include <cstddef>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <numeric>


KmcSys::KmcSys():
    token(generate_token(0)),
    specsCntOfs("specs_cnt_" + token + ".txt"),
    reactCntOfs("react_cnt_" + token + ".txt")
{
    assert(specsCntOfs.good());
    assert(reactCntOfs.good());

    // input
    input = Input("input.txt");

    // specinfoList and reactionList
    specinfoList = SpecInfoVec("specinfolist.txt");
    reactionList = ReactionVec("reactionlist.txt", specinfoList);

    SizeType nSpec = specinfoList.size();
    SizeType nReac = reactionList.size();

    lattice = Lattice(input.rows, input.cols, specinfoList);

    kmcStep = 0;
    kmcTime = 0;

    rCntList = SizeVec(nReac, 0);

    eventList = EventMat(nReac);
    rItemList = RealVec(nReac);
    constList = RealVec(nReac);
    rRateList = RealVec(nReac);

    // generate rate constant
    for (std::size_t i = 0; i < reactionList.size(); ++i) {
        constList[i] = reactionList[i].rate_constant(input.temp);
    }

    for (std::size_t i = 0; i < specinfoList.size(); ++i) {
        double percent = specinfoList[i].iniCover / 100.0;
        std::size_t nAdd = percent * input.rows * input.cols;
        for (std::size_t j = 0; j < nAdd; ++j) {
            lattice.random_insert_item(i);
        }
    }
}

KmcSys::~KmcSys() {
    specsCntOfs.close();
    reactCntOfs.close();
}

void KmcSys::kmc_run() {
    while (true) {

        if (kmcStep > input.maxStep) {
            std::printf("KmcSys: reach max kmc step ...\n");
            break;
        }

        update_eventList_reactItem();   // eventList  reactItem
        calculate_reactionRateList();   // rRateList

        const RealVec &v = rRateList;
        double rateSum = std::accumulate(v.begin(), v.end(), double(0));
        if (rateSum < 1.0e-20) {
            std::printf("KmcSys: no reaction can happen anymore ...\n");
            break;
        }

        SizeType reactHappen = OFF;
        if (input.selectr == 0) reactHappen = reg_event_selector(rRateList);
        if (input.selectr == 1) reactHappen = log_event_selector(rRateList);
        if (input.selectr == 2) reactHappen = sqrt2_event_selector(rRateList);

        assert(reactHappen != OFF);
        if (reactHappen == OFF) {
            throw std::runtime_error("Shouldn't be here...\n");
        }

        // carry out event
        take_reaction(reactHappen);
        ++ rCntList[reactHappen];

        // diffusion
        if (kmcStep % input.diffStep == 0) {
            take_diffusion_type_1();
        }

        // print on screen
        if (kmcStep % input.printStep == 0) {
            std::printf("step = %zu / %zu\n", kmcStep, input.maxStep);
        }

        // print to file
        if (kmcStep % input.printStep == 0) print_kmc_information();

        kmcStep++;
    }
}

void KmcSys::update_eventList_reactItem() {
    for (SizeType i = 0; i < reactionList.size(); ++i) {
        const Reaction &reaction = reactionList[i];
        eventList[i].clear();
        rItemList[i] = scan_eventList_reactItem(eventList[i], lattice, reaction, specinfoList);
    }
}

void KmcSys::calculate_reactionRateList() {
    for (SizeType i = 0; i < rRateList.size(); ++i) {
        rRateList[i] = constList[i] * rItemList[i];
    }
}

void KmcSys::take_reaction(SizeType iReact) {
    if (reactionList[iReact].reactCase == 0) {
        SizeType adsType = reactionList[iReact].reacTypes[0];
        lattice.random_insert_item(adsType);
        return ;
    }
    
    if (reactionList[iReact].reactCase == 1) {
        SizeType desType = reactionList[iReact].reacTypes[0];
        lattice.random_remove_item(desType);
        return ;
    }

    assert(reactionList[iReact].reactCase == 2);

    const Reaction &reaction = reactionList[iReact];

    // about "H + H -> H2"
    if (reaction.reac_num() == 2) {
        SizeType rType0 = reaction.reacTypes[0];
        SizeType rType1 = reaction.reacTypes[1];
        SizeType pType0 = reaction.prodTypes[0];

        if (rType0 == rType1 && specinfoList[rType0].chemSymb == "H") {
            lattice.random_remove_item(rType0); // remove H
            lattice.random_remove_item(rType0); // remove H
            lattice.random_insert_item(pType0); // add H2
            return;
        }
    }

    SizeType rnd = uniform_random_sizet();
    SizeType rev = rnd % eventList[iReact].size();
    const Event &e = eventList[iReact][rev];

    assert(e.num_remove == reaction.reac_num());
    assert(e.num_insert == reaction.prod_num());

    // remove reactants
    for (SizeType i = 0; i < e.num_remove; ++i) {
        SizeType iGrid = e.removeGrid.at(i);
        SizeType iType = e.removeType.at(i);

        assert(iType < specinfoList.size());
        assert(iType == reaction.reacTypes.at(i));
        lattice.remove_item(iGrid, iType);
    }

    // insert products
    for (SizeType i = 0; i < e.num_insert; ++i) {
        SizeType iGrid = e.insertGrid.at(i);
        SizeType iType = e.insertType.at(i);
        assert(iType < specinfoList.size());

        lattice.insert_item(iGrid, iType);
    }

    // keep coverage fix
    for (SizeType i = 0; i < e.num_remove; ++i) {
        SizeType iType = e.removeType.at(i);
        if (specinfoList[iType].fixCover == 1) {
            lattice.random_insert_item(iType);
        }
    }

    for (SizeType i = 0; i < e.num_insert; ++i) {
        SizeType iType = e.insertType.at(i);
        if (specinfoList[iType].fixCover == 1) {
            lattice.random_remove_item(iType);
        }
    }
}

// move to random position
void KmcSys::take_diffusion_type_1() {
    double movePercent = input.diffPerc / 10000.0;
    for (SizeType specType = 0; specType < specinfoList.size(); ++specType) {
        SizeType numItem = lattice.adsorItem[specType].size();
        SizeType numMove = numItem * movePercent;
        if (numItem != 0 && numMove == 0) numMove = 1;

        for (SizeType iMove = 0; iMove < numMove; ++iMove) {
            lattice .random_move_one_item(specType);
        }
    }
}

template <typename T>
static void print_vector(std::ostream &os, const std::vector<T> &v) {
    for (const T &t : v) os << t << " ";
    os << std::endl;
}

void KmcSys::print_kmc_information() {
    // specs_cnt
    print_vector(specsCntOfs, lattice.species_count());

    // react_cnt
    print_vector(reactCntOfs, rCntList);
}