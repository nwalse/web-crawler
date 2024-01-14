#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

static char *curl_buffer = NULL;
static size_t curl_buffer_size = 0;
static size_t curl_buffer_capacity = 128;
static int download_init_called = 0;

static size_t write_cb(char *in, size_t size, size_t mem_count, void *userdata) {
	const size_t byte_count = size * mem_count;

	if (curl_buffer == NULL) {
		curl_buffer = calloc(curl_buffer_capacity, sizeof(char));
		if (curl_buffer == NULL) {
			fprintf(stderr, "calloc returned null!\n");
			exit(10);
		}
	}

	size_t req_capacity = curl_buffer_size + byte_count;
	size_t new_capacity = curl_buffer_capacity;

	while (new_capacity < req_capacity)
		new_capacity *= 2;

	if (new_capacity != curl_buffer_capacity) {
		curl_buffer = realloc(curl_buffer, new_capacity);
		if (curl_buffer == NULL) {
			fprintf(stderr, "realloc returned null!\n");
			exit(11);
		}

		curl_buffer_capacity = new_capacity;
	}

	memcpy(curl_buffer + curl_buffer_size, in, byte_count);
	curl_buffer_size += byte_count;
	return byte_count;
}

static void cleanup_download(void) {
	free(curl_buffer);
}

static void download_init(void) {
	atexit(cleanup_download);
}

char * download(const char *url, size_t *size_out) {
	if (!download_init_called) {
		download_init();
		download_init_called = 1;
	}

	CURL *curl = curl_easy_init();
	char error_buffer[CURL_ERROR_SIZE];
	curl_buffer_size = 0;

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1l);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0l);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36");

	int err = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (err)
		return NULL;

	if (size_out != NULL)
		*size_out = curl_buffer_size;
	return strndup(curl_buffer, curl_buffer_size);
}
