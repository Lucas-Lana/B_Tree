#include "B_Tree.c"

int main() {
  reg x;
  Type_Page *Tree;
  short ctrl = TRUE;
  inicialize(&Tree);

  do {
    printf("----------//----------\n");
    printf("== Select an option ==\n");
    printf("1.Insert a register\n");
    printf("2.Remove a register\n");
    printf("3.Search for a register\n");
    printf("4.Print the entire tree\n");
    printf("5.Close the application\n");
    printf("----------//----------\n");
    printf("Choice: ");
    int choice;
    scanf("%d",&choice);
    fflush(stdin);
    printf("\n");
    
    switch (choice) {
    case 1:
      printf("Enter the input keys. Type '-1' to stop inserting registers\n");
      printf("Register: ");
      scanf("%d%*[^\n]", &x);
      getchar();
      while (x != -1) { 
        insertion(x, &Tree);
        print(Tree);
        printf("\nRegister: ");
        scanf("%d%*[^\n]", &x);
        getchar();
      }
      test(Tree);
      break;

    case 2:
      printf("Enter the output keys. Type '-1' to stop removing registers\n");
      printf("Register: ");
      scanf("%d%*[^\n]", &x);
      getchar();
      while (x != -1) { 
        remove_final(x, &Tree);
        print(Tree);
        printf("Register: ");
        scanf("%d%*[^\n]", &x);
        getchar();
      }
      test(Tree);
      break;

    case 3:
      int search;
      printf("Search for a register\n");
      printf("Register: ");
      scanf("%d%*[^\n]", &search);
      research(search,Tree);
      break;
    case 4:
      print(Tree);
      break;

    case 5:
      print(Tree);
      free_all(Tree);
      printf("Exiting...\n");
      ctrl = FALSE;
      break;

    default:
      printf("Choice invalid, try again\n\n");
      continue;

    }

  }while (ctrl);
}