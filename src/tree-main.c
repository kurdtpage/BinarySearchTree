#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "bst.h"
#include "mylib.h"

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
 *    count to stderr. When -c option is given -d -o are ignored
 * -d Print depth of tree to stdout
 *    (defualt is linear)
 * -f write dot output to file name instead of default if -o is also
 *    given.
 * -o Ouput a representation of the tree in dot form to the file
 *    tree-view.dot
 * -r Make tree an RBT instead of BST
 * -h Displays help message describing program usage.
 *
 * @param argc Total number of arguments used with program
 * @param argv Array of command line arguments
 */

int main(int argc, char **argv) {
  int option_c;
  int option_d;
  int option_f;
  int option_o;
  int option_r;
  int words_and_freq;
  const char *optstring = "c:df:orh";
  char file_name[256];
  char dot_file_name[256];
  char option;
  clock_t clock_start;
  FILE *file;

  while ((option = getopt(argc, argv, optstring)) != EOF) {
    switch (option) {
      case 'c':
        strcpy(file_name, optarg);
        option_c = 1;
        words_and_freq = 1;
        option_d = 0;
        option_o = 0;
        break;
      case 'd':
        option_d = 1;
        break;
      case 'f':
        strcpy(dot_file_name, optarg);
        option_f = 1;
        break;
      case 'o':
        option_o = 1;
        break;
      case 'r':
        option_r = 1;
        break;
      case 'h':
        fprintf(stderr, "Usage: %s [OPTION]... <STDIN>\n\n", argv[0]);
        /*print_help(stderr);*/
        exit(EXIT_FAILURE);
      default:
        fprintf(stderr, "Usage: %s [OPTION]... <STDIN>\n\n", argv[0]);
        /*print_help(stderr);*/
        exit(EXIT_FAILURE);
    }
  }


  return EXIT_SUCCESS;
}

/*
 * Compile command:
 * gcc -W -Wall -ansi -pedantic -lm bst*.c mylib.c -o bst-asgn
 */
