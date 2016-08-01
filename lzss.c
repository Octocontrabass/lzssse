#include "lzss.h"

uint_fast16_t decompress( const uint8_t * input, uint8_t * output, uint_fast16_t inlength, uint_fast16_t outlength )
{
    uint_fast16_t inptr = 0;
    uint_fast16_t outptr = 0;
    while( 1 )
    {
        if( inptr >= inlength ) return outptr;
        uint_fast16_t control = (uint_fast16_t)input[ inptr++ ] << 8 | 0xff;
        while( control & 0xff )
        {
            if( control & 0x8000 )
            {
                if( inptr >= inlength ) return outptr;
                uint_fast16_t run = input[ inptr++ ];
                if( inptr >= inlength ) return outptr;
                run |= input[ inptr++ ] << 8;
                uint_fast16_t runptr = run >> 4;
                runptr = runptr < outptr ? outptr - runptr - 1 : outptr;
                run = (run & 0xf) + 3;
                for( uint_fast16_t i = 0; i < run; i++ )
                {
                    output[ outptr++ ] = output[ runptr++ ];
                    if( outptr >= outlength ) return outptr;
                }
            }
            else
            {
                if( inptr >= inlength ) return outptr;
                output[ outptr++ ] = input[ inptr++ ];
                if( outptr >= outlength ) return outptr;
            }
            control <<= 1;
        }
    }
    return outptr;
}

uint_fast32_t worstcase( uint16_t inlength )
{
    return (uint_fast32_t)inlength + ((uint_fast32_t)inlength + 7) >> 3;
}

uint_fast16_t expand( const uint8_t * input, uint8_t * output, uint_fast16_t inlength )
{
    uint_fast16_t inptr = 0;
    uint_fast16_t outptr = 0;
    if( inptr >= inlength ) return outptr;
    while( 1 )
    {
        output[ outptr++ ] = 0;
        for( uint_fast8_t i = 0; i < 8; i++ )
        {
            output[ outptr++ ] = input[ inptr++ ];
            if( inptr >= inlength ) return outptr;
        }
    }
    return outptr;
}
