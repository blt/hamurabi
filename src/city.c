/*
  The MIT License

  Copyright (c) 2010 Brian L. Troutwine

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated
  documentation files (the "Software"), to deal in the
  Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to
  do so, subject to the following conditions:

  The above copyright notice and this permission notice shall
  be included in all copies or substantial portions of the
  Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
  OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdlib.h>
#include <stdint.h>

#include "city.h"

struct _st {
  uint16_t population;
  uint16_t starved;
  uint16_t migrated;
  uint16_t bushels;
  uint16_t acres;
  uint16_t yield;
  uint16_t pests;
  uint16_t trade_val;
  uint16_t avg_starved;
  uint16_t tot_died;
  uint16_t fed;
  uint16_t planted;
};

city_st *new_city_st() {
  city_st *cty = (city_st *) malloc(sizeof(city_st));

  cty->population  = 95;
  cty->migrated    = 5;
  cty->starved     = 0;
  cty->bushels     = 2800;
  cty->acres       = 1000;
  cty->yield       = 3;
  cty->pests       = 200;
  cty->trade_val   = 17+RAND(10);
  cty->avg_starved = 0;
  cty->tot_died    = 0;
  cty->fed         = 0;
  cty->planted     = 0;

  return cty;
}

void free_city_st(city_st *cty) {
  free(cty);
}

void add_migrants(city_st *cty) {
  cty->population += cty->migrated;
  cty->migrated = 0;
}

void rats(city_st *cty) {
  cty->pests = cty->bushels / (RAND(5)+2);
}

uint16_t plague(city_st *cty) {
  uint16_t died = cty->population / (RAND(4)+2);
  cty->population -= died;
  return died;
}

uint16_t starvation(city_st *cty) {
  /* 20 bushels of grain are needed for each citizen to
     live. */
  cty->starved = cty->population - (cty->fed/20);
  return cty->starved;
}

uint16_t births(city_st *cty) {
  /* For each two people living at the end of the year, one
     child is born. Carrying capacity of city is 6000. */
  uint16_t pop = cty->population;
  return pop*(1 - pop/6000);
}

resp step(city_st *cty) {
  uint16_t died = 0;
  died += starvation(cty);
  if (died > cty->population * 0.45)
    return ESTARVE;
  cty->avg_starved = 0.1*died + 0.9*cty->avg_starved; // EMA

  if (RAND(15) == 0)
    died += plague(cty);
  cty->tot_died += died;

  cty->population += births(cty);

  cty->migrated  = 0.1*RAND(cty->population);
  cty->trade_val = 17+RAND(10);
  cty->yield = RAND(10)+1;

  cty->bushels += cty->yield*cty->planted;
  rats(cty);
  return OKAY;
}

resp buy_acres(city_st *cty, const uint16_t amnt) {
  if (cty->trade_val*amnt > cty->bushels)
    return EBUSHELS;
  cty->bushels -= (cty->trade_val)*amnt;
  cty->acres   += amnt;
  return OKAY;
}

resp sell_acres(city_st *cty, const uint16_t amnt) {
  if (amnt > cty->acres)
    return EACREAGE;
  cty->bushels += (cty->trade_val)*amnt;
  cty->acres   -= amnt;
  return OKAY;
}

resp feed_populace(city_st *cty, const uint16_t amnt) {
  if (amnt >= cty->bushels)
    return EBUSHELS;
  cty->bushels -= amnt;
  cty->fed = amnt;
  return OKAY;
}

resp plant_seed(city_st *cty, const uint16_t amnt) {
  if (amnt > cty->acres)
    return EACREAGE;
  else if (amnt > cty->bushels)
    return EBUSHELS;
  else if (amnt > 10*cty->population)
    return EPOP;

  cty->bushels -= amnt;
  cty->planted = amnt;
  return OKAY;
}

uint16_t population(const city_st *cty) {
  return cty->population;
}

uint16_t acres(const city_st *cty) {
  return cty->acres;
}

uint16_t yield(const city_st *cty) {
  return cty->yield;
}

uint16_t pests(const city_st *cty) {
  return cty->pests;
}

uint16_t bushels(const city_st *cty) {
  return cty->bushels;
}

uint16_t trade_val(const city_st *cty) {
  return cty->trade_val;
}

uint16_t starved(const city_st *cty) {
  return cty->starved;
}

uint16_t migrated(const city_st *cty) {
  return cty->migrated;
}

uint16_t tot_died(const city_st *cty) {
  return cty->tot_died;
}

uint16_t avg_starved(const city_st *cty) {
  return cty->avg_starved;
}
