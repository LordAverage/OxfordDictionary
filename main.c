// main.c

#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICTIONARY_FILE_NAME "Dictionary.dat"

// Function prototypes
void GetString(char** oppcString);
void AddNewWord(char* ipcNewWord, char* ipcWordDefinition);
char* GetWordDefinition(char* ipcWord);
void PrintAllWords(char icLetter);

//---------------------------------------------------------------------------------------
//                                    Oxford Dictionary
//                                   -------------------
//
// General : The program demonstrates a dictionary system using a data file. The main
//			 function contains a loop that prompts the user for a command, and then
//			 performs an action based on the command entered. The system allows you to
//			 search a word in the dictionary, add a word to the dictionary, and print
//			 all the words in the dictionary starting with a given letter.
//
// Input   : The desired command number.
// 
// Process : The program iterates through the words and the definitions in the data file
//			 and handles them with division into functions.
//
// Output  : The appropriate response.
// 
//---------------------------------------------------------------------------------------
// Programmer : Lahav Gafni
// Student No : 325761880
// Date       : 17.12.2022
//---------------------------------------------------------------------------------------
int main()
{
	// Variable definition
	char cCommand;
	char cLetter;
	char* pcWordToSearch;
	char* pcNewWord;
	char* pcWordDefinition;

	// Code section

	do
	{
		// The main loop: prompt the user for a command, and then perform that action.
		printf("Enter your command.\n");
		printf("1 to search a word, 2 to add a new definition, 3 to print all words ");
		printf("with a chosen letter and e, E or 4 to exit\n");
		scanf("%c", &cCommand);
		switch (cCommand)
		{
		case '1':
			printf("Enter a word to search\n");
			pcWordToSearch = (char*)calloc(1, sizeof(char));
			if (!pcWordToSearch)
			{
				printf("Error allocating dynamic memory\n");
				exit(2);
			}
			GetString(&pcWordToSearch);
			pcWordDefinition = GetWordDefinition(pcWordToSearch);
			free(pcWordToSearch);
			if (!pcWordDefinition)
			{
				printf("The word was not found\n");
			}
			else
			{
				printf("%s\n", pcWordDefinition);
				free(pcWordDefinition);
			}
			break;
		case '2':
			pcNewWord = (char*)calloc(1, sizeof(char));
			pcWordDefinition = (char*)calloc(1, sizeof(char));
			if (!pcNewWord || !pcWordDefinition)
			{
				printf("Error allocating dynamic memory\n");
				exit(2);
			}
			printf("Enter the new word\n");
			GetString(&pcNewWord);
			printf("Enter the definition\n");
			GetString(&pcWordDefinition);
			AddNewWord(pcNewWord, pcWordDefinition);
			free(pcNewWord);
			free(pcWordDefinition);

			break;
		case '3':
			printf("Enter a letter\n");
			scanf("\n%c", &cLetter);
			PrintAllWords(cLetter);
			scanf("%c", &cCommand);
		default:
			break;
		}
	} while (cCommand != '4' && cCommand != 'e' && cCommand != 'E');
	return 0;
}
//---------------------------------------------------------------------------------------
//                                        Get String
//                                       ------------
//
// General      : The function receives a pointer to a memory block, and then reads a
//				  string from the user and stores the data dynamically in that location.
//
// Parameters   : 
//      oppcString - Memory block for assignment (Out)
//
// Return Value : The scanned string (until '\n').
// 
//---------------------------------------------------------------------------------------
void GetString(char** oppcString)
{
	// Variable definition
	char cNext;
	size_t sztWordLength;

	// Code section

	sztWordLength = strlen(*oppcString);
	scanf("\n%c", &cNext);
	// Scan for more characters until '\n'
	while (cNext != '\n')
	{
		// Allocate memory for the next character
		*oppcString = (char*)realloc(*oppcString, ++sztWordLength);
		if (!*oppcString)
		{
			printf("Error allocating dynamic memory\n");
			exit(2);
		}
		*(*oppcString + sztWordLength - 1) = cNext;
		scanf("%c", &cNext);
	}
	// Insert null terminator '\0' at the end of the string
	*oppcString = (char*)realloc(*oppcString, ++sztWordLength);
	if (!*oppcString)
	{
		printf("Error allocating dynamic memory\n");
		exit(2);
	}
	*(*oppcString + sztWordLength - 1) = '\0';
}
//---------------------------------------------------------------------------------------
//                                       Add New Word
//                                      --------------
//
// General      : The procedure stores the given word and definition in the dictionary.
//
// Parameters   : 
//      ipcNewWord		  - The word (In)
//		ipcWordDefinition - The definition (In)
//
// Return Value : None (procedure)
// 
//---------------------------------------------------------------------------------------
void AddNewWord(char* ipcNewWord, char* ipcWordDefinition)
{
	// Variable definition
	FILE* pfsFilePointer;
	size_t sztElementsRead;
	size_t sztWordLength;

	// Code section

	sztWordLength = strlen(ipcNewWord);
	pfsFilePointer = fopen(DICTIONARY_FILE_NAME, "ab");
	if (!pfsFilePointer)
	{
		printf("Error opening file\n");
		exit(3);
	}
	// Write data to file
	sztElementsRead = fwrite(ipcNewWord, sizeof(char), sztWordLength, pfsFilePointer);
	if (sztElementsRead != sztWordLength)
	{
		printf("Error writing to file\n");
		exit(1);
	}
	// Seperate word and definition
	sztElementsRead = fwrite(" ", sizeof(char), 1, pfsFilePointer);
	if (sztElementsRead != 1)
	{
		printf("Error writing to file\n");
		exit(1);
	}
	sztWordLength = strlen(ipcWordDefinition);
	// Write data to file
	sztElementsRead = fwrite(ipcWordDefinition, sizeof(char), sztWordLength,
							 pfsFilePointer);
	if (sztElementsRead != sztWordLength)
	{
		printf("Error writing to file\n");
		exit(1);
	}
	sztElementsRead = fwrite("\n", sizeof(char), 1, pfsFilePointer);
	if (sztElementsRead != 1)
	{
		printf("Error writing to file\n");
		exit(1);
	}
	fclose(pfsFilePointer);
}
//---------------------------------------------------------------------------------------
//                                    Get Word Definition
//                                   ---------------------
//
// General      : The function receives a word and searches for it in the dictionary.
//				  When found, it returns the definition for that word. If a word is not
//				  found, NULL is returned.
//
// Parameters   : 
//      ipcWord - Word to search (In)
//
// Return Value : Definition for that word or NULL if not found.
// 
//---------------------------------------------------------------------------------------
char* GetWordDefinition(char* ipcWord)
{
	// Variable definition
	size_t sztElementsRead;
	int nWordLength;
	char cNext;
	char* pcWordRead;
	char* pcDefinition;
	FILE* pfsFilePointer;
	
	// Code section

	pcWordRead = malloc(sizeof(char));
	pcDefinition = malloc(sizeof(char));
	if (!pcWordRead || !pcDefinition)
	{
		printf("Error allocating dynamic memory\n");
		exit(2);
	}
	pfsFilePointer = fopen(DICTIONARY_FILE_NAME, "rb");
	if (!pfsFilePointer)
	{
		printf("Error opening file\n");
		exit(3);
	}
	// Iterate through all words and definitions until word found
	do
	{
		nWordLength = 1;
		sztElementsRead = fread(&cNext, sizeof(char), 1, pfsFilePointer);
		if (sztElementsRead != 1)
		{
			// If EOF is reached, return NULL
			if (feof(pfsFilePointer))
			{
				fclose(pfsFilePointer);
				free(pcWordRead);
				free(pcDefinition);
				return (NULL);
			}
			printf("Error reading from file\n");
			exit(4);
		}
		// Scan next word
		while (cNext != ' ')
		{
			pcWordRead = (char*)realloc(pcWordRead, sizeof(char) * ++nWordLength);
			if (!pcWordRead)
			{
				printf("Error allocating dynamic memory\n");
				exit(2);
			}
			pcWordRead[nWordLength - 2] = cNext;
			sztElementsRead = fread(&cNext, sizeof(char), 1, pfsFilePointer);
			if (sztElementsRead != 1)
			{
				printf("Error reading from file\n");
				exit(4);
			}
		}
		pcWordRead[nWordLength - 1] = '\0';
		nWordLength = 1;
		sztElementsRead = fread(&cNext, sizeof(char), 1, pfsFilePointer);
		if (sztElementsRead != 1)
		{
			printf("Error reading from file\n");
			exit(4);
		}
		// Scan next definition
		while (cNext != '\n')
		{
			pcDefinition = (char*)realloc(pcDefinition, sizeof(char) * ++nWordLength);
			if (!pcDefinition)
			{
				printf("Error allocating dynamic memory\n");
				exit(2);
			}
			pcDefinition[nWordLength - 2] = cNext;
			sztElementsRead = fread(&cNext, sizeof(char), 1, pfsFilePointer);
			if (sztElementsRead != 1)
			{
				printf("Error reading from file\n");
				exit(4);
			}
		}
		pcDefinition[nWordLength - 1] = '\0';
	} while (strcmp(pcWordRead, ipcWord));
	fclose(pfsFilePointer);
	free(pcWordRead);
	return pcDefinition;
}
//---------------------------------------------------------------------------------------
//                                       Print All Words
//                                      --------------
//
// General      : The procedure prints all existing words (not definitions) in the
//				  dictionary, starting with the given letter.
//
// Parameters   : 
//      icLetter - Starting letter of each word. (In)
//
// Return Value : None (procedure)
// 
//---------------------------------------------------------------------------------------
void PrintAllWords(char icLetter)
{
	// Variable definition
	size_t sztElementsRead;
	int nWordLength;
	char cNext;
	char* pcWordRead;
	FILE* pfsFilePointer;

	// Code section

	pcWordRead = malloc(sizeof(char));
	if (!pcWordRead)
	{
		printf("Error allocating dynamic memory\n");
		exit(2);
	}
	pfsFilePointer = fopen(DICTIONARY_FILE_NAME, "rb");
	if (!pfsFilePointer)
	{
		printf("Error opening file\n");
		exit(3);
	}
	// Iterate through all words in the dictionary
	while (1)
	{
		nWordLength = 1;
		sztElementsRead = fread(&cNext, sizeof(char), 1, pfsFilePointer);
		if (sztElementsRead != 1)
		{
			// If EOF is reached, return
			if (feof(pfsFilePointer))
			{
				fclose(pfsFilePointer);
				free(pcWordRead);

				return;
			}
			printf("Error reading from file\n");
			exit(4);
		}
		// Scan next word
		while (cNext != ' ')
		{
			pcWordRead = (char*)realloc(pcWordRead, sizeof(char) * ++nWordLength);
			if (!pcWordRead)
			{
				printf("Error allocating dynamic memory\n");
				exit(2);
			}
			pcWordRead[nWordLength - 2] = cNext;
			sztElementsRead = fread(&cNext, sizeof(char), 1, pfsFilePointer);
			if (sztElementsRead != 1)
			{
				printf("Error reading from file\n");
				exit(4);
			}
		}
		pcWordRead[nWordLength - 1] = '\0';
		if (pcWordRead[0] == icLetter)
		{
			printf("%s\n", pcWordRead);
		}
		// Move to next line
		while (cNext != '\n')
		{
			sztElementsRead = fread(&cNext, sizeof(char), 1, pfsFilePointer);
			if (sztElementsRead != 1)
			{
				printf("Error reading from file\n");
				exit(4);
			}
		}
	}
	fclose(pfsFilePointer);
	free(pcWordRead);
}