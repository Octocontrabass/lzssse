#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lzss.h"

static const uint8_t title[21] = "SIMEARTH             ";

typedef struct patch_t
{
    uint_fast32_t offset;
    uint_fast8_t shift;
    uint_fast8_t block;
} patch_t;

#define patch_count (3*61)

static const patch_t patch[3][patch_count] =
{
    {
        { 0x5f00, 0, 1 }
    },
    {
        { 0x5f88, 0, 1 }
    },
    {
        { 0x04e5, 0,  0 }, { 0x04e6, 8,  0 }, { 0x04e8, 16,  0 },
        { 0x0508, 0,  0 }, { 0x0509, 8,  0 }, { 0x050b, 16,  0 },
        { 0x0c1c, 0, 30 }, { 0x0c1d, 8, 30 },
        { 0x0c1e, 0, 31 }, { 0x0c1f, 8, 31 },
        { 0x0c20, 0, 32 }, { 0x0c21, 8, 32 },
        { 0x0c22, 0, 33 }, { 0x0c23, 8, 33 },
        { 0x0c24, 0, 34 }, { 0x0c25, 8, 34 },
        { 0x0c26, 0, 35 }, { 0x0c27, 8, 35 },
        { 0x0c28, 0, 36 }, { 0x0c29, 8, 36 },
        { 0x0c2a, 0, 37 }, { 0x0c2b, 8, 37 },
        { 0x0c2c, 0, 38 }, { 0x0c2d, 8, 38 },
        { 0x0c2e, 0, 39 }, { 0x0c2f, 8, 39 },
        { 0x0c30, 0, 40 }, { 0x0c31, 8, 40 },
        { 0x0c32, 0, 41 }, { 0x0c33, 8, 41 },
                                              { 0x0c34, 16, 30 },
                                              { 0x0c35, 16, 31 },
                                              { 0x0c36, 16, 32 },
                                              { 0x0c37, 16, 33 },
                                              { 0x0c38, 16, 34 },
                                              { 0x0c39, 16, 35 },
                                              { 0x0c3a, 16, 36 },
                                              { 0x0c3b, 16, 37 },
                                              { 0x0c3c, 16, 38 },
                                              { 0x0c3d, 16, 39 },
                                              { 0x0c3e, 16, 40 },
                                              { 0x0c3f, 16, 41 },
                                              { 0x0ce9, 16, 13 },
                                              { 0x0cea, 16, 14 },
                                              { 0x0ceb, 16, 15 },
                                              { 0x0cec, 16, 16 },
                                              { 0x0ced, 16, 17 },
                                              { 0x0cee, 16, 18 },
        { 0x0cef, 0, 13 }, { 0x0cf0, 8, 13 },
        { 0x0cf1, 0, 14 }, { 0x0cf2, 8, 14 },
        { 0x0cf3, 0, 15 }, { 0x0cf4, 8, 15 },
        { 0x0cf5, 0, 16 }, { 0x0cf6, 8, 16 },
        { 0x0cf7, 0, 17 }, { 0x0cf8, 8, 17 },
        { 0x0cf9, 0, 18 }, { 0x0cfa, 8, 18 },
        { 0x0d06, 0, 42 }, { 0x0d07, 8, 42 }, { 0x0d09, 16, 42 },
        { 0x0d7e, 0, 42 }, { 0x0d7f, 8, 42 }, { 0x0d81, 16, 42 },
        { 0x325b, 0, 20 }, { 0x325c, 8, 20 }, { 0x325e, 16, 20 },
        { 0x326f, 0,  9 }, { 0x3270, 8,  9 }, { 0x3272, 16,  9 },
        { 0x338c, 0, 23 }, { 0x338d, 8, 23 }, { 0x338f, 16, 23 },
        { 0x33a0, 0, 10 }, { 0x33a1, 8, 10 }, { 0x33a3, 16, 10 },
        { 0x34bd, 0, 21 }, { 0x34be, 8, 21 }, { 0x34c0, 16, 21 },
        { 0x34d1, 0, 11 }, { 0x34d2, 8, 11 }, { 0x34d4, 16, 11 },
        { 0x35be, 0, 12 }, { 0x35bf, 8, 12 }, { 0x35c1, 16, 12 },
        { 0x35d5, 0, 19 }, { 0x35d6, 8, 19 }, { 0x35d8, 16, 19 },
        { 0x3e48, 0, 24 }, { 0x3e49, 8, 24 }, { 0x3e4e, 16, 24 },
        { 0x3e62, 0, 25 }, { 0x3e63, 8, 25 }, { 0x3e68, 16, 25 },
        { 0x3ecc, 0, 28 }, { 0x3ecd, 8, 28 }, { 0x3ecf, 16, 28 },
        { 0x3f54, 0, 27 }, { 0x3f55, 8, 27 }, { 0x3f57, 16, 27 },
        { 0x3ffd, 0, 29 }, { 0x3ffe, 8, 29 }, { 0x4000, 16, 29 },
        { 0x40e1, 0, 26 }, { 0x40e2, 8, 26 }, { 0x40e4, 16, 26 },
        { 0x421d, 0,  7 }, { 0x421e, 8,  7 }, { 0x4223, 16,  7 },
        { 0x4237, 0,  8 }, { 0x4238, 8,  8 }, { 0x423d, 16,  8 },
        { 0x425d, 0, 22 }, { 0x425e, 8, 22 }, { 0x4260, 16, 22 },
        { 0x4277, 0,  0 }, { 0x4278, 8,  0 }, { 0x427a, 16,  0 },
        { 0x49d0, 0, 23 }, { 0x49d1, 8, 23 }, { 0x49d3, 16, 23 },
        { 0x4ab1, 0, 21 }, { 0x4ab2, 8, 21 }, { 0x4ab4, 16, 21 },
        { 0x4f3b, 0,  0 }, { 0x4f3c, 8,  0 }, { 0x4f3e, 16,  0 },
        { 0x57bc, 0, 43 }, { 0x57bd, 8, 43 }, { 0x57bf, 16, 43 },
        { 0x6068, 0,  1 }, { 0x6069, 8,  1 }, { 0x606b, 16,  1 },
        { 0x607c, 0,  0 }, { 0x607d, 8,  0 }, { 0x6082, 16,  0 },
        { 0x6096, 0,  2 }, { 0x6097, 8,  2 }, { 0x609c, 16,  2 },
        { 0x60b0, 0,  3 }, { 0x60b1, 8,  3 }, { 0x60b6, 16,  3 },
        { 0x60ca, 0,  4 }, { 0x60cb, 8,  4 }, { 0x60d0, 16,  4 },
        { 0x615a, 0,  1 }, { 0x615b, 8,  1 }, { 0x615d, 16,  1 },
        { 0x616e, 0,  0 }, { 0x616f, 8,  0 }, { 0x6174, 16,  0 },
        { 0x6188, 0,  2 }, { 0x6189, 8,  2 }, { 0x618e, 16,  2 },
        { 0x61a2, 0,  3 }, { 0x61a3, 8,  3 }, { 0x61a8, 16,  3 },
        { 0x61bc, 0,  4 }, { 0x61bd, 8,  4 }, { 0x61c2, 16,  4 },
        { 0x61e2, 0,  5 }, { 0x61e3, 8,  5 }, { 0x61e8, 16,  5 },
        { 0x61fc, 0,  6 }, { 0x61fd, 8,  6 }, { 0x6202, 16,  6 },
        { 0x628b, 0,  0 }, { 0x628c, 8,  0 }, { 0x628e, 16,  0 },
        { 0x62bd, 0,  5 }, { 0x62be, 8,  5 }, { 0x62c3, 16,  5 },
        { 0x62d7, 0,  6 }, { 0x62d8, 8,  6 }, { 0x62dd, 16,  6 },
        { 0x63a1, 0,  5 }, { 0x63a2, 8,  5 }, { 0x63a7, 16,  5 },
        { 0x63bb, 0,  6 }, { 0x63bc, 8,  6 }, { 0x63c1, 16,  6 }
    }
};


