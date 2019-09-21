#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool freemem(hashmap_t hashtable[]);

int main(int argc, char *argv[])
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(argv[2], hashtable);
    char * misspelled[1000];
    FILE* song;
    song = fopen(argv[1], "r");
    int misspell = check_words(song, hashtable, misspelled);
    fclose(song);
    freemem(hashtable);
    for(int i =0;i<misspell;i++)
    {
        free(misspelled[i]);
    }

}

bool freemem(hashmap_t hashtable[])
{
    for (int i = 0; i < HASH_SIZE;i++)
    {
        node* cursor = hashtable[i];
        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    } 
    return true;
}