#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static bool
print(const char *data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF) // Let's just print chars one by one
			return false;
	return true;
}

int
printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') { // Skips until a % is found
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} if (*format == 'd') {
			format++;
			int d = va_arg(parameters, int);

			bool negative = false;
			if (d < 0) {
				negative = true;
				d = -d;
			};

			const uint8_t str_len = 20;
			char str[str_len + 1]; // uint64_t can have up to 18 decimals in its decimal represantation. +1 for sign
			str[str_len] = '\0';
			uint8_t digit_count = 0;

			do {
				char c = d % 10;
				str[str_len - digit_count - 1] = '0' + c;
				d /= 10;
				digit_count++;
			} while(d > 0);
			
			if (negative) str[str_len - digit_count++ - 1] = '-';

			print(&str[str_len - digit_count], digit_count);

		} if (*format == 'x') {
			format++;
			uint32_t d = va_arg(parameters, uint32_t);

			const uint8_t str_len = 16;
			char str[str_len + 1]; // uint64_t can have up to 18 decimals in its decimal represantation. +1 for sign
			str[str_len] = '\0';
			uint8_t digit_count = 0;

			do {
				char c = d % 16;
				str[str_len - digit_count - 1] = (c < 10)?'0' + c:'A' + c - 10;
				d /= 16;
				digit_count++;
			} while(d > 0);
			
			print(&str[str_len - digit_count], digit_count);

		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
