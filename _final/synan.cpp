/*
 * SYNTACTIC ANALYZER
 * synan.cpp - Jakub Janecek, janecj3@fel.cvut.cz
 * This module represents an implementation of a syntactic analyzer. It uses the LEXAN module to get lexical symbols and
 * checks whether their order complies with the grammar. It is implemented as a recursive descent parser based on
 * attribute translation grammar.
 */

#include "labels.h"
#include "symboltable.h"
#include "synan.h"
#include "lexan.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

static void Program();
static void Blok();
static void UsekDeklaraciPromennych();
static void DeklaracePromennych();
static void SeznamIdentifikatoru();
static void ZbytekSeznamuIdentifikatoru();
static void OznaceniTypu();
static void ZbytekDeklaraciPromennych();
static void SlozenyPrikaz();
static void PosloupnostPrikazu();
static void Prikaz();
static void PrazdnyPrikaz();
static void PrirazovaciPrikaz();
static bool Vyraz();
static bool Znamenko();
static bool Term();
static bool Faktor();
static bool ZbytekTermu( bool );
static int MultiplikativniOperator();
static bool ZbytekVyrazu( bool );
static int AditivniOperator();
static void ZbytekPosloupnostiPrikazu();
static void PrikazIf();
static void Podminka( int );
static int RelacniOperator();
static void CastElse();
static void PrikazWhile();
static void PrikazFor();
static int CastFor();
static void PrikazWriteln();

static ofstream outFile;

static void synError( string msg ) {
  cout << endl << "SYNAN ERROR: " << msg << endl << endl;
  exit( -1 );
}

/*
 * Initializes the analyzer and calls the first rule.
 */
void initializeSynan( char * filename ) {
  outFile.open( filename, ifstream::out );
  if ( outFile.is_open() ) {
    Program();
  }
  else {
    synError( "Output couldn't be opened." );
  }
  return;
}

/*
 * Cleans up everything.
 * It needs to be called after all work has been done.
 */
void cleanUpSynan() {
  if ( outFile.is_open() ) {
    outFile.close();
  }
  cleanUpMemory();
  return;
}

static void compare( LexSymbol s ) {
  if ( symb == s ) {
    readSymb();
  }
  else {
    synError( "Comparison error." );
  }
  return;
}

static void compareIdent( char * id ) {
  if ( symb == IDENT ) {
    strcpy( id, ident );
    readSymb();
  }
  else {
    synError( "Comparison error." );
  }
  return;
}

static int compareInt() {
  int retVal;
  if ( symb == INT ) {
    retVal = number;
    readSymb();
  }
  else {
    synError( "Comparison error." );
  }
  return retVal;
}

static double compareFloat() {
  double retVal;
  if ( symb == FLOAT ) {
    retVal = realNumber;
    readSymb();
  }
  else {
    synError( "Comparison error." );
  }
  return retVal;
}

/*
 * Writes an instruction to the output.
 */
static void instruction( char * instr ) {
  outFile << instr << endl;
  return;
}

/*
 * Writes a binary operator to the output.
 */
static void binOp( char * instr, bool left, bool right ) {
  if ( left ) {
    if ( right ) {
      outFile << "d" << instr << endl;
    }
    else {
      outFile << "i2d" << endl << "d" << instr << endl;
    }
  }
  else {
    if ( right ) {
      synError( "Can't convert left operand at binOp()." );
    }
    else {
      outFile << "i" << instr << endl;
    }
  }
  return;
}

static void Program() {
  // cout << "1 Program -> program identifikator ; Blok ." << endl;
  char programName[ 32 ], instr[ 128 ];
  compare( kwPROGRAM );
  compareIdent( programName );
  compare( SCOL );
  sprintf( instr, ".class public %s", programName );
  instruction( instr );
  instruction( ".super java/lang/Object" );
  instruction( ".method public static main([Ljava/lang/String;)V" );
  instruction( ".limit stack 100" );
  instruction( ".limit locals 100" );
  Blok();
  compare( DOT );
  instruction( "return" );
  instruction( ".end method" );
  return;
}

static void Blok() {
  switch ( symb ) {
    case kwVAR: case kwBEGIN:
      // cout << "2 Blok -> UsekDeklaraciPromennych SlozenyPrikaz" << endl;
      UsekDeklaraciPromennych();
      SlozenyPrikaz();   
      break;
    default:
      synError( "At Blok()." );
  }
  return;
}

