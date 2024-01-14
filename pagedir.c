#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "crawler.h"
#include "pagedir.h"

bool pagedir_init(const char *pageDirectory) {
	/*
	struct stat st;
    if (stat(pageDirectory, &st) == 0) {
            return false;
    }
	
	if (mkdir(pageDirectory, 0777) != 0) {
        return false;
    }
	return true;
	*/
	char crawlerPath[256];
    snprintf(crawlerPath, sizeof(crawlerPath), "%s/.crawler", pageDirectory);

    // Create the .crawler folder
    if (mkdir(crawlerPath, 0777) == 0) {
        //printf("Created .crawler folder in '%s'.\n", pageDirectory);
	return true;
    } else {
        perror("Failed to create .crawler folder");
	return false;
    }
}


void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID) {
	char pageFile[1000];  // Assumes a maximum file path length of 1000 characters
    
    // Construct the pathname for the page file in pageDirectory
    snprintf(pageFile, sizeof(pageFile), "%s/%d", pageDirectory, documentID);
    
    // Open the file for writing
    FILE *file = fopen(pageFile, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing: %s\n", pageFile);
        return;
    }
    
    // Print the URL, depth, and contents of the webpage to the file
    fprintf(file, "%s\n%d\n%s\n", page->url, page->depth, page->html);
    
    // Close the file
    fclose(file);
}
