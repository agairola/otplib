// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include <iostream>
#include "key.h"
#include "keygenerator.h"

void genKeyFile();

int main()
{
//    genKeyFile();
//    return 0;

    std::cout << "OTPlib Test v1.0.1\n";
    std::cout << "Enter filename of key file: ";
    std::string filename;
    std::cin >> filename;
    otp::Key keyFile(filename);

    std::cout << "Enter some text:\n";
    std::string text;
    while (getline(std::cin, text))
    {
        //std::cin >> text;

        std::vector<char> data(text.begin(), text.end());

        // Encrypt the user's input
        keyFile.encrypt(data);

        std::cout << "Encrypted: ";
        otp::printBufferInHex(data);
    }

    return 0;
}

void genKeyFile()
{
    std::cout << "Key file generator test\nEnter filename and size: ";
    std::string filename;
    std::size_t amount;
    std::cin >> filename >> amount;
    std::cout << "Generating...";
    otp::KeyGenerator::generateToFile(filename, amount);
    std::cout << " Done.\n";
}