static void UsekDeklaraciPromennych() {
  switch ( symb ) {
    case kwVAR:
      // cout << "3 UsekDeklaraciPromennych -> var DeklaracePromennych ; ZbytekDeklaraciPromennych" << endl;
      readSymb();
      DeklaracePromennych();
      compare( SCOL );
      ZbytekDeklaraciPromennych();
      break;
    default:
      // cout << "4 UsekDeklaraciPromennych -> e" << endl;
      break;
  }
  return;
}

static void DeklaracePromennych() {
  // cout << "5 DeklaracePromennych -> SeznamIdentifikatoru : OznaceniTypu" << endl;
  SeznamIdentifikatoru();
  compare( COL );
  OznaceniTypu();
  return;
}

static void SeznamIdentifikatoru() {
  // cout << "6 SeznamIdentifikatoru -> identifikator ZbytekSeznamuIdentifikatoru" << endl;
  char id[ 32 ];
  compareIdent( id );
  addVariable( id );
  ZbytekSeznamuIdentifikatoru();
  return;
}

static void ZbytekSeznamuIdentifikatoru() {
  switch ( symb ) {
    case COM:
      // cout << "7 ZbytekSeznamuIdentifikatoru -> , SeznamIdentifikatoru" << endl;
      readSymb();
      SeznamIdentifikatoru();
      break;
    default:
      // cout << "8 ZbytekSeznamuIdentifikatoru -> e" << endl;
      break;
  }
  return;
}

static void OznaceniTypu() {
  switch ( symb ) {
    case kwINTEGER:
      // cout << "9 OznaceniTypu -> integer" << endl;
      changeType( INTNUM );
      readSymb();
      break;
    case kwREAL:
      // cout << "10 OznaceniTypu -> real" << endl;
      changeType( REALNUM );
      readSymb();
      break;
    default:
      synError( "At OznaceniTypu()." );
      break;
  }
  return;
}

static void ZbytekDeklaraciPromennych() {
  switch ( symb ) {
    case IDENT:
      // cout << "11 ZbytekDeklaraciPromennych -> DeklaracePromennych ; ZbytekDeklaraciPromennych" << endl;
      DeklaracePromennych();
      compare( SCOL );
      ZbytekDeklaraciPromennych();
      break;
    default:
      // cout << "12 ZbytekDeklaraciPromennych -> e" << endl;
      break;
  }
  return;
}

static void SlozenyPrikaz() {
  // cout << "13 SlozenyPrikaz -> begin PosloupnostPrikazu end" << endl;
  compare( kwBEGIN );
  PosloupnostPrikazu();
  compare( kwEND );
}

static void PosloupnostPrikazu() {
  switch ( symb ) {
    case IDENT: case kwBEGIN: case kwIF: case kwWHILE: case kwWRITELN: case SCOL: case kwEND:
      // cout << "14 PosloupnostPrikazu -> Prikaz ZbytekPosloupnostiPrikazu" << endl;   
      Prikaz();
      ZbytekPosloupnostiPrikazu();
      break;
    default:
      synError( "At PosloupnostPrikazu()." );
  }
  return;
}

static void Prikaz() {
  switch ( symb ) {
    case IDENT:
      // cout << "15 Prikaz -> PrirazovaciPrikaz" << endl;
      PrirazovaciPrikaz();
      break;
    case kwBEGIN:
      // cout << "16 Prikaz -> SlozenyPrikaz" << endl;
      SlozenyPrikaz();
      break;
    case kwIF:
      // cout << "17 Prikaz -> PrikazIf" << endl;
      PrikazIf();
      break;
    case kwWHILE:
      // cout << "18 Prikaz -> PrikazWhile" << endl;
      PrikazWhile();
      break;
    case kwFOR:
      // cout << "19 Prikaz -> PrikazFor" << endl;
      PrikazFor();
      break;
    case kwWRITELN:
      // cout << "20 Prikaz -> PrikazWriteln" << endl;
      PrikazWriteln();
      break;
    case SCOL: case kwELSE: case kwEND:
      // cout << "21 Prikaz -> PrazdnyPrikaz" << endl;
      PrazdnyPrikaz();
      break;
    default:
      synError( "At Prikaz()." );
  }
  return;
}

