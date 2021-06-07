# Commodore 64 hardware test tools

![](img/PLA-arduino.JPG)

I wrote these programs to test the RAM, ROM and PLA chips with an Arduino Uno because I don't own a second known-working C64 to test them on.

With the help of these sketches I found my CHAROM and PLA were fried. I also was able to find that one of the chips in my International Soccer cartridge was failing to obey the /CS signal. If you are trying to repair your C64, I hope they help you too and wish you good luck!

Some sketches work on an Arduino Uno. Some others require an Arduino MEGA (I actually got a Robotdyn Mega Pro 2560, which is a pin-compatible, smaller factor version of the real Mega). The Arduino MEGA has plenty of pins and makes it easier to test the chips and also provide meaningful information through the serial port.

Check the chips for electrical consistence **before** wiring, ie: inputs are not shorted, chip obeys /CS, etc.

See test details on each sketch's heading comments.

## Summary of [64K x 1 bit RAM test](c64_dram_test/4116_test.ino)

Test for the 64K x 1 bit chips on long board breadbin C64s and older C64Cs (there are eight of these chips for a total 64 x 8 Kbit).

```
                                           +-----+
            +----[PWR]---------------------| USB |----+
            |                              +-----+    |
            |           GND/RST2  [ ][ ]              |
            |         MOSI2/SCK2  [ ][ ]    A5/SCL[ ] |
            |            5V/MISO2 [ ][ ]    A4/SDA[ ] |
            |                                 AREF[ ] |
            |                                  GND[ ] |    --- RAM GND, pin 16
            | [ ]N/C        4116/4864       SCK/13[ ] | B5 (LED)
            | [ ]IOREF      +---U---+      MISO/12[ ] | B4 --- RAM DIN (D), pin 2
            | [ ]RST     n/c|1    16|GND   MOSI/11[ ]~| B3 --- RAM /WE, pin 3
            | [ ]3V3       D|2    15|/CAS       10[ ]~| B2 --- RAM /RAS, pin 4
RAM VCC --- | [ ]5v      /WE|3    14|Q           9[ ]~| B1 --- RAM /CAS, pin 15
pin 8       | [ ]GND    /RAS|4    13|A6          8[ ] | B0 --- RAM DOUT (Q), pin 14
            | [ ]GND      A0|5    12|A3               |
            | [ ]Vin      A2|6    11|A4          7[ ] | D7 --- RAM A7, pin 9
            |             A1|7    10|A5          6[ ] | D6 --- RAM A6, pin 13
LED test 0  | [ ]A0   5V,VCC|8     9|A7          5[ ] | D5 --- RAM A5, pin 10
LED test 1  | [ ]A1         +-------+            4[ ] | D4 --- RAM A4, pin 11
LED test CB | [ ]A2                         INT1/3[ ] | D3 --- RAM A3, pin 12
LED test RCB| [ ]A3                         INT0/2[ ] | D2 --- RAM A2, pin 6
            | [ ]A4/SDA     RST SCK MISO      TX>1[ ] | D1 --- RAM A1, pin 7
            | [ ]A5/SCL     [ ] [ ] [ ]       RX<0[ ] | D0 --- RAM A0, pin 5
            |               [ ] [ ] [ ]               |
            |  UNO_R3       GND MOSI 5V _____________/
            \__________________________/
```
- LED (pin 13) off: TEST **PASS**
- LED (pin 13) on: TEST **FAIL** (will turn back off at the end of the test cycle, so PAY ATTENTION)

## Summary of [64K x 4 bit RAM test (Arduino MEGA)](c64_dram_test_mega2560)

Test for the 64K x 4 bit chips on short board C64Cs (TMS 4464, NEC 41464, etc). There are two of these chips on these boards.

The diagram is for a Robotdyn Mega Pro 2560 (smaller factor Arduino Mega clone).

Also works with a real Arduino Mega, just pay attention to the different pin layout.

