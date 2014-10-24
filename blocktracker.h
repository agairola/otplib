#ifndef BLOCKTRACKER_H
#define BLOCKTRACKER_H

#include <cstddef>
#include <string>
#include <set>

namespace otp
{

// File position, which should allow for larger sizes than 32-bit ints
using Position = std::size_t;

/*
This class keeps track of used blocks of a key file.
It uses an index file to store this information.

Notes:
    Build up some kind of tree/map/set/table to lookup
        and maintain everything.
    Have a simple way to handle edge cases and errors, such as when the key
        data is depleted, or there is not enough space to encrypt the data.

Other possibility:
    If this uses a free list instead of a list of the used blocks,
        it could allocate things in O(1), since it would just look directly
        at the end of the free list or something.

Easiest method:
    Used fixed block sizes, so there could be a single list of used blocks.
    The problem with this is that some of the key file will be wasted...
*/
class BlockTracker
{
    public:
        BlockTracker(const std::string& keyFilename);

        // Marks a range of blocks as used
        void markAsUsed(Position pos, unsigned size);

        // Finds a free position in the entire range to fit the size of the data
        Position getFreePosition(unsigned size) const;

        // Same as above, but limits the search to a specific range
        Position getFreePosition(unsigned size, Position left, Position right) const;

        // Returns the size of the total file
        Position getSize() const;
        Position getSize(Position left, Position right) const;

    private:

        // Optimizes the ranges of blocks
        void optimize();

        std::string keyFilename; // Filename of key file (key)
        std::string indexFilename; // Filename of index file (key.index)
        std::set<Position> usedBlocks; // Keeps track of all used blocks
};

}

#endif
