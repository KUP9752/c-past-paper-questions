#include "trie.h"
#define ALPHABET_SIZE 26

dictionary_t *create_dict(void) {
  return create_node();
}

void free_dict(dictionary_t *root) {
  free_node(root);
}

/* ------------ YOUR CODE BELOW -------------- */

bool find(dictionary_t *root, const char *word) {
  dictionary_t *currentNode = root;
  int c;
  if (!root) {
    return false;
  }

  for (int i = 0; word[i]; i++) {

    if (word[i] < 'A' || word[i] > 'Z') {
      return false;
    }

    c = word[i] - 'A';
    currentNode = currentNode -> children[c];

    if (!currentNode) { //child doesn't exist
      return false;
    }
  }
  return currentNode -> end_of_word;
}

bool insert(dictionary_t *root, const char *word) {
  dictionary_t *currentNode = root;
  int c;
  for (int i = 0; word[i]; i++) {
    if (word[i] < 'A' || word[i] > 'Z') {
      return false;
    }
    c = word[i] - 'A';

    if (!currentNode -> children[c]) {
      currentNode -> children[c] = create_node();
    }
    currentNode = currentNode -> children[c];
  }
  // we exit when currentNode is the last letter
  currentNode -> end_of_word = true;
  return true;
}

dictionary_t *create_node(void) {
  dictionary_t *newNode = (dictionary_t *) malloc(sizeof(dictionary_t));
  newNode -> end_of_word = false;
  newNode -> children = calloc(ALPHABET_SIZE, sizeof(dictionary_t *));
  return newNode;
}

void free_node(dictionary_t *root) {
  if (root) {
    for(int i = 0; i < ALPHABET_SIZE; i++) {
      free_node(root -> children[i]);
    }
    free(root -> children);
  free(root);
  }
}

bool load_from_file(dictionary_t *root, const char *filename) {
  FILE *filePtr = fopen(filename, "r");
  if (!filePtr) {
    perror("File could not be opened");
    return false;
  }

  char buffer[MAX_WORD_SIZE];
  while (fgets(buffer, MAX_WORD_SIZE, filePtr)){
    insert(root, strtok(buffer, "\n"));    
  }
  fclose(filePtr);
  return true;

}


