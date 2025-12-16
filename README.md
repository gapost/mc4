# mc4

Programming the Newport/Klinger MC-4 4-axis controller for RBS via IEEE-488 (GPIB).

## Usage

The project creates 2 executables:
- mc4: send commands from the command line
- mc4gui: open a GUI widget to control the axes

The IEEE-488(GPIB) interface programming is based on the example program
given on page 11.11 of the manual.

It is written to link to the linux-gpib library.

It will be compatible to any NI-GPIB compatible interface with small modifications of the include/link libraries.

## Notes on MC4 IEEE-488 interface

### Setup

- Setting the device address

The dip-switch SW1 at the back side sets the address in binary

Swicth 1 = MSB
Switch 5 = LSB

Currently SW1 = 00110 binary = 6 

1 = switch **Up** !!

- Setting the terminator
  
The dip switch S3.4 sets the terminator:
- ON(Up): CR - in C-lang, append an '\r' to the cmd sent to MC4
- OFF(Down): CR+LF or LF - in C-lang, append an '\r\n' to the cmd sent to MC4

### Communication

The communication is by text messages.

The control PC sends a string through the IEEE-488 interface, e.g. "PX1000". This can be done with the IEEE-488 ibsend

The proper terminator must be appended to the string

- Axes

There 4 axes are designated with letters X, Y, Z, W

- Move absolute

To move X to position 1000 send

        PX1000

To move Z to position -500 send

        PZ-500

- Reset axis = set current position to zero

To reset the X axis send

        AX 

The position of X will become 0

### IEEE-488 read issue

When the control PC reads from MC4, e.g. with a command

        ibrd(6, buff.data(), 70);

the MC4 response is received correctly.

However, the MC4 starts beeping indicating error codes 2 & 3 on the display.

The source of the error could not be found, however, the current solution for this is:
- Send another command immediately after the read operation!

This resets the error in MC4 and the beeping stops.

### Reading the status

To read the status, 1st we send a command

        FSXX

where XX is a hex value from 0x00 to 0xFF. The value of XX determines what info the MC4 will send back when we request the status by the command

        ?

The 8 bits of XX have the following meaning:
- Bit 0: if set, info on axis state (moving or idle) will be sent
- Bit 1: W & X axis limits
- Bit 2: Y & Z limits
- Bit 3: Dig. Output
- Bit 4: W position
- Bit 5: X position
- Bit 6: Y position
- Bit 7: Z position

Thus, to get all info set XX=FF = 1111 1111 binary.

After sending FSFF, the response to a "?" will be a string S with the following form

- S[0] = '0'
- S[1]: if (S[1] & 1) then W moving. (S[1] & 2)->X, (S[1] & 4)->Y, (S[1] & 8)->Z
- S[2] = '1'
- S[3]: if (S[3] & 1) then W limit+, (S[3] & 2)->W limit-, (S[3] & 4)->X limit+, (S[3] & 8)->X limit-
- S[4] = '2'
- S[5]: if (S[5] & 1) then Y limit+, (S[5] & 2)->Y limit-, (S[6] & 4)->Z limit+, (S[6] & 8)->Z limit-
- S[6] = '3'
- S[7]: 

The rest of the string S[8..end] has structure

        W= %d X= %d Y= %d Z= %d

where %d means an integer.

Additionally, if the MC4 is waiting inside a program (not used now but maybe interesting for other apps) then it will prepend the response to "?" with

- S[0] = '4'
- S[1]: the 3 lower bits have some info on wait status


