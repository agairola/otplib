// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef BLOCKTRACKER_H
#define BLOCKTRACKER_H

#include <cstddef>
#include <string>
#include <deque>
#include <fstream>
#include "comparablerange.h"

namespace otp
{

// File position, which should allow for larger sizes than 32-bit ints
using Position = std::size_t;

/*
TODO:
    Switch to a free list, it will be much more efficient.
    There will be minimally more fragmentation, but allocating will be
        simpler and faster, since what we need is right there.

This class keeps track of free byte ranges of a key file.
It uses an index file to store this information.
    There is a list of ranges of free bytes. Example:
    Start End
    100 200
    500 550
    600 900
*/
class BlockTracker
{
    public:
        BlockTracker(const std::string& keyFilename);
        ~BlockTracker();

        // Allocates space for a certain number of bytes within a possible range
        // Returns a usable position, or -1 if unable to allocate
        Position allocate(Position size, Position left = 0, Position right = 0);

        // Marks a range of blocks as used
        void markAsUsed(Position pos, Position size);

        // Returns the size of the total file
        Position getSize() const;

        // Returns the total free space
        Position getFreeSpace() const;

        // Returns the free space in the specified range
        Position getFreeSpace(Position left, Position right) const;

    private:

        // Reads an index file into memory
        bool loadIndexFile(const std::string& filename);

        // Writes an index file from memory
        bool saveIndexFile(const std::string& filename);

        using RangeType = ComparableRange<Position>;

        std::string indexFilename; // Filename of index file (key.index)
        std::deque<RangeType> freeBytes; // Ranges of free bytes
        Position fileSize; // Total size of the key file
};

// Returns size of a file
std::ifstream::pos_type getFileSize(const std::string& filename);

}

#endif
