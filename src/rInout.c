#include "rInout.h"

#define _rio_min(a, b) ((a) > (b) ? (b) : (a))

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include <windows.h>

#define RIO_WINDOWS_DEFAULT_FILE_CONTENT_SIZE 4096

rio_file* rio_open_file(const char* filename, RIO_FILE_OPEN_MODE mode){
	DWORD opening_mode = 0;

	if(mode & RIO_READ_MODE)  { 
		opening_mode |= GENERIC_READ;  
	}
	if(mode & RIO_WRITE_MODE) { 
		opening_mode |= GENERIC_WRITE; 
	}

	rio_file* result_file = (rio_file*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(rio_file));

	result_file->handle = CreateFileA(filename, opening_mode, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	int min_size = _rio_min(lstrlenA(filename), MAX_FILENAME_SIZE);
	CopyMemory(result_file->name, filename, min_size);
	result_file->file_size = GetFileSize(result_file->handle, NULL);
	result_file->opening_mode = mode;
	if(result_file->file_size == 0){
		// means we just created the file, didn't open an existing one
		result_file->contents = VirtualAlloc(0, RIO_WINDOWS_DEFAULT_FILE_CONTENT_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		return result_file;
	}else{
		result_file->contents = VirtualAlloc(0, result_file->file_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	}
	
	if(result_file->contents == NULL) { 
		return NULL; 
	}

	DWORD number_of_bytes_read = 0;
	ReadFile(result_file->handle, result_file->contents, result_file->file_size, &number_of_bytes_read, NULL);

	if(number_of_bytes_read != result_file->file_size) { 
		return NULL; 
	}
	
	return result_file;
}

int rio_write_file(rio_file* file, const void* contents, int count){
	if(!(file->opening_mode & RIO_WRITE_MODE)) {
		return 0;
	}
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	int number_of_pages_now = (int)(file->file_size / sys.dwPageSize) + 1;
	int number_of_pages_needed = (int)((file->file_size + count) / sys.dwPageSize) + 1;
	if(number_of_pages_needed > number_of_pages_now){
		// If we need more pages, we'll allocated more memory
		BOOL result_free = VirtualFree(file->contents, 0, MEM_RELEASE);
		file->contents = VirtualAlloc(0, file->file_size + count, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if(!result_free | (file->contents == NULL)){
			return 0;
		}
	}

	// appending contents to the end of file->contents
	CopyMemory((char*)(file->contents) + file->file_size, contents, count);

	file->file_size += count;

	return 1;
}

int rio_read_file(rio_file* file, void* dest, int count, int offset){
	if((unsigned int)count > (file->file_size - offset) || !(file->opening_mode & RIO_READ_MODE)){
		return 0;
	}
	CopyMemory(dest, (char*)(file->contents) + offset, count);
	return 1;
}

int rio_file_exists(const char* filename){
	return (lstrlenA(filename) < 256) && (GetFileAttributesA(filename) != INVALID_FILE_ATTRIBUTES);
}

void rio_close_file(rio_file* file){
	CloseHandle(file->handle);
	file->handle = NULL;
	VirtualFree(file->contents, 0, MEM_RELEASE);
	file->contents = NULL;
	file->name[0] = 0;
	file->file_size = 0;
	file->opening_mode = RIO_INVALID_MODE;
}

int rio_save_changes(rio_file* file){
	if(!(file->opening_mode & RIO_WRITE_MODE)){
		return 0;
	}
	DWORD bytes_written = 0;
	BOOL result_write = WriteFile(file->handle, file->contents, file->file_size, &bytes_written, NULL);
	if(result_write && bytes_written == file->file_size){
		return 1;
	}
	return 0;
}

int rio_save_and_close_file(rio_file* file){
	if(rio_save_changes(file)){
		rio_close_file(file);
		return 1;
	}
	return 0;
}

#elif defined(__linux__) || defined(__gnu_linux__)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define RIO_LINUX_DEFAULT_FILE_CONTENT_SIZE 4096

rio_file* rio_open_file(const char* filename, RIO_FILE_OPEN_MODE mode){
	char opening_mode[4] = {0};

	opening_mode[0] = (mode & RIO_WRITE_MODE) ? 'w' : 'r';
	opening_mode[1] = ((mode & RIO_READ_MODE) ^ (mode & RIO_WRITE_MODE)) ? 'b' : '+';
	opening_mode[2] = ((mode & RIO_READ_MODE) && (mode & RIO_WRITE_MODE)) ? 'b' : '\0';

	rio_file* result_file = (rio_file*)calloc(1, sizeof(rio_file));
	result_file->handle = fopen(filename, opening_mode);

	int min_size = _rio_min(strlen(filename), MAX_FILENAME_SIZE);
	memcpy(result_file->name, filename, min_size);
	
	// file size
	fseek((FILE*)result_file->handle, 0, SEEK_END);
	size_t file_size = ftell((FILE*)result_file->handle);
	result_file->file_size = file_size;
	fseek((FILE*)result_file->handle, 0, SEEK_SET);
	
	result_file->opening_mode = mode;
	if(result_file->file_size == 0){
		// means we just created the file, didn't open an existing one
		result_file->contents = calloc(1, RIO_LINUX_DEFAULT_FILE_CONTENT_SIZE);
		return result_file;
	}else{
		int pages_needed = (result_file->file_size / RIO_LINUX_DEFAULT_FILE_CONTENT_SIZE) + 1;
		result_file->contents = calloc(1, pages_needed * RIO_LINUX_DEFAULT_FILE_CONTENT_SIZE);
	}
	
	if(result_file->contents == NULL) { 
		return NULL; 
	}

	unsigned int number_of_bytes_read = fread(result_file->contents, 1, result_file->file_size, (FILE*)result_file->handle);

	if(number_of_bytes_read != result_file->file_size) { 
		return NULL; 
	}
	
	return result_file;
}

int rio_write_file(rio_file* file, const void* contents, int count){
	if(!(file->opening_mode & RIO_WRITE_MODE)) {
		return 0;
	}

	long page_size = sysconf(_SC_PAGESIZE);
	int number_of_pages_now = (int)(file->file_size / page_size) + 1;
	int number_of_pages_needed = (int)((file->file_size + count) / page_size) + 1;
	if(number_of_pages_needed > number_of_pages_now){
		// If we need more pages, we'll allocated more memory
		free(file->contents);
		file->contents = malloc(file->file_size + count);
		if(file->contents == NULL){
			return 0;
		}
	}

	// appending contents to the end of file->contents
	memcpy((char*)(file->contents) + file->file_size, contents, count);
	file->file_size += count;

	return 1;
}

int rio_read_file(rio_file* file, void* dest, int count, int offset){
	if((unsigned int)count > (file->file_size - offset) || !(file->opening_mode & RIO_READ_MODE)){
		return 0;
	}
	memcpy(dest, (char*)(file->contents) + offset, count);
	return 1;
}

int rio_file_exists(const char* filename){
	return (strlen(filename) < 256) && (access(filename, F_OK) == 0);
}

int rio_save_changes(rio_file* file){
	if(!(file->opening_mode & RIO_WRITE_MODE)){
		return 0;
	}
	unsigned int bytes_written = 0;
	int result_write = fwrite(file->contents, file->file_size, 1, (FILE*)file->handle);
	if(result_write && bytes_written == file->file_size){
		return 1;
	}
	return 0;
}

void rio_close_file(rio_file* file){
	fclose((FILE*)file->handle);
	file->handle = NULL;
	free(file->contents);
	file->contents = NULL;
	file->name[0] = 0;
	file->file_size = 0;
	file->opening_mode = RIO_INVALID_MODE;	
}

int rio_save_and_close_file(rio_file* file){
	if(rio_save_changes(file)){
		rio_close_file(file);
		return 1;
	}
	return 0;
}

#endif