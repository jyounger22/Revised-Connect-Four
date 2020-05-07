/* Jay Younger, jyounger
 * CS 152, Winter 2020
 * Project Phase 2
 */

#include "pos.h"
#include <stdio.h>
#include <stdlib.h>

pos make_pos(unsigned int r, unsigned int c)
{
    struct pos res={r, c};
    return res;
}
