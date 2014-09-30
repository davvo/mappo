#ifndef MAPPO_MAP_H
#define MAPPO_MAP_H

#include <vector>
#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgb.h"

namespace mappo
{
    class Map
    {
        public:
            Map(int width, int height);
            ~Map();
            void clear(int r, int g, int b);
            bool writePPM(const char* filename);
            std::vector<unsigned char>* writePNG();

        private:
            int width;
            int height;
            unsigned char* buffer;
            agg::rendering_buffer* rbuf;
            agg::pixfmt_rgb24* pixf;
    };
}

#endif