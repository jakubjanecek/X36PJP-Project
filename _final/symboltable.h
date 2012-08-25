enum Type {
  INTNUM, REALNUM, NONE
};

struct Item {
  char * ident;
  Type type;
  int address;
  Item * next;
  Item( char * i, Type t, int a, Item * n );
  ~Item();
};

void addVariable( char * i );
Item * find( char * i );
void changeType( Type t );
void cleanUpMemory();