static void PrazdnyPrikaz() {
  // cout << "22 PrazdnyPrikaz -> e" << endl;
  return;
}

static void PrirazovaciPrikaz() {
  // cout << "23 PrirazovaciPrikaz -> identifikator := Vyraz" << endl;
  char instr[ 128 ], id[ 32 ], msg[ 128 ];
  Item * p;
  bool type;
  compareIdent( id );
  p = find( id );
  compare( ASSIGN );
  type = Vyraz();
  if ( p != NULL ) {
    if ( ( p -> type == INTNUM && !type ) || ( p -> type == REALNUM && type ) ) {
      if ( type ) {
        sprintf( instr, "dstore %d", p -> address );
        instruction( instr );
      }
      else {
        sprintf( instr, "istore %d", p -> address );
        instruction( instr );
      }
    }
    else {
      sprintf( msg, "Different variable type. Expected \'%s\'.", ( ( p -> type == INTNUM ) ? "integer" : "real" ) );
      synError( msg );
    }
  }
  else {
    sprintf( msg, "Variable \'%s\' not declared.", id );
    synError( msg );
  }
  return;
}

static bool Vyraz() {
  bool tempType, tempType1;
  switch ( symb ) {
    case PLUS: case MINUS: case IDENT: case INT: case FLOAT: case LPAR:
      // cout << "24 Vyraz -> Znamenko Term ZbytekVyrazu" << endl;
      tempType = Znamenko();
      tempType1 = Term();
      if ( !tempType ) {
        if ( tempType1 ) {
          instruction( "dneg" );
        }
        else {
          instruction( "ineg" );
        }
      }
      break;
    default:
      synError( "At Vyraz()." );
  }
  return ZbytekVyrazu( tempType1 );
}

static bool Znamenko() {
  bool retVal;   
  switch ( symb ) {
    case PLUS:
      // cout << "25 Znamenko -> +" << endl;
      readSymb();
      retVal = true; // positive
      break;
    case MINUS:
      // cout << "26 Znamenko -> -" << endl;
      readSymb();
      retVal = false; // negative
      break;
    default:
      // cout << "27 Znamenko -> e" << endl;
      retVal = true; // positive
  }
  return retVal;
}

static bool Term() {
  bool tempType, retVal;
  switch ( symb ) {
    case IDENT: case INT: case FLOAT: case LPAR:
      // cout << "28 Term -> Faktor ZbytekTermu" << endl;
      tempType = Faktor();
      retVal = ZbytekTermu( tempType );
      break;
    default:
      synError( "At Term()." );
  }
  return retVal;
}

static bool Faktor() {
  bool tempType, retVal;
  char instr[ 128 ];
  Item * p;
  switch ( symb ) {
    case IDENT:
      // cout << "29 Faktor -> identifikator" << endl;
      p = find( ident );
      if ( p != NULL ) {
        if ( p -> type == INTNUM ) {
          sprintf( instr, , p -> address );
          instruction( instr );
          readSymb();
          retVal = false;
        }
        else {
          sprintf( instr, "dload %d", p -> address );
          instruction( instr );
          readSymb();
          retVal = true;
        }
      }
      else {
        synError( "Variable not declared." );
      }
      break;
    case INT:
      // cout << "30 Faktor -> celeCislo" << endl;
      sprintf( instr, "ldc %d", compareInt() );
      instruction( instr );
      retVal = false;
      break;
    case FLOAT:
      // cout << "31 Faktor -> realneCislo" << endl;
      sprintf( instr, "ldc2_w %f", compareFloat() );
      instruction( instr );
      retVal = true;
      break;
    case LPAR:
      // cout << "32 Faktor -> ( Vyraz )" << endl;
      readSymb();
      tempType = Vyraz();
      compare( RPAR );
      retVal = tempType;
      break;
    default:
      synError( "At Faktor()." );
  }
  return retVal;
}

