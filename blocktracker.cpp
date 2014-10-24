#include "blocktracker.h"

namespace otp
{

BlockTracker::BlockTracker(const std::string& keyFilename):
    keyFilename(keyFilename)
{
    // This will be the file to keep track of the used blocks
    indexFilename = keyFilename + ".index";
}

void BlockTracker::markAsUsed(Position pos, unsigned size)
{

}

Position BlockTracker::getFreePosition(unsigned size) const
{

}

Position BlockTracker::getFreePosition(unsigned size, Position left, Position right) const
{

}

Position BlockTracker::getSize() const
{

}

Position BlockTracker::getSize(Position left, Position right) const
{

}

void BlockTracker::optimize()
{

}


}
