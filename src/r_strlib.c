#include "r_strlib.h"

int rs_length(const char* s){
	int current_length = 0;
	while(*s++ && current_length < RS_STRING_MAX_LENGTH) current_length++;
	return current_length;
}

rs_string rs_create(const char* s){
	if(!s) return (rs_string){0};

	rs_string newString = {0};
	newString.length = rs_length(s);
	if(newString.length == RS_STRING_MAX_LENGTH) return (rs_string){0};
	
	newString.buffer = malloc(sizeof(char) * newString.length + 1);

	int currentIndex = 0;
	while(currentIndex < newString.length){
		newString.buffer[currentIndex] = s[currentIndex];
		currentIndex++;
	}
	newString.buffer[currentIndex] = '\0';

	return newString;
}

void rs_delete(rs_string* s){
	if(!s) return;
	if(s->buffer){
		free(s->buffer);
		s->buffer = 0;
		s->length = 0;
	}
}

int	rs_copy(rs_string* src_s, rs_string* dest_s){
	if(!src_s || !dest_s || !src_s->buffer) return RS_FAILURE;

	dest_s->length = src_s->length;

	if(dest_s->buffer) free(dest_s->buffer);
	
	dest_s->buffer = malloc(sizeof(char) * dest_s->length + 1);

	int i = 0;
	while(i < dest_s->length){
		dest_s->buffer[i] = src_s->buffer[i];
		i++;
	}
	dest_s->buffer[i] = '\0';

	return RS_SUCCESS;
}

int rs_concatenate(rs_string* dest_s, rs_string* str_to_append){
	if(!dest_s || !str_to_append){
		return RS_FAILURE;
	}
	if(!str_to_append->buffer){
		return RS_SUCCESS;
	}
	if(!dest_s->buffer){
		return rs_copy(str_to_append, dest_s);
	}

	char* temp = realloc(dest_s->buffer, dest_s->length + str_to_append->length + 1);
	if(!temp) return RS_FAILURE;
	
	dest_s->buffer = temp;

	int i = 0;
	while(i < str_to_append->length){
		dest_s->buffer[dest_s->length + i] = str_to_append->buffer[i];
		i++;
	}
	dest_s->buffer[dest_s->length + str_to_append->length] = '\0';
	dest_s->length += str_to_append->length;
	return RS_SUCCESS;
}

int rs_first_char_occurrence(char c, rs_string* s){
	if(!s || !s->buffer || s->length == 0) return -1;

	int currentIndex = 0;
	while(currentIndex < s->length && s->buffer[currentIndex] != '\0'){
		if(s->buffer[currentIndex] == c) return currentIndex;
		currentIndex++;
	}
	return -1;
}

int rs_is_char_in_string(char c, rs_string* s){
	return rs_first_char_occurrence(c, s) == -1 ? RS_FAILURE : RS_SUCCESS;
}

int rs_compare(rs_string* s1, rs_string* s2){
	if(!s1 || !s2 || !s1->buffer || !s2->buffer || s1->length != s2->length) return RS_FAILURE;

	int currentIndex = 0;
	while(currentIndex < s1->length){
		if(s1->buffer[currentIndex] != s2->buffer[currentIndex]) return RS_FAILURE;
		currentIndex++;
	}
	return RS_SUCCESS;
}

int rs_compare_to_cstr(rs_string* rs_str, char* cstr){
	if(!rs_str || !rs_str->buffer || !cstr) return RS_FAILURE;

	int cStrSize = rs_length(cstr);
	if(cStrSize != rs_str->length) return RS_FAILURE;

	int currentIndex = 0;
	while(currentIndex < rs_str->length){
		if(rs_str->buffer[currentIndex] != cstr[currentIndex]) return RS_FAILURE;
		currentIndex++;
	}
	return RS_SUCCESS;
}

