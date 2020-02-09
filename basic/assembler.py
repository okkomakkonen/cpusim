"""Assembles basic CPU simulator assembly to a binary
"""
import argparse
from collections import defaultdict

# Operation codes
OPCODES = {
    'HLT': 0x0a,
    'NOP': 0x01,
    'LDA': 0x02,
    'LDB': 0x03,
    'JMP': 0x04,
    'ADD': 0x05,
    'SUB': 0x06,
    'XOR': 0x07,
    'NOT': 0x0d,
    'OUT': 0x08,
    'JPZ': 0x09,
    'JPN': 0x0b,
    'JPC': 0x0c,
    'PSH': 0x0e,
    'POP': 0x0f,
    'JSR': 0x10,
    'RSR': 0x11,
    'MAB': 0x12,
    'MBA': 0x13
}

REGISTERS = []  # ['a', 'b']

WHITESPACE = (' ', '\t')  # Characters that can start a command
COMMENT = ';'             # Character that starts a comment
REFERENCE = '%'           # Character that starts a reference
TAGEND = ':'              # Character at the end of a tag

RESERVED_WORDS = list(OPCODES.keys()) + REGISTERS  # Reserved tokens


def is_numeric(string):
    """Check if string contains a numeric value in decimal, binary, hexadecimal
    or octal. A sign is not allowed.
    """
    if string and string.startswith('-'):
        return is_numeric(string[1:])
    hex_chars = '0123456789abcdef'
    bin_chars = '01'
    octal_chars = '01234567'
    if string.startswith('0x') and len(string) > 2:            # hexadecimal
        return all(c in hex_chars for c in string[2:])
    if string.startswith('0b') and len(string) > 2:            # binary
        return all(c in bin_chars for c in string[2:])
    if string.startswith('0') and len(string) > 1:  # octal
        return all(c in octal_chars for c in string[1:])
    if string.isnumeric():                 # decimal
        return True
    return False


def neg(num):
    """Return two's complement of num
    """
    return ((num ^ 0xff) + 1) % 2**8


def strtonum(string):
    """Converts a string to integer in decimal, binary, hexadecimal or octal.
    Strings with signs are not allowed.
    """
    if string and string.startswith('-'):
        return neg(strtonum(string[1:]))
    if string.startswith('0x'):            # hexadecimal
        return int(string[2:], base=16)
    if string.startswith('0b'):            # binary
        return int(string[2:], base=2)
    if string.startswith('0') and len(string) > 1:  # octal
        return int(string[1:], base=8)
    if string.isnumeric():                 # decimal
        return int(string)
    raise Exception('Error in string to integer convert')


def make_binary(in_filename, verbosity=0):
    """Create bytearray from assembly file
    """
    # TODO: divide to other functions
    binary = []  # Final binary
    tags = {}  # Defined tags
    references = defaultdict(list)  # Referenced tags

    with open(in_filename, 'r') as in_file:
        for line in in_file:                  # go through all lines in file
            if line.startswith(WHITESPACE):   # whitespace starts a new command
                if line.startswith(COMMENT):  # comments are ignored
                    continue
                for token in line.split():    # go through all words

                    if token.startswith(COMMENT):  # comments are ignored
                        break

                    elif token in OPCODES:  # token is a reserved opcode
                        binary.append(OPCODES[token])

                    elif token.startswith(REFERENCE):  # token is a reference
                        if token[1:] in RESERVED_WORDS:
                            raise Exception(
                                f'Parse error: {token[1:]} is not allowed as a reference.')
                        if token[1:] in REGISTERS:  # token is referencing a register
                            pass
                        else:  # token is referencing a tag
                            references[token[1:]].append(len(binary))
                            binary.append(0)

                    elif is_numeric(token):     # token is a numeral
                        binary.append(strtonum(token))

                    else:
                        raise Exception(f"Parse error: Couldn't parse {token}")

            else:
                if line.startswith(COMMENT):  # comments are ignored
                    continue
                token = line.strip()
                if token.endswith(TAGEND):  # This is a new tag
                    tags[token[:-1]] = len(binary)
                else:
                    raise Exception(f"Parse error: Couldn't recognise tag {token}")

    # Replace references with real values
    for ref in references:
        if ref not in tags:
            # Couldn't resolve reference to any tag
            raise Exception(f"Parse error: Couldn't resolve reference {ref}")
        val = tags[ref]
        for place in references[ref]:
            binary[place] = val

    if verbosity >= 1:
        print([hex(b) for b in binary])

    return binary + [0]*(256 - len(binary))


def write_binary(binary, out_filename, verbosity=0):
    """Write a bytearray to file
    """
    with open(out_filename, 'wb+') as out_file:
        out_file.write(bytearray(binary))


def assemble(in_filename, out_filename, verbosity=0):
    """Create binary from assebly file and write to file
    """
    binary = make_binary(in_filename, verbosity)
    write_binary(binary, out_filename, verbosity)


def main():
    """Parse command line arguments and assemble code
    """
    parser = argparse.ArgumentParser()
    parser.add_argument("in_filename")
    parser.add_argument('-o', '--out_filename', default='a.out')
    parser.add_argument('-v', '--verbosity', action='count', default=0)
    args = parser.parse_args()

    assemble(args.in_filename, args.out_filename, verbosity=args.verbosity)


if __name__ == '__main__':
    main()
