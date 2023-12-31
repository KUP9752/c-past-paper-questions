#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exam.h"
#define NULLCHECK(ptr, msg) \
  if (ptr == NULL) { \
    perror(strcat("Memory Allocation Failure at ", msg)); \
    return NULL; \
  };
/*
 * Private function prototypes.
 */

static void _print_huffman_tree(const huffman_tree_t *, int);
static void _print_huffman_tree_codes(const huffman_tree_t *, char *, char *);

/*
 * Prints the given Huffman tree.
 */
void print_huffman_tree(const huffman_tree_t *t) {
  printf("Huffman tree:\n");
  _print_huffman_tree(t, 0);
}

/*
 * Private helper function for print_huffman_tree.
 */
static void _print_huffman_tree(const huffman_tree_t *t, int level) {
  int i;
  for (i = 0; i <= level; i++) {
    printf("  ");
  }

  if (t->left == NULL && t->right == NULL) {
    printf("Leaf: '%c' with count %d\n", t->letter, t->count);
  } else {
    printf("Node: accumulated count %d\n", t->count);

    if (t->left != NULL) {
      _print_huffman_tree(t->left, level + 1);
    }

    if (t->right != NULL) {
      _print_huffman_tree(t->right, level + 1);
    }
  }
}

/*
 * Prints the codes contained in the given Huffman tree.
 */
void print_huffman_tree_codes(const huffman_tree_t *t) {
  printf("Huffman tree codes:\n");

  char *code = calloc(MAX_CODE_LENGTH, sizeof(char)), *code_position = code;
  if (code == NULL) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }

  _print_huffman_tree_codes(t, code, code_position);
  free(code);
}

/*
 * Private helper function for print_huffman_tree_codes.
 */
static void _print_huffman_tree_codes(const huffman_tree_t *t,
                                      char *code, char *code_position) {

  if (t->left == NULL && t->right == NULL) {
    printf("'%c' has code \"%s\"\n", t->letter, code);
  } else if (t->left != NULL) {
    *code_position++ = 'L';
    *code_position = '\0';

    _print_huffman_tree_codes(t->left, code, code_position--);
  }

  if (t->right != NULL) {
    *code_position++ = 'R';
    *code_position = '\0';

    _print_huffman_tree_codes(t->right, code, code_position--);
  }
}

/*
 * Prints a list of Huffman trees.
 */
void print_huffman_tree_list(const huffman_tree_list_t *l) {
  printf("Huffman tree list:\n");

  for (; l != NULL; l = l->next) {
    print_huffman_tree(l->tree);
  }
}

/*
 * Frees a Huffman tree.
 */
void huffman_tree_free(huffman_tree_t *t) {
  if (t != NULL) {
    huffman_tree_free(t->left);
    huffman_tree_free(t->right);
    free(t);
  }
}

/*
 * Frees a list of Huffman trees.
 */
void huffman_tree_list_free(huffman_tree_list_t *l) {
  if (l != NULL) {
    huffman_tree_list_free(l->next);
    huffman_tree_free(l->tree);
    free(l);
  }
}

/*
 * Returns 1 if the string s contains the character c and 0 if it does not.
 */
int contains(char *s, char c) {
  for (int i = 0; s[i]; i++) {
    if (s[i] == c) {
      return 1;
    } 
  }
  return 0;
}

/*
 * Returns the number of occurrences of c in s.
 */
int frequency(char *s, char c) {
  int count = 0;
  for (int i = 0; s[i]; i++) {
    if (s[i] == c) {
      count++;
    } 
  }
  return count;
}

/*
 * Takes a string s and returns a new heap-allocated string containing only the
 * unique characters of s.
 *
 * Pre: all strings will have fewer than or equal to MAX_STRING_LENGTH - 1
 *      characters.
 */
char *nub(char *s) {
  char *temp = calloc(strlen(s) + 1, sizeof(char));
  int count = 0;
  NULLCHECK(temp, "nub (temp creation)");
  for (int i = 0; s[i] && i < MAX_STRING_LENGTH - 1; i++){
      if (!contains(temp, s[i])) {
        temp[count] = s[i];
        count++;
      }
  }
  temp = realloc(temp, strlen(temp) + 1);
  NULLCHECK(temp, "nub (temp realloc)");
  return temp;
}

/*
 * Adds the Huffman tree t to the list l, returning a pointer to the new list.
 *
 * Pre:   The list l is sorted according to the frequency counts of the trees
 *        it contains.
 *
 * Post:  The list l is sorted according to the frequency counts of the trees
 *        it contains.
 */
