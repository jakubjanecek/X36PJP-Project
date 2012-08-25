/*
 * input.cpp - Jakub Janecek, janecj3@fel.cvut.cz
 * This module takes care of input from a file. It opens a file, reads it character by character and then closes it.
 */

#include <iostream>
#include <fstream>
using namespace std;

static ifstream inFile;

/*
 * Initializes the input.
 * Returns 0 when successful and -1 when unsuccessful.
 */
int initializeInput(char * filename ) {
  inFile.open( filename, ifstream::in );
  if ( inFile.is_open() ) {
    return( 0 );
  }
  else {
    return( -1 );
  }
}

/*
 * Reads a character from the input and returns it.
 * Return -1 when something is wrong and EOF when it reached end of file.
 */
int readChar() {
  if ( inFile.eof() ) {
    return EOF;
  }
  if ( inFile.good() ) {
    return inFile.get();
  }
  else {
    return( -1 );
  }
}

/*
 * Closes the input. It needs to be called after all work has been done.
 */
void closeInput() {
  if ( inFile.is_open() ) {
    inFile.close();
  }
  return;
}
