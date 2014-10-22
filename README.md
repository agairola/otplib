OTPlib
======

OTPlib is a practical implementation of one-time-pad (OTP) encryption.

OTP Encryption
--------------

This is one of the simplest encryption algorithms, just being an XOR of some random key data with the plain text, to create cipher text. Despite its simplicity, it is mathematically known to be impossible to crack, unless the original key data is obtained.

Practical Use
-------------

OTP has been known to be difficult to use in real-world situations. This library should fix that problem, making it as easy as copying randomly generated key files to be able to open an encrypted channel between clients. Note: This library may have networking support in the near future, if it keeps it mostly generic. Maybe just as an optional module to include.

How The Algorithm Works
-----------------------

1. A portion of the key file is read, which is used to XOR encrypt data of the same size.
2. That portion of the key file is securely erased with random data.
3. The information about the used areas are saved to an index file.

Note: Decrypting is the exact same process, in the same order.

Example Applications
--------------------

Here are some possible examples of software that could use OTPlib:

* Secure communication software
  * Instant messaging
  * Voice & video
* Secure file transfers
* VPN
* Secure Wi-Fi
  * Could sync up a USB drive to connect to the network
  * All data sent/received would be encrypted

Limitations
-----------

How this library should NOT be used:

* Encrypted file storage/archiving
* Keeping key files for long periods of time
* Transferring more data than the size of the key file
  * After the key file is depleted, you must re-sync with new key data.

Code
----

The source code is written in C++ (with C++11 features), and is licensed under the GPLv3. I chose this license because it is difficult to trust "secure" proprietary software, and I don't want to allow developing proprietary applications that use this library.

Author
------

Eric Hebert
