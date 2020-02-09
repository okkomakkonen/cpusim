"""Test numeric methods in assembler"""
import unittest
from assembler import is_numeric, strtonum, neg


class TestIsNumeric(unittest.TestCase):
    """Test is_numeric
    """

    def test_numbers(self):
        """Test valid numbers
        """
        numbers = ['067', '0x67', '0', '0xfe', '780', '0b1010101001']
        for number in numbers:
            self.assertTrue(is_numeric(number))

    def test_not_numbers(self):
        """Test invalid numbers
        """
        not_numbers = ['0xfg', '-089', '089', '0b2102', 'ff']
        for not_number in not_numbers:
            self.assertFalse(is_numeric(not_number))

    def test_negative(self):
        """Test negative numbers
        """
        neg_numbers = ['-12', '-0x56', '-0b101', '-012', '--4']
        for neg_number in neg_numbers:
            self.assertTrue(is_numeric(neg_number))


class TestStrToNum(unittest.TestCase):
    """Test strtonum
    """

    def test_hex(self):
        """Test hex numbers
        """
        hex_numbers = {'0xff': 255, '0x0': 0, '0x12': 0x12,
                       '-0x0': 0, '-0x13': 0xed}
        for hex_number, value in hex_numbers.items():
            self.assertEqual(strtonum(hex_number), value)

    def test_bin(self):
        """Test binary numbers
        """
        bin_numbers = {'0b101': 0b101, '-0b1001': 247, '-0b10010010': 110}
        for bin_number, value in bin_numbers.items():
            self.assertEqual(strtonum(bin_number), value)

    def test_octal(self):
        """Test octal numbers
        """
        octal_numbers = {'023': 19, '-067': 201, '07': 7}
        for octal_number, value in octal_numbers.items():
            self.assertEqual(strtonum(octal_number), value)

    def test_decimal(self):
        """Test decimal numbers
        """
        decimal_numbers = {'34': 34, '-1': 255, '-0': 0, '--4': 4}
        for decimal_number, value in decimal_numbers.items():
            self.assertEqual(strtonum(decimal_number), value)


class TestNeg(unittest.TestCase):
    """Test neg
    """

    def test_neg(self):
        """Test negation
        """
        numbers = {0: 0, 1: 255, 45: 211}
        for number, value in numbers.items():
            self.assertEqual(neg(number), value)


if __name__ == '__main__':
    unittest.main()
