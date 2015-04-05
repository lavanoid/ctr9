<b><center><h1>ctr9</h></center></b>
==========

Library for running code on the 3DS's ARM9 processor.

ctr9 has been developed using the VerifyRsaSha256 ARM9 exploit, and compatibility with other firmware versions and exploits is not guaranteed. Everything except the FS functions, and possibly the GFX functions depending on whether framebuffer addresses differ, should work between versions/exploits.

Requires [devkitARM](http://sourceforge.net/projects/devkitpro/files/devkitARM/) to build. Run 'make' to build, and run 'make install' to install it to your devkitPro directory.

TODO
----
* Compatibility
    * Use EMMC registers instead of Process9 FS functions.
    * Remap framebuffers to known addresses.
    * Find a better base-address to load code to.