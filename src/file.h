/**
   @file file.h
   @brief Convenience functions to load data from files into memory
 */

#ifndef FILE_H
#define FILE_H

/**
   @brief Load a file from a given path.
   @param[in] path The path to load the file from.
   @param[out] data The data that the file will be loaded into (allocated in func).
   @param[out] data_len The final length of the data.
   @return Error codes:
   0 - SUCCESS
   1 - File not opened successfully
   2 - Could not ascertain file size (fseek/ftell error)
   3 - File was empty or fread error
   4 - Failed to allocate enough memory

   Loads the file using the standard C path conventions, relative to the
   working directory or absolute if beginning with a '/'.
   Will allocate enough memory using malloc to store the whole file in.
*/
int load_from_path(const char* path, char** data, long* data_len);


#endif
