#ifndef SPELL_H
#define SPELL_H

/*
 * first_str - first string (vertical)
 * second_str - second string (horizontal)
 * print_table - can have value 1 or other.
 *      Value 1 indicates to print the distance matrix.
 *      Value 0 indicates to NOT print the distance matrix. In this case
 *      the function is used only to compute the distance between 2 words.
 *      You do not have to deal with the other cases (values different than 1 or 0),
 *      but you can if you want to.
 */
int edit_distance(char *first_str, char *second_str, int print_table);

void spell_check(char *dictname, char *testname, int printOn);

#endif  /* SPELL_H */
