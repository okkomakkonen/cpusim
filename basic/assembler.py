import sys
import argparse
import os

opcodes = {
    'NOP': 0x00,
    'HLT': 0x01,
    'LDA': 0x02,
    'ADD': 0x03,
    'SUB': 0x04,
    'STA': 0x05,
    'JMP': 0x06,
    'JPZ': 0x07,
    'JPN': 0x08,
    'JPC': 0x09
}


def assemble(in_filename, out_filename):
    bin = []
    tags = []
    with open(in_filename, 'r') as in_file:
        for line in in_file:
            if line[0] == ' ' or line[0] == '\t':
                for token in line.split():
                    if token in opcodes:
                        bin.append(opcodes[token])
                    elif token.isnumeric():
                        bin.append(int(token))
                    elif len(token) == 4 and token[2:].isnumeric():
                        bin.append(int(token), base=16)
                    else:
                        raise Exception('Parse error')
            else:
                if line.strip().endswith(':'):
                    tags.append(line.strip()[:-1])
                else:
                    raise Exception('Unknown command')

    with open(out_filename, 'wb+') as out_file:
        out_file.write(bytearray(bin))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("in_filename")
    parser.add_argument('-o', '--out_filename')
    parser.parse_args()
    args = parser.parse_args()

    if not os.path.exists('./bin'):
        os.makedirs('./bin')
    assemble(args.in_filename, './bin/' + args.out_filename)


main()
