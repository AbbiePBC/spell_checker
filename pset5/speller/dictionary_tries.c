// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"
#include <ctype.h> 

// Creating trie
typedef struct trie_node
{
    struct trie_node *path[27];
    char word[LENGTH + 1];
    bool end_of_word;
}
trie_node;

trie_node *head;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    //if reach end of path, return true
    //when searching trhough trie, if hit NULL, then traversal unsuccessful and doesnt exist
    //need to check very end, in case the word is a subset of another word
    //need to make sure that the comparison is case insensitive
    int length = strlen(word);
   //can just use the tolower() function
   //char lower_case_word[length + 1];
    //for (int j = 0; j < length; j++) //repeating through characters of word
    //{
      //  lower_case_word[j] = tolower(word[j]);
        //lower_case_word[length] = '\0';
    //}
    if (head == NULL)
    {
        return false;
    }
    trie_node *trav = head;
    for (int i = 0; i < length; i ++)
    {
        if (isalpha(word[i]))
        {
            trav = trav -> path[tolower(word[i]) - 'a'];
        }
        else if (word[i] == '\'')
        {
            trav = trav -> path[26];
        }
        if (trav == NULL)
        {
            return false;
        }
    }
    if (trav -> end_of_word == true) //basically saying is_leaf is true, so could get rid of this?
    {
        return true;
    }
    else
    {
        return false;
    }
}

int counter_of_words;

//create trie nodes

trie_node *create_trie_node()
{
    trie_node *node = malloc(sizeof(trie_node));
    int i = 0;
    while (i < 27)
    {
        node -> path[i] = NULL;
        i++;
    }
    return node;
}
// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf ("error loading file");
        return false;
    }
    head = create_trie_node();
    char word[LENGTH + 1];
    while (fscanf(file, "%s\n", word) != EOF)
    {
    //NEED TO ATTACH HEAD BY CREATING A NODE FOR IT
        int length = strlen(word);
        trie_node *temp = head;
        trie_node *node;

        for (int i = 0; i < length; i ++)
        {
            int value;
            if (isalpha(word[i]))
                value = word[i] - 'a';
            else if (word[i] == '\'')
                value = 26;
            else {
                printf("I hate the character %c because it is neither alphabetic nor an apostrophe.\n", word[i]);
                return false;
            }
            if (temp -> path[value] == NULL)
            {
                node = create_trie_node();
                if (node == NULL)
                {
                    printf ("node = NULL");
                    return false;
                }
                else
                {
                    node -> end_of_word = false; 
                    temp -> path[value] = node;
                }
            }
            temp = temp -> path[value];
        }
        temp -> end_of_word = true;
        counter_of_words ++;
    }
    fclose(file);
    //for each letter in word, if null clear each path by mallocing the node and go down it
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    //if not yet loaded print not loaded and return 0
    //if loaded return size
    if (counter_of_words == 0)
    {
        printf("dictionary not loaded yet!\n");
        return 0;
    }
    else
    {
        //return size
        //printf("there are %i words in the dictionary\n", counter_of_words);
        return counter_of_words;
    }
}


//recursive function to release all paths from head
void release_function(trie_node *cursor)
{    
    //cursor = head;
    for (int i = 0; i < 26; i ++) 
    {
        if (cursor -> path[i] != NULL) //potentially could just use end_of_word here
        {
            release_function(cursor -> path[i]);
        }
    }
     free(cursor);
}
// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO ---- NEED TO PUT IN RECURSIVE FUNCTION HERE TO FREE EACH LEAF
    release_function(head);
    return true;
}
