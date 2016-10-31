/*
*	wc.c
*
*	Written by: Matthew Carrington-Fair
*
*	This program counts the number of words individually in the number of text
*	files provided as command line arguments. It creates a new pthread for
*	each text file and when it rejoins the main process, reports the numbe of
*	words in the file, and sums it to a running total.
*
*
*
*/
/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

/* Global for white space*/
static int WHITE_SPACE = 32;

/* Struct for each thread, storing the name of the file and its word count */
struct textfile {
	char *filename;
	int *word_count;
};

/* The function passed to each thread */
void *count_words(void *file);

int main(int argc, char *argv[]) {
	/* How many files to search through*/
	if (argc < 2) {
		fprintf(stderr, "Please provide one or more text files\n");
		exit(EXIT_FAILURE);
	}
	int file_num = argc - 1;
	int i;
	pthread_t threads[file_num];
	struct textfile *thread_info;
	int total_count = 0;
	/* Creates each thread for how many files there are */
	for (i = 0; i < file_num; i++) {
		if (pthread_create(&threads[i], 
			NULL, 
			count_words,
			(void *)argv[i + 1])) {
			
			fprintf(stderr, "Error making threads\n");
			exit(EXIT_FAILURE);
			}
	}
	/* Rejoins the threads, giving output for word count and adding to total */
	for (i = 0; i < file_num; i++) {
		pthread_join(threads[i], (void *)&thread_info);
		printf("%4d %s\n", *(thread_info->word_count), thread_info->filename);
		total_count += *(thread_info->word_count);
		free(thread_info->word_count);
		free(thread_info);
	}
	printf("%4d total\n", total_count);

}
/* Function passed to each thread for summing total word count */
void *count_words(void *file) {
	/* Stores filename & word count when returning */
	struct textfile *thread_info = (struct textfile *)malloc(sizeof(*thread_info));
	assert(thread_info);
	FILE *fp = fopen((char *)file, "r");
	int *word_count = (int *)malloc(sizeof(*word_count));
	assert(word_count);
	*word_count = 0;
	int c;
	int prev = 0;
	/* Loops through each file */
	while ((c = fgetc(fp)) != EOF) {
		/* Will indicate it's a word if hits whitespace or CRLS*/
		if (c == WHITE_SPACE || c == '\r' || c == '\n') {
			if (prev != WHITE_SPACE && prev != '\r' && prev != '\n') {
				(*word_count)++;
			}
		}
		/* Keeps a previous indicator to check if double whitespace etc*/
		prev = c;
	}
	fclose(fp);
	(*word_count)++;
	thread_info->word_count = word_count;
	thread_info->filename = (char *)file;
	return (void *)thread_info;
}