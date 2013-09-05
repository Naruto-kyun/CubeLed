/*
 * Commande d'une cube à LED
 */

#include "Tlc5940.h"

/* Tableau de correspondance x, y -> Canal TLC :
   tlc_order[y][x] = Canal TLC
 */ 
const TLC_CHANNEL_TYPE tlc_order[][4] = {
  { 15, 14, 13, 11 }, /* y = 0 */
  { 12, 10,  9,  8 }, /* y = 1 */
  {  4,  5,  6,  7 }, /* y = 2 */
  {  0,  1,  2,  3 }, /* y = 3 */
};

/* Tableau de correspondance z -> pin transistor
   transistor_order[z] = pin transistor
 */
const unsigned transistor_order[] = {
  5, /* z = 0 */
  7, /* z = 1 */
  4, /* z = 2 */
  6, /* z = 3 */
};

/* Valeur des LEDS (0 - 4095)
   values[z][y][x] = val
 */
const unsigned F = 4095;
unsigned values[4][4][4] = {
    /* z = 0 */
    { { F, 0, 0, F }, 
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { F, 0, 0, F } },

    /* z = 1 */
    { { 0, 0, 0, 0 },
      { 0, F, F, 0 },
      { 0, F, F, 0 },
      { 0, 0, 0, 0 } },

    /* z= 2 */
    { { 0, 0, 0, 0 },
      { 0, F, F, 0 },
      { 0, F, F, 0 },
      { 0, 0, 0, 0 } },

    /* z = 3 */
    { { F, 0, 0, F },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { F, 0, 0, F } }
};

void setLed(unsigned x, unsigned y, unsigned z, unsigned val)
{
  values[z][y][x] = val;
}

unsigned getLed(unsigned x, unsigned y, unsigned z)
{
  return values[z][y][x];
}

/* Parcourt les 4 étages */
void refresh()
{
  for (unsigned z = 0; z < 4; z++) {
    for (unsigned y = 0; y < 4; y++) {
      for (unsigned x = 0; x < 4; x++) {
        unsigned val = getLed(x, y, z);
        Tlc.set(tlc_order[y][x], val);
      }
    }

    Tlc.update();
    delay(1);
    digitalWrite(transistor_order[z], LOW);
    delay(1);
    digitalWrite(transistor_order[z], HIGH);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);

  Tlc.init();
}

void discard()
{
  while (Serial.available()) {
    char poked = Serial.peek();

    if (poked >= 'A' && poked <= 'Z')
      return;

    Serial.read();
  }
}

void loop()
{
  refresh();

  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 'W') {
      for (unsigned z = 0; z < 4; z++) {
        for (unsigned y = 0; y < 4; y++) {
          for (unsigned x = 0; x < 4; x++) {
            setLed(x, y, z, Serial.parseInt());
          }
        }
      }
      //Serial.println("1");
    }
    else if (cmd == 'S') {
      unsigned x, y, z, val;
      x = Serial.parseInt();
      y = Serial.parseInt();
      z = Serial.parseInt();
      val = Serial.parseInt();

      setLed(x, y, z, val);
      Serial.println("1");
    }
    else if (cmd == 'C') {
      memset(values, 0, 64);
      Serial.println("1");
    }
    else {
      Serial.println("0");
      discard();
    }

    Serial.flush();
  }
}

