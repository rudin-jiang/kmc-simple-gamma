#pragma once

#include "type.hpp"
#include <vector>

class Event {
public:
    SizeType    num_remove;
    SizeType    num_insert;

    SizeArr2    removeGrid;
    SizeArr2    removeType;
    SizeArr2    insertGrid;
    SizeArr2    insertType;

    Event() {}

    Event(
        SizeType num_remove,
        SizeType num_insert,
        SizeArr2 removeGrid,
        SizeArr2 removeType,
        SizeArr2 insertGrid,
        SizeArr2 insertType
    ):  num_remove(num_remove),
        num_insert(num_insert),
        removeGrid(removeGrid),
        removeType(removeType),
        insertGrid(insertGrid),
        insertType(insertType) {}
};

using EventVec = std::vector<Event>;
using EventMat = std::vector<EventVec>;

