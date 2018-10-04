// Sun-Jung Yum
// October 04 2018
// Problem Set 3

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Remember file name
    char *infile = argv[1];

    // Open image file and ensure that it exists
    FILE *rawfile = fopen(infile, "r");
    if (rawfile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // Find size of file
    fseek(rawfile, 0, SEEK_END);
    int size = ftell(rawfile);
    fseek(rawfile, -size, SEEK_CUR);

    // Create an array to store bytes and a counter variable
    unsigned char *buffer = malloc(sizeof(unsigned char) * size);
    int count = 0;

    // Declares new image file outside of while loop scope
    FILE *newimage;

    // Scans through the file until end of file is reached
    while (fread(buffer, 1,  512, rawfile) == 512)
    {
        // JPEG header is found
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Previous file is closed if a JPEG has already been found
            if (count > 0)
            {
                fclose(newimage);
            }
            // Make a new JPEG
            char *filename = malloc(sizeof(unsigned char) * size);
            sprintf(filename, "%03i.jpg", count++);
            newimage = fopen(filename, "w");
            fwrite(buffer, 1, 512, newimage);
        }
        // JPEG header is not found
        else
        {
            // Block belongs to the opened JPEG file
            if (count > 0)
            {
                fwrite(buffer, 1, 512, newimage);
            }
        }
    }

    // Close remaining file
    fclose(newimage);

    // Success
    return 0;
}