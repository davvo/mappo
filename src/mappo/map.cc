#include <iostream>
#include <vector>
#include <stdlib.h>
#include <png.h>
#include "map.h"

using namespace mappo;

typedef unsigned char ui8;

void my_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
    std::vector<ui8> *p = (std::vector<ui8>*) png_get_io_ptr(png_ptr);
    p->insert(p->end(), data, data + length);
}

Map::Map(int w, int h)
{
    width = w;
    height = h;
    buffer = new unsigned char[width * height * 3];

    rbuf = new agg::rendering_buffer(buffer, width, height, width * 3);
    pixf = new agg::pixfmt_rgb24(*rbuf);
}

Map::~Map()
{
    delete pixf;
    delete rbuf;
    delete buffer;
}

void Map::clear(int r, int g, int b)
{
    agg::rgba8 c(r,g,b);
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            pixf->copy_pixel(x, y, c);
        }
    }
}

std::vector<ui8>* Map::writePNG()
{
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep row;

    // Initialize write structure
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        fprintf(stderr, "Could not allocate write struct\n");
        return NULL;
    }

    // Initialize info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        fprintf(stderr, "Could not allocate info struct\n");
        return NULL;
    }

    std::vector<ui8>* state = new std::vector<ui8>(0);

    //png_init_io(png_ptr, fp);
    png_set_write_fn(png_ptr, state, my_png_write_data, NULL);

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, width, height,
        8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for one row (3 bytes per pixel - RGB)
    row = (png_bytep) malloc(3 * width * sizeof(png_byte));

    // Write image data
    for (int x = 0; x < width; ++x)
    {
        int i = 0;
        for (int y = 0; y < height; ++y)
        {
            agg::rgba8 c = pixf->pixel(x, y);
            row[i++] = c.r;
            row[i++] = c.g;
            row[i++] = c.b;
        }
        png_write_row(png_ptr, row);
    }

    // End write
    png_write_end(png_ptr, NULL);

    if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
    if (row != NULL) free(row);

    return state;
}

bool Map::writePPM(const char* filename)
{
    FILE* fd = fopen(filename, "wb");
    if(fd)
    {
        fprintf(fd, "P6 %d %d 255 ", width, height);
        fwrite(buffer, 1, width * height * 3, fd);
        fclose(fd);
        return true;
    }
    return false;
}