int rs_compare_case_insensitive(rs_string* s1, rs_string* s2){
	if(!s1 || !s1->buffer || !s2 || !s2->buffer || s1->length != s2->length) return RS_FAILURE;
	
	int currentIndex = 0;
	while(currentIndex < s1->length){
		char c1 = s1->buffer[currentIndex];
		char c2 = s2->buffer[currentIndex];
		if(c1 != c2 && c1 - 32 != c2 && c1 != c2 - 32) return RS_FAILURE;
		currentIndex++;
	}

	return RS_FAILURE;
}

int rs_compare_to_cstr_case_insensitive(rs_string* rs_str, char* cstr){
	if(!rs_str || !rs_str->buffer || !cstr) return RS_FAILURE;

	int cStrSize = rs_length(cstr);
	if(cStrSize != rs_str->length) return RS_FAILURE;

	int currentIndex = 0;
	while(currentIndex < rs_str->length){
		char c1 = rs_str->buffer[currentIndex];
		char c2 = cstr[currentIndex];
		if(c1 != c2 && c1 - 32 != c2 && c1 != c2 - 32) return RS_FAILURE;
		currentIndex++;
	}
	return RS_SUCCESS;
}

int rs_extract(rs_string* s, int from, int to){
	if(!s || !s->buffer || from > s->length || to > s->length || to < from) return RS_FAILURE;

	int final_length = to - from + 1;
	int currentIndex = 0;
	while(currentIndex < final_length){
		s->buffer[currentIndex] = s->buffer[currentIndex + from];
		currentIndex++;
	}
	s->buffer[final_length] = '\0';
	s->length = final_length;
	return RS_SUCCESS;
}

int rs_extract_left(rs_string* s, int characterCount){
	return rs_extract(s, 0, characterCount - 1);
}

int rs_extract_right(rs_string* s, int characterCount){
	return rs_extract(s, s->length - characterCount, s->length - 1);
}

int rs_trim(rs_string* s){
	if(!s || !s->buffer) return RS_FAILURE;

	int startIndex = 0;
	int endIndex = s->length - 1;
	while(s->buffer[startIndex] == ' ' || s->buffer[endIndex] == ' '){
		if(s->buffer[startIndex] == ' ') startIndex++;
		if(s->buffer[endIndex] 	 == ' ') endIndex--;
	}

	rs_extract(s, startIndex, endIndex);
	return RS_SUCCESS;
}

int rs_trim_left(rs_string* s){
	if(!s || !s->buffer) return RS_FAILURE;

	int startIndex = 0;
	while(s->buffer[startIndex] == ' ') startIndex++;

	rs_extract(s, startIndex, s->length - 1);
	return RS_SUCCESS;
}

int rs_trim_right(rs_string* s){
	if(!s || !s->buffer) return RS_FAILURE;

	int endIndex = s->length - 1;
	while(s->buffer[endIndex] == ' ') endIndex--;

	rs_extract(s, 0, endIndex);
	return RS_SUCCESS;
}


int rs_convert_upper(rs_string* s){
	if(!s || !s->buffer) return RS_FAILURE;
	int currentIndex = 0;
	while(currentIndex < s->length){
		if(rs_is_lower(s->buffer[currentIndex])) s->buffer[currentIndex] -= 32;
		currentIndex++;
	}
	return RS_SUCCESS;
}

int rs_convert_lower(rs_string* s){
	if(!s || !s->buffer) return RS_FAILURE;
	int currentIndex = 0;
	while(currentIndex < s->length){
		if(rs_is_upper(s->buffer[currentIndex])) s->buffer[currentIndex] += 32;
		currentIndex++;
	}
	return RS_SUCCESS;
}

int rs_is_upper(char c){
	return ('A' <= c && c <= 'Z') ? RS_SUCCESS : RS_FAILURE;
}

int rs_is_lower(char c){
	return ('a' <= c && c <= 'z') ? RS_SUCCESS : RS_FAILURE;
}

int rs_is_letter(char c){
	return (rs_is_upper(c) || rs_is_lower(c)) ? RS_SUCCESS : RS_FAILURE;
}

int rs_is_digit(char c){
	return ('0' <= c && c <= '9') ? RS_SUCCESS : RS_FAILURE;
}

