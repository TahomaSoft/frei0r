// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2024 Erik H. Beck, bacon@tahomasoft.com
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html

/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 *
 * Also see:
 *
 * https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
 *
 */


/* This class is intended to operate as a mixer, taking two inputs and
   yielding one output.

   The first input is a reference input, such as a single image or a
   stretched video of a single image.

   The second input is the video stream to operate on.

   The output is a clone of the RGB data of the second input, but with
   the alpha channel modified.

   This mixer takes the (first) reference input, such as a static background,
   and removes it from every frame in the video stream of the second
   input.

   The alpha channel on the output is based on the euclidian distance
   of the two input coordinates in 3-d RGB space. If the calculated
   distance betwen the two inputs for a given pixel is less than a
   provided (variable) threshold amount, that indicates the pixel in
   the background (reference) image is the same or similar enough to
   the operational (second) input that is part of the background to be
   removed, and the transparency is set to fully transparent via the
   alpha channel (set to 0).

   If the calcuated distance exceeds the threshold, then that pixel is
   part of the foreground image to be retained, and the transparency
   of it is set to be fully opaque (alpha channel for that pixel set
   to 255).

   The basic comparison algorithm is:

   ****
   // x is reference image
   // y is comparison image to remove reference from


   float euclidDistance(int x_r, int x_g, int x_b,  int y_r, int y_g, int y_b)
   {
   //calculating color channel differences for next steps
   float red_d = x_r - y_r;
   float green_d = x_g - y_g;
   float blue_d = x_b - y_b; 
  
   float sq_sum, dist;

   //calculating Euclidean distance
   sq_sum = pow(red_d, 2) + pow(green_d, 2) + pow (blue_d)
   dist = sqrt(sq_sum);                  
  
   return dist;
   }
   *****


*/




#include <math.h>
#include "frei0r.hpp"
#include "frei0r_math.h"

#define NBYTES 4
#define ALPHA 3

class euclid_eraser : public frei0r::mixer2
{
  
public:
  euclid_eraser(unsigned int width, unsigned int height)
  {
  }

   void update(double time,
              uint32_t* out,
              const uint32_t* in1,
              const uint32_t* in2)
  {
    uint8_t *dst = reinterpret_cast<uint8_t*>(out);
    const uint8_t *src1 = reinterpret_cast<const uint8_t*>(in1);
    const uint8_t *src2 = reinterpret_cast<const uint8_t*>(in2);

    for (unsigned int i=0; i<size; ++i)
    {
      uint8_t red_src1   = src1[0];
      uint8_t blue_src1  = src1[1];
      uint8_t green_src1 = src1[2];

      uint8_t red_src2   = src2[0];
      uint8_t blue_src2  = src2[1];
      uint8_t green_src2 = src2[2];

      for (int b=0; b<3; ++b)
	{
	  dst[b]=src2[b];
	}
      
    }
    
  }

};


frei0r::construct<euclid_eraser> plugin("euclid_eraser",
	"Erasing backgrounds with euclidian distance",
        "Erik H. Beck",
        0,2,
        F0R_COLOR_MODEL_RGBA8888);
