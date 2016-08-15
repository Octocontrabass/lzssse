#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lzss.h"

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
    if( argc != 3 )
    {
        fprintf( stdout, "Usage: lzssse simearth.sfc graphics.bin\n" );
        return argc > 1 ? EXIT_FAILURE : EXIT_SUCCESS;
    }
    FILE * rom = fopen( argv[1], "rb" );
    if( !rom )
    {
        fprintf( stderr, "Can't read file: %s\n", argv[1] );
        return EXIT_FAILURE;
    }
    FILE * outfile = fopen( argv[2], "wb" );
    if( !outfile )
    {
        fprintf( stderr, "Can't write file: %s\n", argv[2] );
        return EXIT_FAILURE;
    }
    fseek( rom, 0x60000, SEEK_SET );
    //long int romposition = 0x60000;
    for( uint_fast8_t i = 0; i < 61; i++ )
    {
        uint_fast16_t outlength = fgetc( rom );
        outlength |= (uint_fast16_t)fgetc( rom ) << 8;
        uint_fast16_t inlength = fgetc( rom );
        inlength |= (uint_fast16_t)fgetc( rom ) << 8;
        
        uint8_t * input = checked_malloc( inlength );
        uint8_t * output = checked_malloc( outlength );
        fread( input, inlength, 1, rom );
        if( decompress( input, output, inlength, outlength ) != outlength )
        {
            fprintf( stderr, "Decompression of block %" PRIuFAST16 " failed!\n", i );
        }
        free( input );
        fwrite( output, outlength, 1, outfile );
        free( output );
        
        // useful debug stuff
        //fprintf( stdout, "DEBUG: 0x%05lX 0x%04" PRIXFAST16 " 0x%04" PRIXFAST16 "\n", romposition, outlength, inlength );
        //romposition += inlength + 4;
    }
}