static bool ZbytekTermu( bool left ) {
  int retOperation;
  char * operation;
  bool right, tempType, retVal;
  switch ( symb ) {
    case ASTER: case SLASH: case kwDIV: case kwMOD:
      // cout << "33 ZbytekTermu -> MultiplikativniOperator Faktor ZbytekTermu" << endl;
      retOperation = MultiplikativniOperator();
      right = Faktor();
      switch ( retOperation ) {
        case 0:
          operation = "mul";
          break;
        case 1:
          operation = "div";
          break;
        case 2:
          operation = "rem";
          break;
        default:
          synError( "Unexpected state at ZbytekTermu()." );
      }
      binOp( operation, left, right );
      tempType = ZbytekTermu( left || right );
      retVal = tempType;
      break;
    case PLUS: case MINUS: case EQ: case NEQ: case LESS: case GREAT: case LESSEQ: case GREATEQ: case SCOL: case RPAR: case kwDO: case kwIF:
    case kwTHEN: case kwELSE: case kwTO: case kwDOWNTO: case kwWRITELN: case kwFOR: case IDENT:
      // cout << "34 ZbytekTermu -> e" << endl;
      retVal = left;
      break;
    default:
      synError( "At ZbytekTermu()." );
  }
  return retVal;
}

static int MultiplikativniOperator() {
  int retVal;
  switch ( symb ) {
    case ASTER:
      // cout << "35 MultiplikativniOperator -> *" << endl;
      readSymb();
      retVal = 0; // MUL
      break;
    case SLASH:
      // cout << "36 MultiplikativniOperator -> /" << endl;
      readSymb();
      retVal = 1; // DIV
      break;
    case kwDIV:
      // cout << "37 MultiplikativniOperator -> div" << endl;
      readSymb();
      retVal = 1; // DIV
      break;
    case kwMOD:
      // cout << "38 MultiplikativniOperator -> mod" << endl;
      readSymb();
      retVal = 2; // REM
      break;
    default:
      synError( "At MultiplikativniOperator()." );
  }
  return retVal;
}

static bool ZbytekVyrazu( bool left ) {
  int retOperation;
  char * operation;
  bool right, retVal;
  switch ( symb ) {
    case PLUS: case MINUS:
      // cout << "39 ZbytekVyrazu -> AditivniOperator Term ZbytekVyrazu" << endl;
      retOperation = AditivniOperator();
      right = Term();
      switch ( retOperation ) {
        case 0:
          operation = "add";
          break;
        case 1:
          operation = "sub";
          break;
        default:
          synError( "Unexpected state at ZbytekVyrazu()." );
      }
      binOp( operation, left, right );
      retVal = ZbytekVyrazu( left || right );
      break;
    default:
      // cout << "40 ZbytekVyrazu -> e" << endl;
      retVal = left;
  }
  return retVal;
}

static int AditivniOperator() {
  int retVal;
  switch ( symb ) {
    case PLUS:
      // cout << "41 AditivniOperator -> +" << endl;
      readSymb();
      retVal = 0; // PLUS
      break;
    case MINUS:
      // cout << "42 AditivniOperator -> -" << endl;
      readSymb();
      retVal = 1; // MINUS
      break;
    default:
      synError( "At AditivniOperator()." );
  }
  return retVal;
}

static void ZbytekPosloupnostiPrikazu() {
  switch ( symb ) {
    case SCOL:
      // cout << "43 ZbytekPosloupnostiPrikazu -> ; Prikaz ZbytekPosloupnostiPrikazu" << endl;
      readSymb();
      Prikaz();
      ZbytekPosloupnostiPrikazu();
      break;
    default:
      // cout << "44 ZbytekPosloupnostiPrikazu -> e" << endl;
      break;
  }
  return;
}

static void PrikazIf() {
  // cout << "45 PrikazIf -> if Podminka then Prikaz CastElse" << endl;
  compare( kwIF );
  Podminka( get() );
  compare( kwTHEN );
  Prikaz();
  CastElse();
  return;
}

