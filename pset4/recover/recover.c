#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Cannot read image.\n");
        return 1;
    }

    int count = 0, num_bytes = 512;
    bool keep = false;
    FILE *img;
    while (num_bytes == 512)
    {
        BYTE buffer[512];
        num_bytes = fread(buffer, sizeof(BYTE), 512, card);
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && (buffer[3] & 0xF0) == 0xE0)
        {
            if (keep)
            {
                fclose(img);
                keep = false;
            }
            if (!keep)
            {
                char filename[8];
                sprintf(filename, "%03i.jpg", count);
                count++;

                img = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), num_bytes, img);
                keep = true;
            }
        }
        else if (keep)
        {
            fwrite(buffer, sizeof(BYTE), num_bytes, img);
        }
    }
    fclose(card);
}