#include <stdio.h>
#include <stdlib.h>
#include "B_Tree.h"

// Function to initialize the B-tree
void inicialize(Pointer* Tree) {
    *Tree = NULL; // Set the B-tree root pointer to NULL, indicating an empty tree
}


// Function to insert a record into a B-tree page
static void insertion_page(Pointer Page, reg Register, Pointer Right_Brother) {
    short position_not_found;
    int amount_aux;
    amount_aux = Page->amount;

    // Check if the page has any records
    if (amount_aux > 0) {
        position_not_found = TRUE; // Records exist, set flag to find position
    }
    else {
        position_not_found = FALSE; // No records, insert at the beginning
    }

    // Search for the correct position to insert the record
    while (position_not_found == TRUE) {
        // Check if the record should be inserted at the end of the page
        if (Register >= Page->reg[amount_aux - 1]) {
            position_not_found = FALSE; // Record found its position, exit loop
            break;
        }

        // Shift records and pointers to the right to make space for the new record
        Page->reg[amount_aux] = Page->reg[amount_aux - 1];
        Page->downward[amount_aux + 1] = Page->downward[amount_aux];
        amount_aux--;

        // Check if the end of the page is reached
        if (amount_aux < 1) {
            position_not_found = FALSE; // End of the page, exit loop
        }
        else {
            position_not_found = TRUE; // Continue searching for the position
        }
    }

    // Insert the record and update the pointer to the right brother
    Page->reg[amount_aux] = Register;
    Page->downward[amount_aux + 1] = Right_Brother;

    // Increment the number of records in the page
    Page->amount++;
}


// Function to insert a record into the B-tree
static void insertion_primal(reg Register, Pointer Page, short* Addition, reg* Register_Return, Pointer* Page_Return) {
    long j;
    long index = 1;
    Pointer temp_page;

    // Check if the current page (node) is NULL
    if (Page == NULL) {
        *Addition = TRUE; // Tree is empty, set the flag to insert the record
        (*Register_Return) = Register; // Set the record to be inserted
        (*Page_Return) = NULL; // The parent page (node) is not affected
        return;
    }

    // Search for the correct position to insert the record in the current page
    while (index < Page->amount && Register > Page->reg[index - 1]) {
        index ++;
    }

    // Check if the record to be inserted is already present in the page
    if (Register == Page->reg[index - 1]) {
        printf("ERROR: Register already in the tree\n");
        *Addition = FALSE; // The record is already in the tree, set the flag to indicate no insertion
        return;
    }

    // If the record is less than the current record, adjust the index
    if (Register < Page->reg[index - 1]) {
        index --;
    }

    // Recursively call insertion_primal to find the correct page to insert the record
    insertion_primal(Register, Page->downward[index], Addition, Register_Return, Page_Return);

    // If the addition flag is false, it means the record is already in the tree, so no insertion is needed
    if (!*Addition) {
        return;
    }

    // Check if there is enough space in the current page to insert the record
    if (Page->amount < MM) {
        insertion_page(Page, *Register_Return, *Page_Return); // Insert the record into the current page
        *Addition = FALSE; // Set the addition flag to false as the record has been inserted
        return;
    }

    // If there is not enough space in the current page, create a new page and perform splitting
    temp_page = (Pointer)malloc(sizeof(Type_Page));
    temp_page->amount = 0;
    temp_page->downward[0] = NULL;

    // Perform the splitting based on the insertion index
    if (index < M + 1) {
        insertion_page(temp_page, Page->reg[MM - 1], Page->downward[MM]); // Insert the record into the temporary page
        Page->amount --;
        insertion_page(Page, *Register_Return, *Page_Return); // Insert the new record into the current page
    }

    else {
        insertion_page(temp_page, *Register_Return, *Page_Return); // Insert the record into the temporary page
    }

    // Move the remaining records and pointers to the temporary page
    for (j = M + 2; j <= MM; j++) {
        insertion_page(temp_page, Page->reg[j - 1], Page->downward[j]);
    }

    Page->amount = M; // Update the number of records in the current page
    temp_page->downward[0] = Page->downward[M + 1]; // Update the pointer to the right brother of the temporary page
    *Register_Return = Page->reg[M]; // Return the median record from the current page
    *Page_Return = temp_page; // Return the temporary page as the new right brother
}


