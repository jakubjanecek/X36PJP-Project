/*
 * LEXICAL ANALYZER
 * lexan.cpp - Jakub Janecek, janecj3@fel.cvut.cz
 * This module represents an implementation of a lexical analyzer. It uses the INPUT module to get characters from a file 
 * and connects them into lexical symbols.
 */

#include "lexan.h"
#include "input.h"
#include <iostream>
#include <string>
using namespace std;

LexSymbol symb; // lexical symbol currently read
char ident[ 32 ];  // identifier, maximum length of 32 characters
int number;  // value of INT number
double realNumber; // value of FLOAT number

static int character; // currently read character
static char input; // currently read type of character
static int commentFlag = 0; // false, indicates whether we are in a comment

/*
 * Table of keywords allows us convert certain identifiers to special lexical symbols.
 */
const struct { char * word; LexSymbol symb; } tableOfKeywords[] = {
             { "program", kwPROGRAM },
             { "var", kwVAR },
             { "integer", kwINTEGER },
             { "real", kwREAL },
             { "begin", kwBEGIN },
             { "end", kwEND },
             { "div", kwDIV },
             { "mod", kwMOD },
             { "and", kwAND },
             { "or", kwOR },
             { "if", kwIF },
             { "then", kwTHEN },
             { "else", kwELSE },
             { "while", kwWHILE },
             { "do", kwDO },
             { "for", kwFOR },
             { "to", kwTO },
             { "downto", kwDOWNTO },
             { "writeln", kwWRITELN },
             { NULL, EOI }
             };

/*
 * Decides whether word is a keyword or not.
 * Returns either the keyword or the word.
 */            
static LexSymbol processKeyword( char * word ) {
  int i = 0;
  // converts to lowercase
  while ( word[ i ] != '\0' ) {
    word[ i ] = tolower( word[ i ] );
    i++;
  }
  i = 0;
  while ( tableOfKeywords[ i ].word ) 
    if ( strcmp( word, tableOfKeywords[ i ].word ) == 0 ) { 
      return tableOfKeywords[ i ].symb; // FOUND, return lexical symbol of the keyword
    }
    else {
      i++;
    }
  return IDENT; // NOT FOUND, return identifier
}

static void error( string msg ) {
  cout << "LEXAN ERROR: " << msg << endl;
  exit( -1 );
}

static void readInput() {
  char msg[ 128 ];
  character = readChar();
  if ( character == -1 && character != EOF ) {
    error( "Input error." );
  }
  if ( ( character >= 'A' && character <= 'Z' ) || ( character >= 'a' && character <= 'z' ) ) {
    input = 'l'; // letter
  }
  else if ( character >= '0' && character <= '9' ) {
    input = 'n'; // number (digit)
  }
  else if ( character == '(' || character == ')' || character == ',' || character == ':' || character == ';' || character == '=' || character == '+' || character == '-' || character == '*' || character == '/' || character == '<' || character == '>' || character == '.') {
    input = 's'; // special symbol
  }
  else if ( character == '{' || character == '}' ) {
    input = 'c'; // comment
  }
  else if ( character == ' ' || character == '\n' ) {
    input = 'd'; // delimiter
  }
  else if ( character == EOF ) {
    input = 'e'; // end
  }
  else {
    if ( !commentFlag ) {
      sprintf( msg, "Character \'%c\' not allowed here.", character );
      error( msg );
    }
    else {
      input = ( char ) character;
    }
  }
  return;
}

/*
static void processComment() {
  readInput();
  commentFlag = 1; // true
  while ( input != 'c' && character != '}' ) {
    readInput();
  }
  symb = COMMENT;
  commentFlag = 0; // false
  readInput();
  return;
}
*/

