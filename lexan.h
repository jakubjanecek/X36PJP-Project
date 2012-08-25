typedef enum { IDENT, INT, FLOAT, kwPROGRAM, kwVAR, kwINTEGER, kwREAL, 
               kwBEGIN, kwEND, kwDIV, kwMOD, kwAND, kwOR, kwIF, kwTHEN, 
               kwELSE, kwWHILE, kwDO, kwFOR, kwTO, kwDOWNTO, kwWRITELN,
               LPAR, RPAR, COM, COL, SCOL, ASSIGN, PLUS, MINUS, ASTER,
               SLASH, EQ, NEQ, LESS, GREAT, LESSEQ, GREATEQ, DOT, COMMENT, EOI } LexSymbol;

extern LexSymbol symb;
extern char ident[ 32 ];
extern int number;
extern double realNumber;

void initializeLexan( char * );
void cleanUp();
void readSymb();
