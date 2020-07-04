#pragma warning(disable: 4996)

#include <string>
#include <stdlib.h>
#include <time.h>

/*
 * Name: Trevor Benyack
 * Date: 2020-07-04
 * Class: CIT-145-Z01B
 * Assignment
 * Notes: This program is an exercise in implementing a doubly linked list in C. The user can add or delete a char one
 * at a time. The program then prints the output in ascending and descending order.
 * This implementation uses one pointer to point to the head of the list and another pointer to point the tail of the
 * list. Each node points to both the node behind and the node ahead of itself.
 * This program is an adaptation of Figure 12.3 from "How to Program, 8e" by
 * Paul Deitel & Harvey Deitel ((C) Copyright 1992-2015)
 */

// self-referential structure
struct listNode {
    char data; // each listNode contains a character
    struct listNode *prevPtr; // pointer to previous node
    struct listNode *nextPtr; // pointer to next node
};

typedef struct listNode ListNode; // synonym for struct listNode
typedef ListNode *ListNodePtr; // synonym for ListNode*

// prototypes
void insert(ListNodePtr *sPtr, ListNodePtr *ePtr, char value);

char deleteNode(ListNodePtr *sPtr, ListNodePtr *ePtr, char value);

int isEmpty(ListNodePtr sPtr);

void printList(ListNodePtr startPtr, ListNodePtr endPtr);

void instructions(void);

int main(void) {
    ListNodePtr startPtr = NULL; // initially there are no nodes
    ListNodePtr endPtr = NULL; // initially there are no nodes

    char item; // char entered by user

    instructions(); // display the menu
    printf("%s", "? ");
    unsigned int choice; // user's choice
    scanf("%u", &choice);

    // loop while user does not choose 3
    while (choice != 3) {

        switch (choice) {
            case 1:
                printf("%s", "\nEnter a character: ");
                scanf("\n%c", &item);
                insert(&startPtr, &endPtr, item); // insert item in list
                printList(startPtr, endPtr);
                break;
            case 2: // delete an element
                // if list is not empty
                if (!isEmpty(startPtr)) {
                    printf("%s", "\nEnter character to be deleted: ");
                    scanf("\n%c", &item);

                    // if character is found, remove it
                    if (deleteNode(&startPtr, &endPtr, item)) { // remove item
                        printf("%c deleted.\n", item);
                        printList(startPtr, endPtr);
                    } else {
                        printf("%c not found.\n\n", item);
                    }
                } else {
                    puts("List is empty.\n");
                }

                break;
            default:
                puts("Invalid choice.\n");
                instructions();
                break;
        } // end switch

        printf("%s", "? ");
        scanf("%u", &choice);
    }

    puts("End of run.");

    // Comment out the next line (system("pause")) if using a Mac
    system("pause");
    return 0;
}

// display program instructions to user
void instructions(void) {
    puts("Enter your choice:\n"
         "   1 to insert an element into the list.\n"
         "   2 to delete an element from the list.\n"
         "   3 to end.");
}

// insert a new value into the list in sorted order
void insert(ListNodePtr *sPtr, ListNodePtr *ePtr, char value) {
    ListNodePtr newPtr = (ListNodePtr) malloc(sizeof(ListNode)); // create node

    if (newPtr != NULL) { // is space available
        newPtr->data = value; // place value in node
        newPtr->prevPtr = NULL; // node does not link to another node
        newPtr->nextPtr = NULL; // node does not link to another node

        ListNodePtr previousPtr = NULL;
        ListNodePtr currentPtr = *sPtr;

        // loop to find the correct location in the list for insertion
        //
        // if currentPtr is not pointing at an empty list AND value is greater
        // than the value of the current node, then proceed to next node
        //
        // previousPtr = NULL at the beginning of a list
        // currentPtr = NULL at the end of a list
        // newPtr goes in between previousPtr and currentPtr
        while (currentPtr != NULL && value > currentPtr->data) {
            previousPtr = currentPtr; // walk to ...
            currentPtr = currentPtr->nextPtr; // ... next node
        }

        // This structure implements how the new node should be inserted into the list

        if (*sPtr == NULL && *ePtr == NULL) // if the list is empty
        {

            *sPtr = newPtr; // assign the head to newPtr
            *ePtr = newPtr; // assign the tail to newPtr


        } else if (previousPtr == NULL) // if newPtr should be inserted at the beginning of the list
        {

            currentPtr->prevPtr = newPtr; // points post-insertion node backwards to the inserted node (newPtr)
            newPtr->nextPtr = *sPtr; // points the inserted node forward to the former head node
            *sPtr = newPtr; // points the head of the list to the newly inserted node (newPtr)

        }
        else if (currentPtr == NULL) // if newPtr should be inserted at the end of the list
        {
            previousPtr->nextPtr = newPtr; // points pre-insertion node forward to the inserted node (newPtr)
            newPtr->prevPtr = previousPtr; // points the inserted node backwards to the former last node
            *ePtr = newPtr; // points the tail of the list to the newly inserted node (newPtr)

        } else // if newPtr should be inserted somewhere inside of the list
        {
            currentPtr->prevPtr = newPtr; // points post-insertion node backwards to the inserted node (newPtr)
            previousPtr->nextPtr = newPtr; // points pre-insertion node forward to the inserted node (newPtr)

            newPtr->prevPtr = previousPtr; // points the inserted node forward to the post-insertion node
            newPtr->nextPtr = currentPtr; // points the inserted node backwards to the pre-insertion node

        }
    } else {
        printf("%c not inserted. No memory available.\n", value);
    }
}