static void Podminka( int label ) {
  bool left, right;
  int operation;
  char instr[ 128 ];
  switch ( symb ) {
    case PLUS: case MINUS: case IDENT: case INT: case FLOAT: case LPAR:
      // cout << "46 Podminka -> Vyraz RelacniOperator Vyraz" << endl;
      left = Vyraz();
      operation = RelacniOperator();
      right = Vyraz();
      if ( !left && right ) {
        synError( "Can't convert left operand." );
      }
      if ( !left && !right ) {
         switch ( operation ) {
           case 0:
             sprintf( instr, "if_icmpne Label%d", label );
             instruction( instr );
             break;
           case 1:
             sprintf( instr, "if_icmpeq Label%d", label );
             instruction( instr );
             break;
           case 2:
             sprintf( instr, "if_icmpge Label%d", label );
             instruction( instr );
             break;
           case 3:
             sprintf( instr, "if_icmple Label%d", label );
             instruction( instr );
             break;
           case 4:
             sprintf( instr, "if_icmpgt Label%d", label );
             instruction( instr );
             break;
           case 5:
             sprintf( instr, "if_icmplt Label%d", label );
             instruction( instr );
             break;
           default:
             synError( "Unexpected state at Podminka()." );
         } 
      }
      if ( left && right ) {
         switch ( operation ) {
           case 0:
             instruction( "dcmpg" );
             sprintf( instr, "ifne Label%d", label );
             instruction( instr );
             break;
           case 1:
             instruction( "dcmpg" );
             sprintf( instr, "ifeq Label%d", label );
             instruction( instr );
             break;
           case 2:
             instruction( "dcmpg" );
             sprintf( instr, "ifge Label%d", label );
             instruction( instr );
             break;
           case 3:
             instruction( "dcmpg" );
             sprintf( instr, "ifle Label%d", label );
             instruction( instr );
             break;
           case 4:
             instruction( "dcmpg" );
             sprintf( instr, "ifgt Label%d", label );
             instruction( instr );
             break;
           case 5:
             instruction( "dcmpg" );
             sprintf( instr, "iflt Label%d", label );
             instruction( instr );
             break;
           default:
             synError( "Unexpected state at Podminka()." );
         } 
      }
      if ( left && !right ) {
         switch ( operation ) {
           case 0:
             instruction( "i2d" );
             instruction( "dcmpg" );
             sprintf( instr, "ifne Label%d", label );
             instruction( instr );
             break;
           case 1:
             instruction( "i2d" );
             instruction( "dcmpg" );
             sprintf( instr, "ifeq Label%d", label );
             instruction( instr );
             break;
           case 2:
             instruction( "i2d" );
             instruction( "dcmpg" );
             sprintf( instr, "ifge Label%d", label );
             instruction( instr );
             break;
           case 3:
             instruction( "i2d" );
             instruction( "dcmpg" );
             sprintf( instr, "ifle Label%d", label );
             instruction( instr );
             break;
           case 4:
             instruction( "i2d" );
             instruction( "dmpcg" );
             sprintf( instr, "ifgt Label%d", label );
             instruction( instr );
             break;
           case 5:
             instruction( "i2d" );
             instruction( "dcmpg" );
             sprintf( instr, "iflt Label%d", label );
             instruction( instr );
             break;
           default:
             synError( "Unexpected state at Podminka()." );
         } 
      }
      break;
    default:
      synError( "At Podminka()." );
  }
  return;
}

static int RelacniOperator() {
  int retVal;
  switch ( symb ) {
    case EQ:
      // cout << "47 RelacniOperator -> =" << endl;
      readSymb();
      retVal = 0;
      break;
    case NEQ:
      // cout << "48 RelacniOperator -> <>" << endl;
      readSymb();
      retVal = 1;
      break;
    case LESS:
      // cout << "49 RelacniOperator -> <" << endl;
      readSymb();
      retVal = 2;
      break;
    case GREAT:
      // cout << "50 RelacniOperator -> >" << endl;
      readSymb();
      retVal = 3;
      break;
    case LESSEQ:
      // cout << "51 RelacniOperator -> <=" << endl;
      readSymb();
      retVal = 4;
      break;
    case GREATEQ:
      // cout << "52 RelacniOperator -> >=" << endl;
      readSymb();
      retVal = 5;
      break;
    default:
      synError( "Unexpected relational operator." );
  }
  return retVal;
}

static void CastElse() {
  char instr[ 128 ];
  int lab;
  switch ( symb ) {
    case kwELSE:
      lab = get();
      // cout << "53 CastElse -> else Prikaz" << endl;
      readSymb();
      sprintf( instr, "goto Label%d", lab );
      instruction( instr );
      sprintf( instr, "Label%d:", lab - 1 );
      instruction( instr );
      Prikaz();
      sprintf( instr, "Label%d:", lab );
      instruction( instr );
      break;
    default:
      // cout << "54 CastElse -> e" << endl;
      sprintf( instr, "Label%d:", get() - 1 );
      instruction( instr );
  }
  return;
}

