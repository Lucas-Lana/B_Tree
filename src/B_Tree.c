#include <stdio.h>
#include <stdlib.h>
#include "B_Tree.h"

void inicialize(Pointer* Tree) {
    *Tree = NULL;
}

static void insertion_page(Pointer Page,reg Register,Pointer Right_Brother) {
    short position_not_found;
    int amount_aux;
    amount_aux = Page->amount;
    if( amount_aux > 0) {
        position_not_found = TRUE;
    }
    else {
        position_not_found = FALSE;
    }
    
    while (position_not_found == TRUE) {
        if (Register >= Page->reg[amount_aux - 1]) {
            position_not_found = FALSE;
            break;
        }
        Page->reg[amount_aux] = Page->reg[amount_aux - 1];
        Page->downward[amount_aux + 1] = Page->downward[amount_aux];
        amount_aux --;
        if (amount_aux < 1) {
            position_not_found = FALSE;
        }
        else {
            position_not_found = TRUE;
        }
    }
    Page->reg[amount_aux] = Register;
    Page->downward[amount_aux + 1] = Right_Brother;
    Page->amount++;
}

static void insertion_primal(reg Register,Pointer Page,short* Addition, reg* Register_Return, Pointer *Page_Return) {
    long j;
    long index = 1;
    Pointer temp_page;

    if (Page == NULL) {
        *Addition = TRUE;
        (*Register_Return) = Register;
        (*Page_Return) = NULL;
        return;
    }

    while (index < Page->amount && Register > Page->reg[index - 1]) {
        index ++;
    }

    if (Register == Page->reg[index-1]) {
        printf("ERROR: Register already in the tree\n");
        *Addition = FALSE;
        return;
    }
    
    if (Register < Page->reg[index-1]) {
        index --;
    }

    insertion_primal(Register,Page->downward[index],Addition,Register_Return,Page_Return);

    if (!*Addition) {
        return;
    }
    
    if (Page->amount < MM) {
        insertion_page(Page,*Register_Return,*Page_Return);
        *Addition = FALSE;
        return;
    }

    temp_page = (Pointer)malloc(sizeof(Type_Page));
    temp_page->amount = 0;
    temp_page->downward[0] = NULL;

    if (index < M + 1) {
        insertion_page(temp_page,Page->reg[MM - 1],Page->downward[MM]);
        Page->amount--;
        insertion_page(Page,*Register_Return,*Page_Return);
    }

    else {
        insertion_page(temp_page,*Register_Return,*Page_Return);
    }

    for (j = M + 2; j<= MM; j++) {
        insertion_page(temp_page,Page->reg[j-1],Page->downward[j]);
    }

    Page->amount = M;
    temp_page->downward[0] = Page->downward[M + 1];
    *Register_Return = Page->reg[M];
    *Page_Return = temp_page;
}

void insertion(reg Register, Pointer *Page) {
    short Addition;
    reg return_register;
    Type_Page* return_page, *temp_page;
    insertion_primal(Register,*Page,&Addition,&return_register,&return_page);
    if(Addition){
        temp_page = (Type_Page*) malloc(sizeof(Type_Page));
        temp_page->amount = 1;
        temp_page->reg[0] = return_register;
        temp_page->downward[1] = return_page;
        temp_page->downward[0] = *Page;
        *Page = temp_page;
    }
}


void research(reg Reg, Pointer Page) {
    long index = 1;
    if (Page == NULL) {
        printf("ERROR: Page empty or register not found\n");
        return;
    }
    while (index < Page->amount && Reg > Page->reg[index - 1])
        index++;
    if (Reg == Page->reg[index - 1]) {
        Reg = Page->reg[index - 1];
        printf("The register is in the tree\n");
        return;
    }
    if (Reg < Page->reg[index - 1])
        research(Reg, Page->downward[index - 1]);
    else
        research(Reg,Page->downward[index]);
}

void rebuild(Pointer Page,Pointer Page_Father,int Pos_Father,short* Subtract) {
    Type_Page* Aux;
    long DispAux, j;
    if(Pos_Father < Page_Father->amount) {
        Aux = Page_Father->downward[Pos_Father + 1];
        DispAux = (Aux->amount - M + 1)/2;
        Page->reg[Page->amount] = Page->reg[Pos_Father];
        Page->downward[Page->amount + 1] = Aux->downward[0];
        Page->amount++;
        
        if (DispAux > 0) {
            for(j = 1; j < DispAux; j++) {
                insertion_page(Page,Aux->reg[j-1],Aux->downward[j]);
            }
            Page_Father->reg[Pos_Father] = Aux->reg[DispAux - 1];
            Aux->amount -= DispAux;
            for(j = 0; j< Aux->amount; j++) {
                Aux->reg[j] = Aux->reg[j + DispAux];
            }
            for(j = 0; j<= Aux->amount; j++) {
                Aux->downward[j] = Aux->downward[j + DispAux];    
            }
            *Subtract = FALSE;
        }

        else {
            for(j = 1; j <= M; j++) {
                insertion_page(Page,Aux->reg[j-1],Aux->downward[j]);
            }
            free(Aux);
            for (j = Pos_Father + 1; j < Page_Father->amount; j++) {
                Page_Father->reg[j-1] = Page_Father->reg[j];
                Page_Father->downward[j] = Page_Father->downward[j+1];
            }

            Page_Father->amount--;
            if (Page_Father->amount >= M) {
                *Subtract = FALSE;
            }
        }
    }

    else {
        Aux = Page_Father->downward[Pos_Father-1];
        DispAux = (Aux->amount - M + 1)/2;
        for (j = Page->amount; j >= 1; j--) {
            Page->reg[j] = Page->reg[j-1];
        }
        Page->reg[0] = Page_Father->reg[Pos_Father-1];
        for (j = Page->amount; j >= 0; j--) {
            Page->downward[j+1] = Page->downward[j];
        }
        Page->amount++;
        if (DispAux > 0) {
            for (j = 1; j < DispAux; j++) {
                insertion_page(Page,Aux->reg[Aux->amount-j],Aux->downward[Aux->amount - j + 1]);
            }
            Page->downward[0] = Aux->downward[Aux->amount - DispAux + 1];
            Page->reg[Pos_Father-1] = Aux->reg[Aux->amount - DispAux];
            Aux->amount -= DispAux;
            *Subtract = FALSE;
        }
        else {
            for (j = 1; j<= M; j++) {
                insertion_page(Aux,Page->reg[j-1],Page->downward[j]);
            }
            free(Page);
            Page_Father->amount--;
            if (Page_Father->amount >= M) {
                *Subtract = FALSE;
            }
        }
    }
}

