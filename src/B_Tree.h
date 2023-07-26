// Constants
#define TRUE 1
#define FALSE 0
#define M 2 // Minimum number of keys in a node
#define MM (M * 2) // Maximum number of keys in a node

// Data structure representing the B-tree node
typedef int reg; // Assuming the keys are of integer type

typedef struct Page* Pointer; // Define a new type 'Pointer' as an alias for a pointer to struct Page
typedef struct Page {
    short amount; // Number of keys in the node
    reg reg[MM]; // Array to store keys in the node
    Pointer downward[MM + 1]; // Array of pointers to child nodes
} Type_Page;




//Insert functions
void insertion(reg Register, Pointer *Page);

//Remove functions
void rebuild(Pointer Page,Pointer Page_Father,int Pos_Father,short* Subtract);

void predecessor(Pointer Page, int Index,Pointer Page_Father,short* Subtract);

void remove_final(reg Delete,Pointer* Page);

//Auxiliaries functions
void inicialize(Pointer* Tree);

void research(reg Reg, Pointer Page);

void test(Pointer Tree);

void print(Pointer Tree);

void free_all(Pointer Tree);
