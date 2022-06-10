#include "layer.h"

/* The sigmoid function and derivative. */
double sigmoid(double x)
{
  /**** PART 1 - QUESTION 1 ****/
  
return 1 / (1 + exp(-x));
  /* 1 MARK */
}

double sigmoidprime(double x)
{
  return x*(1 - x);
}

/* Creates a single layer. */
layer_t *layer_create()
{
  /**** PART 1 - QUESTION 2 ****/
  return (layer_t *) calloc(1, sizeof(layer_t));
  /* 2 MARKS */
}

/* Initialises the given layer. */
bool layer_init(layer_t *layer, int num_outputs, layer_t *prev)
{
  /**** PART 1 - QUESTION 3 ****/
  layer -> num_outputs = num_outputs;
  layer -> prev = prev;
  layer -> outputs = (double *) calloc(num_outputs, sizeof(double));

  if (!layer-> outputs) {
    perror("Memory Allocation error");
    return true;
  }

  if (prev) { //not input layer:
    layer -> num_inputs = prev -> num_outputs;

    //array allocation
    layer -> biases = (double *) calloc(num_outputs, sizeof(double));
    layer -> deltas = (double *) calloc(num_outputs, sizeof(double));
    layer -> weights = (double **) calloc(layer -> num_inputs, sizeof(double *));

    if (!layer -> biases || !layer -> deltas || !layer -> weights) {
      perror("Memory Allocation error");
      return true;
    }

    for (int i = 0; i < layer -> num_inputs; i++) {
      layer -> weights[i] = (double *) malloc(num_outputs * sizeof(double));
      if (!layer -> weights[i]) {
        perror("Memory Allocation error!");
        return true;

        for (int j = 0; j < num_outputs; j++) {
          layer -> weights[i][j] = ANN_RANDOM();
        }

      }
    }

    (layer -> prev) -> next = layer;
  }
  return false;
  /* 4 MARKS */
}

/* Frees a given layer. */
void layer_free(layer_t *layer)
{
  /**** PART 1 - QUESTION 4 ****/
  free(layer -> outputs);
  if (layer -> prev) { //not first layer!
    free(layer -> deltas);
    free(layer -> biases);
    for (int i = 0; i < layer -> num_inputs; i++) {
      free(layer -> weights[i]);
    }
    free(layer -> weights);
  }
  free(layer);
  /* 2 MARKS */
}

/* Computes the outputs of the current layer. */
void layer_compute_outputs(layer_t const *layer)
{
  /**** PART 1 - QUESTION 5 ****/
  /* objective: compute layer->outputs */
  if (!layer -> prev) {
    perror ("Cannot caluclate outputs on the input layer");
    return;
  }

  for (int j=0; j < layer -> num_outputs; j++) {
    double acc = 0;
    for (int i=0; i < layer -> num_inputs; i++) {
    acc += layer -> weights[i][j] * layer -> prev -> outputs[i];
  }
    layer -> outputs[j] = sigmoid(layer -> biases[j] + acc);
  }

  /* 3 MARKS */
}

/* Computes the delta errors for this layer. */
void layer_compute_deltas(layer_t const *layer)
{
  /**** PART 1 - QUESTION 6 ****/
  /* objective: compute layer->deltas */
  for (int i = 0; i < layer -> num_outputs; i++) {
    double acc = 0;
    for (int j = 0; j < (layer -> next) -> num_outputs; j++) {
      acc += layer -> next -> weights[i][j] * layer -> next -> deltas[j]; 
    }
    layer -> deltas[i] = sigmoidprime(layer -> outputs[i]) * acc;
  }

  /* 2 MARKS */
}

/* Updates weights and biases according to the delta errors given learning rate. */
void layer_update(layer_t const *layer, double l_rate)
{
  /**** PART 1 - QUESTION 7 ****/
  /* objective: update layer->weights and layer->biases */
  for (int j = 0; j < layer -> num_outputs; j++) {
    for (int i = 0; i < layer -> num_inputs; i++) {
      layer -> weights[i][j] = layer -> weights [i][j] + l_rate 
        * layer -> prev -> outputs[i]
        * layer -> deltas[j];
    }
  layer -> biases[j] = layer -> biases[j] +  l_rate * layer -> deltas[j];
  }
  /* 1 MARK */
}
