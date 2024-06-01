#include "scan-simple.hpp"

SizeType scan_eventList_reactItem(
    EventVec &events, 
    SizeType &nEvent,
    const Lattice &lattice, 
    const Reaction &reaction, 
    const SpecInfoVec &specinfoList
) {
    // Adsorption reaction
    if (reaction.reactCase == 0) {
        assert(reaction.reac_num() == 1);
        SizeType adsType = reaction.reacTypes[0];
        assert(adsType < specinfoList.size());
        nEvent = lattice.emptyGrid.size();
        return nEvent;
    }
    
    // Desorption reaction
    if (reaction.reactCase == 1) {
        assert(reaction.reac_num() == 1);
        SizeType desType = reaction.reacTypes[0];
        assert(desType < specinfoList.size());
        nEvent = lattice.adsorItem[desType].size();
        return nEvent;
    }

    assert(reaction.reactCase == 2);

    SizeType nReac = reaction.reac_num();
    SizeType nProd = reaction.prod_num();

    if (nReac == 1 && nProd == 1) return scan_eventList_reactItem_1_1_(events, nEvent, lattice, reaction, specinfoList);  
    if (nReac == 1 && nProd == 2) return scan_eventList_reactItem_1_2_(events, nEvent, lattice, reaction, specinfoList);
    if (nReac == 2 && nProd == 1) return scan_eventList_reactItem_2_1_(events, nEvent, lattice, reaction, specinfoList);
    if (nReac == 2 && nProd == 2) return scan_eventList_reactItem_2_2_(events, nEvent, lattice, reaction, specinfoList);
  
    assert(false);
    throw std::runtime_error("Shouldn't be here.");
    return 0;
}


SizeType scan_eventList_reactItem_1_1_(
    EventVec &events, 
    SizeType &nEvent,
    const Lattice &lattice, 
    const Reaction &reaction, 
    const SpecInfoVec &specinfoList
) {
    assert(nEvent == 0);
    assert(reaction.reac_num() == 1);
    assert(reaction.prod_num() == 1);

    SizeType rType0 = reaction.reacTypes[0];
    SizeType pType0 = reaction.prodTypes[0];

    for (const SizeType rPos0 : lattice.adsorItem[rType0]) {
        events[nEvent++] = {
            1, 1, 
            { rPos0, OFF},      // removeGrid
            {rType0, OFF},      // removeType
            { rPos0, OFF},      // insertGrid
            {pType0, OFF}       // insertType
        };
    }

    return nEvent;
}



SizeType scan_eventList_reactItem_1_2_(
    EventVec &events, 
    SizeType &nEvent,
    const Lattice &lattice, 
    const Reaction &reaction, 
    const SpecInfoVec &specinfoList
) {
    assert(nEvent == 0);
    assert(reaction.reac_num() == 1);
    assert(reaction.prod_num() == 2);

    SizeType rType0 = reaction.reacTypes[0];

    SizeType pType0 = reaction.prodTypes[0];
    SizeType pType1 = reaction.prodTypes[1];

    for (const SizeType rPos0 : lattice.adsorItem[rType0]) {
        const SizeVec &adja0 = lattice.adjalist_1st[rPos0];

        for (const SizeType rPos1 : adja0) {
            if (lattice.grid_empty(rPos1)) {
                events[nEvent++] = {
                    1, 2, 
                    { rPos0,    OFF},   // removeGrid
                    {rType0,    OFF},   // removeType
                    { rPos0,  rPos1},   // insertGrid
                    {pType0, pType1}    // insertType
                };
            }
        } 
    }

    return nEvent;
}


