#include <stdint.h>

#define SET_REPEAT(x) (x | (1 << 7))

uint32_t rle7_compress(uint8_t *out, const uint8_t *in, uint32_t in_len)
{
    uint32_t cur_out = 0;
    uint32_t cur_in  = 0;

    uint8_t  repeat  = 0;
    uint8_t  count   = 1;

    uint8_t  last    = *in;

    for (cur_in = 1; cur_in < in_len; cur_in++)
    {
        if (*(in + cur_in) == last)
        {
            if (1 != count)
            {
                // copy no repeat data
                *(out + cur_out++) = --count - 2;

                while (1 != count)
                {
                    *(out + cur_out++) = *(in + cur_in - count--);
                }
            }

            repeat++;
        }
        else
        {
            if (0 != repeat)
            {
                // write repeat data
                *(out + cur_out++) = SET_REPEAT(repeat);
                *(out + cur_out++) = last;

                repeat = 0;
            }

            count++;

            last = *(in + cur_in);
        }
    }

    if (1 != count)
    {
        // copy no repeat data
        *(out + cur_out++) = --count - 1;
        while (0 != count)
        {
            *(out + cur_out++) = *(in + cur_in - count--);
        }
    }

    if (0 != repeat)
    {
        // write repeat data
        *(out + cur_out++) = SET_REPEAT(repeat);
        *(out + cur_out++) = last;
    }

    return cur_out;
}

#define C_COUNT(x) (x & ~(1 << 7))
#define IS_REPEAT(x)  (x & (1 << 7))

uint32_t rle7_decompress(uint8_t *out, const uint8_t *in, uint32_t out_len)
{
    uint32_t cur_out = 0;
    uint32_t cur_in  = 0;
    uint8_t  cc      = 0;
    int8_t   count   = 0;

    while (cur_out < out_len)
    {
        cc    = *(in + cur_in);
        count = C_COUNT(cc) + 1;

        if (IS_REPEAT(cc))
        {
            while (count--)
            {
                *(out + cur_out++) = *(in + cur_in + 1);
            }
            cur_in += 2;
        }
        else
        {
            while (count--)
            {
                *(out + cur_out++) = *(in + ++cur_in);
            }
            cur_in++;
        }
    }

    // decompress len mismatch
    while (cur_out != out_len) {};

    return cur_in;
}
