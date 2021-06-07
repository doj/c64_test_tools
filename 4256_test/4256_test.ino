// -*- c-basic-offset: 2; tab-width: 2; indent-tabs-mode: nil; mode: c++ -*-
/** @file
  Wiring diagram:
                                           +-----+
            +----[PWR]---------------------| USB |----+
            |                              +-----+    |
            |           GND/RST2  [ ][ ]              |
            |         MOSI2/SCK2  [ ][ ]    A5/SCL[ ] |
            |            5V/MISO2 [ ][ ]    A4/SDA[ ] |
            |                                 AREF[ ] |
            |                                  GND[ ] |    --- RAM GND, pin 16
            | [ ]N/C           4256         SCK/13[ ] | B5 (LED)
            | [ ]IOREF      +---U---+      MISO/12[ ] | B4 --- RAM DIN (D), pin 2
            | [ ]RST      A8|1    16|GND   MOSI/11[ ]~| B3 --- RAM /WE, pin 3
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
RAM A8,pin 1| [ ]A5/SCL     [ ] [ ] [ ]       RX<0[ ] | D0 --- RAM A0, pin 5
            |               [ ] [ ] [ ]               |
            |  UNO_R3       GND MOSI 5V _____________/
            \__________________________/

http://busyducks.com/ascii-art-arduinos

The 4 LEDs on Port A need to be connected to GND with a resistor. For
regular 5V LEDs a resistor of 220 Ohm or 470 Ohm should be used.

  This is for (TMS) 4256 DRAM chipds.

  How to diagnose:

  1. TO AVOID FRYING YOUR ARDUINO, test electrical condition of the chip **FIRST**:

  - Check for shorted Vcc
  - Inputs Ax, /CAS, /RAS, /WE, DIN must be high-Z. If shorted to 0v/5v, it's broken.
  - Output DOUT must be high-Z in the absence of RAS/CAS activity.
  - Output DOUT is push-pull AFTER the RAS/CAS read sequence (see datasheet). If it's high-Z, it's broken.

  2. Run this test:

  On startup the program will light up the 5 LEDs in sequence, to verify that the LEDs are working.
  Then they are turned off and testing starts.

  It will write 1s, then 0s, then two checkerboard patterns in a never-ending loop.

  RAM is automatically refreshed on row selection. As long as every row is selected once
  every 2 ms there is no need for RAS-only refresh cycle.

  TODO: a delay of <2ms could be added between the write and read loops to test data retention.

  - LED (pin 13) off -> TEST PASS
  - LED (pin 13) on  -> TEST FAIL (will turn back off at the end of the test cycle, so PAY ATTENTION)

  - LED port A0 -> write 0 failed
  - LED port A1 -> write 1 failed
  - LED port A2 -> checkerboard failed
  - LED port A3 -> inverse checkerboard failed

\author jamarju
\author modified from 4116_test.ino by Dirk Jagdmann <doj@cubic.org>
\date 2021-06-06
*/

#define DOUT 0
#define nCAS 1
#define nRAS 2
#define nWE  3
#define DIN  4
#define LED_13  5

// LEDs on port A
#define LED_TEST0 0
#define LED_TEST1 1
#define LED_TESTCB 2
#define LED_TESTRCB 3

#define SETB(port, b)  port |= _BV(b)
#define CLRB(port, b)  port &= ~_BV(b)
#define ISSET(port, b) ((port & _BV(b)) != 0)

#define TEST_1
#define TEST_0
#define TEST_CHECKERBOARD
#define TEST_REVERSE_CHECKERBOARD

