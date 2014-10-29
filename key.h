// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef KEY_H
#define KEY_H

#include <string>
#include <vector>
#include "blocktracker.h"

namespace otp
{

/*
File layout/design

key
    Random data
    Used blocks are securely erased with random data
    This key file could be safely reused, by deleting the .index file and re-syncing.
        However, it increases the chances of someone obtaining your common unused key data.

key.index
    List of all free blocks (ranges of bytes)
        Using ranges of bytes instead of fixed block sizes to prevent wasting valuable key data.
    2345-2371
    100-150
    0-20
    9999-10000
    Ranges could be merged once loaded into memory and when the index file is rewritten.
        0-50
        51-100
        101-200
    Would turn into:
        0-200

*/

/*
This class reads from a key file of (ideally) random data.
Everytime something is encrypted with a portion of the key file, that part
    gets securely erased, and then removed from the free ranges of bytes.
*/
class Key
{
    public:
        Key(const std::string& filename);

        // Encryption ---------------------------------------------------------

        // Encrypts (or decrypts) a buffer of data in place
        // The position used is up to the algorithm, but will be within range
        // Returns the used position, so that something else knows how to decrypt it
        // Note: This is commonly used for encryption
        Status encrypt(std::vector<char>& data);

        // Same as above, but the position specified will be used to encrypt/decrypt the data
        // Note: This is commonly used for decryption
        Status encrypt(std::vector<char>& data, Position pos, bool needToAllocate = true);


        // Status -------------------------------------------------------------

        // Returns true if the key file was opened successfully (based on its size)
        explicit operator bool() const;

        // Returns the size of the entire key file
        Position bytesTotal() const;

        // Returns how much more of the key file can be used
        Position bytesFree() const;

        // Returns how much of the key file was already used
        Position bytesUsed() const;

        // Returns the size of the range
        Position bytesTotalRange() const;

        // Returns the available bytes in the current range
        Position bytesFreeRange() const;

        // Returns the used bytes in the current range
        Position bytesUsedRange() const;


        // Positions/blocks ---------------------------------------------------

        // Set the range of key data to use (default is entire file)
        // This is necessary so that multiple things don't use the same key data
        // (maximum == 0 || minimum >= maximum) means the maximum will be at
        //     the end of the file.
        void setKeyRange(Position minimum = 0, Position maximum = 0);

    private:
        std::string filename; // Name of the key file
        BlockTracker usedBlocks; // List of already used blocks that cannot be reused

        // Range of bytes to use
        Position left;
        Position right;
};

}

#endif