// delete a list element
char deleteNode(ListNodePtr *sPtr, ListNodePtr *ePtr, char value) {

    if (value == (*sPtr)->data) // delete first node if a match is found
    {
        ListNodePtr tempPtr = *sPtr; // hold onto node being removed

        *sPtr = (*sPtr)->nextPtr; // de-thread the node

        if (*sPtr != NULL ) // if list is not empty, set head to point backwards to NULL;
        {
            (*sPtr)->prevPtr = NULL;

        } else // else the list must be empty, so set tail to NULL
        {
            (*ePtr) = NULL;
        }

        free(tempPtr); // free the de-threaded node
        return value;

    } else {
        ListNodePtr prevNodePtr = *sPtr;
        ListNodePtr currNodePtr = (*sPtr)->nextPtr;
        ListNodePtr nextNodePtr = NULL;

        // loop to find the correct location in the list
        while (currNodePtr != NULL && currNodePtr->data != value) {
            prevNodePtr = currNodePtr; // walk to ...
            currNodePtr = currNodePtr->nextPtr; // ... next node
        }
        nextNodePtr = currNodePtr->nextPtr;

        // delete node at currNodePtr
        if (currNodePtr != NULL) {
            ListNodePtr tempPtr = currNodePtr;
            if (nextNodePtr != NULL) // if NOT at the end of the list
            {
                nextNodePtr->prevPtr = prevNodePtr; // points post-deletion node backwards to pre-deletion node
            } else // at the end of the list
            {
                *ePtr = prevNodePtr; // points tail at the pre-deletion node
            }
            prevNodePtr->nextPtr = nextNodePtr; // points pre-deletion node to post-deletion node
            free(tempPtr);
            return value;
        }
    }
}

// return 1 if the list is empty, 0 otherwise
int isEmpty(ListNodePtr sPtr) {
    return sPtr == NULL;
}

// print the list
void printList(ListNodePtr sPtr, ListNodePtr ePtr) {
    // if list is empty
    if (isEmpty(sPtr)) {
        puts("\nList is empty.\n");
    } else {
        puts("\nThe list is:");

        // Prints list in ascending order
        // while not the end of the list
        while (sPtr != NULL) {
            printf("%c --> ", sPtr->data);
            sPtr = sPtr->nextPtr;
        }
        puts("NULL\n");

        puts("The list in reverse is:");

        // Prints list in descending order
        // while not the end of the list
        while (ePtr != NULL) {
            printf("%c --> ", ePtr->data);
            ePtr = ePtr->prevPtr;
        }

        puts("NULL\n");
    }
}




/**************************************************************************
 * (C) Copyright 1992-2015 by Deitel & Associates, Inc. and               *
 * Pearson Education, Inc. All Rights Reserved.                           *
 *                                                                        *
 * DISCLAIMER: The authors and publisher of this book have used their     *
 * best efforts in preparing the book. These efforts include the          *
 * development, research, and testing of the theories and programs        *
 * to determine their effectiveness. The authors and publisher make       *
 * no warranty of any kind, expressed or implied, with regard to these    *
 * programs or to the documentation contained in these books. The authors *
 * and publisher shall not be liable in any event for incidental or       *
 * consequential damages in connection with, or arising out of, the       *
 * furnishing, performance, or use of these programs.                     *
 *************************************************************************/
