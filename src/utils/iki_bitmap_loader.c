#include "iki_bitmap_loader.h"
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct iki_bitmap_data_t
{
	int32_t width;
	int32_t height;
	uint32_t bitCount;
	uint8_t error;
	iki_color* colorMemory;
} iki_bitmap_data;

void IkiLoadBitmap(const char* filename, iki_bitmap_data** bitmapData)
{
    iki_bitmap_data * newBitmapData = (iki_bitmap_data * )malloc(sizeof(iki_bitmap_data));

    if (newBitmapData)
    {
	    FILE * file = 0;
	    errno_t error;
	    BITMAPFILEHEADER bmpFileHeader = { 0 };
	    BITMAPINFOHEADER bmpInfoHeader = { 0 };

        error = fopen_s(&file, filename, "rb");
        if (file)
        {
            if (error != 0)
            {
                newBitmapData->error = true;
                fclose(file);
                return;
            };

            fread_s(&bmpFileHeader,
                    sizeof(bmpFileHeader),
                    sizeof(bmpFileHeader),
                    1,
                    file);

            fread_s(&bmpInfoHeader,
                    sizeof(bmpInfoHeader),
                    sizeof(bmpInfoHeader),
                    1,
                    file);

            newBitmapData->width = (int32_t)bmpInfoHeader.biWidth;

            int yMove = 0;
            int yStart = 0;
            int yEnd = 0;

            // If biHeight is negative the bitmap is a top-down DIB with the origin at the upper left corner.
            if (bmpInfoHeader.biHeight < 0)
            {
                newBitmapData->height = (int32_t)bmpInfoHeader.biHeight * (-1);
                yMove = 1;
                yStart = 0;
                yEnd = newBitmapData->height;
            }
            // if biHeight is positive the bitmap is a bottom-up DIB with the origin at the lower left corner
            else
            {
                newBitmapData->height = (int32_t)bmpInfoHeader.biHeight;
                yMove = -1;
                yStart = newBitmapData->height - 1;
                yEnd = 0;
            }

            // Allocate memory for pixel data
            int colorMemorySize = newBitmapData->width * newBitmapData->height;
            newBitmapData->colorMemory = (iki_color*)malloc(sizeof(iki_color) * colorMemorySize);

            // Move file pointer to the beginning of the pixel data
            fseek(file, bmpFileHeader.bfOffBits, SEEK_SET);

            // Memory padding
            // Get 3 from (r,g,b,a) but minus the a.
            int32_t width_in_bytes = 3 * newBitmapData->width;
            int32_t padding = (4 - width_in_bytes % 4) % 4; // Magic formula for padding

            // Load pixel data into memory
            for (int y = yStart; y != yEnd; y += yMove)
            {
                for (int x = 0; x < newBitmapData->width; ++x)
                {
                    iki_color color;
                    fread(&color.b, sizeof(uint8_t), 1, file);
                    fread(&color.g, sizeof(uint8_t), 1, file);
                    fread(&color.r, sizeof(uint8_t), 1, file);
                    color.a = 255;

                    // Fill each pixel with color
                    int pixelLocation = newBitmapData->width * y + x;
                    newBitmapData->colorMemory[pixelLocation] = color;

                    if (newBitmapData->bitCount == 32)
                    {
                        fseek(file, 1, SEEK_CUR);
                    }
                }

                if (newBitmapData->bitCount == 24)
                {
                    fseek(file, padding, SEEK_CUR);
                }
            }

            fclose(file);

            *bitmapData = newBitmapData;
        }
    }
    else
    {
        *bitmapData = 0;
    }
}

int32_t IkiGetBitmapWidth(const iki_bitmap_data* bitmapData)
{
    return bitmapData->width;;
}

int32_t IkiGetBitmapHeight(const iki_bitmap_data* bitmapData)
{
    return bitmapData->height;
}

iki_color IkiGetBitmapData(const iki_bitmap_data* bitmapData, int32_t x, int32_t y)
{
    iki_color color = bitmapData->colorMemory[y * bitmapData->width + x];
    return color;
}

void IkiFreeBitmap(iki_bitmap_data** bitmapData)
{
    iki_bitmap_data * data = *bitmapData;

    if (data)
    {
        int colorMemorySize = (data->width * data->height) * sizeof(iki_color);
        if (data->colorMemory)
        {
            free(data->colorMemory);
            data->colorMemory = 0;

            free(data);
            data = 0;
        }
    }
}
