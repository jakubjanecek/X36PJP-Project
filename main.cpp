/*
 * main.cpp - Jakub Janecek, janecj3@fel.cvut.cz
 * This is just a test application which initializes lexical analyzer and syntactic analyzer, it finds out whether 
 * the input was accepted and takes care of final clean up.
 */

#include "lexan.h"
#include "synan.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main( int argc, char * argv[] ) {
  char input[ 128 ], output[ 128 ];
  if ( argc != 3 ) {
    cout << "USAGE: compiler [input] [output]" << endl;
    cout << "But I'll give you another chance ;)" << endl << "Input: ";
    cin >> setw ( sizeof ( input ) ) >> input;
    cout << "Output: ";
    cin >> setw ( sizeof ( output ) ) >> output;
    cout << endl;
  }
  else {
    strncpy( input, argv[ 1 ], ( strlen( input ) < strlen( argv[ 1 ] ) ? 128 : strlen( argv[ 1 ] ) ) );
    strncpy( output, argv[ 2 ], ( strlen( output ) < strlen( argv[ 2 ] ) ? 128 : strlen( argv[ 2 ] ) ) );
  }
  initializeLexan( input );
  readSymb();
  initializeSynan( output );
  cout << endl << "Compiling " << input << "..." << endl;
  if ( symb != EOI ) {
    cout << "Unexpected symbol at the end. Expected end of file." << endl;
  }
  else {
    cout << input << " successfully compiled to " << output << endl;
  }
  cleanUpSynan();
  cleanUp();
  return( 0 );
}
