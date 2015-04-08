<b><center><h1>VerifyRsaSha256 Loader</h></center></b>
==========

Loads "arm9.bin" into the appropriate address for ctr9 binaries and jumps to it, using the 4.x VerifyRsaSha256 exploit to gain code execution.

Building produces three files:
* payload.bin: Execute from any implementation of the exploit.
* MsetForBoss.dat: Execute from homebrew ROP.
* Launcher.dat: Execute from Gateway's ROP.

Requires [devkitARM](http://sourceforge.net/projects/devkitpro/files/devkitARM/) to build.