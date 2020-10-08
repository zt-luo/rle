#! /bin/python

import struct
import io
import sys


def rle7_compress(data_in):
    """
    compress data using zero rle7 methread
    """
    in_len = len(data_in)
    data_out = io.BytesIO()

    cur_in = 0

    repeat = 0
    count = 1

    last = data_in[0]

    for cur_in in range(1, in_len):
        if data_in[cur_in] == last:
            if count != 1:
                # copy no repeat data
                count -= 1
                data_out.write(struct.pack('B', count - 2))

                while count != 1:
                    data_out.write(struct.pack('B', data_in[cur_in - count]))
                    count -= 1

            repeat += 1
        else:
            if repeat != 0:
                # write repeat data
                data_out.write(struct.pack('B', repeat | (1 << 7)))
                data_out.write(struct.pack('B', last))

                repeat = 0

            count += 1

            last = data_in[cur_in]

    if count != 1:
        # copy no repeat data
        count -= 1
        data_out.write(struct.pack('B', count - 1))

        while count != 0:
            data_out.write(struct.pack('B', data_in[cur_in - count]))
            count -= 1

    if repeat != 0:
        # write repeat data
        data_out.write(struct.pack('B', repeat | (1 << 7)))
        data_out.write(struct.pack('B', last))

    return data_out.getvalue()


def rle7_decompress(data_in, out_len):
    """
    decompress data using zero rle7 methread
    """
    data_out = io.BytesIO()

    cur_out = 0
    cur_in = 0
    cc = 0
    count = 0

    while cur_out < out_len:
        cc = data_in[cur_in]
        count = (cc & (~(1 << 7))) + 1

        if cc & (1 << 7):
            while count != 0:
                data_out.write(struct.pack('B', data_in[cur_in + 1]))
                cur_out += 1
                count -= 1

            cur_in += 2
        else:
            while count != 0:
                cur_in += 1
                data_out.write(struct.pack('B', data_in[cur_in]))
                cur_out += 1
                count -= 1

            cur_in += 1

    if cur_out != out_len:
        raise RuntimeError("decompress len mismatch")

    return data_out.getvalue()


def zero_rle_test():
    data = struct.pack('BBBBBBBBBBBBBBBBBBBBBBB', 0, 0, 0, 0,
                       0, 0, 1, 0, 0, 0, 5, 0, 6, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0)
    print(data)
    print(rle7_compress(data))
    print(rle7_decompress(rle7_compress(data), len(data)))


if __name__ == "__main__":
    zero_rle_test()
