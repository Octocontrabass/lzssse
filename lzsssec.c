#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lzss.h"

static const uint8_t title[21] = "SIMEARTH             ";

typedef struct patch_t
{
    uint_fast16_t offset;
    uint_fast8_t shift;
    uint_fast8_t block;
} patch_t;

#define patch_count (3*79)

static const patch_t patch[3][patch_count] =
{
    {
        { 0x04df, 0,  0 }, { 0x04e0, 8,  0 }, { 0x04e2, 16,  0 },
        { 0x050c, 0,  0 }, { 0x050d, 8,  0 }, { 0x050f, 16,  0 },
        { 0x0c15, 0, 30 }, { 0x0c16, 8, 30 },
        { 0x0c17, 0, 31 }, { 0x0c18, 8, 31 },
        { 0x0c19, 0, 32 }, { 0x0c1a, 8, 32 },
        { 0x0c1b, 0, 33 }, { 0x0c1c, 8, 33 },
        { 0x0c1d, 0, 34 }, { 0x0c1e, 8, 34 },
        { 0x0c1f, 0, 35 }, { 0x0c20, 8, 35 },
        { 0x0c21, 0, 36 }, { 0x0c22, 8, 36 },
        { 0x0c23, 0, 37 }, { 0x0c24, 8, 37 },
        { 0x0c25, 0, 38 }, { 0x0c26, 8, 38 },
        { 0x0c27, 0, 39 }, { 0x0c28, 8, 39 },
        { 0x0c29, 0, 40 }, { 0x0c2a, 8, 40 },
        { 0x0c2b, 0, 41 }, { 0x0c2c, 8, 41 },
                                              { 0x0c2d, 16, 30 },
                                              { 0x0c2e, 16, 31 },
                                              { 0x0c2f, 16, 32 },
                                              { 0x0c30, 16, 33 },
                                              { 0x0c31, 16, 34 },
                                              { 0x0c32, 16, 35 },
                                              { 0x0c33, 16, 36 },
                                              { 0x0c34, 16, 37 },
                                              { 0x0c35, 16, 38 },
                                              { 0x0c36, 16, 39 },
                                              { 0x0c37, 16, 40 },
                                              { 0x0c38, 16, 41 },
                                              { 0x0ce2, 16, 13 },
                                              { 0x0ce3, 16, 14 },
                                              { 0x0ce4, 16, 15 },
                                              { 0x0ce5, 16, 16 },
                                              { 0x0ce6, 16, 17 },
                                              { 0x0ce7, 16, 18 },
        { 0x0ce8, 0, 13 }, { 0x0ce9, 8, 13 },
        { 0x0cea, 0, 14 }, { 0x0ceb, 8, 14 },
        { 0x0cec, 0, 15 }, { 0x0ced, 8, 15 },
        { 0x0cee, 0, 16 }, { 0x0cef, 8, 16 },
        { 0x0cf0, 0, 17 }, { 0x0cf1, 8, 17 },
        { 0x0cf2, 0, 18 }, { 0x0cf3, 8, 18 },
        { 0x0cff, 0, 42 }, { 0x0d00, 8, 42 }, { 0x0d02, 16, 42 },
        { 0x0d77, 0, 42 }, { 0x0d78, 8, 42 }, { 0x0d7a, 16, 42 },
        { 0x2b9f, 0, 46 }, { 0x2ba0, 8, 46 }, { 0x2ba2, 16, 46 },
        { 0x3105, 0, 20 }, { 0x3106, 8, 20 }, { 0x3108, 16, 20 },
        { 0x3119, 0,  9 }, { 0x311a, 8,  9 }, { 0x311c, 16,  9 },
        { 0x3236, 0, 23 }, { 0x3237, 8, 23 }, { 0x3239, 16, 23 },
        { 0x324a, 0, 10 }, { 0x324b, 8, 10 }, { 0x324d, 16, 10 },
        { 0x3367, 0, 21 }, { 0x3368, 8, 21 }, { 0x336a, 16, 21 },
        { 0x337b, 0, 11 }, { 0x337c, 8, 11 }, { 0x337e, 16, 11 },
        { 0x3468, 0, 12 }, { 0x3469, 8, 12 }, { 0x346b, 16, 12 },
        { 0x347f, 0, 19 }, { 0x3480, 8, 19 }, { 0x3482, 16, 19 },
        { 0x3bc1, 0, 51 }, { 0x3bc2, 8, 51 }, { 0x3bc4, 16, 51 },
        { 0x3c97, 0, 24 }, { 0x3c98, 8, 24 }, { 0x3c9d, 16, 24 },
        { 0x3cb1, 0, 25 }, { 0x3cb2, 8, 25 }, { 0x3cb7, 16, 25 },
        { 0x3d1b, 0, 28 }, { 0x3d1c, 8, 28 }, { 0x3d1e, 16, 28 },
        { 0x3da3, 0, 27 }, { 0x3da4, 8, 27 }, { 0x3da6, 16, 27 },
        { 0x3e4c, 0, 29 }, { 0x3e4d, 8, 29 }, { 0x3e4f, 16, 29 },
        { 0x3f30, 0, 26 }, { 0x3f31, 8, 26 }, { 0x3f33, 16, 26 },
        { 0x407c, 0,  7 }, { 0x407d, 8,  7 }, { 0x4082, 16,  7 },
        { 0x4096, 0,  8 }, { 0x4097, 8,  8 }, { 0x409c, 16,  8 },
        { 0x40bc, 0, 22 }, { 0x40bd, 8, 22 }, { 0x40bf, 16, 22 },
        { 0x40d6, 0,  0 }, { 0x40d7, 8,  0 }, { 0x40d9, 16,  0 },
        { 0x482f, 0, 23 }, { 0x4830, 8, 23 }, { 0x4832, 16, 23 },
        { 0x4910, 0, 21 }, { 0x4911, 8, 21 }, { 0x4913, 16, 21 },
        { 0x4a69, 0, 54 }, { 0x4a6a, 8, 54 }, { 0x4a6f, 16, 54 },
        { 0x4a83, 0, 55 }, { 0x4a84, 8, 55 }, { 0x4a89, 16, 55 },
        { 0x4aa9, 0, 56 }, { 0x4aaa, 8, 56 }, { 0x4aac, 16, 56 },
        { 0x4d3a, 0, 52 }, { 0x4d3b, 8, 52 }, { 0x4d40, 16, 52 },
        { 0x4d54, 0, 53 }, { 0x4d55, 8, 53 }, { 0x4d5a, 16, 53 },
        { 0x4d7a, 0, 47 }, { 0x4d7b, 8, 47 }, { 0x4d7d, 16, 47 },
        { 0x4d9a, 0,  0 }, { 0x4d9b, 8,  0 }, { 0x4d9d, 16,  0 },
        { 0x5199, 0, 58 }, { 0x519a, 8, 58 }, { 0x519c, 16, 58 },
        { 0x51f7, 0, 59 }, { 0x51f8, 8, 59 }, { 0x51fa, 16, 59 },
        { 0x5252, 0, 60 }, { 0x5253, 8, 60 }, { 0x5255, 16, 60 },
        { 0x5598, 0, 44 }, { 0x5599, 8, 44 }, { 0x559b, 16, 44 },
        { 0x55a2, 0, 57 }, { 0x55a3, 8, 57 }, { 0x55a5, 16, 57 },
        { 0x5639, 0, 43 }, { 0x563a, 8, 43 }, { 0x563c, 16, 43 },
        { 0x577c, 0, 45 }, { 0x577d, 8, 45 }, { 0x577f, 16, 45 },
        { 0x58c2, 0, 49 }, { 0x58c3, 8, 49 }, { 0x58c5, 16, 49 },
        { 0x5bdc, 0, 48 }, { 0x5bdd, 8, 48 }, { 0x5bdf, 16, 48 },
        { 0x5f00, 0,  1 }, { 0x5f01, 8,  1 }, { 0x5f03, 16,  1 },
        { 0x5f14, 0,  0 }, { 0x5f15, 8,  0 }, { 0x5f1a, 16,  0 },
        { 0x5f2e, 0,  2 }, { 0x5f2f, 8,  2 }, { 0x5f34, 16,  2 },
        { 0x5f48, 0,  3 }, { 0x5f49, 8,  3 }, { 0x5f4e, 16,  3 },
        { 0x5f62, 0,  4 }, { 0x5f63, 8,  4 }, { 0x5f68, 16,  4 },
        { 0x6001, 0,  1 }, { 0x6002, 8,  1 }, { 0x6004, 16,  1 },
        { 0x6015, 0,  0 }, { 0x6016, 8,  0 }, { 0x601b, 16,  0 },
        { 0x602f, 0,  2 }, { 0x6030, 8,  2 }, { 0x6035, 16,  2 },
        { 0x6049, 0,  3 }, { 0x604a, 8,  3 }, { 0x604f, 16,  3 },
        { 0x6063, 0,  4 }, { 0x6064, 8,  4 }, { 0x6069, 16,  4 },
        { 0x6089, 0,  5 }, { 0x608a, 8,  5 }, { 0x608f, 16,  5 },
        { 0x60a3, 0,  6 }, { 0x60a4, 8,  6 }, { 0x60a9, 16,  6 },
        { 0x613e, 0,  0 }, { 0x613f, 8,  0 }, { 0x6141, 16,  0 },
        { 0x6170, 0,  5 }, { 0x6171, 8,  5 }, { 0x6176, 16,  5 },
        { 0x618a, 0,  6 }, { 0x618b, 8,  6 }, { 0x6190, 16,  6 },
        { 0x6260, 0,  5 }, { 0x6261, 8,  5 }, { 0x6266, 16,  5 },
        { 0x627a, 0,  6 }, { 0x627b, 8,  6 }, { 0x6280, 16,  6 },
        { 0x62be, 0, 46 }, { 0x62bf, 8, 46 }, { 0x62c1, 16, 46 },
        { 0x6374, 0, 50 }, { 0x6375, 8, 50 }, { 0x6377, 16, 50 }
    },
    {
        { 0x04df, 0,  0 }, { 0x04e0, 8,  0 }, { 0x04e2, 16,  0 },
        { 0x050c, 0,  0 }, { 0x050d, 8,  0 }, { 0x050f, 16,  0 },
        { 0x0c15, 0, 30 }, { 0x0c16, 8, 30 },
        { 0x0c17, 0, 31 }, { 0x0c18, 8, 31 },
        { 0x0c19, 0, 32 }, { 0x0c1a, 8, 32 },
        { 0x0c1b, 0, 33 }, { 0x0c1c, 8, 33 },
        { 0x0c1d, 0, 34 }, { 0x0c1e, 8, 34 },
        { 0x0c1f, 0, 35 }, { 0x0c20, 8, 35 },
        { 0x0c21, 0, 36 }, { 0x0c22, 8, 36 },
        { 0x0c23, 0, 37 }, { 0x0c24, 8, 37 },
        { 0x0c25, 0, 38 }, { 0x0c26, 8, 38 },
        { 0x0c27, 0, 39 }, { 0x0c28, 8, 39 },
        { 0x0c29, 0, 40 }, { 0x0c2a, 8, 40 },
        { 0x0c2b, 0, 41 }, { 0x0c2c, 8, 41 },
                                              { 0x0c2d, 16, 30 },
                                              { 0x0c2e, 16, 31 },
                                              { 0x0c2f, 16, 32 },
                                              { 0x0c30, 16, 33 },
                                              { 0x0c31, 16, 34 },
                                              { 0x0c32, 16, 35 },
                                              { 0x0c33, 16, 36 },
                                              { 0x0c34, 16, 37 },
                                              { 0x0c35, 16, 38 },
                                              { 0x0c36, 16, 39 },
                                              { 0x0c37, 16, 40 },
                                              { 0x0c38, 16, 41 },
                                              { 0x0ce2, 16, 13 },
                                              { 0x0ce3, 16, 14 },
                                              { 0x0ce4, 16, 15 },
                                              { 0x0ce5, 16, 16 },
                                              { 0x0ce6, 16, 17 },
                                              { 0x0ce7, 16, 18 },
        { 0x0ce8, 0, 13 }, { 0x0ce9, 8, 13 },
        { 0x0cea, 0, 14 }, { 0x0ceb, 8, 14 },
        { 0x0cec, 0, 15 }, { 0x0ced, 8, 15 },
        { 0x0cee, 0, 16 }, { 0x0cef, 8, 16 },
        { 0x0cf0, 0, 17 }, { 0x0cf1, 8, 17 },
        { 0x0cf2, 0, 18 }, { 0x0cf3, 8, 18 },
        { 0x0cff, 0, 42 }, { 0x0d00, 8, 42 }, { 0x0d02, 16, 42 },
        { 0x0d77, 0, 42 }, { 0x0d78, 8, 42 }, { 0x0d7a, 16, 42 },
        { 0x2c11, 0, 46 }, { 0x2c12, 8, 46 }, { 0x2c14, 16, 46 },
        { 0x3182, 0, 20 }, { 0x3183, 8, 20 }, { 0x3185, 16, 20 },
        { 0x3196, 0,  9 }, { 0x3197, 8,  9 }, { 0x3199, 16,  9 },
        { 0x32b3, 0, 23 }, { 0x32b4, 8, 23 }, { 0x32b6, 16, 23 },
        { 0x32c7, 0, 10 }, { 0x32c8, 8, 10 }, { 0x32ca, 16, 10 },
        { 0x33e4, 0, 21 }, { 0x33e5, 8, 21 }, { 0x33e7, 16, 21 },
        { 0x33f8, 0, 11 }, { 0x33f9, 8, 11 }, { 0x33fb, 16, 11 },
        { 0x34e5, 0, 12 }, { 0x34e6, 8, 12 }, { 0x34e8, 16, 12 },
        { 0x34fc, 0, 19 }, { 0x34fd, 8, 19 }, { 0x34ff, 16, 19 },
        { 0x3c3e, 0, 51 }, { 0x3c3f, 8, 51 }, { 0x3c41, 16, 51 },
        { 0x3d14, 0, 24 }, { 0x3d15, 8, 24 }, { 0x3d1a, 16, 24 },
        { 0x3d2e, 0, 25 }, { 0x3d2f, 8, 25 }, { 0x3d34, 16, 25 },
        { 0x3d98, 0, 28 }, { 0x3d99, 8, 28 }, { 0x3d9b, 16, 28 },
        { 0x3e20, 0, 27 }, { 0x3e21, 8, 27 }, { 0x3e23, 16, 27 },
        { 0x3ec9, 0, 29 }, { 0x3eca, 8, 29 }, { 0x3ecc, 16, 29 },
        { 0x3fad, 0, 26 }, { 0x3fae, 8, 26 }, { 0x3fb0, 16, 26 },
        { 0x40f9, 0,  7 }, { 0x40fa, 8,  7 }, { 0x40ff, 16,  7 },
        { 0x4113, 0,  8 }, { 0x4114, 8,  8 }, { 0x4119, 16,  8 },
        { 0x4139, 0, 22 }, { 0x413a, 8, 22 }, { 0x413c, 16, 22 },
        { 0x4153, 0,  0 }, { 0x4154, 8,  0 }, { 0x4156, 16,  0 },
        { 0x48ac, 0, 23 }, { 0x48ad, 8, 23 }, { 0x48af, 16, 23 },
        { 0x498d, 0, 21 }, { 0x498e, 8, 21 }, { 0x4990, 16, 21 },
        { 0x4ae6, 0, 54 }, { 0x4ae7, 8, 54 }, { 0x4aec, 16, 54 },
        { 0x4b00, 0, 55 }, { 0x4b01, 8, 55 }, { 0x4b06, 16, 55 },
        { 0x4b26, 0, 56 }, { 0x4b27, 8, 56 }, { 0x4b29, 16, 56 },
        { 0x4db7, 0, 52 }, { 0x4db8, 8, 52 }, { 0x4dbd, 16, 52 },
        { 0x4dd1, 0, 53 }, { 0x4dd2, 8, 53 }, { 0x4dd7, 16, 53 },
        { 0x4df7, 0, 47 }, { 0x4df8, 8, 47 }, { 0x4dfa, 16, 47 },
        { 0x4e17, 0,  0 }, { 0x4e18, 8,  0 }, { 0x4e1a, 16,  0 },
        { 0x5216, 0, 58 }, { 0x5217, 8, 58 }, { 0x5219, 16, 58 },
        { 0x5274, 0, 59 }, { 0x5275, 8, 59 }, { 0x5277, 16, 59 },
        { 0x52cf, 0, 60 }, { 0x52d0, 8, 60 }, { 0x52d2, 16, 60 },
        { 0x5615, 0, 44 }, { 0x5616, 8, 44 }, { 0x5618, 16, 44 },
        { 0x561f, 0, 57 }, { 0x5620, 8, 57 }, { 0x5622, 16, 57 },
        { 0x56b6, 0, 43 }, { 0x56b7, 8, 43 }, { 0x56b9, 16, 43 },
        { 0x5804, 0, 45 }, { 0x5805, 8, 45 }, { 0x5807, 16, 45 },
        { 0x594a, 0, 49 }, { 0x594b, 8, 49 }, { 0x594d, 16, 49 },
        { 0x5c64, 0, 48 }, { 0x5c65, 8, 48 }, { 0x5c67, 16, 48 },
        { 0x5f88, 0,  1 }, { 0x5f89, 8,  1 }, { 0x5f8b, 16,  1 },
        { 0x5f9c, 0,  0 }, { 0x5f9d, 8,  0 }, { 0x5fa2, 16,  0 },
        { 0x5fb6, 0,  2 }, { 0x5fb7, 8,  2 }, { 0x5fbc, 16,  2 },
        { 0x5fd0, 0,  3 }, { 0x5fd1, 8,  3 }, { 0x5fd6, 16,  3 },
        { 0x5fea, 0,  4 }, { 0x5feb, 8,  4 }, { 0x5ff0, 16,  4 },
        { 0x6089, 0,  1 }, { 0x608a, 8,  1 }, { 0x608c, 16,  1 },
        { 0x609d, 0,  0 }, { 0x609e, 8,  0 }, { 0x60a3, 16,  0 },
        { 0x60b7, 0,  2 }, { 0x60b8, 8,  2 }, { 0x60bd, 16,  2 },
        { 0x60d1, 0,  3 }, { 0x60d2, 8,  3 }, { 0x60d7, 16,  3 },
        { 0x60eb, 0,  4 }, { 0x60ec, 8,  4 }, { 0x60f1, 16,  4 },
        { 0x6111, 0,  5 }, { 0x6112, 8,  5 }, { 0x6117, 16,  5 },
        { 0x612b, 0,  6 }, { 0x612c, 8,  6 }, { 0x6131, 16,  6 },
        { 0x61c6, 0,  0 }, { 0x61c7, 8,  0 }, { 0x61c9, 16,  0 },
        { 0x61f8, 0,  5 }, { 0x61f9, 8,  5 }, { 0x61fe, 16,  5 },
        { 0x6212, 0,  6 }, { 0x6213, 8,  6 }, { 0x6218, 16,  6 },
        { 0x62e8, 0,  5 }, { 0x62e9, 8,  5 }, { 0x62ee, 16,  5 },
        { 0x6302, 0,  6 }, { 0x6303, 8,  6 }, { 0x6308, 16,  6 },
        { 0x6346, 0, 46 }, { 0x6347, 8, 46 }, { 0x6349, 16, 46 },
        { 0x6400, 0, 50 }, { 0x6401, 8, 50 }, { 0x6403, 16, 50 }
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
        { 0x2cf5, 0, 46 }, { 0x2cf6, 8, 46 }, { 0x2cf8, 16, 46 },
        { 0x325b, 0, 20 }, { 0x325c, 8, 20 }, { 0x325e, 16, 20 },
        { 0x326f, 0,  9 }, { 0x3270, 8,  9 }, { 0x3272, 16,  9 },
        { 0x338c, 0, 23 }, { 0x338d, 8, 23 }, { 0x338f, 16, 23 },
        { 0x33a0, 0, 10 }, { 0x33a1, 8, 10 }, { 0x33a3, 16, 10 },
        { 0x34bd, 0, 21 }, { 0x34be, 8, 21 }, { 0x34c0, 16, 21 },
        { 0x34d1, 0, 11 }, { 0x34d2, 8, 11 }, { 0x34d4, 16, 11 },
        { 0x35be, 0, 12 }, { 0x35bf, 8, 12 }, { 0x35c1, 16, 12 },
        { 0x35d5, 0, 19 }, { 0x35d6, 8, 19 }, { 0x35d8, 16, 19 },
        { 0x3d17, 0, 51 }, { 0x3d18, 8, 51 }, { 0x3d1a, 16, 51 },
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
        { 0x4c0a, 0, 54 }, { 0x4c0b, 8, 54 }, { 0x4c10, 16, 54 },
        { 0x4c24, 0, 55 }, { 0x4c25, 8, 55 }, { 0x4c2a, 16, 55 },
        { 0x4c4a, 0, 56 }, { 0x4c4b, 8, 56 }, { 0x4c4d, 16, 56 },
        { 0x4edb, 0, 52 }, { 0x4edc, 8, 52 }, { 0x4ee1, 16, 52 },
        { 0x4ef5, 0, 53 }, { 0x4ef6, 8, 53 }, { 0x4efb, 16, 53 },
        { 0x4f1b, 0, 47 }, { 0x4f1c, 8, 47 }, { 0x4f1e, 16, 47 },
        { 0x4f3b, 0,  0 }, { 0x4f3c, 8,  0 }, { 0x4f3e, 16,  0 },
        { 0x531c, 0, 58 }, { 0x531d, 8, 58 }, { 0x531f, 16, 58 },
        { 0x537a, 0, 59 }, { 0x537b, 8, 59 }, { 0x537d, 16, 59 },
        { 0x53d5, 0, 60 }, { 0x53d6, 8, 60 }, { 0x53d8, 16, 60 },
        { 0x571b, 0, 44 }, { 0x571c, 8, 44 }, { 0x571e, 16, 44 },
        { 0x5725, 0, 57 }, { 0x5726, 8, 57 }, { 0x5728, 16, 57 },
        { 0x57bc, 0, 43 }, { 0x57bd, 8, 43 }, { 0x57bf, 16, 43 },
        { 0x590a, 0, 45 }, { 0x590b, 8, 45 }, { 0x590d, 16, 45 },
        { 0x5a50, 0, 49 }, { 0x5a51, 8, 49 }, { 0x5a53, 16, 49 },
        { 0x5d6a, 0, 48 }, { 0x5d6b, 8, 48 }, { 0x5d6d, 16, 48 },
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
        { 0x63bb, 0,  6 }, { 0x63bc, 8,  6 }, { 0x63c1, 16,  6 },
        { 0x63ff, 0, 46 }, { 0x6400, 8, 46 }, { 0x6402, 16, 46 },
        { 0x64ae, 0, 50 }, { 0x64af, 8, 50 }, { 0x64b1, 16, 50 }
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
    long int romposition = 0x60000;
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
        newinlength = compress( newoutput, newinput, outlength );
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
    //fputc( 0xff, outrom ); // dummy byte to test patch calculation
    for( uint_fast8_t i = 0; i < 61; i++ )
    {
        fputc( outlengthtable[i], outrom );
        fputc( outlengthtable[i] >> 8, outrom );
        fputc( inlengthtable[i], outrom );
        fputc( inlengthtable[i] >> 8, outrom );
        fwrite( datatable[i], inlengthtable[i], 1, outrom );
    }
}