int rs_count_letters(rs_string* s){
	if(!s || !s->buffer) return 0;
	int count = 0;

	int currentIndex = 0;
	while(currentIndex < s->length){
		if(rs_is_letter(s->buffer[currentIndex])) count++;
		currentIndex++;
	}

	return count;
}

int rs_count_digits(rs_string* s){
	if(!s || !s->buffer) return 0;
	int count = 0;

	int currentIndex = 0;
	while(currentIndex < s->length){
		if(rs_is_digit(s->buffer[currentIndex])) count++;
		currentIndex++;
	}

	return count;
}

int rs_split_by_delimiter(rs_string* s, char delimiter, rs_string* token){
	if(!s || !s->buffer || !token) return RS_FAILURE;

	int currentIndex = 0;
	while(s->buffer[currentIndex] != delimiter && s->buffer[currentIndex] != '\0') 
		currentIndex++;
	if(currentIndex <= s->length - 1){
		if(currentIndex > token->length){
			char* temp = realloc(token->buffer, currentIndex + 1);
			if(!temp) return RS_FAILURE;
			token->buffer = temp;
		}
		//we don't need to reallocate if we want to shrink, just use the same buffer and terminate it with '\0'

		int copyIndex = 0;
		while(copyIndex < currentIndex){
			token->buffer[copyIndex] = s->buffer[copyIndex];
			copyIndex++;
		}
		token->buffer[currentIndex] = '\0';
		token->length = currentIndex;
		int extraction_result = rs_extract_right(s, s->length - (currentIndex + 1));
		if(extraction_result == RS_FAILURE){
			s->buffer[0] = '\0';
			s->length = 0;
		}
		return 1;
	}
	token->buffer = 0;
	token->length = 0;
	return RS_FAILURE; //coudn't tokenize
}

int rs_reverse(rs_string* s){
	if(!s || !s->buffer) return RS_FAILURE;
	if(s->length == 1 || s->length == 0) return RS_SUCCESS;

	char aux = 0;
	int startIndex = 0;
	int endIndex = s->length - 1;

	while(startIndex < endIndex){
		aux = s->buffer[startIndex];
		s->buffer[startIndex] = s->buffer[endIndex];
		s->buffer[endIndex] = aux;
		startIndex++;
		endIndex--;
	}
	return RS_SUCCESS;
}

int rs_convert_to_float(rs_string* s, float* n){
	if(!s || !n || !s->buffer || rs_count_letters(s) != 0) return RS_FAILURE;

	*n = 0.0f;
	int dot_index = rs_first_char_occurrence('.', s);

	// convert integer part (0 to dot_index - 1)
	for (int i = 0; i < dot_index; ++i) {
		*n = *n * 10 + (float)(s->buffer[i] - '0');
	}

	// convert decimal part (dot_index + 1 to s->length - 1)
	float decimal_power_of_10 = 10.0f;
	for (int i = dot_index + 1; i < s->length; ++i) {
		float digit = (float)(s->buffer[i] - '0');
		*n += digit / decimal_power_of_10;
		decimal_power_of_10 *= 10;
	}

	return RS_SUCCESS;
}

int rs_convert_to_int(rs_string* s, int* n){
	if(!s || !n || !s->buffer || rs_count_letters(s) != 0) return RS_FAILURE;

	*n = 0.0f;

	int dot_index = rs_first_char_occurrence('.', s);
	int index_limit = s->length;
	if(dot_index != -1){
		index_limit = dot_index;
	}

	// convert integer part
	for (int i = 0; i < index_limit; ++i) {
		*n = *n * 10 + (s->buffer[i] - '0');
	}

	return RS_SUCCESS;
}

