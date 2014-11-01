// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "key.h"
#include <fstream>
#include <iostream>
#include "keygenerator.h"

namespace otp
{

Key::Key(const std::string& filename):
    filename(filename),
    freeBlocks(filename)
{
    left = 0;
    right = freeBlocks.getSize() - 1;
}

Status Key::encrypt(std::vector<char>& data)
{
    Status status;

    // Don't try to encrypt 0 bytes
    if (!data.empty())
    {
        // Allocate a range of bytes to use
        status = freeBlocks.allocate(data.size(), left, right);

        // Encrypt the data if there was enough space
        if (status.status)
            status.status = crypt(data, status.position);
    }

    return status;
}

bool Key::decrypt(std::vector<char>& data, Position pos)
{
    bool status = false;

    // Don't try to decrypt 0 bytes, and make sure the part of the key hasn't been used
    if (!data.empty() && freeBlocks.isFree(pos, data.size()))
    {
        // Decrypt the data
        status = crypt(data, pos);

        // Save the used range of data
        if (status)
            freeBlocks.markAsUsed(pos, data.size());
    }

    return status;
}

Key::operator bool() const
{
    return (freeBlocks.getSize() > 0);
}

Position Key::bytesTotal() const
{
    return freeBlocks.getSize();
}

Position Key::bytesFree() const
{
    return freeBlocks.getFreeSpace();
}

Position Key::bytesUsed() const
{
    return (bytesTotal() - bytesFree());
}

Position Key::bytesTotalRange() const
{
    return (right - left + 1);
}

Position Key::bytesFreeRange() const
{
    return freeBlocks.getFreeSpace(left, right);
}

Position Key::bytesUsedRange() const
{
    return (bytesTotalRange() - bytesFreeRange());
}

void Key::setKeyRange(Position minimum, Position maximum)
{
    left = minimum;
    right = maximum;
}

bool Key::crypt(std::vector<char>& data, Position pos) const
{
    bool status = false;

    // Open the key file for reading and writing
    std::fstream keyFile(filename.c_str(), std::ios::binary | std::ios::in | std::ios::out);
    if (keyFile)
    {
        // Read/write at the specified position
        keyFile.seekg(pos);
        keyFile.seekp(pos);

        // Read the needed portion of the key file
        std::vector<char> keyData(data.size());
        keyFile.read(keyData.data(), keyData.size());

        if (keyFile)
        {
            // Encrypt/decrypt the data using the key data
            unsigned i = 0;
            for (char& c: data)
                c ^= keyData[i++];

            // Generate new random data
            KeyGenerator::generate(keyData, keyData.size());

            // Overwrite the old key data with the new random data
            keyFile.write(keyData.data(), keyData.size());

            // Try to immediately write the data
            keyFile.flush();
            keyFile.close();

            status = true;
        }
    }

    return status;
}

}
