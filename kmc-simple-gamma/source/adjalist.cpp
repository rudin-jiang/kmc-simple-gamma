#include "adjalist.hpp"
#include <cassert>
#include <cmath>

SizeMat generate_hexagonal_adjalist_1st(SizeType a, SizeType b) {
    assert(a > 5);
    assert(b > 5);
    assert(a % 2 == 0);

    SizeType nGrid = a * b;
    SizeMat adjaList1st = SizeMat(nGrid, SizeVec(6, 0));
    for (SizeType i = 0; i < nGrid; ++i) {
        int x = int(i) / int(b);
        int y = int(i) % int(b);

        std::vector<int> xVec(6, 0);
        std::vector<int> yVec(6, 0);

        xVec[0] = x - 0;  yVec[0] = y - 1;
        xVec[1] = x - 1;  yVec[1] = y - 0;
        xVec[2] = x - 1;  yVec[2] = y + 1;
        xVec[3] = x + 0;  yVec[3] = y + 1;
        xVec[4] = x + 1;  yVec[4] = y + 0;
        xVec[5] = x + 1;  yVec[5] = y - 1;

        for (SizeType j = 0; j < 6; ++j) {
            xVec[j] += a;  xVec[j] %= a;
            yVec[j] += b;  yVec[j] %= b;
            adjaList1st[i][j] = xVec[j] * b + yVec[j];
        }
    }

    return adjaList1st;
}


// TODO: to be tested
SizeMat generate_hexagonal_adjalist_2nd(SizeType a, SizeType b) {
    assert(a > 5); assert(a < 500);
    assert(b > 5); assert(b < 500);
    assert(a % 2 == 0);

    SizeType nGrid = a * b;
    SizeMat adjaList2nd = SizeMat(nGrid, SizeVec(12, 0));
    for (SizeType i = 0; i < nGrid; ++i) {
        int x = int(i) / int(b);
        int y = int(i) % int(b);
        
        std::vector<int> xVec(12, 0);
        std::vector<int> yVec(12, 0);

        xVec[ 0] = x - 0;  yVec[ 0] = y - 2;
        xVec[ 1] = x - 1;  yVec[ 1] = y - 1;
        xVec[ 2] = x - 2;  yVec[ 2] = y - 0;
        xVec[ 3] = x - 2;  yVec[ 3] = y + 1;
        xVec[ 4] = x - 2;  yVec[ 4] = y + 2;
        xVec[ 5] = x - 1;  yVec[ 5] = y + 2;
        xVec[ 6] = x + 0;  yVec[ 6] = y + 2;
        xVec[ 7] = x + 1;  yVec[ 7] = y + 1;
        xVec[ 8] = x + 2;  yVec[ 8] = y + 0;
        xVec[ 9] = x + 2;  yVec[ 9] = y - 1;
        xVec[10] = x + 2;  yVec[10] = y - 2;
        xVec[11] = x + 1;  yVec[11] = y - 2;

        for (std::size_t j = 0; j < 12; ++j) {
            xVec[j] += a;  xVec[j] %= a;
            yVec[j] += b;  yVec[j] %= b;
            adjaList2nd[i][j] = xVec[j] * b + yVec[j];
        }
    }

    return adjaList2nd;
}