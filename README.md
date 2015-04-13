<b><center>ctr9</center></b>
==========

Library for running code on the 3DS's ARM9 processor.

ctr9 has been developed using the VerifyRsaSha256 ARM9 exploit, and compatibility with other firmware versions and exploits is not guaranteed. Everything except the GFX functions should work between versions and exploits, and improved compatibility is planned.

Requires [devkitARM](http://sourceforge.net/projects/devkitpro/files/devkitARM/) to build. Run 'make' to build, and run 'make install' to install it to your devkitPro directory.


Launching
---------

ctr9 binaries can be launched through a variety of methods.
* [VerifyRsaSha256 Loader (4.x)](https://github.com/Steveice10/ctr9/tree/master/loader/VerifyRsaSha256/): The included VerifyRsaSha256 loader can be used to load code from arm9.bin on the SD card.


TODO
----
* Compatibility
    * Remap framebuffers to known addresses.
* FS
    * Make NAND access easier, including decryption.