#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

bool contains_non_ascii(const char *str) {
    while (*str) {
        if ((unsigned char)*str > 127) {
            return true;
        }
        str++;
    }
    return false;
}

void toupperstr(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int rand_lim(int limit) {
    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: c-wordle /path/to/wordlist");
        return EXIT_FAILURE;
    }
    
    srand(time(NULL));
    FILE *file_ptr = fopen(argv[1], "r");
    if (!file_ptr) {
        perror("Failed to open file.");
        return EXIT_FAILURE;
    }

    size_t cap = 1024;
    char **words = malloc(cap * sizeof(char *));

    char buf[128];
    size_t count = 0;
    while (fgets(buf, sizeof(buf), file_ptr)) {
        buf[strcspn(buf, "\n")] = '\0';
        if (!strstr(buf, "'") && !contains_non_ascii(buf) && strlen(buf) == 5) {
            if (count >= cap) {
                cap *= 2;
                char **temp = realloc(words, cap * sizeof(char *));
                words = temp;
            }

            words[count] = malloc(strlen(buf) + 1);
            toupperstr(buf);
            strcpy(words[count], buf);
            
            count++;
        }
    }
    fclose(file_ptr);

    int word_index = rand_lim(count - 1);

    printf("%s", words[word_index]);

    for (size_t i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
    return 0;
}