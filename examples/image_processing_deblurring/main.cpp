/*

PICCANTE Examples
The hottest examples of Piccante:
http://vcg.isti.cnr.it/piccante

Copyright (C) 2014
Visual Computing Laboratory - ISTI CNR
http://vcg.isti.cnr.it
First author: Francesco Banterle

This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3.0 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    See the GNU Lesser General Public License
    ( http://www.gnu.org/licenses/lgpl-3.0.html ) for more details.
*/

//This means that OpenGL acceleration layer is disabled
#define PIC_DISABLE_OPENGL

#include "../common_code/image_qimage_interop.hpp"

#include "piccante.hpp"

int main(int argc, char *argv[])
{
    std::string img_str;

    if(argc == 2) {
        img_str = argv[1];
    } else {
        img_str = "../data/input/bottles.hdr";
    }

    printf("Reading an HDR file...");

    pic::Image img;
    ImageRead(img_str, &img);

    printf("Ok\n");

    printf("Is it valid? ");
    if(img.isValid()) {
        printf("Ok\n");

        printf("Filtering the image with a PSF read from file...");
        pic::Image psf;

        ImageRead("../data/input/kernel_psf.png", &psf);

        //normalization of the PSF
        psf /= psf.getSumVal(NULL, NULL)[0];

        pic::Image *conv = pic::FilterConv2D::Execute(&img, &psf, NULL);
        ImageWrite(conv, "../data/output/bottles_conv_kernel_psf.hdr");

        printf("Ok!\n");

        printf("Deconvolving the image with the PSF read from file...");
        pic::Image *deconv = pic::computeRichardsonLucyDeconvolution(conv, &psf, 1000, NULL);

        printf("Ok!\n");

        printf("Writing the file to disk...");
        bool bWritten = ImageWrite(deconv, "../data/output/bottles_deconv_kernel_psf.hdr");

        if(bWritten) {
            printf("Ok\n");
        } else {
            printf("Writing had some issues!\n");
        }
    } else {
        printf("No, the file is not valid!\n");
    }

    return 0;
}
