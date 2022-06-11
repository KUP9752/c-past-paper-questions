#include "doublets.h"
#include<assert.h>

/* ------------------ YOUR CODE HERE ------------------- */

bool valid_step(dictionary_t *dict, const char *curr_word, const char *next_word) {
  //check rule 2:
  bool oneChange = false;
  //Same length?
  if (strlen(curr_word) != strlen(next_word)) return false;

  // only 1 letter difference.
  for (int i = 0; curr_word[i]; i++) {
    if (curr_word[i] != next_word[i]) {
      if (oneChange) {
        return false;
      } else {
          oneChange = true;
        }
    }
  }

  return find(dict, next_word) && oneChange;
}

void printOut(const char *word, bool upCond) {
  for (int i = 0; word[i]; i++) {
    printf("%c", upCond ? toupper(word[i]) : tolower(word[i]));
  }
  printf("\n");
}
void print_chain(const char **chain) {
  assert(chain);
  printOut(chain[0], true);
  int i = 1;
  for(; chain[i + 1]; i++) {
    printOut(chain[i], false);
  }
  //i is now the final word, before the NULL end
  printOut(chain[i], true);
  return;
}

bool valid_chain(dictionary_t *dict, const char **chain) {

  for (int i = 0; chain[i] && chain[i+1]; i++) {
    if (!valid_step(dict, chain[i], chain[i+1])) {
      return false;
    }
    //check if everything before i, equals i
    for (int j = 0; j < i; j++) { 
      if (!strcmp(chain[i], chain[j])){
        return false;
      }
    }
  }

  return true;
}

void chainadd(const char *str, int place, const char **chain) {
  chain[place] = strcpy(malloc(strlen(str) + 1), str);
}

bool used(const char **chain, char* word) {
  for (char** w = (char**)chain; *w; w++) {
    if (!strcmp(*w, word)) return true;
  }
  return false;
}

bool find_chain_aux(dictionary_t *dict, const char *start_word, const char *target_word, const char **chain, int max_words, int word_no) {
  if (word_no == max_words) return false;
  else if (valid_step(dict, start_word, target_word)) {
    chainadd(target_word, word_no, chain);
    return true;
  } else {
    for (int c = 0; start_word[c]; c++) {
      for (char i = 'A'; i <= 'Z'; i++) {
        char newword[MAX_WORD_SIZE];
        strcpy(newword, start_word)[c] = i;
        if (!used(chain, newword) && i != start_word[c] && find(dict, newword)) {
          chainadd(newword, word_no, chain);
          if (find_chain_aux(dict, newword, target_word, chain, max_words, word_no + 1)) return true;
        }
      }
    }
    return false;
  }
}

bool find_chain(dictionary_t *dict, const char *start_word, const char *target_word, const char **chain, int max_words) {
  chainadd(start_word, 0, chain);
  return find_chain_aux(dict, start_word, target_word, chain, max_words, 1);
}

