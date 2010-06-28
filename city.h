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

#ifndef __CITY_H
#define __CITY_H

#define RAND(N) (uint16_t)((double)rand() / ((double)RAND_MAX + 1) * N)

typedef struct _st city_st;
typedef enum _resp {
  OKAY,     /* Is there a problem here? There's no problem here. */
  EACREAGE, /* Insufficient acres in city. */
  EPOP,     /* Insufficient population in city. */
  EBUSHELS, /* Insufficient bushels in city storage. */
  ESTARVE   /* More than 45% of people starved in a year. */
} resp;

city_st *new_city_st();
void free_city_st(city_st *cty);
void add_migrants(city_st *cty);
void rats(city_st *cty);
uint16_t plague(city_st *cty);
uint16_t starvation(city_st *cty);
uint16_t births(city_st *cty);
resp step(city_st *cty);
resp buy_acres(city_st *cty, const uint16_t amnt);
resp sell_acres(city_st *cty, const uint16_t amnt);
resp feed_populace(city_st *cty, const uint16_t amnt);
resp plant_seed(city_st *cty, const uint16_t amnt);

uint16_t population(const city_st *cty);
uint16_t acres(const city_st *cty);
uint16_t yield(const city_st *cty);
uint16_t pests(const city_st *cty);
uint16_t bushels(const city_st *cty);
uint16_t trade_val(const city_st *cty);
uint16_t starved(const city_st *cty);
uint16_t migrated(const city_st *cty);
uint16_t tot_died(const city_st *cty);
uint16_t avg_starved(const city_st *cty);

#endif
