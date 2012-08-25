/*
 * labels.cpp - Jakub Janecek, janecj3@fel.cvut.cz
 * This module takes care of labels. It provides global counter of labels and you can get first available number of 
 * label from it.
 */

static int counter = 0; // counter of labels

/*
 * Returns first available number of label and increments the counter.
 */
int get() {
  counter++;
  return ( counter - 1 );
}
