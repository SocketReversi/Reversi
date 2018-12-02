#include <stdio.h>
#include <stdlib.h>
#include "../../libs/file.h"

FILE *open(char *filename, char *action) {
	FILE *file = fopen(filename, action);

	if (file == NULL) {
		printf("File Can\'t Open\n");
		return NULL;
	} else {
		return file;
	}
}

// void *writeFile(FILE *file, LinkList *head) {
// 	if (file == NULL) {
// 		printf("File can not found\n");
// 	} else {
// 		LinkList *current = head;

// 		fseek(file, 0, SEEK_SET);

// 		while (current->next != NULL) {
// 			fprintf(file, "%s %s %d", current->username, current->password, current->status);

// 			if (current->next != NULL) {
// 				fprintf(file, "\n");
// 			}

// 			current = current->next;
// 		}
// 	}
// }
