#! /bin/python

import struct
import io
import sys


def zero_rle_compress(data_in):
    """
    compress data using zero rle methread
    """
    in_len = len(data_in)
    data_out = io.BytesIO()

    cur_in = 0
    repeat = -1

    while cur_in < in_len:
        while cur_in < in_len and data_in[cur_in] == 0:
            repeat += 1
            cur_in += 1

            if repeat == 0xFF:
                data_out.write(struct.pack('B', 0))
                data_out.write(struct.pack('B', 0xFF))
                repeat = -1

        if repeat != -1:
            data_out.write(struct.pack('B', 0))
            data_out.write(struct.pack('B', repeat))
            repeat = -1

        if cur_in < in_len:
            data_out.write(struct.pack('B', data_in[cur_in]))
            cur_in += 1

    return data_out.getvalue()


def zero_rle_decompress(data_in, out_len):
    """
    decompress data using zero rle methread
    """
    data_out = io.BytesIO()

    cur_out = 0
    cur_in = 0
    repeat = 0

    while cur_out < out_len:
        if data_in[cur_in] != 0:
            data_out.write(struct.pack('B', data_in[cur_in]))
            cur_out += 1
        else:
            cur_in += 1
            repeat = data_in[cur_in] + 1

        while repeat != 0:
            data_out.write(struct.pack('B', 0))
            cur_out += 1
            repeat -= 1

        cur_in += 1

    if cur_out != out_len:
        raise RuntimeError("decompress len mismatch")

    return data_out.getvalue()


def zero_rle_test():
    data = struct.pack('BBBBBBBBBBBBBBBBBBBBBBB', 0, 0, 0, 0,
                       0, 0, 1, 0, 0, 0, 5, 0, 6, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0)
    print(data)
    print(zero_rle_compress(data))
    print(zero_rle_decompress(zero_rle_compress(data), len(data)))


if __name__ == "__main__":
    zero_rle_test()