int rs_find_substring(rs_string* s, const char* cstr){
	int substring_size = rs_length(cstr);
	
	if(!s || !cstr || !s->buffer || substring_size == 0) return -1;

	for(int i = 0; i < s->length; i++){
		// try to find the first character in the string
		if(s->buffer[i] == cstr[0]){
			// Once found, check if the next substring_size - 1 characters from cstr match
			int result_index = i; // saving current i in case we found the substring, then we return the first character index
			int j;
			for (j = 1; j < substring_size; ++j) {
				if(i + j >= s->length){
					// In case we're out of bounds of the strings
					// It means we didn't find the substring before
					// 		so the substring isn't in the string
					return -1;
				} else if (s->buffer[i + j] == cstr[j]) {
					// If the characters match, keep going to see if the other characters match too
					continue;
				} else {
					// If the characters don't match, just break out of the loop and look for the next time the first character is found in the string
					break;
				}
			}
			if(j == substring_size){
				return result_index;
			}
		}
	}
	return -1;
}

int rs_starts_with_substring(rs_string* s, char* cstr){
	int substring_size = rs_length(cstr);
	
	if(!s || !cstr || !s->buffer || substring_size == 0) return RS_FAILURE;

	for (int i = 0; i < substring_size; ++i) {
		if(s->buffer[i] != cstr[i]){
			return RS_FAILURE;
		}
	}

	return RS_SUCCESS;
}

int rs_ends_with_substring(rs_string* s, char* cstr){
	int substring_size = rs_length(cstr);
	
	if(!s || !cstr || !s->buffer || substring_size == 0) return RS_FAILURE;

	for (int i = 0; i < substring_size; ++i) {
		if(s->buffer[s->length - substring_size + i] != cstr[i]){
			return RS_FAILURE;
		}
	}

	return RS_SUCCESS;
}

int rs_trim_delimiter(rs_string* s, char delimiter) {
	if(!s || !s->buffer) return RS_FAILURE;

	int startIndex = 0;
	int endIndex = s->length - 1;
	while(s->buffer[startIndex] == delimiter || s->buffer[endIndex] == delimiter){
		if(s->buffer[startIndex] == delimiter) startIndex++;
		if(s->buffer[endIndex] 	 == delimiter) endIndex--;
	}

	rs_extract(s, startIndex, endIndex);
	return RS_SUCCESS;
}

// This assumes that dest is valid of course, you'll need enough memory before hand
int rs_copy_to_cstr(rs_string* src_s, char* dest, int dest_length) {
	if(!src_s || !dest || !src_s->buffer || dest_length < src_s->length) return RS_FAILURE;

	int i = 0;
	while(i < dest_length){
		dest[i] = src_s->buffer[i];
		i++;
	}
	dest[i] = '\0';

	return RS_SUCCESS;	
}

int rs_replace_character(rs_string* s, const char char_to_replace, const char replacement) {
	if(!s || !s->buffer) return RS_FAILURE;

	for(int i = 0; i < s->length; i++){
		if(s->buffer[i] == char_to_replace){
			s->buffer[i] = replacement;
		}
	}

	return RS_SUCCESS;
}