void predecessor(Pointer Page, int Index,Pointer Page_Father,short* Subtract) {
    if(Page_Father->downward[Page_Father->amount] != NULL) {
        predecessor(Page,Index,Page_Father->downward[Page_Father->amount],Subtract);
        if (*Subtract == TRUE) {
            rebuild(Page_Father->downward[Page_Father->amount],Page_Father,Page_Father->amount,Subtract);
            return;
        }
    }
    Page->reg[Index-1] = Page_Father->reg[Page_Father->amount-1];
    Page_Father->amount--;
    *Subtract = (Page_Father->amount < M);
}

static void remove_primal(reg Delete,Pointer* Page,short* Subtract) {
    long j;
    long Index = 1;
    Pointer Pag;
    if (*Page == NULL) {
        printf("ERROR: This register it isn't in the tree\n");
        return;
    }
    Pag = *Page;
    while (Index < Pag->amount && Delete > Pag->reg[Index - 1])
        Index ++;
    if (Delete == Pag->reg[Index - 1]) {
        if (Pag->downward[Index - 1] == NULL) {
            Pag->amount --;
            if (Pag->amount < M) {
            *Subtract = TRUE;
            }
            else {
                *Subtract = FALSE;
            }
            for (j = Index; j<= Pag->amount; j++) {
                Pag->reg[j - 1] = Pag->reg[j];
                Pag->reg[j] = Pag->reg[j + 1];
            }
            return;
        }
        predecessor(*Page,Index,Pag->downward[Index - 1],Subtract);
        if (*Subtract)
            rebuild(Pag->downward[Index - 1],*Page,Index - 1,Subtract);
        return;
    }
    if (Delete > Pag->reg[Index - 1])
        Index ++;
    remove_primal(Delete, &Pag->downward[Index - 1],Subtract);
    if (*Subtract)
        rebuild(Pag->downward[Index - 1],*Page,Index - 1,Subtract);
}

void remove_final(reg Delete,Pointer* Page) {
    short Substract;
    Pointer aux;
    remove_primal(Delete,Page,&Substract);
    if (Substract == TRUE && (*Page)->amount == 0) {
        aux = *Page;
        *Page = aux->downward[0];
        free(aux);
    }
}

static void test_primal(Pointer Tree,int Chave,short Right_Brother) {
    int index;
    int predecessor = 0;
    if (Tree == NULL)
        return;
    if (Tree->reg[0] > Chave && Right_Brother == FALSE) {
        printf("ERROR: Son %12d bigger than father %d\n", Tree->reg[0],Chave);
        return;
    }
    for (index = 0; index < Tree->amount; index++) {
        if (Tree->reg[index] <= predecessor) {
            printf("ERROR: Brother %ld bigger than left brother %d\n", (long)Tree->reg[index],predecessor);
            return;
        }
        predecessor = Tree->reg[index];
    }
    for (index = 0; index < Tree->amount; index++) {
        test_primal(Tree->downward[index],Tree->reg[index], FALSE);
    }
    test_primal(Tree->downward[Tree->amount],Tree->reg[index],TRUE);
}

void test(Pointer Tree) {
    int i;
    if (Tree == NULL)
        return;
    for (i = 0; i < Tree->amount; i++)
        test_primal(Tree->downward[i],Tree->reg[i],FALSE);
    test_primal(Tree->downward[Tree->amount],Tree->reg[i],TRUE);
}

static void print_primal(Pointer Tree, int High) {
    long index;
    if (Tree == NULL) {
        return;
    }
    printf("High of tree %d: ",High);
    for (index =0; index < Tree->amount; index++) {
        printf("%d ",Tree->reg[index]);
    }
    putchar('\n');
    High++;
    for (index = 0; index <= Tree->amount; index++) {
        print_primal(Tree->downward[index],High);
    }
}

void print(Pointer Tree) {
    int index = 0;
    print_primal(Tree,index);
}

void free_all(Pointer Tree) {
    int i;
    if(Tree == NULL)
        return;
    for(i = 0; i <= Tree->amount; i++) {
        free_all(Tree->downward[i]);
    }
    free(Tree);
}