SizeType scan_eventList_reactItem_2_1_(
    EventVec &events, 
    SizeType &nEvent,
    const Lattice &lattice, 
    const Reaction &reaction, 
    const SpecInfoVec &specinfoList
) {
    assert(nEvent == 0);
    assert(reaction.reac_num() == 2);
    assert(reaction.prod_num() == 1);

    SizeType rType0 = reaction.reacTypes[0];
    SizeType rType1 = reaction.reacTypes[1];
    SizeType pType0 = reaction.prodTypes[0];

    SizeType rNum0 = lattice.adsorItem[rType0].size();
    SizeType rNum1 = lattice.adsorItem[rType1].size();

    if (rNum0 == 0 || rNum1 == 0) return 0;

    // about "H + H -> H2"
    if (rType0 == rType1 && specinfoList[rType0].chemSymb == "H"
        && lattice.adsorItem[rType0].size() > 0.9 * lattice.size) {
        
        nEvent = lattice.adjalist_1st.front().size()
                    * lattice.size / 2;
        return nEvent;
    }

    // about "CO + H -> CHO"
    if (specinfoList[rType0].chemSymb == "CO" && specinfoList[rType1].chemSymb == "H") {
        nEvent = rNum0 * lattice.adjalist_1st.front().size();
        return nEvent;
    }


    if (rNum0 <= rNum1) {
        for (const SizeType rPos0 : lattice.adsorItem[rType0]) {
            const SizeVec &adja0 = lattice.adjalist_1st[rPos0];
            for (const SizeType rPos1 : adja0) {
                if (lattice.typeOnGrid[rPos1] == rType1) {
                    events[nEvent++] = {
                        2, 1, 
                        { rPos0,  rPos1},   // removeGrid
                        {rType0, rType1},   // removeType
                        { rPos0,    OFF},   // insertGrid
                        {pType0,    OFF}    // insertType
                    };
                }
            }
        }
    }
    else {  // rNum0 > rNum1
        for (const SizeType rPos1 : lattice.adsorItem[rType1]) {
            const SizeVec &adja1 = lattice.adjalist_1st[rPos1];
            for (const SizeType rPos0 : adja1) {
                if (lattice.typeOnGrid[rPos0] == rType0) {
                    events[nEvent++] = {
                        2, 1, 
                        { rPos0,  rPos1},   // removeGrid
                        {rType0, rType1},   // removeType
                        { rPos0,    OFF},   // insertGrid
                        {pType0,    OFF}    // insertType
                    };
                }
            }
        }
    }

    return nEvent;
}


SizeType scan_eventList_reactItem_2_2_(
    EventVec &events, 
    SizeType &nEvent,
    const Lattice &lattice, 
    const Reaction &reaction, 
    const SpecInfoVec &specinfoList
) {
    assert(nEvent == 0);
    assert(reaction.reac_num() == 2);
    assert(reaction.prod_num() == 2);

    SizeType rType0 = reaction.reacTypes[0];
    SizeType rType1 = reaction.reacTypes[1];
    SizeType pType0 = reaction.prodTypes[0];
    SizeType pType1 = reaction.prodTypes[1];

    SizeType rNum0 = lattice.adsorItem[rType0].size();
    SizeType rNum1 = lattice.adsorItem[rType1].size();

    if (rNum0 <= rNum1) {
        for (const SizeType rPos0 : lattice.adsorItem[rType0]) {
            const SizeVec &adja0 = lattice.adjalist_1st[rPos0];
            for (const SizeType rPos1 : adja0) {
                if (lattice.typeOnGrid[rPos1] == rType1) {
                    events[nEvent++] = {
                        2, 2, 
                        { rPos0,  rPos1},   // removeGrid
                        {rType0, rType1},   // removeType
                        { rPos0,  rPos1},   // insertGrid
                        {pType0, pType1}    // insertType
                    };
                }
            }
        }
    }
    else {  // rNum0 > rNum1
        for (const SizeType rPos1 : lattice.adsorItem[rType1]) {
            const SizeVec &adja1 = lattice.adjalist_1st[rPos1];
            for (const SizeType rPos0 : adja1) {
                if (lattice.typeOnGrid[rPos0] == rType0) {
                    events[nEvent++] = {
                        2, 2, 
                        { rPos0,  rPos1},   // removeGrid
                        {rType0, rType1},   // removeType
                        { rPos0,  rPos1},   // insertGrid
                        {pType0, pType1}    // insertType
                    };
                }
            }
        }
    }

    return nEvent;
}