huffman_tree_list_t *huffman_tree_list_add(huffman_tree_list_t *l, huffman_tree_t *t) {
  huffman_tree_list_t *newListNode = malloc(sizeof(huffman_tree_list_t));
  newListNode->tree = t;
  newListNode->next = NULL;

  if (l == NULL) { //NULL LIST;
    return newListNode;
  }

  if (t -> count < l -> tree -> count) {
    newListNode -> next = l;
    return newListNode;
  }

  huffman_tree_list_t *prev = l;
  huffman_tree_list_t *current = l -> next;

  while (current != NULL) {
    if (t -> count < current-> tree -> count) { 
      //found where we need to insert: prev => t => current
      break;
    }
    prev = current;
    current = current -> next;
  }
  //found where to insert:
  prev -> next = newListNode;
  newListNode -> next = current;

  return l;
}

/*
 * Takes a string s and a lookup table and builds a list of Huffman trees
 * containing leaf nodes for the characters contained in the lookup table. The
 * leaf nodes' frequency counts are derived from the string s.
 *
 * Pre:   t is a duplicate-free version of s.
 *
 * Post:  The resulting list is sorted according to the frequency counts of the
 *        trees it contains.
 */
huffman_tree_list_t *huffman_tree_list_build(char *s, char *t) {
  huffman_tree_list_t *list = NULL; //(huffman_tree_list_t *) malloc(sizeof(huffman_tree_list_t));
  char c;
  t = nub(t);

  for(int i = 0; i < strlen(t); i++) {
    huffman_tree_t *treeNode = (huffman_tree_t *) malloc(sizeof(huffman_tree_t));
    NULLCHECK(treeNode, "list_build");
    c = t[i];
    treeNode -> count = frequency(s, c);
    treeNode -> letter = c;
    treeNode -> left = NULL;
    treeNode -> right = NULL;

    // printf("\n %d \n", i);
    list = huffman_tree_list_add(list, treeNode);
  }
  free(t);
  return list;
}

/*
 * Reduces a sorted list of Huffman trees to a single element.
 *
 * Pre:   The list l is non-empty and sorted according to the frequency counts
 *        of the trees it contains.
 *
 * Post:  The resuling list contains a single, correctly-formed Huffman tree.
 */
huffman_tree_list_t *huffman_tree_list_reduce(huffman_tree_list_t *l) {
  if (l == NULL) {
    return l;
  }

  if (l->next != NULL) {
    huffman_tree_t *newTree = malloc(sizeof(huffman_tree_t));
    NULLCHECK(newTree, "reduce");

    newTree -> count = l -> tree -> count + l -> next -> tree -> count;
    newTree -> left = l -> tree;
    newTree -> right = l -> next -> tree;
    free(l);
    free(l -> next);
    huffman_tree_list_t *list = huffman_tree_list_add(l -> next ->next, newTree);
    return huffman_tree_list_reduce(list);
  }
  return l;
}

/*
 * Accepts a Huffman tree t and a string s and returns a new heap-allocated
 * string containing the encoding of s as per the tree t.
 *
 * Pre: s only contains characters present in the tree t.
 */
char *huffman_tree_encode(huffman_tree_t *t, char *s) {
  char *encoded = malloc(sizeof(char) * MAX_CODE_LENGTH) ;
  NULLCHECK(encoded, "encode");
  strcpy(encoded, "");
  huffman_tree_t *current = t;
  while (*s != '\0') {
    if (current->left == NULL && current->right == NULL) {
            current = t;
            s++;
        } else if (current->left->letter == *s) {
            strcat(encoded, "L");
            current = current->left;
        } else if (current->right->letter == *s) {
            strcat(encoded, "R");
            current = current->right;
        } else {
            if (!current->right->letter) { // right is not leaf
                strcat(encoded, "R");
                current = current->right;
            } else {
                strcat(encoded, "L");
                current = current->left;
            }
        }
  }
  encoded = realloc(encoded, strlen(encoded) + 1);
  return encoded;
}

/*
 * Accepts a Huffman tree t and an encoded string and returns a new
 * heap-allocated string contained the decoding of the code as per the tree t.
 *
 * Pre: the code given is decodable using the supplied tree t.
 */
char *huffman_tree_decode(huffman_tree_t *t, char *code) {
  char *decoded = malloc(sizeof(char) * MAX_CODE_LENGTH);
  NULLCHECK(decoded, "decode");

  strcpy(decoded, "");
  huffman_tree_t *current = t;
  int pos = 0;
  while (*code != '\0') {
    if (current->left == NULL && current->right == NULL) {
        decoded[pos++] = current->letter;
        current = t;
      } else if ('L' == *code) {
        current = current->left;
        code++;
      } else if ('R' == *code) {
        current = current->right;
        code++;
      }
  }
  decoded[pos] = current -> letter;
  return decoded;
}
