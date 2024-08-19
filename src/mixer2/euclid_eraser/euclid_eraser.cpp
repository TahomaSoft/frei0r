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

#include<iostream>
#include <fstream>
using namespace std;

#include <math.h>
#include "frei0r.hpp"
#include "frei0r/math.h"

#define NBYTES 4
#define ALPHA 3

double euclidDistance(uint8_t x_r, uint8_t x_g, uint8_t x_b,
		      uint8_t y_r, uint8_t y_g, uint8_t y_b)
   {
   //calculating color channel differences for next steps
   double red_d = x_r - y_r;
   double green_d = x_g - y_g;
   double blue_d = x_b - y_b; 
  
   double sq_sum, dist;

   //calculating Euclidean distance
   sq_sum = pow(red_d, 2) + pow(green_d, 2) + pow (blue_d, 2);
   dist = sqrt(sq_sum);                  
  
   return dist;
   }


class euclid_eraser : public frei0r::mixer2
{
  ofstream loggingFile;
private:
  double threshold;
  
public:
  euclid_eraser(unsigned int width, unsigned int height)
  {
    threshold = 5.6;      // Default distance threshold value
    register_param(threshold, "threshold", "Matching Threshold");

   
    loggingFile.open("/home/erikbeck/euclidlog.txt");
    loggingFile << "Threshold is: " << threshold;
    loggingFile << endl;
   
  }
  ~euclid_eraser()
  {
    loggingFile.close(); 
  }
  
  void update(double time,
              uint32_t* out,
              const uint32_t* in1,
              const uint32_t* in2)
  {
    // Destination File
    // dst[0] to dst[3] is rgb
    // dst[4] is alpha channel
    loggingFile << "Threshold is: " << threshold;
    loggingFile << endl;
    uint8_t *dst = reinterpret_cast<uint8_t*>(out);

    // First source file (video track 0)
    const uint8_t *src1 = reinterpret_cast<const uint8_t*>(in1);
    
    // Second source file (video track 1)
    const uint8_t *src2 = reinterpret_cast<const uint8_t*>(in2);
    
    /* Note from MLT Framework Documentation:

       "by default, the higher numbered track takes precedence over
       the lower numbered track)."

     https://www.mltframework.org/docs/framework/
     
    */

    double e_dist;

    
    for (unsigned int i=0; i<size; ++i)
      {
	uint8_t red_src1    = src1[0];
	uint8_t green_src1  = src1[1];
	uint8_t blue_src1   = src1[2];
	
	uint8_t red_src2    = src2[0];
	uint8_t green_src2  = src2[1];
	uint8_t blue_src2   = src2[2];

	// Loop over rgb
	// Copy pixels from src2 to destination

	
	  
	for (int b=0; b<3; ++b)
	  {
	    dst[b]=src2[b];
	  }

	
	e_dist=euclidDistance(red_src1, green_src1, blue_src1,
			      red_src2, green_src2, blue_src2);

        loggingFile << "Euclid Distance is: " << e_dist;
	loggingFile << endl;
	if (e_dist <=  euclid_eraser::threshold) {
	    // Make alpha channel for pixel fully transparent
	    dst[4]=0;
	  }
	else {
	  // Make alpha channel for the pixel fully opaque
	  dst[4]=255;
	}
	
      }
    
  }

  
};


frei0r::construct<euclid_eraser> plugin("euclid_eraser",
	"Erasing backgrounds with euclidian distance",
        "Erik H. Beck",
        0,1,
        F0R_COLOR_MODEL_RGBA8888);

