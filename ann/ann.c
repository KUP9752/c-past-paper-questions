#include "ann.h"

/* Creates and returns a new ann. */
ann_t *ann_create(int num_layers, int *layer_outputs)
{
  /**** PART 2 - QUESTION 1 ****/
  ann_t *ann = (ann_t *) malloc(sizeof(ann_t));
  if (!ann) {
    perror("Memory Allocation Error");
    return NULL;
  }

  ann -> input_layer = layer_create();
  layer_init(ann -> input_layer, layer_outputs[0], NULL);
  layer_t *prev = ann -> input_layer;

  for (int layer = 1; layer < num_layers; layer++) {
    layer_t *current = layer_create();
    if (!current || layer_init(current, layer_outputs[layer], prev)){
      perror("Memory Allocation error");
      return NULL;
    }
    prev = current;
  }
  
  ann -> output_layer = prev;
  return ann;
  /* 4 MARKS */
}

/* Frees the space allocated to ann. */
void ann_free(ann_t *ann)
{
  /**** PART 2 - QUESTION 2 ****/
  layer_t *prev;
  layer_t *current = ann -> input_layer;
  while (current != ann->output_layer) {
      prev = current;
      current = current -> next;
      layer_free(prev);
  }
  layer_free(current); //current is now outputlayer.
  free(ann);
  /* 2 MARKS */
}

/* Forward run of given ann with inputs. */
void ann_predict(ann_t const *ann, double const *inputs)
{
  /**** PART 2 - QUESTION 3 ****/
  for (int i = 0; i < ann -> input_layer -> num_outputs; i++) {
    ann -> input_layer -> outputs[i] = inputs[i];
  }
  
  layer_t *current = ann -> input_layer;
  while (current != ann-> output_layer) {
    layer_compute_outputs(current -> next);
    current = current -> next;
  }
  /* 2 MARKS */
}

/* Trains the ann with single backprop update. */
void ann_train(ann_t const *ann, double const *inputs, double const *targets, double l_rate)
{
  /* Sanity checks. */
  assert(ann != NULL);
  assert(inputs != NULL);
  assert(targets != NULL);
  assert(l_rate > 0);

  /* Run forward pass. */
  ann_predict(ann, inputs);

  /**** PART 2 - QUESTION 4 ****/
  layer_t *outLayer = ann -> output_layer;
  for (int j = 0; j < outLayer -> num_outputs; j++) {
    outLayer -> deltas[j] = sigmoidprime(outLayer -> outputs[j])
      * (targets[j] - outLayer -> outputs[j]);
  }

  layer_t *current = outLayer -> prev;
  while (current != ann -> input_layer) {
    layer_compute_deltas(current);
    current = current -> prev;
  }
  // current = input_layer at this point

  while (current != ann -> output_layer) {
    current = current -> next;
    layer_update(current, l_rate);
  }


  /* 3 MARKS */
}
