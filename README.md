#millionboot - AVR bootloader
millionboot is a I2C bootloader for the AVR platform./Users/najiji/Dropbox/banabird/Development/Technical Docs/banabird_android_manual.md

* Can accept firmware updates over I2C
* Provides checksum checks on received data


##Startup behaviour
Assumption: `BOOTRST`fuse is enabled:

The AVR jumps straight into millionboot on reset. The bootloader loads its 7-bit device id from EEPROM at address `0x00` and initializes the I2C interface in slave mode with that address.

The bootloader switches into **firmware update mode** if in the first second after reset a general call packet with content `0xAA` *(firmware update command)* is received on I2C.

If another or no packet is received, the bootloader exits and jumps to address `0x000` in order to execute the application software.


