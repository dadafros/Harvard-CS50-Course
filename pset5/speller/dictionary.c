// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 33 * 33 * 33;
// Number of words
int num_words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    for (node *n = table[hash(word)]; n != NULL; n = n->next)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int h = 5381;
    for (int i = 0; word[i] != '\0'; i++)
    {
        h = 33 * h + tolower(word[i]);
    }
    return h % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *d = fopen(dictionary, "r");
    if (d == NULL)
    {
        return false;
    }
    fseek(d, 0L, SEEK_END);
    int num_bytes = ftell(d);
    fseek(d, 0L, SEEK_SET);
    char *buffer_dictionary = malloc(num_bytes * sizeof(char));
    if (buffer_dictionary == NULL)
    {
        return false;
    }
    fread(buffer_dictionary, sizeof(char), num_bytes, d);
    fclose(d);

    char word[LENGTH + 1] = "";
    for (int i = 0, j = 0; i < num_bytes; i++)
    {
        if (buffer_dictionary[i] == '\n')
        {
            word[j] = '\0';
            j = 0;

            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }
            strcpy(n->word, word);
            unsigned int index = hash(word);
            n->next = table[index];
            table[index] = n;
            num_words++;
        }
        else
        {
            if (buffer_dictionary[i] != word[j])
            {
                word[j] = buffer_dictionary[i];
            }
            j++;
        }
    }
    free(buffer_dictionary);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return num_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *n, *tmp;
    for (int index = 0; index < N; index++)
    {
        n = table[index];
        while (n != NULL)
        {
            tmp = n;
            n = n-> next;
            free(tmp);
        }
    }
    return true;
}