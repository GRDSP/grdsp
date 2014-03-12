//
// flexframesync_example.c
//
// This example demonstrates the basic interface to the flexframegen and
// flexframesync objects used to completely encapsulate raw data bytes
// into frame samples (nearly) ready for over-the-air transmission. A
// 14-byte header and variable length payload are encoded into baseband
// symbols using the flexframegen object.  The resulting symbols are
// interpolated using a root-Nyquist filter and the resulting samples are
// then fed into the flexframesync object which attempts to decode the
// frame. Whenever frame is found and properly decoded, its callback
// function is invoked.
//
// SEE ALSO: flexframesync_reconfig_example.c
//           framesync64_example.c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <getopt.h>
#include <assert.h>

#include "liquid.h"

void usage()
{
    printf("ofdmflexframesync_example [options]\n");
    printf("  u/h   : print usage\n");
    printf("  s     : signal-to-noise ratio [dB], default: 20\n");
    printf("  F     : carrier frequency offset, default: 0.01\n");
    printf("  n     : payload length [bytes], default: 120\n");
    printf("  m     : modulation scheme (qpsk default)\n");
    liquid_print_modulation_schemes();
    printf("  v     : data integrity check: crc32 default\n");
    liquid_print_crc_schemes();
    printf("  c     : coding scheme (inner): h74 default\n");
    printf("  k     : coding scheme (outer): none default\n");
    liquid_print_fec_schemes();
    printf("  d     : enable debugging\n");
}

// flexframesync callback function
static int callback(unsigned char *  _header,
                    int              _header_valid,
                    unsigned char *  _payload,
                    unsigned int     _payload_len,
                    int              _payload_valid,
                    framesyncstats_s _stats,
                    void *           _userdata);

int main(int argc, char *argv[])
{
    srand( time(NULL) );

    // options
    modulation_scheme ms     =  LIQUID_MODEM_QPSK; // mod. scheme
    crc_scheme check         =  LIQUID_CRC_32;     // data validity check
    fec_scheme fec0          =  LIQUID_FEC_NONE;   // fec (inner)
    fec_scheme fec1          =  LIQUID_FEC_NONE;   // fec (outer)
    unsigned int payload_len =  120;               // payload length
    int debug_enabled        =  0;                 // enable debugging?
    float noise_floor        = -60.0f;             // noise floor
    float SNRdB              =  20.0f;             // signal-to-noise ratio
    float dphi               =  0.01f;             // carrier frequency offset

    // get options
    int dopt;
    while((dopt = getopt(argc,argv,"uhs:F:n:m:v:c:k:d")) != EOF){
        switch (dopt) {
        case 'u':
        case 'h': usage();                                       return 0;
        case 's': SNRdB         = atof(optarg);                  break;
        case 'F': dphi          = atof(optarg);                  break;
        case 'n': payload_len   = atol(optarg);                  break;
        case 'm': ms            = liquid_getopt_str2mod(optarg); break;
        case 'v': check         = liquid_getopt_str2crc(optarg); break;
        case 'c': fec0          = liquid_getopt_str2fec(optarg); break;
        case 'k': fec1          = liquid_getopt_str2fec(optarg); break;
        case 'd': debug_enabled = 1;                             break;
        default:
            exit(-1);
        }
    }

    // derived values
    unsigned int i;
    float nstd  = powf(10.0f, noise_floor/20.0f);         // noise std. dev.
    float gamma = powf(10.0f, (SNRdB+noise_floor)/20.0f); // channel gain

    // create flexframegen object
    flexframegenprops_s fgprops;
    flexframegenprops_init_default(&fgprops);
    fgprops.mod_scheme  = ms;
    fgprops.check       = check;
    fgprops.fec0        = fec0;
    fgprops.fec1        = fec1;
    flexframegen fg = flexframegen_create(&fgprops);

    // frame data (header and payload)
    unsigned char header[14];
    unsigned char payload[payload_len];

    // create flexframesync object
    flexframesync fs = flexframesync_create(callback,NULL);
    flexframesync_print(fs);
    if (debug_enabled)
        flexframesync_debug_enable(fs);

    // initialize header, payload
    for (i=0; i<14; i++)
        header[i] = i;
    for (i=0; i<payload_len; i++)
        payload[i] = rand() & 0xff;

    // assemble the frame
    flexframegen_assemble(fg, header, payload, payload_len);
    flexframegen_print(fg);

    // generate the frame
    unsigned int frame_len = flexframegen_getframelen(fg);
    unsigned int num_samples = frame_len + 100;
    printf("frame length : %u samples\n", frame_len);
    float complex x[num_samples];
    float complex y[num_samples];

    int frame_complete = 0;
    unsigned int n;
    for (n=0; n<50; n++)
        x[n] = 0.0f;
    while (!frame_complete) {
        //printf("assert %6u < %6u\n", n, num_samples);
        assert(n < num_samples);
        frame_complete = flexframegen_write_samples(fg, &x[n]);
        n += 2;
    }
    for (; n<num_samples; n++)
        x[n] = 0.0f;
    assert(n == num_samples);

    // add noise and push through synchronizer
    for (i=0; i<num_samples; i++) {
        // apply channel gain and carrier offset to input
        y[i] = gamma * x[i] * cexpf(_Complex_I*dphi*i);

        // add noise
        y[i] += nstd*( randnf() + _Complex_I*randnf())*M_SQRT1_2;
    }

    // run through frame synchronizer
    flexframesync_execute(fs, y, num_samples);

    // export debugging file
    if (debug_enabled)
        flexframesync_debug_print(fs, "flexframesync_debug.m");

    // destroy allocated objects
    flexframegen_destroy(fg);
    flexframesync_destroy(fs);

    printf("done.\n");
    return 0;
}

static int callback(unsigned char *  _header,
                    int              _header_valid,
                    unsigned char *  _payload,
                    unsigned int     _payload_len,
                    int              _payload_valid,
                    framesyncstats_s _stats,
                    void *           _userdata)
{
    printf("******** callback invoked\n");

    framesyncstats_print(&_stats);
    printf("    header crc          :   %s\n", _header_valid ?  "pass" : "FAIL");
    printf("    payload length      :   %u\n", _payload_len);
    printf("    payload crc         :   %s\n", _payload_valid ?  "pass" : "FAIL");

    return 0;
}

