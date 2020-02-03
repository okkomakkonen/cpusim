#! /usr/bin/python3
# Assembles basic CPU simulator assembly to a binary
import argparse
import os
from collections import defaultdict

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

whitespace = (' ', '\t')
comment = ';'
reference = '%'
tag_end = ':'


def is_numeric(n):
    return n.isnumeric() or (n[2:].isnumeric() and n[:2] in ['0x', '0b'])


def strtonum(n):
    """Converts a string to integer in decimal, binary, hexadecimal or octal.
    """
    if n.startswith('0x'):
        return int(n[2:], base=16)
    elif n.startswith('0b'):
        return int(n[2:], base=2)
    elif n.startswith('0'):
        return int(n[1:], base=8)
    elif n.isnumeric():
        return int(n)
    else:
        raise Exception('Error in string to integer convert')


def assemble(in_filename, out_filename, verbosity=0):
    # Final binary
    binary = []

    # Defined tags
    tags = {}
    # Referenced tags
    references = defaultdict(lambda: list())

    with open(in_filename, 'r') as in_file:
        for line in in_file:
            if line.startswith(whitespace):
                if line.startswith(comment):
                    continue
                for token in line.split():

                    if token.startswith(comment):
                        break

                    elif token in opcodes:  # Token is a reserved opcode
                        binary.append(opcodes[token])

                    elif token.startswith(reference):  # Token is a reference
                        references[token[1:]].append(len(binary))
                        binary.append(0)

                    elif is_numeric(token):
                        binary.append(strtonum(token))

                    else:
                        raise Exception(f'Parse error: Couldn\'t parse {token}')

            else:
                token = line.strip()
                if token.endswith(tag_end):  # This is a new tag
                    tags[token[:-1]] = len(binary)
                else:
                    raise Exception(f'Parse error: Couldn\'t recognise tag {token}')

    # Replace references with real values
    for ref in references:
        if not ref in tags:
            # Couldn't resolve reference to any tag
            raise Exception(f'Parse error: Couldn\'t resolve reference {ref}')
        val = tags[ref]
        for place in references[ref]:
            binary[place] = val

    if verbosity >= 1:
        print(binary)

    with open(out_filename, 'wb+') as out_file:
        out_file.write(bytearray(binary))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("in_filename")
    parser.add_argument('-o', '--out_filename')
    parser.add_argument('-v', '--verbosity', action='count', default=0)
    args = parser.parse_args()

    assemble(args.in_filename, args.out_filename, verbosity=args.verbosity)


main()
