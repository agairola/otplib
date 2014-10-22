#ifndef BLOCKTRACKER_H
#define BLOCKTRACKER_H

namespace otp
{

/*
This class keeps track of USED blocks of a key file.
It uses an index file to store this information.

Notes:
    Build up some kind of tree/map/set/table to lookup
        and maintain everything.
*/
class BlockTracker
{
    public:
        BlockTracker();

    private:
};

}

#endif
