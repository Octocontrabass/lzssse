#include <stdint.h>

uint_fast16_t decompress( const uint8_t * input, uint8_t * output, uint_fast16_t inlength, uint_fast16_t outlength );
uint_fast32_t worstcase( uint16_t inlength );
uint_fast16_t expand( const uint8_t * input, uint8_t * output, uint_fast16_t inlength );
