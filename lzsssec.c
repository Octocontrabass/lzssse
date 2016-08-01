#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lzss.h"

static const uint8_t title[21] = "SIMEARTH             ";


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
    long int romposition = 0x60000;
    for( uint_fast8_t i = 0; i < 61; i++ )
    {
        
    }
}
