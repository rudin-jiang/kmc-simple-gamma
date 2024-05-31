#pragma once

#include "type.hpp"
#include "specinfo.hpp"
#include <cstddef>

class Lattice {
public:
    SizeType    rows;
    SizeType    cols;
    SizeType    size;

    // specinfo list
    SpecInfoVec specTab;

    SizeMat     adjalist_1st;
    SizeMat     adjalist_2nd;

    SizeVec     typeOnGrid;     // size

    SizeSet     emptyGrid;      // SizeSet
    SizeSetVec  adsorItem;      // nSpec x SizeSet

    Lattice();
    Lattice(SizeType rows, SizeType cols, SpecInfoVec specTab);

    bool grid_empty(SizeType pos) const;

    SizeType row_idx(SizeType pos) const;
    SizeType col_idx(SizeType pos) const;

    SizeType remove_item(SizeType pos, SizeType itemType);
    SizeType insert_item(SizeType pos, SizeType itemType);

    SizeType random_remove_item(SizeType itemType);
    SizeType random_insert_item(SizeType itemType);

    SizeType random_move_one_item(SizeType itemType);
    SizeType random_move_all_item(SizeType itemType);

    SizeVec species_count() const;
};


