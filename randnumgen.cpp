// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "randnumgen.h"

namespace otp
{

RandNumGen::RandNumGen(int minimum, int maximum):
    gen(rd()),
    dist(minimum, maximum)
{
}

int RandNumGen::generate()
{
    return dist(gen);
}

}
