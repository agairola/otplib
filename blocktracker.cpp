// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "blocktracker.h"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace otp
{

BlockTracker::BlockTracker(const std::string& keyFilename)
{
    // Calculate the total key file size
    fileSize = getFileSize(keyFilename);

    // Load the index file
    indexFilename = keyFilename + ".index";
    loadIndexFile(indexFilename);
}

BlockTracker::~BlockTracker()
{
    saveIndexFile(indexFilename);
}

Position BlockTracker::allocate(Position size, Position left, Position right)
{
    // By default (only passing in the size), the entire range of bytes will be used
    if (right == 0 || left > right)
        right = fileSize;

    // TODO: Handle special cases, like when size > right - left

    // Find an available position for this size
    Position available = 0;
    for (unsigned i = 0; i < freeBytes.size(); ++i)
    {
        // Split case example
        // Free ranges: 0-1000
        // Left: 200, Right: 700
        // Size: 100
        // New ranges: 0-199, 300-1000

        // If we found a range that will fit the data
        if (size < freeBytes[i].getSize(left, right))
        {
            // Check if we need to split the ranges
            if (left > freeBytes[i].minimum)
            {
                std::cout << "Left > min (" << left << " > " << freeBytes[i].minimum << ") inserted range at " << i << ".\n";
                // Create a new range, and split at the left
                freeBytes.insert(freeBytes.begin() + i, freeBytes[i]);
                freeBytes[i].maximum = left - 1;
                freeBytes[i + 1].minimum = left + size;
            }
            else
            {
                // Save the available position
                available = freeBytes[i].minimum;

                // Resize the range (marking the space as used)
                freeBytes[i].minimum += size;
            }
            return available;
        }
    }

    return available;
}

void BlockTracker::markAsUsed(Position pos, Position size)
{
    // Find the range to subtract this from
    for (unsigned i = 0; i < freeBytes.size(); ++i)
    {
        // If this position is within the range
        if (freeBytes[i] == pos)
        {
            if (freeBytes[i].minimum == pos)
            {
                // Simply change the range (100-200 becomes 160-200)
                freeBytes[i].minimum += std::min(size, freeBytes[i].getSize());
            }
            else
            {
                std::cout << "Pos != min (" << pos << " != " << freeBytes[i].minimum << ") inserted range at " << i << ".\n";
                // Create a new range (100-200 becomes 100-120, 180-200)
                freeBytes.insert(freeBytes.begin() + i, freeBytes[i]);
                freeBytes[i].maximum = pos - 1;
                freeBytes[i + 1].minimum = pos + size;
            }
            break;
        }
    }
}

Position BlockTracker::getSize() const
{
    return fileSize;
}

Position BlockTracker::getFreeSpace() const
{
    // Add up all of the ranges of free bytes to get the total free space
    Position freeSpace = 0;
    for (const auto& range: freeBytes)
        freeSpace += range.getSize();
    return freeSpace;
}

Position BlockTracker::getFreeSpace(Position left, Position right) const
{
    // TODO: Implement this
    return (right - left);
}

bool BlockTracker::loadIndexFile(const std::string& filename)
{
    // Open the index file
    std::ifstream indexFile(filename);
    if (indexFile)
    {
        // Read each line
        std::string line;
        while (getline(indexFile, line))
        {
            // Split the line into a minimum and maximum value
            std::istringstream stream(line);
            Position minimum, maximum;
            if (stream >> minimum >> maximum)
                freeBytes.emplace_back(minimum, maximum); // Add the block range
        }
    }
    else
    {
        // Initialize the byte ranges
        freeBytes.clear();
        freeBytes.emplace_back(0, fileSize - 1);
    }

    // TODO: Determine if file exists but could not be opened
    return true;
}

bool BlockTracker::saveIndexFile(const std::string& filename)
{
    bool status = false;

    // Open the index file
    std::ofstream indexFile(filename);
    if (indexFile)
    {
        // Write the byte ranges to the file
        for (const auto& range: freeBytes)
            indexFile << range.toString() + '\n';
        status = true;
    }

    return status;
}

std::ifstream::pos_type getFileSize(const std::string& filename)
{
    std::ifstream file(filename.c_str(), std::ios::binary | std::ios::ate);
    return file.tellg();
}

}