void setup()
{
  // light up each LED for 200ms
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(13, OUTPUT);

  digitalWrite(A0, HIGH);
  delay(200);
  digitalWrite(A1, HIGH);
  delay(200);
  digitalWrite(A2, HIGH);
  delay(200);
  digitalWrite(A3, HIGH);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);

  // turn off LEDs
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);

  // configure RAM chip
  // bit for the data direction config:
  // 1 = out, 0 = in

  DDRD    = 0b11111111;
  // PD7  (7) 1------- A7
  // PD6  (6) -1------ A6
  // PD5  (5) --1----- A5
  // PD4  (4) ---1---- A4
  // PD3  (3) ----1--- A3
  // PD2  (2) -----1-- A2
  // PD1  (1) ------1- A1
  // PD0  (0) -------1 A0

  PORTD   = 0;

  DDRB    = 0b11111110;
  // PB5 (13) --1----- LED_13
  // PB4 (12) ---1---- DIN
  // PB3 (11) ----1--- nWE
  // PB2 (10) -----1-- nRAS
  // PB1 ( 9) ------1- nCAS
  // PB0 ( 8) -------0 DOUT

  PORTB   = 0b11011110;
  // PB4 (13) --0----- LED_13 = 0
  // PB4 (12) ---1---- DIN = 1
  // PB3 (11) ----1--- nWE = read
  // PB2 (10) -----1-- nRAS = 1
  // PB1 ( 9) ------1- nCAS = 1
  // PB0 ( 8) -------0 DOUT = no pullup

  pinMode(A5, OUTPUT);
}

#define setA(v) PORTD=v; digitalWrite(A5, (v&256)?HIGH:LOW)

inline void
w(const uint16_t row, const uint16_t col, const uint8_t v)
{
  if (v) {
    SETB(PORTB, DIN);
  } else {
    CLRB(PORTB, DIN);
  }

  setA(row);
  CLRB(PORTB, nRAS);
  CLRB(PORTB, nWE);

  setA(col);
  // hold nWE on
  CLRB(PORTB, nCAS);

  asm("nop");  // > 100 ns
  SETB(PORTB, nWE);
  SETB(PORTB, nRAS);
  SETB(PORTB, nCAS);
}

inline uint8_t
r(const uint16_t row, const uint16_t col)
{
  // read
  setA(row);
  CLRB(PORTB, nRAS);
  setA(col);
  CLRB(PORTB, nCAS);

  asm("nop"); // > 100n
  asm("nop");

  const uint8_t v = ISSET(PINB, DOUT);

  SETB(PORTB, nRAS);
  SETB(PORTB, nCAS);

  return v;
}

void loop()
{
  uint16_t col = 0, row = 0;
  uint8_t v = 0;
  while(1)
    {
#ifdef TEST_1
      // WRITE 1s
      col = row = 0;
      do {
        do {
          w(row, col, 1);
          row++;
        } while (row < 512);
        col++;
      } while (col < 512);

      // VERIFY
      col = row = 0;
      do {
        do {
          if (! r(row, col))
            {
              SETB(PORTB, LED_13);
              digitalWrite(A1, HIGH);
            }
          row++;
        } while (row < 512);
        col++;
      } while (col < 512);
#endif

#ifdef TEST_0
      // WRITE 0s
      col = row = 0;
      do {
        do {
          w(row, col, 0);
          row++;
        } while (row < 512);
        col++;
      } while (col < 512);

      // VERIFY
      col = row = 0;
      do {
        do {
          if (r(row, col))
            {
              SETB(PORTB, LED_13);
              digitalWrite(A0, HIGH);
            }
          row++;
        } while (row < 512);
        col++;
      } while (col < 512);
#endif

#ifdef TEST_CHECKERBOARD
      // WRITE CHECKERBOARD
      col = row = 0;
      v = 0;
      do {
        do {
          w(row, col, v);
          row++;
          v = !v;
        } while (row < 512);
        col++;
        v = !v;
      } while (col < 512);

      // VERIFY CHECKERBOARD
      col = row = 0;
      v = 0;
      do {
        do {
          if (r(row, col) != v)
            {
              SETB(PORTB, LED_13);
              digitalWrite(A2, HIGH);
            }
          row++;
          v = !v;
        } while (row < 512);
        col++;
        v = !v;
      } while (col < 512);
#endif

#ifdef TEST_REVERSE_CHECKERBOARD
      // WRITE REVERSE CHECKERBOARD
      col = row = 0;
      v = 1;
      do {
        do {
          w(row, col, v);
          row++;
          v = !v;
        } while (row < 512);
        col++;
        v = !v;
      } while (col < 512);

      // VERIFY REVERSE CHECKERBOARD
      col = row = 0;
      v = 1;
      do {
        do {
          if (r(row, col) != v)
            {
              SETB(PORTB, LED_13);
              digitalWrite(A3, HIGH);
            }
          row++;
          v = !v;
        } while (row < 512);
        col++;
        v = !v;
      } while (col < 512);
#endif

      delay(100);
      CLRB(PORTB, LED_13);
    }
}
