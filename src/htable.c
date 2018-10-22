#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

struct htablerec {
    int capacity;
    int num_keys;
    int *freqeuncies;
    char **keys;
    int *stats;
    hashing_t method;
};

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            }
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }

        fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full,
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\n%s\n\n",
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing");
    fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
    fprintf(stream, "-----------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "-----------------------------------------------------\n\n");
}

/*
  convert string to integer
  returns and unsigned long
*/
static unsigned int htable_word_to_int(char *word){
    unsigned int result = 0;

    /*add an offset of size 31 bits multiplied by result to get an unsigned int*/
    while(*word != '\0'){
        result = (*word++ + 31 * result);
    }
    return result;
}

/*
  To improve collision rate we employ double hashing
*/
static unsigned int htable_step(htable h, unsigned int i_key){
  return 1 + (i_key % (h->capacity -1));
}

/*
  Clean up the hashtable after its not used and any associated data.
  Memory for the struct must be deallocated in reverse order, this is to
  prevent losing pointer references to anything "inside" the struct.
*/
void htable_free(htable h){
    /*Free memory for the used by the keys*/
    free(h->keys);
    /*Free memory for the frequencies array*/
    free(h->freqeuncies);
    /*Free memory for the struct*/
    free(h);
}

/*insert a key into the hash table specified*/
int htable_insert(htable h, char *str){
    /*convert string to integer*/
    unsigned int int_word = htable_word_to_int(str);

    /*calculate index to put string*/
    int index = htable_step(h, int_word);

    if(NULL == h->keys[index]){
      /*allocate memory for size if string*/
      h->keys[index] = emalloc((strlen(str) + 1) * sizeof str[0]);
      /*copy string into index position*/
      strcpy(h->keys[index], str);
      /*increase key and frequency count and return 1*/
      h->freqeuncies[index]++;
      h->num_keys++;
      return 1;
    }else if(strcmp(h->keys[index], str) == 0){
      h->freqeuncies[index]++;
      return h->freqeuncies[index];
  } else if (strcmp(h->keys[index], str) != 0){
    int i;
    for (i = 0; i < h->capacity; i += htable_step(h, int_word)) {
      /*if current position is equal to capacity in first loop then wrap around*/
      if(i == h->capacity){
        i = (i%h->capacity);
      }
      if(NULL == h->keys[i]){
        h->keys[i] = emalloc((strlen(str) +1) * sizeof str[0]);
        strcpy(h->keys[i], str);
        h->freqeuncies[i]++;
        h->num_keys++;
        return 1;
      }
      if(strcmp(h->keys[i], str) == 0){
        h->freqeuncies[i]++;
        return h->freqeuncies[i];
      }
    }
  }
  return 0;
}

/*Creates a new hashtable of the specified capacity*/
htable htable_new(int capacity){
    int i;
    /*Allocate memory to the htable according to its size*/
    htable table = emalloc(sizeof * table);
    table->capacity = capacity;
    table->num_keys = 0;

    /*allocate memory for the frequency array relative to table capacity*/
    table->freqeuncies = emalloc(table->capacity * sizeof table->freqeuncies[0]);

    /*allocate memory for the keys based on the capacity and allocate enough memory to store the key*/
    table->keys = emalloc(table->capacity * sizeof table->keys[0]);

    /*set the frequency and initial key values to nothing*/
    for(i = 0; i < capacity; i++){
        table->freqeuncies[i] = 0;
        table->keys[i] = NULL;
    }
    return table;
}

void htable_print(htable h, FILE *stream){
  int i;
  for (i = 0; i < (*h).capacity; i++) {
    if (h->freqeuncies[i] != 0) {
      fprintf(stream, "%s\n %d\n", h->keys[i], h->freqeuncies[i]);
    }
  }
}

int htable_search(htable h, char *str){
  int colides = 0;
  unsigned int int_str = htable_word_to_int(str);

  unsigned int key_index = htable_step(h, int_str);
  while(strcmp(h->keys[key_index], str) != 0 && h->keys[key_index] != NULL
        && key_index < h->capacity){
        key_index = (key_index + htable_step(h, int_str));/*wrap around when index is bigger then capacity*/
        colides++;
  }
  if (colides == h->capacity) {
    return 0;
  }
  return h->freqeuncies[key_index];
}
