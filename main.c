//
//  main.c
//  Raycasting
//  Junzhe Wang jw2323
//	jwa2323@nau.edu
//  2017/10/11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raycast.h"


int main(int argc, const char * argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Error: main: You must have 4 arguments\n");
        exit(1);
    }
    if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0) {
        fprintf(stderr, "Error: main: width and height parameters must be > 0\n");
        exit(1);
    }
    // Check the imput argument
	
	
    /*open the input CSV file*/
    const char *input = argv[3];
    //Error Check
    if (input == NULL) {
        fprintf(stderr, "Error: main: Failed to open input file '%s'\n", argv[3]);
        exit(1);
    }
    read_scene(input);

    Image *image =(Image *)malloc(sizeof(Image));
    //create an image
   
    image->width = atoi(argv[1]);
    // image width 
    image->height = atoi(argv[2]);
    // image height
    image->maxval = 255;
    image->data =(unsigned char*) malloc(sizeof(unsigned char) * image->width * image->height*4);

    
    int pos = get_camera(objects);
    //printf("get_camera function finished work\n");
    raycast_scene(image, objects[pos].data.camera.width, objects[pos].data.camera.height, objects);
    
    const char *output = argv[4];
    //create_ppm(output, 6, &image);
    ImageWrite(image, output,6);
    return 0;
}