static inline void * checked_malloc( size_t size )
{
    void * x = malloc( size );
    if( !x )
    {
        fprintf( stderr, "Allocation of 0x%zX bytes failed\n", size );
        exit( EXIT_FAILURE );
    }
    return memset( x, 0, size );
}

static inline uint_fast32_t snesaddress( uint_fast32_t address )
{
    return (address & 0x7F8000) << 1 | (address & 0x7FFF) | 0x8000;
}

int main( const int argc, const char * const * const argv )
{
    if( argc != 4 )
    {
        fprintf( stdout, "Usage: lzssse simearth.sfc graphics.bin patched.sfc\n" );
        return argc > 1 ? EXIT_FAILURE : EXIT_SUCCESS;
    }
    FILE * inrom = fopen( argv[1], "rb" );
    if( !inrom )
    {
        fprintf( stderr, "Can't read file: %s\n", argv[1] );
        return EXIT_FAILURE;
    }
    FILE * infile = fopen( argv[2], "rb" );
    if( !infile )
    {
        fprintf( stderr, "Can't read file: %s\n", argv[2] );
        return EXIT_FAILURE;
    }
    FILE * outrom = fopen( argv[3], "wb" );
    if( !outrom )
    {
        fprintf( stderr, "Can't write file: %s\n", argv[3] );
        return EXIT_FAILURE;
    }
    uint8_t * rom = checked_malloc( 0x60000 );
    fread( rom, 0x60000, 1, inrom );
    if( memcmp( rom + 0x7fc0, title, 21 ) )
    {
        fprintf( stderr, "Warning: This doesn't look like SimEarth!\n" );
    }
    uint_fast8_t version = rom[0x7fd9] ? 2 : rom[0x7fdb] ? 1 : 0;
    long int romposition = 0x60001;
    uint_least16_t outlengthtable[61];
    uint_least16_t inlengthtable[61];
    uint8_t * datatable[61];
    uint_least32_t rompositiontable[61];
    for( uint_fast8_t i = 0; i < 61; i++ )
    {
        rompositiontable[i] = snesaddress( romposition );
        uint_fast16_t outlength = fgetc( inrom );
        outlength |= (uint_fast16_t)fgetc( inrom ) << 8;
        outlengthtable[i] = outlength;
        uint_fast16_t oldinlength = fgetc( inrom );
        oldinlength |= (uint_fast16_t)fgetc( inrom ) << 8;
        uint_fast32_t newinlength = worstcase( outlength );
        if( newinlength > UINT16_MAX )
        {
            fprintf( stderr, "Block %" PRIuFAST8 " is too big!\n", i );
        }
        uint8_t * oldinput = checked_malloc( oldinlength );
        uint8_t * oldoutput = checked_malloc( outlength );
        fread( oldinput, oldinlength, 1, inrom );
        if( decompress( oldinput, oldoutput, oldinlength, outlength ) != outlength )
        {
            fprintf( stderr, "Decompression of block %" PRIuFAST16 " failed!\n", i );
        }
        uint8_t * newoutput = checked_malloc( outlength );
        fread( newoutput, outlength, 1, infile );
        if( !memcmp( oldoutput, newoutput, outlength ) )
        {
            romposition += 4 + oldinlength;
            inlengthtable[i] = oldinlength;
            datatable[i] = oldinput;
            free( oldoutput );
            free( newoutput );
            continue;
        }
        free( oldinput );
        uint8_t * newinput = checked_malloc( newinlength );
        newinlength = expand( newoutput, newinput, outlength );
        romposition += 4 + newinlength;
        inlengthtable[i] = newinlength;
        datatable[i] = newinput;
        free( oldoutput );
        free( newoutput );
    }
    romposition = 0;
    for( uint_fast8_t i = 0; i < patch_count; i++ )
    {
        fwrite( &rom[romposition], patch[version][i].offset - romposition, 1, outrom );
        fputc( rompositiontable[patch[version][i].block] >> patch[version][i].shift, outrom );
        romposition = patch[version][i].offset + 1;
    }
    fwrite( &rom[romposition], 0x60000 - romposition, 1, outrom );
    fputc( 0xff, outrom ); // dummy byte to test patch calculation
    for( uint_fast8_t i = 0; i < 61; i++ )
    {
        fputc( outlengthtable[i], outrom );
        fputc( outlengthtable[i] >> 8, outrom );
        fputc( inlengthtable[i], outrom );
        fputc( inlengthtable[i] >> 8, outrom );
        fwrite( datatable[i], inlengthtable[i], 1, outrom );
    }
}