// Function to insert a record into the B-tree
void insertion(reg Register, Pointer* Page) {
    short Addition;
    reg return_register;
    Type_Page* return_page, *temp_page;

    // Call the insertion_primal function to find the correct location for the record
    insertion_primal(Register, *Page, &Addition, &return_register, &return_page);

    // If the addition flag is true, it means the record needs to be inserted into the tree
    if (Addition) {
        // Create a new page to hold the record and pointers
        temp_page = (Type_Page*)malloc(sizeof(Type_Page));
        temp_page->amount = 1;
        temp_page->reg[0] = return_register;
        temp_page->downward[1] = return_page;
        temp_page->downward[0] = *Page;

        // Set the new page as the root of the tree
        *Page = temp_page;
    }
}


// Function to search for a record in the B-tree
void research(reg Reg, Pointer Page) {
    long index = 1;

    // Check if the current page (node) is NULL or if the B-tree is empty
    if (Page == NULL) {
        printf("ERROR: Page empty or register not found\n"); // Page is empty or the record is not found
        return;
    }

    // Search for the correct position of the record in the current page
    while (index < Page->amount && Reg > Page->reg[index - 1])
        index++;

    // Check if the record is found in the current page
    if (Reg == Page->reg[index - 1]) {
        Reg = Page->reg[index - 1]; // Update the passed parameter with the found record
        printf("The register is in the tree\n"); // Record is found, print message
        return;
    }

    // If the record is less than the current record, search in the left subtree
    if (Reg < Page->reg[index - 1])
        research(Reg, Page->downward[index - 1]);
    else // If the record is greater than the current record, search in the right subtree
        research(Reg, Page->downward[index]);
}

// Function to rebuild the B-tree after a deletion
void rebuild(Pointer Page, Pointer Page_Father, int Pos_Father, short* Subtract) {
    Type_Page* Aux;
    long DispAux, j;

    // Check if there is a sibling to the right of the current page
    if (Pos_Father < Page_Father->amount) {
        Aux = Page_Father->downward[Pos_Father + 1];
        DispAux = (Aux->amount - M + 1) / 2;

        // Move the separator (key) from the parent page to the current page
        Page->reg[Page->amount] = Page_Father->reg[Pos_Father];
        Page->downward[Page->amount + 1] = Aux->downward[0];
        Page->amount++;

        // Check if there is enough space in the current page for the records from the sibling
        if (DispAux > 0) {
            // Transfer the records from the sibling to the current page
            for (j = 1; j < DispAux; j++) {
                insertion_page(Page, Aux->reg[j - 1], Aux->downward[j]);
            }
            // Update the separator in the parent page
            Page_Father->reg[Pos_Father] = Aux->reg[DispAux - 1];
            // Adjust the sibling's records and pointers
            Aux->amount -= DispAux;
            for (j = 0; j < Aux->amount; j++) {
                Aux->reg[j] = Aux->reg[j + DispAux];
            }
            for (j = 0; j <= Aux->amount; j++) {
                Aux->downward[j] = Aux->downward[j + DispAux];
            }
            *Subtract = FALSE; // No need to subtract from the parent page
        } else {
            // There is not enough space in the current page for the sibling's records
            // Transfer some records from the sibling to the current page
            for (j = 1; j <= M; j++) {
                insertion_page(Page, Aux->reg[j - 1], Aux->downward[j]);
            }
            free(Aux); // Free the sibling page
            // Adjust the parent page after the removal of the separator
            for (j = Pos_Father + 1; j < Page_Father->amount; j++) {
                Page_Father->reg[j - 1] = Page_Father->reg[j];
                Page_Father->downward[j] = Page_Father->downward[j + 1];
            }

            Page_Father->amount--;
            if (Page_Father->amount >= M) {
                *Subtract = FALSE; // No need to subtract from the parent page
            }
        }
    } else {
        // The current page has no right sibling, so check the left sibling
        Aux = Page_Father->downward[Pos_Father - 1];
        DispAux = (Aux->amount - M + 1) / 2;
        
        // Move the separator (key) from the parent page to the current page
        for (j = Page->amount; j >= 1; j--) {
            Page->reg[j] = Page->reg[j - 1];
        }
        Page->reg[0] = Page_Father->reg[Pos_Father - 1];
        for (j = Page->amount; j >= 0; j--) {
            Page->downward[j + 1] = Page->downward[j];
        }
        Page->amount++;

        // Check if there is enough space in the current page for the records from the sibling
        if (DispAux > 0) {
            // Transfer the records from the sibling to the current page
            for (j = 1; j < DispAux; j++) {
                insertion_page(Page, Aux->reg[Aux->amount - j], Aux->downward[Aux->amount - j + 1]);
            }
            Page->downward[0] = Aux->downward[Aux->amount - DispAux + 1];
            Page_Father->reg[Pos_Father - 1] = Aux->reg[Aux->amount - DispAux];
            Aux->amount -= DispAux;
            *Subtract = FALSE; // No need to subtract from the parent page
        } else {
            // There is not enough space in the current page for the sibling's records
            // Transfer some records from the current page to the sibling
            for (j = 1; j <= M; j++) {
                insertion_page(Aux, Page->reg[j - 1], Page->downward[j]);
            }
            free(Page); // Free the current page
            Page_Father->amount--;
            if (Page_Father->amount >= M) {
                *Subtract = FALSE; // No need to subtract from the parent page
            }
        }
    }
}


