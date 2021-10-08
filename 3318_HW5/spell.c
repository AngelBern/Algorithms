//author 
//CSE 3318 Fall 2020
//
//code campiled with the following command on virtual machine
//gcc -o hw5.out spell_checker.c spell.h spell.c -std=c99 -g -O0 -Wall

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "spell.h"

/*  Suggestions
 * - When you work with 2D arrays, be careful. Either manage the memory yourself, or
 * work with local 2D arrays. Note C99 allows parameters as array sizes as long as
 * they are declared before the array in the parameter list. See:
 * https://www.geeksforgeeks.org/pass-2d-array-parameter-c/
 *
 * Worst case time complexity to compute the edit distance from T test words
 * to D dictionary words where all words have length MAX_LEN:
 * Student answer:  Theta(............)
 *
 *
 * Worst case to do an unsuccessful binary search in a dinctionary with D words, when
 * all dictionary words and the searched word have length MAX_LEN
 * Student answer:  Theta(............)
 */

/* You can write helper functions here */

struct Node
{
  char *          string;
  int distance;
  struct Node *   next;
};

int get_value(int _left, int _up, int _equal);
void print_arr(int x, int y, int arr[x][y], char *str1, char *str2);
void printArr(int size, char **arr);
int compare(const void *x, const void *y);
int binarySearch(char **dictArr, int bottom, int top, char *target, int verbose, int count,
                 char *original);
void processWord(char *inputWord, char **dictArr, int dictSize, FILE *outputPtr, int verbose);
struct Node *createNode(char *str, int dist);
void insertNode(struct Node *head, struct Node *new);
void deleteNode(struct Node *head);
char *toLowerCase(char *input);
void calcDis(struct Node *head, char **dictArr, int size);
int printNodes(struct Node *head, int size);

int edit_distance(char *first_string, char *second_string, int print_table)
{
  int x = strlen(first_string) + 1;
  int y = strlen(second_string) + 1;
  int arr[x][y];
  
  for (int i = 0; i < x; i++)
    for (int j = 0; j < y; j++)
    {
      if (!i && !j)
        arr[i][j] = 0;
      else if (!i)
        arr[i][j] = j;
      else if (!j)
        arr[i][j] = i;
      else
        arr[i][j] =
          get_value(arr[i - 1][j], arr[i][j - 1],
                    (arr[i - 1][j - 1] + (first_string[i - 1] != second_string[j - 1])));
    }
  if (print_table)
    print_arr(x, y, arr, first_string, second_string);
  
  return arr[x - 1][y - 1];        // replace this line with your code
}

void print_arr(int x, int y, int arr[x][y], char *str1, char *str2)
{
  printf("\n");
  for (int i = -1; i < x; i++)
  {
    for (int j = -1; j < y; j++)
    {
      if (j == -1)
      {
        if (i == -1 || i == 0)
          printf("  |");
        else
          printf(" %c|", str1[i - 1]);
      }
      else if (i == -1)
      {
        if (j == -1)
          printf("  |");
        else if (j == 0)
          printf("   |");
        else
          printf("  %c|", str2[j - 1]);
      }
      else
      {
        printf("%3d|", arr[i][j]);
      }
    }
    printf("\n");
    for (int t = 0; t < 4 * y + 3; t++)
      printf("-");
    printf("\n");
  }
  printf("\n");
}

int get_value(int _left, int _up, int _diagonal)
{
  char min = _left + 1;
  
  _up = _up + 1;
  if (min > _up)
    min = _up;
  if (min > _diagonal)
    min = _diagonal;
  return min;
}

void spell_check(char *testname, char *dictname, int printOn)
{
  char corrected[101];
  int dictSize = 0;
  char **dictArr;
  
  
  
  printf("Corrected output filename: ");
  scanf("%s", corrected);
  printf("\n\nLoading the dictionary file: %s\n\n", dictname);
  FILE *dictFPtr = fopen(dictname, "r");
  FILE *inputPtr = fopen(testname, "r");
  FILE *outputPtr = fopen(corrected, "w");
  
  if (dictFPtr == NULL)
  {
    printf("DICTIONARY FILE NOT FOUND... EXITING\n");
    return;
  }
  if (inputPtr == NULL)
  {
    printf("INPUT FILE NOT FOUND... EXITING\n");
    return;
  }
  if (outputPtr == NULL)
  {
    printf("ERROR CREATING OUTPUT FILE... EXITING\n");
    return;
  }
  
  fscanf(dictFPtr, "%d", &dictSize);
  
  dictArr = (char **)malloc(sizeof(char *) * dictSize);
  
  for (int i = 0; i < dictSize; i++)
  {
    char temp[101];
    fscanf(dictFPtr, "%s", temp);
    dictArr[i] = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
    strcpy(dictArr[i], temp);
  }
  printf("Dictionary has size: %d\n", dictSize);
  if (printOn)
  {
    printf(" Original dictionary:\n");
    printArr(dictSize, dictArr);
    printf("\n\n Sorted dictionary (alphabetical order) :\n");
  }
  
  qsort(dictArr, dictSize, sizeof(dictArr[0]), compare);
  if (printOn)
    printArr(dictSize, dictArr);
  printf("\n");
  
  //getting words
  
  char inputChar;
  int inputIndex = 0;
  char *inputWord = (char *)malloc(sizeof(char) * 101);
  
  fscanf(inputPtr, "%c", &inputChar);
  while (1)
  {
    if (feof(inputPtr))
    {
      inputWord[inputIndex] = '\0';
      processWord(inputWord, dictArr, dictSize, outputPtr, printOn);
      break;
    }
    if (inputChar != 32 && inputChar != 33 && inputChar != 44 && inputChar != 46 && inputChar != 63)
    {
      inputWord[inputIndex] = inputChar;
      inputIndex++;
    }
    else if (inputIndex == 0)
    {
      fprintf(outputPtr, "%c", inputChar);
    }
    else
    {
      inputWord[inputIndex] = '\0';
      processWord(inputWord, dictArr, dictSize, outputPtr, printOn);
      fprintf(outputPtr, "%c", inputChar);
      inputIndex = 0;
    }
    fscanf(inputPtr, "%c", &inputChar);
  }
  
  
  //free all
  for (int i = 0; i < dictSize; i++)
  {
    free(dictArr[i]);
    dictArr[i] = NULL;
  }
  free(dictArr);
  dictArr = NULL;
  free(inputWord);
  inputWord = NULL;
  fclose(dictFPtr);
  fclose(inputPtr);
  fclose(outputPtr);
}

