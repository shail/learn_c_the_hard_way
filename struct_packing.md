Struct Packing
— basic C datatypes on x86 or ARM processors don’t normally start at arbitrary byte address, each type has an 
  alignment requirement; 2-byte shorts must start on an even address, 4-byte ints or floats must start on an 
  address by 4
— jargon for this is that basic C types on x86 and ARM are self-aligned
— self-alignment makes access faster because it facilitates generating single instruction fetches and puts of
  the typed data. without alignment constraints, code might end up having to do two or more accesses spanning 
  machine-word boundaries.

Consider the following series of variable declarations in a top level C module:
char *p;
char c;
int x;

- The storage for p starts on a self-aligned 4 or 8 byte boundary depending on the machine word size. This is
  pointer alignment - strictest possible. The storage for c follows immediately, but the 4-byte alignment
  requirement of x forces a gap in the layout. It comes out as though there were a fourth intervening
  variable like this:

  char* p; 4 or 8 bytes
  char c; 1 byte
  char pad[3]; 3 bytes
  int x; 4 bytes

- The pad[3] character array represents the fact there are three bytes of waste paste in the structure. 
  If x is a long on a 64-bit machine:

  char *p; 8 bytes
  char c; 1 byte
  char pad[7]; 7 bytes
  long x; 8 bytes

- What if you put the shorter variable declaration first:

  char c;
  char pad1[M];
  char *p;
  char pad2[N];
  int x;

- In this case N will be zero because the address of x coming right after p is guaranteed to be
  pointer-aligned. Most likely c will be mapped to the FIRST byte of a machine word, in that case M will be 
  whatever padding is needed to ensure that p has pointer alignment.

- To make the variables take up less space, you could get that effect by swapping x with c in the original
  sequence. 

  char *p; 8 bytes
  long x; 8 bytes
  char c; 1 byte

- Usually for the small number of scalar variables in your C programs, a few bytes by changing the order of
  declaration will not save you enough to be significant. This technique becomes interesting when applied to
  nonscalar variables - especially structs.

- On a platform with self-aligned types, arrays of char/short/int/long/pointers have no internal padding, each
  member is automatically self-aligned at the end of the next one.
