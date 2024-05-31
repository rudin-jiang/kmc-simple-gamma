#pragma once

#include "type.hpp"

SizeMat generate_hexagonal_adjalist_1st(SizeType a, SizeType b);
SizeMat generate_hexagonal_adjalist_2nd(SizeType a, SizeType b);


// 4 nearest neighbour grids
SizeMat generate_square_4n_adjalist_1st(SizeType a, SizeType b);
SizeMat generate_square_4n_adjalist_2nd(SizeType a, SizeType b);

// 8 nearest neighbour grids
SizeMat generate_square_8n_adjalist_1st(SizeType a, SizeType b);
SizeMat generate_square_8n_adjalist_2nd(SizeType a, SizeType b);