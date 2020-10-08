#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

const uint8_t in[] = {0, 0, 0 ,0 ,0, 0, 1, 0, 0, 0, 5, 0, 6, 0, 0, 0, 1, 2, 3, 4};
uint8_t out[sizeof(in)] = {0};
uint8_t in_temp[sizeof(in)] = {0};

extern uint32_t rle7_compress(uint8_t *out, const uint8_t *in, uint32_t in_len);
extern uint32_t rle7_decompress(uint8_t *out, const uint8_t *in, uint32_t out_len);
extern uint32_t zero_rle_compress(uint8_t *out, const uint8_t *in, uint32_t in_len);
extern uint32_t zero_rle_decompress(uint8_t *out, const uint8_t *in, uint32_t out_len);

static void print_arr(uint8_t *arr, uint32_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%.2x ", arr[i]);
    }
    
    printf("\r\n");
}

int main(int argc,char argv[])
{
    print_arr(in, sizeof(in));
    // int compress_cnt = zero_rle_compress(out, in, sizeof(in));
    int compress_cnt = rle7_compress(out, in, sizeof(in));
    print_arr(out, compress_cnt);
    rle7_decompress(in_temp, out, sizeof(in_temp));
    // zero_rle_decompress(in_temp, out, sizeof(in_temp));
    print_arr(in_temp, sizeof(in_temp));

    return 0;
}
