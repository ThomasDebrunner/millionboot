#millionboot - AVR bootloader
millionboot is a I2C bootloader for the AVR platform./Users/najiji/Dropbox/banabird/Development/Technical Docs/banabird_android_manual.md

* Can accept firmware updates over I2C
* Provides checksum checks on received data


##Startup behaviour
Assumption: `BOOTRST`fuse is enabled:

The AVR jumps straight into millionboot on reset. The bootloader loads its 7-bit device id from EEPROM at address `0x00` and initializes the I2C interface in slave mode with that address.

The bootloader switches into **firmware update mode** if in the first second after reset a general call packet with content `0xAA` *(firmware update command)* is received on I2C.

If another or no packet is received, the bootloader exits and jumps to address `0x000` in order to execute the application software.

The bootloader jumps into **firmware upgrade mode**

##Firmware upgrade mode
Once the bootloader has entered firmware upgrade mode it **no longer respects general calls**. The firmware has to be transmitted to every node individually. Nodes that do not receive firmware upgrades stay on current firmware. 

The bootloader accepts the **intel hex format** with a maximum line length of 45 characters, every line has to start with `:`and end with `\r\n`. The first byte is interpreted as the payload length, the following two bytes as the destination (byte) address followed by 1 byte operation code, payload and a 1 byte checksum.

All lines of the hex file have to be transmitted serially over I2C, one line a transmission. The bootloader can take at most **45 bytes** per transmission.
After that, no further actions on the bus are Acknowledged until the hex file is succesfully checked and programmed.

It is suggested that the sender polls the bootloader with read inquiries, until the bootloader acknowledges again. The bootloader returns a byte that can contain the following:

Value		|Interpretation
----------|-----------------------------------------------------
101			|The provided data is missing the : starting character
102			|The provided data is malformatted. E.g. Size does not match
103			|Checksum failed
201			|Data could not be written because the page to be written is already full. (There is some serious error!)
202			|The supplied data is in wrong order.
203			|Illegal hex record. The bootloader only supports the *data* (type 0) and *end of file* (type 1) record. Especially the *Start Segment Address Record* (type 3) is **not** supported, which means that every new software has to start at offset `0x0000`

In all of these cases the hex line has to be retransmitted again.