```
                   +---------------
                   | VIN[ ][ ]VIN  \
                   | GND[ ][ ]GND  /
                   |  5V[ ][ ]5V   \
                   | 3V3[ ][ ]3V3  /
                   | RST[ ][ ]AREF \
               E1  |  TX[ ][ ]RX   /  E0
               E5  |  D3[ ][ ]D2   \  E4
               E3  |  D5[ ][ ]D4   /  G5
               H4  |  D7[ ][ ]D6   \  H3
               H6  |  D9[ ][ ]D8   /  H5
               B5  | D11[ ][ ]D10  \  B4
               B7  | D13[ ][ ]D12  /  B6
               J0  | D15[ ][ ]D14  \  J1
               H0  | D17[ ][ ]D16  /  H1
               D2  | D19[ ][ ]D18  \  D3
               D0  | D21[ ][ ]D20  /  D1
    RAM A1 --- A1  | D23[ ][ ]D22  \  A0 --- RAM A0
    RAM A3 --- A3  | D25[ ][ ]D24  /  A2 --- RAM A2
    RAM A5 --- A5  | D27[ ][ ]D26  \  A4 --- RAM A5
    RAM A7 --- A7  | D29[ ][ ]D28  /  A6 --- RAM A7
               C6  | D31[ ][ ]D30  \  C7
                   +----------------

                    --+-----+-------+
                    \ |     |       |
                    / | USB | [RST] |
                    \ +-----+       |
                    /               |
                    \               |
    RAM IO2 --- F1  /   A1[ ][ ]A0  |  F0 --- RAM IO1
    RAM IO4 --- F3  \   A3[ ][ ]A2  |  F2 --- RAM IO3
                F5  /   A5[ ][ ]A4  |  F4
                F7  \   A7[ ][ ]A6  |  F6
                K1  /   A9[ ][ ]A8  |  K0
                K3  \  A11[ ][ ]A10 |  K2
                K5  /  A13[ ][ ]A12 |  K4
                K7  \  A15[ ][ ]A14 |  K6
                C4  /  D33[ ][ ]D32 |  C5
   RAM /RAS --- C2  \  D35[ ][ ]D34 |  C3 --- RAM /CAS
    RAM /OE --- C0  /  D37[ ][ ]D36 |  C1 --- RAM /WE
                G2  \  D39[ ][ ]D38 |  D7
                G0  /  D41[ ][ ]D40 |  G1
                L6  \  D43[ ][ ]D42 |  L7
                L4  /  D45[ ][ ]D44 |  L5
                L2  \  D47[ ][ ]D46 |  L3
                     ---------------+
```

Sample serial output (115200 bps):

```
1111... PASS
0000... PASS
0101... PASS
1010... PASS
1111... PASS
0000... PASS
```

##  Summary of [ROM test](c64_rom_test)

```
                                               +-----+
                  +----[PWR]-------------------| USB |--+
                  |                            +-----+  |
                  |         GND/RST2  [ ][ ]            |
                  |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   C5
                  |          5V/MISO2 [ ][ ]  A4/SDA[ ] |   C4
                  |                             AREF[ ] |
                  |                              GND[ ] |
                  | [ ]N/C                    SCK/13[ ] |   B5 --- ROM O8
                  | [ ]IOREF                 MISO/12[ ] |   B4 --- ROM O7
                  | [ ]RST                   MOSI/11[ ]~|   B3 --- ROM A11
                  | [ ]3V3    +---+               10[ ]~|   B2 --- ROM A10
                  | [ ]5v    -| A |-               9[ ]~|   B1 --- ROM A9
                  | [ ]GND   -| R |-               8[ ] |   B0 --- ROM A8
                  | [ ]GND   -| D |-                    |
                  | [ ]Vin   -| U |-               7[ ] |   D7 --- ROM A7
                  |          -| I |-               6[ ]~|   .
   ROM O1 --- C0  | [ ]A0    -| N |-               5[ ]~|   .
   ROM O2 --- C1  | [ ]A1    -| O |-               4[ ] |   .
   ROM O3 --- C2  | [ ]A2     +---+           INT1/3[ ]~|   .
   ROM O4 --- C3  | [ ]A3                     INT0/2[ ] |   .
   ROM O5 --- C4  | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   .
   ROM O6 --- C5  | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   D0 --- ROM A0
                  |            [ ] [ ] [ ]              |
                  |  UNO_R3    GND MOSI 5V  ____________/
                  \_______________________/
```

- Random activity on LED (pin 13): TEST **PASS**
- LED is stuck either on or off: TEST **FAIL**


##  Summary of [PLA test](c64_pla_test)

```
                                               +-----+
                  +----[PWR]-------------------| USB |--+
                  |                            +-----+  |
                  |         GND/RST2  [ ][ ]            |
                  |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   C5
                  |          5V/MISO2 [ ][ ]  A4/SDA[ ] |   C4
                  |                             AREF[ ] |
                  |                              GND[ ] |
                  | [ ]N/C                    SCK/13[ ] |   B5 --- PLA I13
                  | [ ]IOREF                 MISO/12[ ] |   B4 --- PLA I12
                  | [ ]RST                   MOSI/11[ ]~|   B3 --- PLA I11
                  | [ ]3V3    +---+               10[ ]~|   B2 --- PLA I10
                  | [ ]5v    -| A |-               9[ ]~|   B1 --- PLA I9
                  | [ ]GND   -| R |-               8[ ] |   B0 --- PLA I8
                  | [ ]GND   -| D |-                    |
                  | [ ]Vin   -| U |-               7[ ] |   D7 --- PLA I7
                  |          -| I |-               6[ ]~|   .
  PLA I14 --- C0  | [ ]A0    -| N |-               5[ ]~|   .
  PLA I15 --- C1  | [ ]A1    -| O |-               4[ ] |   .
 PLA F0/4 --- C2  | [ ]A2     +---+           INT1/3[ ]~|   .
 PLA F1/5 --- C3  | [ ]A3                     INT0/2[ ] |   .
 PLA F2/6 --- C4  | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   .
 PLA F3/7 --- C5  | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   D0 --- PLA I0
                  |            [ ] [ ] [ ]              |
                  |  UNO_R3    GND MOSI 5V  ____________/
                  \_______________________/
```

