#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "image.h"
#include "dragon.h"

/* x, y: coordinates of turtle */
static long x, y;

/* When drawing a pixel to the image, x and y must be divided by this value.
 * This enables both the dragon curve and twin dragon to rendered without
 * clipping.
 */
static long scale;

/* drawn_pixels: the length of the path travelled. */
static long drawn_pixels;

/* direction: the current direction of the turtle. */
static vector_t direction;

/* Returns a vector that describes the initial direction of the turtle. Each
 * iteration corresponds to a 45 degree rotation of the turtle anti-clockwise.  */
vector_t starting_direction(int total_iterations)
{
  switch (total_iterations % 8) {
	case 1:
		direction.dx = 1;
		direction.dy = 1;
		break;
	case 2:
		direction.dx = 0;
		direction.dy = 1;
		break;
	case 3:
		direction.dx = -1;
		direction.dy = 1;
		break;
	case 4:
		direction.dx = -1;
		direction.dy = 0;
		break;
	case 5:
		direction.dx = -1;
		direction.dy = -1;
		break;
	case 6:
		direction.dx = 0;
		direction.dy = -1;
		break;
	case 7:
		direction.dx = 1;
		direction.dy = -1;
		break;
	case 0:
	default:
		direction.dx = 1;
		direction.dy = 0;
    }
	return direction;
}

/* Draws a pixel to dst at location (x, y). The pixel intensity is chosen as a
 * function of image size and the number of pixels drawn.
 *
 * The gray level changes over entire size of path; the pixel value oscillates
 * along the path to give some contrast between segments close to each other
 * spatially.
 */
void draw_greyscale(image_t *dst,long x, long y)
{
	int level = LEVEL * drawn_pixels / (dst -> height * dst -> height);
	int greyScale;
	switch (level) {
		case 0: 
			greyScale = 100;
			break;
		case 1:
			greyScale = 120;
			break;
		case 2:
			greyScale = 150;
			break;
		case 3:
			greyScale = 180;
			break;
		case 4:
			greyScale = 200;
			break;
		default:
			greyScale = 255;
	}
	set_pixel(dst, x, y, greyScale);

}
/* Iterates though the characters of str, recusively calling string_iteration()
 * until rules have been applied iterations times, or no rule is applicable.
 * and update the image.
 */
void string_iteration(image_t *dst, const char *str, int iterations)
{
  if (iterations < 0) {
	return;
  }
  for (; *str != '\0'; str++) { //*str = head of str
  	long temp;
	switch (*str) {
		case 'X':
			//string_iteration(dst, "X+YF+", iterations - 1);
            string_iteration(dst, "X+YF", iterations - 1);
			break;
		case 'Y':
			//string_iteration(dst, "-FX-Y", iterations - 1);
            string_iteration(dst, "FX-Y", iterations - 1);
			break;
		case '-': // rotate anti-clockwise 90 degrees
			temp = direction.dx;
			direction.dx = -direction.dy;
			direction.dy = temp;
			break;
		case '+': // rotate clockwise 90 degrees
			temp = direction.dx;
			direction.dx = direction.dy;
			direction.dy = -temp;
			break;
		case 'F':
			drawn_pixels++;
			draw_greyscale(dst, x / scale, y / scale);
			x += direction.dx;
			y += direction.dy;
			break;
	}
  }	

}

/* Creates an image of requested size, calls starting_direction() to compute
 * initial turtle direction then calls string_iteration() to construct the
 * image. The constructed image is saved to a file in the output directory.
 */
void dragon(long size, int total_iterations)
{
	image_t **dst = malloc(sizeof(image_t *));
	image_error_t err = init_image(dst, 1.5 * size, size, 1, 255);
	if(err != IMG_OK) {
		image_print_error(err);
		exit(EXIT_FAILURE);
	}
	char *axiom = "FX+FX+";
    char *filename = "../output/twindragon.pgm";
    x = size;
    y = size;
    scale = 2;
/*
     * x = size/3;
     * y = size/3;
     * scale = 1;
     * char *axiom = "FX";
     * char *filename = "../output/jurassicdragon.pgm";
    */

	direction = starting_direction(total_iterations);
	string_iteration(*dst, axiom, total_iterations);
	err = image_write(filename, *dst,PBM_FORMAT);
	if(err != IMG_OK) {   //could be made a macro
		image_print_error(err);
		exit(EXIT_FAILURE);
	}
	image_free(*dst);
	free(dst);
}

/* The main function. When called with an argument, this should be considered
 * the number of iterations to execute. Otherwise, it is assumed to be 9. Image
 * size is computed from the number of iterations then dragon() is used to
 * generate the dragon image. */
int main(int argc, char **argv)
{
	int iter;
	switch (argc) {
		case 1:
			iter = 9;
			break;
		case 2:
			iter = atoi(argv[1]);
			break;
		default:
			perror("Wrong number of arguments given");
			exit(EXIT_FAILURE);
	}

	dragon(pow(2, iter), 2 * iter);
	return EXIT_SUCCESS;




}
