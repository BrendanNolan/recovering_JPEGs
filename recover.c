// recovers JPEGs from a camera's memory card (assuing the memory card had been zeroed before the deleted photos were taken)

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "recover.h"

int main(int argc, char* argv[])
{
    //ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover rawfile\n");
        return 1;
    }

    char* infile = argv[1];

    FILE* raw_ptr = fopen(argv[1], "r");
    if (raw_ptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // declare a pointer to a block and allocate sufficient memory
    block* current_block_ptr = malloc(sizeof(block));
    // check that the memory allocation was successful
    if (current_block_ptr == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        return 3;
    }

    // store the first block of the raw file in the pointee block
    fread(current_block_ptr, sizeof(block), 1, raw_ptr);
    // keep reading blocks at the beginning of the raw file into the buffer current_block_ptr while those blocks do not
    // look like the beginning of a jpeg and the end of the file is not reached
    while (beg_jpeg(*current_block_ptr) == false && !feof(raw_ptr))
        fread(current_block_ptr, sizeof(block), 1, raw_ptr);
    // at this point, we must have beg_jpeg(*current_block_ptr == true) , so that
    // current_block_ptr points to a block which is the start of a JPEG
    // this will be the 0th image, so let's initialise an int i to 0
    int i = 0;

    while (!feof(raw_ptr))
    {
        char image_name[8];

        // naming the output JPEG according to i (allowing ourselves up to 1000 JPEGs)
        if (i < 10)
            sprintf(image_name, "00%i%s", i, ".jpg");
        else if (i < 100)
            sprintf(image_name, "0%i%s", i, ".jpg");
        else if (i < 1000)
            sprintf(image_name, "%i%s", i, ".jpg");

        FILE* image_ptr = fopen(image_name, "w");
        fwrite(current_block_ptr, sizeof(block), 1, image_ptr);
        fread(current_block_ptr, sizeof(block), 1, raw_ptr);
        // the while loop below will stop at the next JPEG or the end of the file
        while (beg_jpeg(*current_block_ptr) == false && !feof(raw_ptr))
        {
            fwrite(current_block_ptr, sizeof(block), 1, image_ptr);
            fread(current_block_ptr, sizeof(block), 1, raw_ptr);
        }
        fclose(image_ptr);
        i++;
    }

    fclose(raw_ptr);
    free(current_block_ptr);

    // success
    return 0;
}

