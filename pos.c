#include "pos.h"
#include <stdio.h>
#include <stdlib.h>

pos make_pos(unsigned int r, unsigned int c)
{
    struct pos res={r, c};
    return res;
}
