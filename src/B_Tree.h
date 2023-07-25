#define M 2
#define MM (M*2)
#define TRUE 1
#define FALSE 0

typedef int reg;

typedef struct Page* Pointer;
typedef struct Page {
    int amount;
    reg reg[MM];
    Pointer downward[MM + 1];
}Type_Page;


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
