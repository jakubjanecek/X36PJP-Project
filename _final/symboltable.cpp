/*
 * symboltable.cpp - Jakub Janecek, janecj3@fel.cvut.cz
 * This module takes care of symbol table necessary for syntactic analyzer. It stores names of variables, their types 
 * (integer, real) and addresses. It is implemented as one-way linked list of 'Items'.
 */

#include "symboltable.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

/*
 * Constructor
 */
Item::Item( char * i, Type t, int a, Item * n ) {
  ident = new char[ strlen( i ) + 1 ];
  strcpy( ident, i );
  type = t;
  address = a;
  next = n;
}

/*
 * Destructor
 */
Item::~Item() {
  delete [] ident;
}

static Item	* head; // points to the head of the list
static int availableAddress = 0; // first available address

static void tabError( string msg ) {
  cout << endl << "SYMBOL TABLE ERROR: " << msg << endl << endl;
  exit( -1 );
}

/*
 * Adds a variable to the list (without type and address).
 */
void addVariable( char * id ) {
  char msg[ 128 ];
  Item * p = find( id );
  if ( p ) {
    sprintf( msg, "Identifier \'%s\' already used.", id );
    tabError( msg );
  }
  head = new Item( id, NONE, 0, head );
  return;
}

/*
 * Finds a variable in the list by its name otherwise it returns NULL.
 */
Item * find( char * i ) {
  Item * p = head;
  while( p ) {
    if ( strcmp( i, p -> ident ) == 0 ) { // FOUND
      return p;
    }
    else { // continue...
      p = p -> next;
    }
  }
  return NULL; // NOT FOUND
}

/*
 * Changes type of all yet undefined variables to a given type and gives them available address.
 */
void changeType( Type t ) {
  Item * p = head;
  while( p ) {
    if ( p -> type == NONE ) { // if undefined
      p -> type = t; // change type
      p -> address = availableAddress; // give address
      if ( t == REALNUM ) { // increase availableAddress according to the given type
        availableAddress += 2;
      }
      else {
        availableAddress++;
      }
    }
    p = p -> next; // continue with other Item
  }
  return;
}

/*
 * Cleans up dynamically allocated for the table.
 * It needs to be called after all work has been done.
 */
void cleanUpMemory() {
  Item * p = head;
  Item * q;
  while ( p ) {
    q = p;
    p = p -> next;
    delete q;
  }
  return;
}
