/* c-wordle: a simple wordle clone written in C.
    Copyright (C) 2025  Leo Lifely

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define VERSION "v1.0.0"

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
    int divisor = RAND_MAX / (limit + 1);
    int retval;
    do { 
        retval = rand() / divisor;
    } while (retval > limit);
    return retval;
}

void print_colored_guess(const char *guess, const char *target_word) {
    for (size_t i = 0; i < 5; i++) {
        if (guess[i] == target_word[i]) {
            // Green for correct position
            printf("\033[1;32m%c", guess[i]);
        } else {
            bool found = false;
            for (size_t j = 0; j < 5; j++) {
                if (guess[i] == target_word[j] && guess[j] != target_word[j]) {
                    found = true;
                    break;
                }
            }
            if (found) {
                // Yellow
                printf("\033[1;33m%c", guess[i]);
            } else {
                // Gray
                printf("\033[1;90m%c", guess[i]);
            }
        }
    }
    printf("\033[0m"); // Reset colors
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("c-wordle %s\nUsage: c-wordle /path/to/wordlist number_of_attempts\nc-wordle /usr/share/dict/words 5", VERSION);
        return EXIT_FAILURE;
    }

    char* endptr;
    int attemps_limit = (int) strtol(argv[2], &endptr, 10);
    
    
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
        buf[strcspn(buf, "\n")] = '\0'; // Remove newline
        if (!strstr(buf, "'") && !contains_non_ascii(buf) && strlen(buf) == 5) {
            if (count >= cap) {
                cap *= 2;
                char **temp = realloc(words, cap * sizeof(char *));
                if (!temp) {
                    perror("Failed to allocate memory.");
                    return EXIT_FAILURE;
                }
                words = temp;
            }
            words[count] = malloc(strlen(buf) + 1);
            toupperstr(buf);
            strcpy(words[count], buf);
            count++;
        }
    }
    fclose(file_ptr);

    if (count == 0) {
        printf("No valid words found in the word list.\n");
        free(words);
        return EXIT_FAILURE;
    }

    int word_index = rand_lim(count - 1);
    char *target_word = words[word_index];


    char word_guess[64];
    bool win = false;

    size_t attempts = 0;

    while (!win && attempts < attemps_limit) {
        printf("Enter your guess: ");
        if (!scanf("%5s", word_guess)) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        toupperstr(word_guess);

        if (strlen(word_guess) != 5) {
            printf("Your guess must be exactly 5 letters.\n");
            continue;
        }

        if (attempts > 0) {
            printf("\033[1A"); // Move cursor up
            printf("\033[2K"); // Clear the line
        }

        print_colored_guess(word_guess, target_word);
        printf("\n");

        win = !strcmp(target_word, word_guess);
        attempts++;
    }

    if (attempts >= attemps_limit) {
        printf("You lose. The word was %s\n", target_word);
    } else {
        printf("You win!\n");
    }

    for (size_t i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
    return 0;
}
