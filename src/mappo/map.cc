#include <iostream>
#include <vector>
#include <stdlib.h>
#include <png.h>
#include "map.h"

using namespace mappo;

void my_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
    std::vector<ui8> *p = (std::vector<ui8>*) png_get_io_ptr(png_ptr);
    p->insert(p->end(), data, data + length);
}

agg::path_storage get_path(std::vector<int> xcoords, std::vector<int> ycoords, bool close_polygon)
{
    agg::path_storage path;
    path.move_to(xcoords[0], ycoords[0]);

    int len = std::min(xcoords.size(), ycoords.size());
    for (int i = 1; i < len; ++i) {
        path.line_to(xcoords[i], ycoords[i]);
    }

    if (close_polygon)
        path.close_polygon();

    return path;
}


Map::Map(int w, int h):
    width(w),
    height(h),
    buffer(width * height * 3),
    rbuf(&buffer[0], w, h, w * 3),
    pixf(rbuf),
    rb(pixf)
{

}

Map::~Map()
{
}

void Map::clear(int r, int g, int b)
{
    rb.clear(agg::rgba8(r, g, b));
}

void Map::drawPolygon(std::vector<int> xcoords, std::vector<int> ycoords)
{
    agg::rasterizer_scanline_aa<> m_ras;
    agg::scanline_p8 m_sl_p8;

    agg::path_storage path = get_path(xcoords, ycoords, true);
    m_ras.add_path(path);

    renderer_aa ren_aa(rb);
    ren_aa.color(agg::rgba8(160, 32, 240));

    agg::render_scanlines(m_ras, m_sl_p8, ren_aa);
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
            agg::rgba8 c = pixf.pixel(x, y);
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
