#include "lattice.hpp"
#include "adjalist.hpp"
#include "specinfo.hpp"
#include "type.hpp"
#include <cassert>

Lattice::Lattice(): rows(0), cols(0), size(0) {}

Lattice::Lattice(SizeType rows, SizeType cols, SpecInfoVec specTab)
: rows(rows), cols(cols), size(rows * cols), specTab(specTab)
{
    SizeType nSpec= specTab.size();

    adjalist_1st = generate_hexagonal_adjalist_1st(rows, cols);
    adjalist_2nd = generate_hexagonal_adjalist_2nd(rows, cols);

    typeOnGrid = SizeVec(size, OFF);

    // emptyGrid
    emptyGrid = SizeSet();
    for (SizeType pos = 0; pos < size; ++pos) {
        emptyGrid.insert(pos);
    }

    // adsorItem
    adsorItem = SizeSetVec(nSpec, SizeSet());
}


bool Lattice::grid_empty(SizeType pos) const {

    // empty
    if (typeOnGrid[pos] == OFF) {
        assert(emptyGrid.find(pos) != emptyGrid.end());
        return true;
    }

    // not empty
    assert(emptyGrid.find(pos) == emptyGrid.end());
    return false;
}


SizeType Lattice::row_idx(SizeType pos) const {return pos / cols; }

SizeType Lattice::col_idx(SizeType pos) const {return pos % cols; }


SizeType Lattice::remove_item(SizeType pos, SizeType itemType) {
    assert(typeOnGrid[pos] == itemType);
    assert(emptyGrid.find(pos) == emptyGrid.end());
    assert(adsorItem[itemType].find(pos) != adsorItem[itemType].end());

    typeOnGrid[pos] = OFF;
    emptyGrid.insert(pos);
    adsorItem[itemType].erase(pos);

    return pos;
}


SizeType Lattice::insert_item(SizeType pos, SizeType itemType) {
    assert(typeOnGrid[pos] == OFF);
    assert(emptyGrid.find(pos) != emptyGrid.end());
    assert(adsorItem[itemType].find(pos) == adsorItem[itemType].end());

    typeOnGrid[pos] = itemType;
    emptyGrid.erase(pos);
    adsorItem[itemType].insert(pos);

    return pos;
}


SizeType Lattice::random_remove_item(SizeType itemType) {
    assert(itemType < specTab.size());
    assert(! adsorItem[itemType].empty());

    SizeType rdPos = adsorItem[itemType].random_element();
    remove_item(rdPos, itemType);
    return rdPos;
}

SizeType Lattice::random_insert_item(SizeType itemType) {
    assert(itemType < specTab.size());
    assert(! emptyGrid.empty());

    SizeType rdPos = emptyGrid.random_element();
    insert_item(rdPos, itemType);
    return rdPos;
}


SizeType Lattice::random_move_one_item(SizeType itemType) {
    assert(itemType < specTab.size());
    assert(! adsorItem[itemType].empty());

    random_remove_item(itemType);
    random_insert_item(itemType);
    return 1;
}

SizeType Lattice::random_move_all_item(SizeType itemType) {
    assert(itemType < specTab.size());
    
    SizeType nItem = adsorItem[itemType].size();
    for (SizeType i = 0; i < nItem; ++i) {
        random_remove_item(itemType);
        random_insert_item(itemType);
    }

    return nItem;
}

SizeVec Lattice::species_count() const {
    SizeType nSpec = specTab.size();
    SizeVec specCnt(nSpec, 0);

    for (SizeType i = 0; i < nSpec; ++i) {
        specCnt[i] = adsorItem[i].size();
    }

    return specCnt;
}