/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */

#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
#define HASH_P 31
#define TABLE_SIZE 97

HashTable *dictionary;
char *parseWord(FILE *);
/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {	
if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(TABLE_SIZE, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();
	freeTable(dictionary);
  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string)
 */
unsigned int stringHash(void *s) {
  // -- TODO --
//  fprintf(stderr, "need to implement stringHash\n");
	char *word = (char *) s;
	int power = 1;
	unsigned int hash = 0;
	while (*word) { 
		hash = hash + (*word * power);
		power = power * HASH_P;
		word++;
	}
	hash = hash % TABLE_SIZE;
	return hash;
  /* To suppress compiler warning until you implement this function, */
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  // -- TODO --
  //fprintf(stderr, "You need to implement stringEquals");
	char *string1 = (char *) s1;
	char *string2 = (char *) s2;
	while (*string1 &&  *string2) {
		if (*string1 != *string2) return 0;
		string1++;
		string2++;
	}  
	if (*string1 || *string2) return 0;
	return 1;
/* To suppress compiler warning until you implement this function */
}

/*
 * This function should read in every word and replacement from the dictionary
 * and store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final bit of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(61)
 * to cleanly exit the program.
 */
char *parseWord(FILE *fp) {
	int c; 
	c = getc(fp);
	while (!(c==EOF) && isspace(c)) { //skip over any leading whitespace
		c = getc(fp);
	} 
	char *word = malloc(sizeof(char) * 60); //hardcoding to 60
	int word_size = 0;
	while (!isspace(c) && !(c==EOF)) {
		word[word_size] = c;
		word_size++;
		c = getc(fp);
	}
	word = realloc(word, sizeof(char) * word_size + 1);
	return word;
}

void readDictionary(char *dictName) { //errors repeating words
  // -- TODO --
  	FILE *fp = fopen(dictName, "r");
	if (fp == NULL) {
		 fprintf(stderr, "You need to implement readDictionary\n");
		return;
	}
	while (!feof(fp)) {
		char *nextKey = parseWord(fp);
		char *nextVal = parseWord(fp);
		insertData(dictionary, (void *) nextKey, (void *) nextVal);
	}
	fclose(fp);
}

/*
 * This should process standard input (stdin) and perform replacements as 
 * described by the replacement set then print either the original text or 
 * the replacement to standard output (stdout) as specified in the spec (e.g., 
 * if a replacement set of `taest test\n` was used and the string "this is 
 * a taest of  this-proGram" was given to stdin, the output to stdout should be 
 * "this is a test of  this-proGram").  All words should be checked
 * against the replacement set as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the replacement set shoud 
 * it report the original word.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final bit of your grade, you cannot assume words have a bounded length.
 */
int inAlphabet(int c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
	return 0;
} 
// --TODO -- 
// MAKE SURE ALL MALLOCS HAVE NULL CHECKS AFTER

char *firstNotLower(char *str, int length) {
	if (str == NULL) return NULL;
	int c;
	for (int i = 1; i < length; i++) {
		c = str[i];
		if (c >= 'A' && c <= 'Z') c += 32;
		str[i] = c;
	}
	return str;	
}


char *toLower(char *str, int length) {
	if (str == NULL) return NULL;
	int c;
	for (int i = 0; i < length; i++) {
		c = str[i];
		if (c >= 'A' && c <= 'Z') c += 32;
		str[i] = c;
	}
	return str;
}
	 

void processInput() {
  // -- TODO --
  	int c = getchar();
	char *currentWord = (char *) malloc(60 * sizeof(char));
	int word_length = 0;
	while (c != EOF) {// while 1 and special case for EOF	
		if (inAlphabet(c) && !word_length) { //beginning a word
			free(currentWord);
			currentWord = (char *) malloc(60 * sizeof(char));
		} 
		if (inAlphabet(c)) {
			if (word_length > 58) { fprintf(stderr, "word length %d too long", word_length); break;}
			currentWord[word_length] = c; //invalid write of size 1
			currentWord[word_length+1] = '\0';
			word_length++;
		}
		if (!inAlphabet(c) && word_length) {	
			//do stuff with the word
			char *wordCopy = malloc(sizeof(char) * word_length + 1);
			strcpy(wordCopy, currentWord);
			char *retWord;
			char *a;
			if (a = (char *) findData(dictionary, currentWord)) {retWord = a;}
			else if (a = (char *) findData(dictionary, firstNotLower(currentWord, word_length))) {retWord = a;}
			else if (a = (char *) findData(dictionary, toLower(currentWord, word_length))){ retWord = a;} //should we be mallocing stuff here?
			else { retWord = wordCopy;}
			printf("%s", retWord);
			word_length = 0;
			free(wordCopy); 
		}
		if (!inAlphabet(c)) {
			printf("%c", c);
		}
		c = getchar();
	}
	if (currentWord) free(currentWord);
}
		
	
	

