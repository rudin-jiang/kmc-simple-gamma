#pragma once

#include "type.hpp"
#include "specinfo.hpp"
#include "reaction.hpp"
#include "lattice.hpp"
#include "event-simple.hpp"

SizeType scan_eventList_reactItem(
    EventVec &events, 
    SizeType &nEvent,
    const Lattice &lattice, 
    const Reaction &reaction, 
    const SpecInfoVec &specinfoList
);

SizeType scan_eventList_reactItem_1_1_(
    EventVec &events, 
    SizeType &nEvent,
    const Lattice &lattice, 
    const Reaction &reaction, 
    const SpecInfoVec &specinfoList
);

SizeType scan_eventList_reactItem_1_2_(
    EventVec &events, 
    SizeType &nEvent,
    const Lattice &lattice, 
    const Reaction &reaction, 
    const SpecInfoVec &specinfoList
);

SizeType scan_eventList_reactItem_2_1_(
    EventVec &events, 
    SizeType &nEvent,
    const Lattice &lattice, 
    const Reaction &reaction, 
    const SpecInfoVec &specinfoList
);

SizeType scan_eventList_reactItem_2_2_(
    EventVec &events, 
    SizeType &nEvent,
    const Lattice &lattice, 
    const Reaction &reaction, 
    const SpecInfoVec &specinfoList
);