// Function to find the predecessor of a record in the B-tree
void predecessor(Pointer Page, int Index, Pointer Page_Father, short* Subtract) {
    // Check if there is a rightmost child of the parent page (Page_Father)
    if (Page_Father->downward[Page_Father->amount] != NULL) {
        // Recursively find the predecessor in the rightmost child
        predecessor(Page, Index, Page_Father->downward[Page_Father->amount], Subtract);
        
        // Check if the rightmost child requires rebalancing
        if (*Subtract == TRUE) {
            // Rebalance the rightmost child and update the Subtract flag
            rebuild(Page_Father->downward[Page_Father->amount], Page_Father, Page_Father->amount, Subtract);
            return;
        }
    }

    // Set the predecessor key to the current page (Page) from the parent page (Page_Father)
    Page->reg[Index - 1] = Page_Father->reg[Page_Father->amount - 1];
    Page_Father->amount--;
    *Subtract = (Page_Father->amount < M);
}


// Function to remove a record from the B-tree (primal removal)
static void remove_primal(reg Delete, Pointer* Page, short* Subtract) {
    long j;
    long Index = 1;
    Pointer Pag;

    // Check if the current page is empty (NULL)
    if (*Page == NULL) {
        printf("ERROR: This register is not in the tree\n");
        return;
    }

    // Assign the current page to the variable Pag
    Pag = *Page;

    // Find the index of the record to be deleted in the current page
    while (Index < Pag->amount && Delete > Pag->reg[Index - 1])
        Index++;

    // Check if the record to be deleted is found in the current page
    if (Delete == Pag->reg[Index - 1]) {
        // Check if the record to be deleted is in a leaf node
        if (Pag->downward[Index - 1] == NULL) {
            // Remove the record from the current page
            Pag->amount--;
            if (Pag->amount < M) {
                *Subtract = TRUE;
            } else {
                *Subtract = FALSE;
            }

            // Shift the records to fill the gap left by the removed record
            for (j = Index; j <= Pag->amount; j++) {
                Pag->reg[j - 1] = Pag->reg[j];
                Pag->reg[j] = Pag->reg[j + 1];
            }
            return;
        }

        // If the record to be deleted is not in a leaf node,
        // find the predecessor of the record in the current page's child
        predecessor(*Page, Index, Pag->downward[Index - 1], Subtract);

        // Check if the child requires rebalancing
        if (*Subtract)
            rebuild(Pag->downward[Index - 1], *Page, Index - 1, Subtract);
        return;
    }

    // If the record to be deleted is not found in the current page,
    // check in the appropriate child of the current page
    if (Delete > Pag->reg[Index - 1])
        Index++;

    // Recursively call remove_primal on the appropriate child
    remove_primal(Delete, &Pag->downward[Index - 1], Subtract);

    // Check if the child requires rebalancing
    if (*Subtract)
        rebuild(Pag->downward[Index - 1], *Page, Index - 1, Subtract);
}


