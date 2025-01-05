#ifndef R_IN_OUT_H
#define R_IN_OUT_H

#define MAX_FILENAME_SIZE 64

typedef enum RIO_FILE_OPEN_MODE {
	RIO_INVALID_MODE 	= 0x00000000,
	RIO_READ_MODE 		= 0x00000001,
	RIO_WRITE_MODE 		= 0x00000002
} RIO_FILE_OPEN_MODE;

typedef struct rio_file {
	char name[MAX_FILENAME_SIZE];
	void* handle;
	void* contents;
	unsigned int file_size; // in bytes
	int opening_mode;
} rio_file;

/**
 * Opens a file in case it already exists in the root directory, creates a file in case it doesn't exist
 * 	- filename: name of the file, make sure it is zero terminated.
 * 	- mode: opening mode, RIO_READ_MODE for reading contents, RIO_WRITE_MODE for writing contents.	
 * */
rio_file* rio_open_file(const char* filename, RIO_FILE_OPEN_MODE mode);

/**
 * Writes a string to the end of an opened file. If the file wasn't opened/created with 
 * 	the RIO_WRITE_MODE, nothing is written in memory
 * 	- file: rio_file struct returned from rio_open_file()
 * 	- contents: the string to be written
 * 	- count: quantity of characters from the string *contents*
 * */
int rio_write_file(rio_file* file, const void* contents, int count);

/**
 * Reads contents from an opened file. If the file wasn't opened/created with 
 * 	the RIO_READ_MODE, nothing gets read from memory
 * 	- file: rio_file struct returned from rio_open_file()
 * 	- dest: destination pointer for the contents
 * 	- count: quantity of characters to be read from the file
 * 	- offset: offset from the start of the file
 * */
int rio_read_file(rio_file* file, void* dest, int count, int offset);

/**
 * Verifies if a file name corresponds to an existing file.
 * 	- filename: name of the file to be found
 * */
int rio_file_exists(const char* filename);

/**
 * This library doesn't actually interface with I/O directly with functions
 * 	such as rio_read_file and rio_write_file. When we open the file (rio_open_file),
 * 	the bytes from the file are copied to the file.contents pointer, which means 
 * 	we're just operating in memory. To actually persist changes the file, you'll 
 * 	need to call this function.
 * 	- file: rio_file struct with valid file name and contents
 * */
int rio_save_changes(rio_file* file);

/**
 * Closes a file opened/created by the function rio_open_file()
 * 	- file: the handle returned from the function rio_open_file()
 * */
void rio_close_file(rio_file* file);

/**
 * Persists the contents of the rio_file struct and then closes a file opened/created by 
 * 	the function rio_open_file()
 * 	- file: the handle returned from the function rio_open_file()
 * */
int rio_save_and_close_file(rio_file* file);

#endif // R_IN_OUT_H