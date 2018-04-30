#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//struct holding contact data 
typedef struct{
	char fname[30];
	char lname[30];
	char phoneNumber[30];
}NodeData;

#include "pbooklinkedlist.h" //stores functions for linkedlist
#include "phonebtree.h" //stores functions for binary trees

//load file contents of contactList.txt into linkedlist and binary trees
void loadList(LinkedList * ll, BinaryTree * btf, BinaryTree * btl, BinaryTree * btp){
	
	//temp strings to hold contact list data
	char tmpfname[30] = "";
	char tmplname[30] = "";
	char tmpphoneNumber[30] = "";

	char buffer[100];

	FILE * fp;
	fp = fopen("contactList.txt", "r");
	if (fp == NULL){
		printf("File does not exist. \n");
		return;
	} else {	
		//loop to read each line and add to linkedlist and binary tree if valid
		while(fgets(buffer, sizeof(buffer), fp) != NULL){
			if (sscanf(buffer, "%s %s %s", tmpfname, tmplname, tmpphoneNumber) == 3){
				if (isValidInput(tmpfname, tmplname, tmpphoneNumber)){
					if (findfromLL(tmpfname, ll->head, 0) == NULL || findfromLL(tmplname, ll->head, 1) == NULL || findfromLL(tmpphoneNumber, ll->head, 2) == NULL){
						//add to linkedlist
						addtoLL(tmpfname, tmplname, tmpphoneNumber, ll);
						
						//add to binary trees
						btf->root = addbt(ll->tail, btf->root, btf->type);
						btl->root = addbt(ll->tail, btl->root, btf->type);
						btp->root = addbt(ll->tail, btp->root, btf->type);
					}
				}
			}
		}
		fclose(fp);
	}	

}

//fill up binary trees with data from the linked list
void fillBtrees(LinkedList ll, BinaryTree * btfname, BinaryTree * btlname, BinaryTree * btphoneNum){
		NodePtr curr = ll.head;
		while (curr != NULL){
			btfname->root = addbt(curr, btfname->root, btfname->type);
			btlname->root = addbt(curr, btlname->root,btlname->type);
			btphoneNum->root = addbt(curr,btphoneNum->root, btphoneNum->type);
			curr = curr->next;
		}
}

//validate user input
char *getUserInput(void){
	char buffer[60];
	char * str = malloc(30);
	bool check = false;
	while (!check){
		fgets(buffer, sizeof(buffer), stdin);
		sscanf(buffer,"%s", str);
		if (strcmp(str,"") != 0)check = true;
		else printf("Not valid entry. Please enter again: ");
	}
	return str;
}

