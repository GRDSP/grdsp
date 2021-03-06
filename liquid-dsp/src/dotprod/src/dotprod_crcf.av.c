/*
 * Copyright (c) 2007 - 2014 Joseph Gaeddert
 *
 * This file is part of liquid.
 *
 * liquid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid.  If not, see <http://www.gnu.org/licenses/>.
 */

// 
// Complex floating-point dot product (altivec velocity engine)
//

#include <stdio.h>
#include <stdlib.h>

#include "liquid.internal.h"

#define DEBUG_DOTPROD_CRCF_AV   0

// basic dot product

// basic dot product
//  _h      :   coefficients array [size: 1 x _n]
//  _x      :   input array [size: 1 x _n]
//  _n      :   input lengths
//  _y      :   output dot product
void dotprod_crcf_run(float *         _h,
                      float complex * _x,
                      unsigned int    _n,
                      float complex * _y)
{
    float complex r=0;
    unsigned int i;
    for (i=0; i<_n; i++)
        r += _h[i] * _x[i];
    *_y = r;
}

// basic dot product, unrolling loop
//  _h      :   coefficients array [size: 1 x _n]
//  _x      :   input array [size: 1 x _n]
//  _n      :   input lengths
//  _y      :   output dot product
void dotprod_crcf_run4(float *         _h,
                       float complex * _x,
                       unsigned int    _n,
                       float complex * _y)
{
    float complex r=0;

    // t = 4*(floor(_n/4))
    unsigned int t=(_n>>2)<<2; 

    // compute dotprod in groups of 4
    unsigned int i;
    for (i=0; i<t; i+=4) {
        r += _h[i]   * _x[i];
        r += _h[i+1] * _x[i+1];
        r += _h[i+2] * _x[i+2];
        r += _h[i+3] * _x[i+3];
    }

    // clean up remaining
    for ( ; i<_n; i++)
        r += _h[i] * _x[i];

    *_y = r;
}


//
// structured dot product
//

struct dotprod_crcf_s {
    // dotprod length (number of coefficients)
    unsigned int n;

    // coefficients arrays: the altivec velocity engine operates
    // on 128-bit registers which can hold four 32-bit floating-
    // point values.  We need to hold 4 copies of the coefficients
    // to meet all possible alignments to the input data.
    float *h[4];
};

// create the structured dotprod object
dotprod_crcf dotprod_crcf_create(float *      _h,
                                 unsigned int _n)
{
    dotprod_crcf dp = (dotprod_crcf)malloc(sizeof(struct dotprod_crcf_s));
    dp->n = _n;

    // create 4 copies of the input coefficients (one for each
    // data alignment).  For example: _h[4] = {1,2,3,4,5,6}
    //  dp->h[0] = {1,1,2,2,3,3,4,4,5,5,6,6}
    //  dp->h[1] = {. 1,1,2,2,3,3,4,4,5,5,6,6}
    //  dp->h[2] = {. . 1,1,2,2,3,3,4,4,5,5,6,6}
    //  dp->h[3] = {. . . 1,1,2,2,3,3,4,4,5,5,6,6}
    // NOTE: double allocation size; coefficients are real, but
    //       need to be multiplied by real and complex components
    //       of input.
    unsigned int i,j;
    for (i=0; i<4; i++) {
        dp->h[i] = calloc(1+(2*dp->n+i-1)/4,2*sizeof(vector float));
        for (j=0; j<dp->n; j++) {
            dp->h[i][2*j+0+i] = _h[j];
            dp->h[i][2*j+1+i] = _h[j];
        }
    }

    return dp;
}

// re-create the structured dotprod object
dotprod_crcf dotprod_crcf_recreate(dotprod_crcf _q,
                                   float *      _h,
                                   unsigned int _n)
{
    // completely destroy and re-create dotprod object
    dotprod_crcf_destroy(_q);
    return dotprod_crcf_create(_h,_n);
}

// destroy the structured dotprod object
void dotprod_crcf_destroy(dotprod_crcf _q)
{
    // clean up coefficients arrays
    unsigned int i;
    for (i=0; i<4; i++)
        free(_q->h[i]);

    // free allocated object memory
    free(_q);
}

// print the dotprod object
void dotprod_crcf_print(dotprod_crcf _q)
{
    printf("dotprod_crcf [altivec, %u coefficients]:\n", _q->n);
    unsigned int i;
    for (i=0; i<_q->n; i++)
        printf("  %3u : %12.9f\n", i, _q->h[0][2*i]);
}

// exectue vectorized structured inner dot product
void dotprod_crcf_execute(dotprod_crcf    _q,
                          float complex * _x,
                          float complex * _r)
{
    int al; // input data alignment

    vector float *ar,*d;
    vector float s0,s1,s2,s3;
    union { vector float v; float w[4];} s;
    unsigned int nblocks;

    ar = (vector float*)( (int)_x & ~15);
    al = ((int)_x & 15)/sizeof(float);

    d = (vector float*)_q->h[al];

    // number of blocks doubles because of complex type
    nblocks = (2*_q->n + al - 1)/4 + 1;

    // split into four vectors each with four 32-bit
    // partial sums.  Effectively each loop iteration
    // operates on 16 input samples at a time.
    s0 = s1 = s2 = s3 = (vector float)(0);
    while (nblocks >= 4) {
        s0 = vec_madd(ar[nblocks-1],d[nblocks-1],s0);
        s1 = vec_madd(ar[nblocks-2],d[nblocks-2],s1);
        s2 = vec_madd(ar[nblocks-3],d[nblocks-3],s2);
        s3 = vec_madd(ar[nblocks-4],d[nblocks-4],s3);
        nblocks -= 4;
    }

    // fold the resulting partial sums into vector s0
    s0 = vec_add(s0,s1);    // s0 = s0+s1
    s2 = vec_add(s2,s3);    // s2 = s2+s3
    s0 = vec_add(s0,s2);    // s0 = s0+s2

    // finish partial summing operations
    while (nblocks-- > 0)
        s0 = vec_madd(ar[nblocks],d[nblocks],s0);

    // move the result into the union s (effetively,
    // this loads the four 32-bit values in s0 into
    // the array w).
    s.v = vec_add(s0,(vector float)(0));

    // sum the resulting array
    //*_r = s.w[0] + s.w[1] + s.w[2] + s.w[3];
    *_r = (s.w[0] + s.w[2]) + (s.w[1] + s.w[3]) * _Complex_I;
}

