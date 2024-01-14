#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "crawler.h"
#include "curl.h"
#include "pagedir.h"
#include "url.h"



/**
 * Parses command-line arguments, placing the corresponding values into the pointer arguments seedURL,
 * pageDirectory and maxDepth. argc and argv should be passed in from the main function.
 */
static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth) {
	if (argc < 4) {
        fprintf(stderr, "Insufficient number of arguments\n");
        exit(1);
    }

    *seedURL = argv[1];
    *pageDirectory = argv[2];
    *maxDepth = atoi(argv[3]);
}



char* extractURL(char* str, int startIndex) {
    int len = strlen(str);
    char* url = NULL;

    for (int i = startIndex; i < len-9; i++) {
		if (strncmp(&str[i], "<a href=\"", 9) == 0) {
            int urlStart = i + 9;
            int urlEnd = urlStart;
            while (str[urlEnd] != '\"' && urlEnd < len) {
                urlEnd++;
            }

            int urlLength = urlEnd - urlStart;
            if (urlLength > 0) {
                url = (char*)malloc((urlLength + 1) * sizeof(char));
                strncpy(url, &str[urlStart], urlLength);
                url[urlLength] = '\0';
                break;
            }
        }
    }

    return url;
}


int findLinkStartIndex(const char* html, int position) {
    const char* start = strstr(html + position, "<a href=\"");
    if (start == NULL)
        return -1;

    return start - html + strlen("<a href=\"");
}

/**
 * Scans a webpage for URLs.
 */
static void pageScan(webpage_t *page, set_t *pagesToCrawl, hashtable_t *pagesSeen) {
	
	int pos = 0;
	char *nurl = NULL;
	printf("%d Fetched: %s\n", page->depth, page->url);
    printf("%d Scanning: %s\n", page->depth, page->url);
	
	while(extractURL(page->html, pos) != NULL){
		nurl = normalizeURL(page->url, extractURL(page->html, pos));
	
		
		if(isInternalURL(page->url, nurl) &&  strlen(extractURL(page->html, pos)) !=1 ){
			printf("%d  Found: %s\n",page->depth, nurl);
			
			if(hashtable_find(pagesSeen, nurl) == NULL){
				printf("%d  Added: %s\n",page->depth, nurl);
				hashtable_insert(pagesSeen, nurl, nurl);
				webpage_t *np = webpage_new();
				np->url = nurl;
				np->html = download(nurl, NULL);
				np->depth = page->depth +1;
				set_insert(pagesToCrawl, nurl, np);
				pagesToCrawl = pagesToCrawl->next;
				//printf("%s\n", nurl);
			}	
		}
		if( extractURL(page->html, pos) != NULL && findLinkStartIndex(page->html, pos)!= -1 )
			pos = strlen(extractURL(page->html, pos)) + findLinkStartIndex(page->html, pos);
		else {
			break;
		}	
	}
	free(nurl);
}



/**
 * Crawls webpages given a seed URL, a page directory and a max depth.
 */
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth) {

    hashtable_t *pagesSeen = hashtable_create();
    hashtable_insert(pagesSeen, seedURL, seedURL);

    webpage_t *page = webpage_new();
    char *link = seedURL;
    page->url = strdup(link);
    char *html = download(page->url, &page->length);
    page->html = strdup(html);

    set_t *pagesToCrawl = set_new();
    set_insert(pagesToCrawl, seedURL, page);
	
    pagedir_init(pageDirectory);
    int documentID = 0;
	
	//pagedir_save(page, pageDirectory, documentID);
	//set_print(pagesToCrawl);
	//hashtable_print(pagesSeen);
	set_t *head = pagesToCrawl;
	
	while(head->next != NULL){
		
		documentID++;
		//printf("adding %s\n", head->key);
		pagedir_save(head->item, pageDirectory, documentID);
        webpage_t *pg;
		pg  = head->item;
		//printf("%d\n", pg->depth);
		
        if(pg->depth < maxDepth){
			webpage_t *pgc;
			pgc = head->item;
			//printf("page scanning %s\n", pgc->url);
            
			pageScan(head->item,pagesToCrawl,pagesSeen);
		}
		head = head->next;
        //webpage_delete(pg);
    }
	
    //hashtable_delete(pagesSeen);
    //set_delete(pagesToCrawl);
	
    //free(html);
}




int main(const int argc, char *argv[]) {
	
	char *seedURL;
	char *pageDirectory;
	int maxDepth;
	
	parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
	crawl(seedURL, pageDirectory, maxDepth);
	
	return 0;
}