static void PrikazWhile() {
  // cout << "55 PrikazWhile -> while Podminka do Prikaz" << endl;
  char instr[ 128 ];
  int lab1, lab2;
  compare( kwWHILE );
  lab1 = get();
  lab2 = get();
  sprintf( instr, "Label%d:", lab1 );
  instruction( instr );
  Podminka( lab2 );
  compare( kwDO );
  Prikaz();
  sprintf( instr, "goto Label%d", lab1 );
  instruction( instr );
  sprintf( instr, "Label%d:", lab2 );
  instruction( instr );
  return;
}

static void PrikazFor() {
  //cout << "56 PrikazFor -> for identifikator := Vyraz CastFor Vyraz do Prikaz" << endl;
  char instr[ 128 ], id[ 32 ], msg[ 128 ];
  Item * p;
  bool type;
  int lab1, lab2, lab3, direction;
  compare( kwFOR );
  compareIdent( id );
  p = find( id );
  compare( ASSIGN );
  type = Vyraz();
  if ( p != NULL ) {
    if ( ( p -> type == INTNUM && !type ) || ( p -> type == REALNUM && type ) ) {
      if ( type ) {
        sprintf( instr, "dstore %d", p -> address );
        instruction( instr );
      }
      else {
        sprintf( instr, "istore %d", p -> address );
        instruction( instr );
      }
    }
    else {
      sprintf( msg, "Different variable type. Expected \'%s\'.", ( ( p -> type == INTNUM ) ? "integer" : "real" ) );
      synError( msg );
    }
  }
  else { 
    sprintf( msg, "Variable \'%s\' not declared.", id );
    synError( msg );
  }
  lab1 = get();
  lab2 = get();
  lab3 = get();
  sprintf( instr, "goto Label%d", lab1 );
  instruction( instr );
  sprintf( instr, "Label%d:", lab2 );
  instruction( instr );
  direction = CastFor();
  if ( p != NULL ) {
    if ( direction == 0 ) {
      sprintf( instr, "iinc %d %d", p -> address , 1 );
      instruction( instr );
    }
    else if ( direction == 1 ) {
      sprintf( instr, "iinc %d %d", p -> address , -1 );
      instruction( instr );
    }
  }
  sprintf( instr, "Label%d:", lab1 );
  instruction( instr );
  sprintf( instr, "iload %d", p -> address );
  instruction( instr );
  Vyraz();
  if ( direction == 0 ) {
    sprintf( instr, "if_icmpgt Label%d", lab3 );
  instruction( instr );
  }
  else if ( direction == 1 ) {
    sprintf( instr, "if_icmplt Label%d", lab3 );
  instruction( instr );
  }
  compare( kwDO );
  Prikaz();
  sprintf( instr, "goto Label%d", lab2 );
  instruction( instr );
  sprintf( instr, "Label%d:", lab3 );
  instruction( instr );
  return;
}

static int CastFor() {
  char msg[ 128 ];
  int retVal;
  switch ( symb ) {
    case kwTO:
      // cout << "57 CastFor -> to" << endl;
      readSymb();
      retVal = 0;
      break;
    case kwDOWNTO:
      // cout << "58 CastFor -> downto" << endl;
      readSymb();
      retVal = 1;
      break;
    default:
      sprintf( msg, "Expected keyword 'to' or 'downto' but found \'%s\'.", ident );
      synError( msg );
  }
  return retVal;
}

static void PrikazWriteln() {
  // cout << "59 PrikazWriteln -> writeln ( Vyraz )" << endl;
  bool tempType;
  char cmd[ 128 ];
  compare( kwWRITELN );
  instruction( "getstatic java/lang/System/out Ljava/io/PrintStream;" );
  compare( LPAR );
  tempType = Vyraz();
  compare( RPAR );
  char ch = ( tempType == true ) ? 'D' : 'I';
  sprintf( cmd, "invokevirtual java/io/PrintStream/println(%c)V", ch);
  instruction( cmd );
  return;
}