// Function to remove a record from the B-tree (final removal)
void remove_final(reg Delete, Pointer* Page) {
    short Substract;
    Pointer aux;

    // Call remove_primal function to perform the primal removal of the record
    remove_primal(Delete, Page, &Substract);

    // Check if the Substract flag is TRUE and the current page is empty
    if (Substract == TRUE && (*Page)->amount == 0) {
        // If the current page is empty after removal, remove it from the tree
        aux = *Page;
        *Page = aux->downward[0];
        free(aux);
    }
}

// Function to test the B-tree's primal properties
static void test_primal(Pointer Tree, int Chave, short Right_Brother) {
    int index;
    int predecessor = 0;

    // Check if the current tree node is empty (NULL)
    if (Tree == NULL)
        return;

    // Check if the leftmost son's key is greater than its father's key (Right_Brother == FALSE)
    if (Tree->reg[0] > Chave && Right_Brother == FALSE) {
        printf("ERROR: Son %12d is bigger than father %d\n", Tree->reg[0], Chave);
        return;
    }

    // Check if each key in the current node is greater than its left brother's key
    for (index = 0; index < Tree->amount; index++) {
        if (Tree->reg[index] <= predecessor) {
            printf("ERROR: Brother %ld is bigger than left brother %d\n", (long)Tree->reg[index], predecessor);
            return;
        }
        predecessor = Tree->reg[index];
    }

    // Recursively call test_primal on each child node of the current node (except the rightmost child)
    for (index = 0; index < Tree->amount; index++) {
        test_primal(Tree->downward[index], Tree->reg[index], FALSE);
    }

    // Recursively call test_primal on the rightmost child node of the current node
    test_primal(Tree->downward[Tree->amount], Tree->reg[index], TRUE);
}


// Function to test the entire B-tree for structural properties
void test(Pointer Tree) {
    int i;

    // Check if the current tree node is empty (NULL)
    if (Tree == NULL)
        return;

    // Recursively call test_primal on each child node of the current node (except the rightmost child)
    for (i = 0; i < Tree->amount; i++)
        test_primal(Tree->downward[i], Tree->reg[i], FALSE);

    // Recursively call test_primal on the rightmost child node of the current node
    test_primal(Tree->downward[Tree->amount], Tree->reg[i], TRUE);
}


// Function to print the B-tree in a primal order
void print_primal(Pointer Tree, int High) {
    long index;

    // Check if the current tree node is empty (NULL)
    if (Tree == NULL) {
        return;
    }

    // Print the level (height) of the current tree node
    printf("Height of tree %d: ", High);

    // Print the keys (registers) of the current tree node
    for (index = 0; index < Tree->amount; index++) {
        printf("%d ", Tree->reg[index]);
    }
    putchar('\n');

    // Increment the height for the next level (child nodes)
    High++;

    // Recursively call print_primal on each child node of the current node (including the rightmost child)
    for (index = 0; index <= Tree->amount; index++) {
        print_primal(Tree->downward[index], High);
    }
}


// Wrapper function to print the entire B-tree
void print(Pointer Tree) {
    int index = 0; // Initialize the height (level) to 0
    print_primal(Tree, index); // Call the print_primal function to print the B-tree
}


// Recursive function to free the memory allocated for the entire B-tree
void free_all(Pointer Tree) {
    int i;

    // Base case: If the current node is empty (NULL), return
    if (Tree == NULL) {
        return;
    }

    // Recursively free the memory for each child node of the current node
    // by calling the free_all function on each child
    for (i = 0; i <= Tree->amount; i++) {
        free_all(Tree->downward[i]);
    }

    // Free the memory allocated for the current node
    free(Tree);
}