void processWord(char *inputWord, char **dictArr, int dictSize, FILE *outputPtr, int verbose)
{
  char *sanitized = (char *)malloc(sizeof(char) * (strlen(inputWord) + 1));
  
  struct Node *head;
  
  strcpy(sanitized, inputWord);
  printf("Binary search for: %s\n", sanitized);
  if (binarySearch(dictArr, 0, dictSize - 1, toLowerCase(sanitized), verbose, 0, sanitized) == -1)
  {
    head = createNode(sanitized, 0);
    calcDis(head, dictArr, dictSize);
    int choice = printNodes(head, dictSize);
    if (choice == -1)
    {
      char new[101];
      printf("correction?: ");
      scanf("%s", new);
      fprintf(outputPtr, "%s", new);
    }
    struct Node *headCpy = head;
    while (choice != 0 && headCpy != NULL)
    {
      headCpy = headCpy->next;
      choice--;
    }
    
    if (headCpy == NULL)
      printf("ERROR, CHOICE OUT OF BOUNDS. DEFAULTING TO ORIGINAL\n");
    else
      fprintf(outputPtr, "%s", headCpy->string);
    deleteNode(head);
  }
  else
  {
    fprintf(outputPtr, "%s", sanitized);
  }
  free(sanitized);
}

void calcDis(struct Node *head, char **dictArr, int size)
{
  insertNode(head, createNode(dictArr[0], edit_distance(head->string, dictArr[0], 0)));
  for (int i = 1; i < size; i++)
  {
    int dist = edit_distance(head->string, dictArr[i], 0);
    if (dist < head->next->distance)
    {
      deleteNode(head->next);
      head->next = NULL;
      insertNode(head, createNode(dictArr[i], dist));
    }
    else if (dist == head->next->distance)
    {
      insertNode(head, createNode(dictArr[i], dist));
    }
  }
}

int printNodes(struct Node *head, int size)
{
  printf("-1 - type correction\n");
  printf(" 0 - leave word as is (do not fix spelling)\n");
  printf("     Minimum distance: %d  (computed edit distances: %d)\n", head->next->distance, size);
  printf("     Words that give minimum distance:\n");
  struct Node *curr = head->next;
  int i = 1;
  
  while (curr != NULL)
  {
    printf(" %d - %s\n", i, curr->string);
    i++;
    curr = curr->next;
  }
  printf("Enter your choice (from the above options): ");
  scanf("%d", &i);
  return i;
}

char *toLowerCase(char *input)
{
  char *res = (char *)malloc(sizeof(char) * (strlen(input) + 1));
  int i = 0;
  
  while (input[i] != '\0')
  {
    res[i] = tolower(input[i]);
    i++;
  }
  res[i] = '\0';
  return res;
}

int binarySearch(char **dictArr, int bottom, int top, char *target, int verbose, int count,
                 char *original)
{
  if (top >= bottom)
  {
    int mid = bottom + (top - bottom) / 2;
    if (verbose)
      printf("dict[%d] = %s\n", mid, dictArr[mid]);
    if (strcmp(target, dictArr[mid]) == 0)
    {
      if (verbose)
        printf("%s found at location %d.\n", target, mid + 1);
      free(target);
      printf("---> |%s| (words compared when searching: %d)\n", original, count + 1);
      printf("     - OK\n\n\n");
      return mid;
    }
    if (strcmp(target, dictArr[mid]) < 0)
      return binarySearch(dictArr, bottom, mid - 1, target, verbose, ++count, original);
    else
      return binarySearch(dictArr, mid + 1, top, target, verbose, ++count, original);
  }
  if (verbose)
    printf("Not found\n\n");
  printf("---> |%s| (words compared when searching: %d)\n", original, count + 1);
  free(target);
  return -1;
}

struct Node *createNode(char *str, int dist)
{
  struct Node *res = (struct Node *)malloc(sizeof(struct Node));
  
  res->string = (char *)malloc(sizeof(char) * (strlen(str) + 1));
  strcpy(res->string, str);
  res->distance = dist;
  res->next = NULL;
  return res;
}

void insertNode(struct Node *head, struct Node *new)
{
  if (head->next == NULL)
  {
    head->next = new;
    return;
  }
  insertNode(head->next, new);
}

void deleteNode(struct Node *head)
{
  if (head == NULL)
    return;
  deleteNode(head->next);
  free(head->string);
  head->string = NULL;
  free(head);
  head = NULL;
}

int compare(const void *x, const void *y)
{
  return strcmp(*(const char **)x, *(const char **)y);
}

void printArr(int size, char **arr)
{
  for (int i = 0; i < size; i++)
    printf("%d. %s\n", i, arr[i]);
}