static void processLetter() {
  for ( int i = 0; i < 32; i++ ) { // clear the identifier
    ident[ i ] = '\0';
  }
  ident[ 0 ] = character;
  int pos = 1;
  readInput();
  while ( ( input == 'l' || input == 'n' ) && ( input != 's' || input != 'c' || input != 'd' || input != 'e' ) ) {
    if ( pos > 31 ) {
      error( "Identifier too long." );
    }
    ident[ pos ] = character;
    pos++;
    readInput();
  }
  symb = processKeyword( ident );
  return;
}

static float processFloat() {
  float foo = 0; // the part after decimal dot
  int c = 0; // counter of digits after decimal dot
  readInput();
  // reads and stores number just like processNumber
  while ( input == 'n' && ( input != 's' || input != 'c' || input != 'd' || input != 'e' ) ) {
    foo = 10 * foo + ( character - '0' );
    c++;
    readInput();
  }
  float x = 0.1;
  // converts number to 0.number (real number)
  for ( int i = 1; i < c; i++ ) {
    x *= 0.1;
  }
  return ( x * foo );
}

static void processNumber() {
  int floatFlag = 0; // false
  float f = 0;
  number = 0;
  realNumber = 0;
  number = character - '0';
  readInput();
  while ( ( input == 'n' || input == 's' ) && ( input != 'c' || input != 'd' || input != 'e' ) ) {
    if ( input == 's' && character == '.' ) {
      f = processFloat();
      floatFlag = 1; // true
      break; 
    }
    if ( input == 'n' ) {
      number = 10 * number + ( character - '0' );
      readInput();
    }
    else {
      break;
    }
  }
  if ( floatFlag == 0 ) {
    symb = INT;
  }
  else {
    realNumber = number + f;
    symb = FLOAT;
  }
  return;
}

static void processSpecial() {
  switch ( character ) {
    case '(':
      symb = LPAR;
      break;
    case ')':
      symb = RPAR;
      break;
    case ',':
      symb = COM;
      break;
    case ':':
      readInput();
      if ( character == '=' ) {
        symb = ASSIGN;
      }
      else {
        symb = COL;
      }
      break;
    case ';':
      symb = SCOL;
      break;
    case '+':
      symb = PLUS;
      break;
    case '-':
      symb = MINUS;
      break;
    case '*':
      symb = ASTER;
      break;
    case '/':
      symb = SLASH;
      break;
    case '=':
      symb = EQ;
      break;
    case '<' :
      readInput();
      switch ( character ) {
        case '=':
          symb = LESSEQ;
          break;
        case '>':
          symb = NEQ;
          break;
        default:
          symb = LESS;
      }
      break;
    case '>':
      readInput();
      if ( character == '=' ) {
        symb = GREATEQ;
      }
      else {
        symb = GREAT;
      }
      break;
    case '.':
      symb = DOT;
      break;
    default:
      error( "Unexpected state at processSpecial()." );
  }
  readInput();
  return;
}

/*
 * Initializes the analyzer.
 */
void initializeLexan( char * filename ) {
  if ( initializeInput( filename ) == -1 ) {
    error( "Input couldn't be opened." );
  }
  readInput();
  return;
}

/*
 * Cleans up everything.
 * It needs to be called after all work has been done.
 */
void cleanUp() {
  closeInput();
  return;
}

void readSymb() {
  while ( input == 'd' ) {
    readInput();
  }
  switch ( input ) {
    case 'l':
      processLetter();
      break;
    case 'n':
      processNumber();
      break;
    case 's':
      processSpecial();
      break;
    case 'c':
      // processComment(); - we could process the comment and get its content
      if ( character != '{' ) {
        error( "Character '}' not allowed here. Comment must start with '{'." );
      }
      readInput();
      while ( input != 'c' && character != '}' ) { // discarding comment
        readInput();
        if ( input == 'e' ) {
          error( "Unexpected end of input. Comment not closed. Must be closed with '}'." );
        }
      }
      readInput();
      readSymb();
      break;
    case 'e':
      symb = EOI;
      break;
    default:
      error( "Unexpected input." );
  }
  return;
}
