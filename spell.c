#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool containNonAlphaOnly(char c[]);

bool containallnum(const char* c);

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
	int pointer = 0, i, j = 0, k, wordlen;

	if (fp == NULL)
	{
		printf("Failed to open file");
		return false;
	}

	char sentence[32768], word[LENGTH];

	while (fgets(sentence, sizeof(sentence), fp) > 0) {
		int len = strlen(sentence); //where sentence is your char array fgets is using
		if (sentence[len - 1] == '\n')
			sentence[len - 1] = '\0';

        len = strlen(sentence);
		for (i = 0; i <= len; i++)
		{

			if (!isspace(sentence[i]) && sentence[i] != '\0' && sentence[i] != '\n' && j < LENGTH - 2)
			{
				word[j] = sentence[i];
				j += 1;
			}
			else
			{
				word[j] = '\0';
				if (strlen(word)>0)
				{
					//remove punctation from beginning and end
					if (!isalpha(word[0]) && !isdigit(word[0]))
					{
                        wordlen = strlen(word);
                        memmove((void*)word, (void*)(word + 1), wordlen * sizeof(char));
					}

                    wordlen = strlen(word);

					if (!isalpha(word[wordlen - 1]) && !isdigit(word[wordlen - 1]))
						word[wordlen - 1] = '\0';
						

					if (!containNonAlphaOnly(word) && !containallnum(word))
					{
						misspelled[pointer] = malloc(strlen(word) + 1);
						strcpy(misspelled[pointer], word);
						pointer += 1;
					}
					else
					{
						if (!check_word(word, hashtable))
						{
							for (k = 0; k < strlen(word); k++)
							{
                                if(word[k]!='\'' && !isdigit(word[k]))
								   word[k] = tolower(word[k]);
							}
							if (!check_word(word, hashtable))
							{
								misspelled[pointer] = malloc(strlen(word) + 1);
								strcpy(misspelled[pointer], word);
								pointer += 1;
							}
						}
					}
				}

				j = 0;
			}
		}


	}
	return pointer;
}

bool check_word(const char* word, hashmap_t hashtable[])
{
	int bucket = hash_function(word);

	node* cursor = hashtable[bucket];
	
	while (cursor != NULL)
	{
		if (strcmp(word, cursor->word) == 0)
		{

			return true;
		}
		cursor = cursor->next;
	}
	return containallnum(word);
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
	for (int i = 0; i < HASH_SIZE; i++)
	{
		hashtable[i] = NULL;
	}


	FILE* dictionary;
	dictionary = fopen(dictionary_file, "r");


	if (dictionary == NULL)
	{
		printf("Failed to load dictionary");
		return false;
	}

	char buffer[LENGTH + 1];

	while (fgets(buffer, sizeof(buffer), dictionary) > 0)
	{
		int len = strlen(buffer);
		if (buffer[len - 1] == '\n')
			buffer[len - 1] = '\0';

		node* new_node = malloc(sizeof(node));

		new_node->next = NULL;

		strcpy(new_node->word, buffer);

		int bucket = hash_function(new_node->word);

		if (hashtable[bucket] == NULL)
		{
			hashtable[bucket] = new_node;
		}

		else
		{
			new_node->next = hashtable[bucket];
			hashtable[bucket] = new_node;
		}


	}

	fclose(dictionary);

	return true;
}

bool containNonAlphaOnly(char c[])
{
	int i;

	for (i = 0; i < strlen(c); i++)
	{
		if (!isalpha(c[i]) && (c[i]!='\''))
		{
			return false;
		}
			
	}

	return true;

}

bool containallnum(const char* c)
{
   int i;

   for (i = 0; i < strlen(c); i++)
	{
		if (!isdigit(c[i]))
		{
			return false;
		}
			
	}

	return true;

}