- Random activity on LED (pin 13): **TEST PASS**
- A series of _n_ LED flashes: **TEST FAIL** output F<sub>n-1</sub>

## Summary of [ROM dump (Arduino MEGA)](c64_rom_dump_mega2560)

The diagram is for a Robotdyn Mega Pro 2560 (smaller factor Arduino Mega clone).

Also works with a real Arduino Mega, just pay attention to the different pin layout.

This is for 2364 (8 KB) ROMs commonly used for cartridges, BASIC and KERNAL ROMs. See sketch comments for 2332 (4 KB) ROMs.

```
                   +---------------
                   | VIN[ ][ ]VIN  \
                   | GND[ ][ ]GND  /
                   |  5V[ ][ ]5V   \
                   | 3V3[ ][ ]3V3  /
                   | RST[ ][ ]AREF \
               E1  |  TX[ ][ ]RX   /  E0
               E5  |  D3[ ][ ]D2   \  E4
               E3  |  D5[ ][ ]D4   /  G5
               H4  |  D7[ ][ ]D6   \  H3
               H6  |  D9[ ][ ]D8   /  H5
               B5  | D11[ ][ ]D10  \  B4
               B7  | D13[ ][ ]D12  /  B6
               J0  | D15[ ][ ]D14  \  J1
               H0  | D17[ ][ ]D16  /  H1
               D2  | D19[ ][ ]D18  \  D3
               D0  | D21[ ][ ]D20  /  D1
    ROM A1 --- A1  | D23[ ][ ]D22  \  A0 --- ROM A0
    ROM A3 --- A3  | D25[ ][ ]D24  /  A2 --- ROM A2
    ROM A5 --- A5  | D27[ ][ ]D26  \  A4 --- ROM A5
    ROM A7 --- A7  | D29[ ][ ]D28  /  A6 --- ROM A7
               C6  | D31[ ][ ]D30  \  C7
                   +----------------
```
```
                    --+-----+-------+
                    \ |     |       |
                    / | USB | [RST] |
                    \ +-----+       |
                    /               |
                    \               |
    ROM  D1 --- F1  /   A1[ ][ ]A0  |  F0 --- ROM D0
    ROM  D3 --- F3  \   A3[ ][ ]A2  |  F2 --- ROM D2
    ROM  D5 --- F5  /   A5[ ][ ]A4  |  F4 --- ROM D4
    ROM  D7 --- F7  \   A7[ ][ ]A6  |  F6 --- ROM D6
                K1  /   A9[ ][ ]A8  |  K0
                K3  \  A11[ ][ ]A10 |  K2
                K5  /  A13[ ][ ]A12 |  K4
                K7  \  A15[ ][ ]A14 |  K6
    ROM A11 --- C4  /  D33[ ][ ]D32 |  C5 --- ROM A12
    ROM  A9 --- C2  \  D35[ ][ ]D34 |  C3 --- ROM A10
    ROM  A8 --- C0  /  D37[ ][ ]D36 |  C1 --- ROM A9
                G2  \  D39[ ][ ]D38 |  D7
    ROM /CE --- G0  /  D41[ ][ ]D40 |  G1
                L6  \  D43[ ][ ]D42 |  L7
                L4  /  D45[ ][ ]D44 |  L5
                L2  \  D47[ ][ ]D46 |  L3
                     ---------------+
```

The program will prompt whether you want to drive /CE low or high and then dump the ROM over to the serial port (115.2 Kbps).

Driving /CE low enables the chip and produces a hex dump you can convert to bin with `xxd -r <dump.txt >dump.bin`:

```
/CE high or low? [h/l] l

00000000: 27 80 a8 80 c3 c2 cd 38 30 00 40 c0 40 c0 40 c0
00000010: 40 c0 40 c0 40 c0 40 c0 00 80 80 10 10 10 10 20
...
```

Driving /CE high disables the chip and should read `ff` throughout the entire dump:

```
/CE high or low? [h/l] h

00000000: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
00000010: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
...
```

A valid dump with /CE high means the chip **FAILS** to obey the /CE line.

## Thanks

 * [Zimmers invaluable archive of pinouts, schematics and documents](http://www.zimmers.net/anonftp/pub/cbm/index.html)
 * [~rcarlsen archive](http://personalpages.tds.net/~rcarlsen/cbm/c64/)
 * [PLA C code equations](http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c64/pla.c)
 * [Commodore 64: how to repair it, step by step](http://retro64.altervista.org/blog/commodore-64-repair-a-quick-guide-on-the-steps-required-to-fix-it/)
 * [Arduino ASCII diagrams](http://busyducks.com/ascii-art-arduinos)
 * ... and Commodore Business Machines for making the BEST COMPUTER EVER
