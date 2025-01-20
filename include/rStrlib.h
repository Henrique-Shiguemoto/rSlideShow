#ifndef RSTRLIB_H
#define RSTRLIB_H

#include <stdlib.h>

#define RS_STRING_MAX_LENGTH 64000

#define RS_SUCCESS 1
#define RS_FAILURE 0

typedef struct rs_string{
	int length;
	char* buffer;
} rs_string;

int rs_length(const char* s);
rs_string rs_create(const char* s);
void rs_delete(rs_string* s);
int rs_set(rs_string* s, const char* string);
int rs_copy(rs_string* src_s, rs_string* dest_s);
int rs_copy_to_cstr(rs_string* src_s, char* dest, int dest_length);
int rs_concatenate(rs_string* dest, rs_string* str_to_append);
int rs_first_char_occurrence(rs_string* s, char c);
int rs_first_substring_ocurrence(rs_string* s, const char* cstr);
int rs_is_char_in_string(rs_string* s, char c);
int rs_count_character(rs_string* s, const char character);
int rs_count_substring(rs_string* s, const char* substring);
int rs_count_letters(rs_string* s);
int rs_count_digits(rs_string* s);
int rs_compare(rs_string* s1, rs_string* s2);
int rs_compare_to_cstr(rs_string* rs_str, char* cstr);
int rs_compare_case_insensitive(rs_string* s1, rs_string* s2);
int rs_compare_to_cstr_case_insensitive(rs_string* rs_str, char* cstr);
int rs_extract(rs_string* s, int from, int to);
int rs_extract_left(rs_string* s, int characterCount);
int rs_extract_right(rs_string* s, int characterCount);
int rs_remove_delimiter(rs_string* s, char delimiter);
int rs_trim(rs_string* s);
int rs_trim_left(rs_string* s);
int rs_trim_right(rs_string* s);
int rs_trim_delimiter(rs_string* s, char delimiter);
int rs_convert_upper(rs_string* s);
int rs_convert_lower(rs_string* s);
int rs_convert_to_float(rs_string* s, float* n);
int rs_convert_to_int(rs_string* s, int* n);
int rs_convert_hex_to_uint(rs_string* s, unsigned int* n);
int rs_reverse(rs_string* s);
int rs_is_upper(char c);
int rs_is_lower(char c);
int rs_is_letter(char c);
int rs_is_digit(char c);
int rs_split_by_delimiter(rs_string* s, char delimiter, rs_string* token);
int rs_split_by_substring(rs_string* s, char* substring_delimiter, rs_string* token);
int rs_starts_with_substring(rs_string* s, char* cstr);
int rs_ends_with_substring(rs_string* s, char* cstr);
int rs_replace_character(rs_string* s, const char char_to_replace, const char replacement);
int rs_replace_substring(rs_string* s, const char* substring_to_replace, const char* substring_replacement);

#endif // RSTRLIB_H