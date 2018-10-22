#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

#define DEFAULT_HASH_TABLE_SIZE 113

static void print_info(int freq, char *word){
  printf("%-4d %s\n", freq, word);
}

/**
 * Main entry point for using hash table. Words are read in from
 * stdin and read into the hash table, they are then printed out
 * with there freqeuencies to stdout. Uses getopt library for
 * parsing arguments.
 * -c [filename] file to check spelling of words in using words read
 *    from stdin as dictionary. Prints timing info, unknown word
 *    count to stderr.
 * -d Use double hashing as collision resolution strategy
 *    (defualt is linear)
 * -e Display entire contents of hash table to stderr
 * -p Display stats info about the hash table
 * -s [snapshot] Display given nu,ner of stats snapshot when given
 *    -p as argument
 * -t [tablesize] Use the first prime >= [tabelsize] as size of hash table
 * -h Displays help message describing program usage.
 *
 * @param argc Total number of arguments used with program
 * @param argv Array of command line arguments
 */
int main(int argc, char **argv) {
  hashing_t option_d = LINEAR_P;
  int option_c = 0;
  int option_e = 0;
  int option_p = 0;
  clock_t clock_start;
  double time_search = 0;
  double clock_time_fill = 0;
  int count_unknown_words = 0;
  int option_t = DEFAULT_HASH_TABLE_SIZE;
  int option_s;
  int words_and_freq = 1;
  const char *optstring = "c:deps:t:h";
  char option;
  char file_name[256];
  htable hash_table;
  FILE *file;
  char word[256];

  while ((option = getopt(argc, argv, optstring))) {
    switch (option) {
      case 'c':
        strcpy(file_name, optarg);
        words_and_freq = 1;
        option_c = 1;
        option_p = 0;
        break;
      case 'd':
        option_d = 1;
        break;
      case 'e':
        option_e = 1;
        break;
      case 'p':
        words_and_freq = 0;
        if (!option_c) {
          option_p = 1;
        }
        break;
      case 's':
        option_s = atoi(optarg);
        break;
      case 't':
        option_t = atoi(optarg);
        break;
      case 'h':
        fprintf(stderr, "Usage: %s [OPTION]... <STDIN>\n\n", argv[0]);
        /* print_help(stderr);*/
        exit(EXIT_FAILURE);
      default :
        fprintf(stderr, "Usage: %s [OPTION]... <STDIN>\n\n", argv[0]);
        /*print_help(stderr);*/
        exit(EXIT_FAILURE);
      }
  }

  hash_table = htable_new(option_t);

  while (getword(word, sizeof word, stdin) != EOF) {
    clock_start = clock();
    htable_insert(hash_table, word);
    clock_time_fill += (clock() - clock_start) / (double) CLOCKS_PER_SEC;
  }

  if (option_e) {
    htable_print_entire_table(hash_table, stdout);
  }

  if (option_p) {
    htable_print_stats(hash_table, stderr, option_s);
  }

  if (option_c) {
    file = fopen(file_name, "r");
    while (getword(word, sizeof word, stdin) != EOF) {
      clock_start = clock();
      if (htable_insert(hash_table, word) == 0) {
        fprintf(stderr, "%s\n", word);
        count_unknown_words++;
      }
      time_search += (clock() - clock_start) / (double) CLOCKS_PER_SEC;
    }
    printf("Fill time     : %f\n", clock_time_fill);
    printf("Search time   : %f\n", time_search);
    printf("Unknown words = %d\n", count_unknown_words);
  }

  if(words_and_freq){
    htable_print(hash_table, );
  }

  htable_free(hash_table);
  return EXIT_SUCCESS;
}

/*
 * Compile command:
 * gcc -W -Wall -ansi -pedantic -lm htable*.c mylib.c -o htable-asgn
 */
