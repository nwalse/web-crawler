#ifndef CURL_WRAPPER_H
#define CURL_WRAPPER_H

#include <stddef.h>

/**
 * This function attempts to download a webpage from a given URL. If it succeeds, it returns a
 * pointer to a new string containing the contents of the webpage and sets the value at size_out to
 * the length of the webpage contents. Otherwise, it returns null.
 *
 * Be sure to free the return value of this function when you're done with it.
 */
char * download(const char *url, size_t *size_out);

#endif