//clear all dynamically allocated memory from linkedlist, binarytrees and strings
void clearAllMemory(
	LinkedList ll, BinaryTree btfname, BinaryTree btlname, BinaryTree btphoneNum, char * fname, char * lname, char * phoneNumber){
	freeLinkedList(ll.head);
	deletePostOrder(btfname.root);
	deletePostOrder(btlname.root);
	deletePostOrder(btphoneNum.root);

	free(fname);
	fname = NULL;
	free(lname);
	lname = NULL;
	free(phoneNumber);
	phoneNumber = NULL;
}
int main(int argc,char** argv){
    
    int selection = -1; //used for selecting which action to take in menu
	int attribute = -1; //use to determine which contact data to use: 0 = fname, 1 = lname, 2 = phoneNumber
	
	//declare linkedlists and binary trees
	LinkedList ll;
	ll.head = NULL;
	ll.tail = NULL;

	BinaryTree btfname;
	btfname.root = NULL;
	btfname.type = 0;

	BinaryTree btlname;
	btlname.root = NULL;
	btlname.type = 1;

	BinaryTree btphoneNum;
	btphoneNum.root = NULL;
	btphoneNum.type = 2;
	
	//temp strings to hold user input
	char * fname = NULL;
	char * lname = NULL;
	char * phoneNumber = NULL;

    char buffer[31]; //used to store user input and then formatted later
	printf("Attempting to load phone book contacts from file.\n");
    loadList(&ll, &btfname, &btlname, &btphoneNum);
    do {
		//build ui for menu
        printf("===========================================\n");
        printf("Welcome To the Phone Book\n");
        printf("Press 1 to add a Contact\n");
        printf("Press 2 to find a contact by name\n");
        printf("Press 3 to sort contacts and display them\n");
        printf("Press 4 to delete a contact\n");
        printf("Press 5 to save your changes\n");
        printf("Press 6 to load from file\n");
        printf("Press 0 to exit this application\n");
        printf("===========================================\n");
       
		selection = -1;
		attribute = -1;

        printf("\nEnter your selection: ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer,"%d", &selection);
      	
		
        if (selection > -1 && selection < 7){
			TreeNodePtr searchTreeNode = NULL;
            switch(selection){
				//add a new contact to the phonebook app
                case 1: 
                    printf("Enter first name: ");
                    fname = getUserInput();

					printf("Enter last name: ");
                    lname = getUserInput();
				                
					printf("Enter phone number: ");
                    phoneNumber = getUserInput();
                    
					//add to linkedlists and binary trees
					addtoLL(fname, lname,phoneNumber, &ll);
					btfname.root = addbt(ll.tail,btfname.root, btfname.type);
					btlname.root = addbt(ll.tail, btlname.root, btlname.type);
					btphoneNum.root = addbt(ll.tail, btphoneNum.root, btphoneNum.type);
					printf("Contact has been added to the phone book.\n");
					
                    break;
				//search for a certain contact by either fname, lname, phoneNumber
                case 2:
					printf("Enter attribute to search\n(0 for first name, 1 for last name, 2 for phonenumber): ");
                    fgets(buffer, sizeof(buffer), stdin);
       				sscanf(buffer,"%d", &attribute);
					
					//search through a binary tree depending on the attribute chosen
					if (attribute == 0){
						printf("Enter first name to search for: ");
	                    fname = getUserInput();
						searchTreeNode = retrievebt(fname, btfname.root, btfname.type);

					} else if (attribute == 1){
						printf("Enter last name to search for: ");
	                    lname = getUserInput();
						searchTreeNode = retrievebt(lname, btlname.root, btlname.type);
						
					} else if (attribute == 2) {
						printf("Enter phone number to search for: ");
	                    phoneNumber = getUserInput();
						searchTreeNode = retrievebt(phoneNumber, btphoneNum.root, btphoneNum.type);
					} else {
						printf("Not valid entry.\n");
					}

        			//return the results of the search
                    if (searchTreeNode == NULL)
                        printf("Contact was not found.\n");
                    else {
                        printf("Contact was found.\n");
                        printf("\nFound First name: %s\nFound Last Name: %s\nFound Phone Number: %s\n", 
							searchTreeNode->data->ndPtr->fname, searchTreeNode->data->ndPtr->lname, searchTreeNode->data->ndPtr->phoneNumber);
                    }
					break;
				//get a sorted list of contact names by either fname, lname, or phoneNumber
                case 3:
					printf("Enter attribute to sort by\n(0 for first name, 1 for last name, 2 for phonenumber): ");
                    fgets(buffer, sizeof(buffer), stdin);
       				sscanf(buffer,"%d", &attribute);
					
					//print list of sorted contacts through a binary tree depending on attriubute chosen
					if (attribute == 0){
						if (btfname.root == NULL) printf("No contacts in phone book.\n");
						else printInorder(btfname.root);
					} else if (attribute == 1){
						if (btlname.root == NULL) printf("No contacts in phone book.\n");
						else printInorder(btlname.root);
						
					} else if (attribute == 2) {
						if (btphoneNum.root == NULL) printf("No contacts in phone book.\n");
						else printInorder(btphoneNum.root);
					} else {
						printf("Not valid entry.");
					}
                    
                    break;
				//delete a contact by finding the contact through  fname, lname, or phoneNumber
                case 4:

					printf("Enter attribute to delete by\n(0 for first name, 1 for last name, 2 for phonenumber): ");
                    fgets(buffer, sizeof(buffer), stdin);
       				sscanf(buffer,"%d", &attribute);
					
					//delete contact from linked list depending on which attribute chosen
					if (attribute == 0){
						printf("Enter first name to delete: ");
	                   	fname = getUserInput();
						deletefromLL(fname, &ll, attribute);
					} else if (attribute == 1){
						printf("Enter last name to delete: ");
	                    lname = getUserInput();
						deletefromLL(lname, &ll, attribute);
					} else if (attribute == 2) {
						printf("Enter phone number to delete: ");
						phoneNumber = getUserInput();
						deletefromLL(phoneNumber, &ll, attribute);
					} else {
						printf("Not valid entry.");
					} 
				
					//delete binary trees
					deletePostOrder(btfname.root);
					deletePostOrder(btlname.root);
					deletePostOrder(btphoneNum.root);
					
					//assign NULL to tree root pointers
					btfname.root = NULL;
					btlname.root = NULL;
					btphoneNum.root = NULL;
					
					//rebuild the binary trees through the linkedlist
					fillBtrees(ll,&btfname, &btlname, &btphoneNum);
                    break;
				//write current contents of phonebook to writeContact.txt
                case 5:
                   	writeBt(btlname);
					break;
				//load file contents of contentList.txt into linkedlist and binary trees
                case 6: 
                    loadList(&ll, &btfname, &btlname, &btphoneNum);
					printf("Load attempt complete.\n");
                    break;
            }
	
            printf("\n");
        } else {
			//if user picks an option not availabe on the menu
            printf("Selection must be between 0 and 6\n");
        }
		
    }while(selection != 0);
    
	//clear all dynamically located memory
	clearAllMemory(ll, btfname, btlname, btphoneNum, fname, lname, phoneNumber);    
    printf("Phone book is closed.\n");    
    return 0;
} 
