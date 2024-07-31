#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int debug_mode = 0;

const char *get_base_digits() { return "0123456789abcdefghijklmnopqrstuvwxyz"; }
const size_t TOTAL_DIGIT_COUNT = 36;

void free_number(char *num) { free(num); }

const char *add_base_numbers(unsigned int base, const char *num1, const char *num2) {
    num1 = remove_leading_zeros(num1);
    num2 = remove_leading_zeros(num2);
    size_t len1 = strlen(num1);
    size_t len2 = strlen(num2);
    size_t max_length = (len1 > len2) ? len1 : len2;
    
    char *result = (char *)malloc(max_length + 2);
    int carry = 0;
    int pos = 0;

    if (debug_mode) {
        fprintf(stderr, "Adding %u %s %s\n", base, num1, num2);
        fprintf(stderr, "Entering addition function\n");
    }

    for (int i = 0; i < max_length || carry; i++) {
        int digit1 = (i < len1) ? get_digit_value(num1[len1 - 1 - i]) : 0;
        int digit2 = (i < len2) ? get_digit_value(num2[len2 - 1 - i]) : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        sum = sum % base;

        result[pos] = convert_to_char(sum);
        
        if (debug_mode) {
            fprintf(stderr, "Addition step: digit1 %d digit2 %d carry %d\n", digit1, digit2, carry);
            fprintf(stderr, "Result digit: %c carry %d\n", result[pos], carry);
        }
        pos++;
    }

    result[pos] = '\0';
    reverse_string(result);

    if (debug_mode) {
        fprintf(stderr, "Final carry: %d\n", carry);
        fprintf(stderr, "Addition result %s + %s = %s\n", num1, num2, result);
    }

    return (char *)remove_leading_zeros(result);
}

const char *subtract_base_numbers(unsigned int base, const char *num1, const char *num2) {
    num1 = remove_leading_zeros(num1);
    num2 = remove_leading_zeros(num2);
    size_t len1 = strlen(num1);
    size_t len2 = strlen(num2);
    char *result = (char *)malloc(len1 + 1);
    int borrow = 0;
    int pos = 0;

    if (debug_mode) {
        fprintf(stderr, "Subtracting %u %s %s\n", base, num1, num2);
        fprintf(stderr, "Entering subtraction function\n");
    }

    for (int i = 0; i < len1; i++) {
        int digit1 = get_digit_value(num1[len1 - 1 - i]);
        int digit2 = (i < len2) ? get_digit_value(num2[len2 - 1 - i]) : 0;
        int diff = digit1 - digit2 - borrow;

        if (diff < 0) {
            diff += base;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result[pos] = convert_to_char(diff);
        
        if (debug_mode) {
            fprintf(stderr, "Subtraction step: digit1 %d digit2 %d borrow %d\n", digit1, digit2, borrow);
            fprintf(stderr, "Result digit: %c borrow %d\n", result[pos], borrow);
        }
        pos++;
    }

    if (borrow != 0) {
        free(result);
        return NULL;
    }

    result[pos] = '\0';
    reverse_string(result);

    if (debug_mode) {
        fprintf(stderr, "Final borrow: %d\n", borrow);
        fprintf(stderr, "Subtraction result %s - %s = %s\n", num1, num2, result);
    }

    return (char *)remove_leading_zeros(result);
}

const char *multiply_base_numbers(unsigned int base, const char *num1, const char *num2) {
    num1 = remove_leading_zeros(num1);
    num2 = remove_leading_zeros(num2);
    size_t len1 = strlen(num1);
    size_t len2 = strlen(num2);
    size_t result_length = len1 + len2;
    char *result = (char *)calloc(result_length + 1, sizeof(char));
    memset(result, '0', result_length);

    if (debug_mode) {
        fprintf(stderr, "Multiplying %u %s %s\n", base, num1, num2);
        fprintf(stderr, "Entering multiplication function\n");
    }

    int carry = 0;
    for (size_t i = 0; i < len2; i++) {
        int digit2 = get_digit_value(num2[len2 - 1 - i]);
        carry = 0;
        for (size_t j = 0; j < len1 || carry; j++) {
            int digit1 = (j < len1) ? get_digit_value(num1[len1 - 1 - j]) : 0;
            int product = digit2 * digit1 + get_digit_value(result[result_length - 1 - (i + j)]) + carry;
            carry = product / base;
            result[result_length - 1 - (i + j)] = convert_to_char(product % base);

            if (debug_mode) {
                fprintf(stderr, "Multiplication step: digit1 %d digit2 %d carry %d\n", digit1, digit2, carry);
                fprintf(stderr, "Result digit: %c carry %d\n", result[result_length - 1 - (i + j)], carry);
            }
        }
    }

    if (debug_mode) {
        fprintf(stderr, "Final carry: %d\n", carry);
        fprintf(stderr, "Multiplication result %s * %s = %s\n", num1, num2, result);
    }

    return remove_leading_zeros(result);
}

// Utility functions

unsigned int get_digit_value(char digit) {
    // Convert character digit to its integer value
    if (digit >= '0' && digit <= '9') {
        return digit - '0';
    }
    if (digit >= 'a' && digit <= 'z') {
        return 10 + (digit - 'a');
    }
    return -1;
}

char convert_to_char(unsigned int value) {
    // Convert integer value to corresponding digit character
    if (value >= TOTAL_DIGIT_COUNT) {
        abort_with_message("Invalid value for convert_to_char()");
        return 0;
    }
    return get_base_digits()[value];
}

char *reverse_string(char *str) {
    // Reverse the string in place
    size_t length = strlen(str);
    for (size_t i = 0; i < length / 2; ++i) {
        char temp = str[i];
        size_t mirror = length - i - 1;
        str[i] = str[mirror];
        str[mirror] = temp;
    }
    return str;
}

const char *remove_leading_zeros(const char *number) {
    // Skip leading zeros
    while (*number == '0') {
        ++number;
    }
    return (*number == '\0') ? number - 1 : number;
}

void abort_with_message(const char *message) {
    // Print error message and exit
    fprintf(stderr, message);
    exit(EXIT_FAILURE);
}
