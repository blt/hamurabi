/*
   This an ancient game. Have a gander at this:

   10 REM *** CONVERTED FROM THE ORIGINAL FOCAL PROGRAM AND MODIFIED
   20 REM *** FOR EDUSYSTEM 70 BY DAVID AHL, DIGITAL
   30 REM *** MODIFIED FOR 8K MICROSOFT BASIC BY PETER TURNBULL

   Well, now it has been transliterated from the BASIC to C.

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
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

#include "city.h"

uint16_t input(void);

void retire();
void storm_out();
void ejected();
void game_end(const city_st *cty);
void nero_end();
void not_so_bad_end(const city_st *cty);

uint16_t input(void) {
  const uint16_t inp_size = 10;
  char input[inp_size], *endptr;
  assert( fgets(input, inp_size, stdin) );
  errno = 0;
  uint16_t q = (uint16_t) strtol(input, &endptr, inp_size);
  if ((errno == ERANGE && (q == LONG_MAX || q == LONG_MIN))
      || (errno != 0 && q == 0)) {
    perror("strtol");
    exit(EXIT_FAILURE);
  }
  if (endptr == input) {
    fprintf(stderr, "No digits were found\n");
    exit(EXIT_FAILURE);
  }
  return q;
}

/*
   Client functions

   Someday I dream of being in curses.
 */
void retire() {
  printf("So long for now.\n");
  exit(EXIT_SUCCESS);
}

void storm_out() {
  printf("Hamurabi: I cannot do what you wish.\n");
  printf("Get yourself another steward!!!!!\n");
  retire();
}

void ejected() {
  printf("Due to this extreme mismanagement you have not only\n");
  printf("been impeached and thrown out of office but you have\n");
  printf("also been declared 'National Fink' !!\n");
  retire();
}

void game_end(const city_st *cty) {
  uint16_t l = (uint16_t) (acres(cty)/(population(cty)));
  printf("In your 10-year term of office %d percent of the\n",
         avg_starved(cty));
  printf("population starved per year on average, i.e., a total of\n");
  printf("%d people died!!\n\n", tot_died(cty));
  printf("You started with 10 acres per person and ended with\n");
  printf("%d acres per person.\n\n", l);

  if ((avg_starved(cty)>33) || (l<7)) ejected();
  if ((avg_starved(cty)>10) || (l<9)) nero_end();
  if ((avg_starved(cty)>3)  || (l<10)) not_so_bad_end(cty);
}

void nero_end() {
  printf("Your heavy handed performance smacks of Nero and Ivan IV.\n");
  printf("The people (remaining) find you an unpleasant ruler, and,\n");
  printf("frankly, hate your guts!\n");
  retire();
}

void not_so_bad_end(const city_st *cty) {
  printf("Your performance could have been somewhat better, but\n");
  printf("really wasn't too bad at all. ");
  printf("%d people would ", RAND(population(cty)));
  printf("dearly like to see you assassinated but we all have our\n");
  printf("trivial problems.\n");
  retire();
}

int main(void) {
  puts("Try your hand at governing Ancient Sumeria\n"
       "successfully for a 10 year term of office.\n");

  city_st *cty = new_city_st();
  uint16_t inp;

  for (uint16_t year=0; year < 11; year++) {
    printf("Hamurabi: I beg to report to you,\nin year %d, ", year);
    printf("%d people starved %d came to the city.\n",
           starved(cty), migrated(cty));

    add_migrants(cty);

    if (RAND(2) == 1) {
      printf("A horrible plague struck! Many people have died!\n");
      plague(cty);
    }

    printf("\nPopulation is now %d\n", population(cty));
    printf("The city now owns %d acres\n", acres(cty));
    printf("You have harvested %d bushels per acre.\n",
           yield(cty));
    printf("Rats ate %d bushels.\n\n",
           pests(cty));
    printf("You now have %d bushels in store.\n\n",
           bushels(cty));
    printf("Land is trading at %d bushels per acre.\n",
           trade_val(cty));
  buy_acres:
    printf("How many acres do you wish to buy?\n");
    inp = input();
    if (inp < 0) storm_out();
    else if (inp == 0) goto sell_acres;
    if (buy_acres(cty, inp) != 0) {
      printf("Hamurabi: Think again. You have only %d",
             bushels(cty));
      printf(" bushels of grain. Now then,\n");
      goto buy_acres;
    }
    goto feed_people;

  sell_acres:
    printf("How many acres do you wish to sell?\n");
    inp = input();
    if (inp < 0) storm_out();
    else if (inp == 0) goto feed_people;
    if (sell_acres(cty, inp) != 0) {
      printf("Hamurabi: Think again. You have only %d acres.",
             acres(cty));
      printf(" Now then,\n");
      goto sell_acres;
    }

  feed_people:
    printf("How many bushels do you wish to feed your people?\n");
    printf("(No starvation: %d bushels.)\n",
           population(cty)*20);
    inp = input();
    if (inp < 0) storm_out();
    else if (inp == 0) goto plant_seed;
    if (feed_populace(cty, inp) != 0) {
      printf("Hamurabi: Think again. We only have %d bushels.",
             bushels(cty));
      printf(" Now then,\n");
      goto feed_people;
    }

  plant_seed:
    printf("How many acres do you wish to plant with seed?\n");
    printf("(MAX: %d acres)\n", 10*population(cty));
    inp = input();
    if (inp < 0) storm_out();
    else if (inp == 0) goto progress;
    switch(plant_seed(cty, inp)) {
    case EACREAGE: {
      printf("Hamurabi: Think again. You have only %d acres.",
             acres(cty));
      printf(" Now then,\n");
      goto plant_seed;
    } break;
    case EPOP: {
      printf("But you only have %d people to tend the fields.",
             population(cty));
      printf(" Now then,\n");
      goto plant_seed;
    } break;
    case EBUSHELS: {
      printf("Hamurabi: Think again. We only have %d bushels.",
             bushels(cty));
      printf(" Now then,\n");
      goto plant_seed;
    } break;
    case ESTARVE:
    case OKAY: break;
    }

  progress:
    if (step(cty) == ESTARVE) {
      printf("You starved %d people in one year!!!\n",
             starved(cty));
      ejected();
    }

  }

  game_end(cty);
  free_city_st(cty);
}
