#include <stdint.h>

uint32_t zero_rle_compress(uint8_t *out, const uint8_t *in, uint32_t in_len)
{
    uint32_t cur_out = 0;
    uint32_t cur_in  = 0;
    int16_t  repeat  = -1;

    for(cur_in = 0; cur_in < in_len; cur_in++)
    {
        while (cur_in < in_len && (0 == *(in + cur_in)) )
        {
            repeat++;
            cur_in++;

            if (0xFF == repeat)
            {
                *(out + cur_out++) = 0;
                *(out + cur_out++) = 0xFF;
                repeat = -1;
            }
        }

        if (-1 != repeat)
        {
            *(out + cur_out++) = 0;
            *(out + cur_out++) = repeat;
            repeat = -1;
        }

        if (cur_in < in_len)
        {
            // copy as it
            *(out + cur_out++) = *(in + cur_in);
        }
    }

    return cur_out;
}

uint32_t zero_rle_decompress(uint8_t *out, const uint8_t *in, uint32_t out_len)
{
    uint32_t cur_out = 0;
    uint32_t cur_in  = 0;
    uint8_t  repeat  = 0;

    while (cur_out < out_len)
    {
        if (0 != *(in + cur_in))
        {
            // copy as it
            *(out + cur_out++) = *(in + cur_in);
        }
        else
        {
            repeat = *(in + ++cur_in) + 1;
        }

        while (0 != repeat)
        {
            *(out + cur_out++) = 0;
            repeat--;
        }

        cur_in++;
    }

    // decompress len mismatch
    while (cur_out != out_len) {};

    return cur_in;
}