int rs_replace_substring(rs_string* s, const char* substring_to_replace, const char* substring_replacement) {
	int substring_to_replace_size = rs_length(substring_to_replace);
	int substring_replacement_size = rs_length(substring_replacement);
	int substring_to_replace_index = rs_find_substring(s, substring_to_replace);
	if(!s || !s->buffer || !substring_to_replace_size || !substring_replacement || substring_to_replace_index < 0) return RS_FAILURE;

	int size_diff = substring_to_replace_size - substring_replacement_size;

	char huge_buffer[RS_STRING_MAX_LENGTH] = {0};

	if(size_diff == 0){
		do {
			int i = substring_to_replace_index;

			for (int _i = 0; _i < substring_replacement_size; ++_i) {
				s->buffer[i + _i] = substring_replacement[_i];
			}

			substring_to_replace_index = rs_find_substring(s, substring_to_replace);
		} while (substring_to_replace_index >= 0);
	} else if(size_diff > 0){
		// The substring inside rstring is bigger than the replacement
		// Example: string = "Hello World", to_replace = "Hello W", replacement = "To", to_replace is bigger than replacement
		do {
			int i = substring_to_replace_index;

			for (int _i = 0; _i < substring_replacement_size; ++_i) {
				s->buffer[i + _i] = substring_replacement[_i];
			}

			// Here we still have size_diff characters from the old substring, we need to copy characters over to fill this occupied space
			for (int _i = 0; _i < s->length - substring_to_replace_size; ++_i) {
				s->buffer[substring_replacement_size + _i] = s->buffer[substring_replacement_size + size_diff + _i];
			}
			s->length -= size_diff;

			substring_to_replace_index = rs_find_substring(s, substring_to_replace);
		} while (substring_to_replace_index >= 0);
	} else {
		return RS_FAILURE;
		// TODO(Rick): Finish implementing rs_replace_substring
		// we can call rs_count_substring here to estimate how much more memory we'll need to reallocate
		// The substring inside rstring is smaller than the replacement
		// Example: string = "Hello World", to_replace = "Hello", replacement = "Hello123", to_replace is smaller than replacement

		// TODO(Rick): try to come up with a "huge buffer" solution to this, if possible, not obligatory

		// int substring_count = rs_count_substring(s, substring_to_replace);
		// int new_memory_estimate = substring_count * (size_diff) + s->length;
		// char* temp = realloc(s->buffer, new_memory_estimate);
		// if(!temp) return RS_FAILURE;
		// s->buffer = temp;

		// do {
		// 	int i = substring_to_replace_index;

		// 	for (int _i = 0; _i < substring_replacement_size; ++_i) {
		// 		s->buffer[i + _i] = substring_replacement[_i];
		// 	}
		// } while (substring_to_replace_index >= 0);
	}
	return RS_SUCCESS;
}

int rs_split_by_substring(rs_string* s, char* substring_delimiter, rs_string* token) {
	// TODO(Rick): Finish implementing rs_split_by_substring
	return RS_FAILURE;
}

int rs_remove_delimiter(rs_string* s, char delimiter) {
	if(!s || !s->buffer) return RS_FAILURE;
	
	char buffer[RS_STRING_MAX_LENGTH] = {0};

	int i = 0;
	int j = 0;
	while(i < s->length){
		if(s->buffer[i] != delimiter){
			buffer[j] = s->buffer[i];
			j++;
		}
		i++;
	}

	if(!rs_set(s, buffer)) return RS_FAILURE;
	
	return RS_SUCCESS;
}

int rs_count_character(rs_string* s, const char character) {
	if(!s || !s->buffer) return -1;

	int result = 0;
	for (int i = 0; i < s->length; ++i){
		if (s->buffer[i] == character){
			result++;
		}
	}

	return result;
}

int rs_count_substring(rs_string* s, const char* substring) {
	int substring_length = rs_length(substring);
	if(!s || !s->buffer || !substring || substring_length == 0) return -1;

	rs_string temp = rs_create(NULL);
	if(!rs_copy(s, &temp)) return -1;

	int substring_count = 0;
	int substring_index = rs_find_substring(&temp, substring);
	while(substring_index != -1){
		substring_count++;
		// Just incrementing the character value so the next rs_find_substring 
		// 		looks for a different place in the rstring to find the substring
		// If we didn't do this, rs_find_substring would return the same index everytime
		temp.buffer[substring_index] = temp.buffer[substring_index] + 1;
		substring_index = rs_find_substring(&temp, substring);
	}

	rs_delete(&temp);
	return substring_count;
}

int rs_set(rs_string* s, const char* string){
	int string_length = rs_length(string);
	if(!s || !s->buffer || string_length == 0) return RS_FAILURE;

	if(s->length < string_length) {
		char* temp = realloc(s->buffer, string_length);
		if(!temp) return RS_FAILURE;
		s->buffer = temp;
	}

	int i = 0;
	while(i < string_length){
		s->buffer[i] = string[i];
		i++;
	}
	s->buffer[i] = '\0';
	s->length = string_length;
	return RS_SUCCESS;
}

// For strings like "0xFFFFFFFF" or "0xCDC"
// TODO(Rick): Finish implementing rs_convert_from_hex_to_unsigned_int
int rs_convert_from_hex_to_unsigned_int(rs_string* s, unsigned int* n) {
	return RS_FAILURE;
}