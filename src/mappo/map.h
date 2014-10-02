#ifndef MAPPO_MAP_H
#define MAPPO_MAP_H

#include <vector>
#include "agg_rendering_buffer.h"
#include "agg_basics.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_stroke.h"

#define AGG_RGB24
#include "pixel_formats.h"

typedef unsigned char ui8;
typedef agg::renderer_base<pixfmt> renderer_base;
typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_aa;

namespace mappo
{
    class Map
    {
        public:
            Map(int width, int height);
            ~Map();
            void clear(int r, int g, int b);
            void drawPolygon(std::vector<int> xcoords, std::vector<int> ycoords);
            std::vector<unsigned char>* writePNG();

        private:
            int width;
            int height;
            std::vector<agg::int8u> buffer;
            agg::rendering_buffer rbuf;
            pixfmt pixf;
            renderer_base rb;
    };
}

#endif