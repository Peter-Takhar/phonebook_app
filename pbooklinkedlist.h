//struct for Node for linkedlist
typedef struct node{
	NodeData *ndPtr;
	struct node *next;
}Node, *NodePtr;

//struct for linkedlist
typedef struct{
	NodePtr head, tail;
}LinkedList;

//function to initialize NodeData struct with contact information
void makeNodeData(char * fname, char * lname, char * phoneNumber, NodeData * temp){

    strcpy(temp->fname, fname);
    strcpy(temp->lname, lname);
    strcpy(temp->phoneNumber, phoneNumber);

    return;
}

//funct to initailize pointer to Node containing contact information
NodePtr makeNode(char * fname, char * lname, char * phoneNumber) {
	//initialize pointer to nodedata
	NodeData * ndtemp = (NodeData *)malloc(sizeof(NodeData));
	if (ndtemp == NULL){
		printf("Dynamic memory allocation error!\n");
		exit(1);
	}

	makeNodeData(fname, lname, phoneNumber, ndtemp);
	
	//initialize struct to contain pointer to node data
	NodePtr nptemp = (NodePtr)malloc(sizeof(Node));
	if (nptemp == NULL){
		printf("Dynamic memory allocation error!\n");
		exit(1);
	}
	nptemp->ndPtr = ndtemp;
	nptemp->next = NULL;
	return nptemp;

}

//add to linkedlist
void addtoLL(char * fname, char * lname, char * phoneNumber,LinkedList *ll){
	NodePtr temp = makeNode(fname,lname, phoneNumber);
	if (ll->head == NULL){
		ll->head = temp;
		ll->tail = temp;
	} else {
		if (ll->head->next == NULL){
			ll->head = ll->tail;
		}
		ll->tail->next = temp;
		ll->tail = temp;
	}
	return;
}

//get the correct string to compare depending on the binary tree being used
char * getNodeString(NodePtr curr, int type){
	if (type == 0){
		return curr->ndPtr->fname;
	} else if (type == 1){
		return curr->ndPtr->lname;
	} else {
		return curr->ndPtr->phoneNumber;
	}	
}

//search binary tree 
NodePtr findfromLL(char * str, NodePtr curr, int type){
	while (curr != NULL){
		if (strcmp(getNodeString(curr, type),str) == 0){
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

//delete from linkedlist
void deletefromLL(char * str, LinkedList * ll, int type){
	
	//list is empty
	if(ll->head == NULL){
		printf("No contacts to delete.");
		return;
	}
	
	//if node to delete is the only node in the list
	NodePtr temp = ll->head;
	if (strcmp(getNodeString(temp,type),str) == 0){
		NodePtr hold = temp;
		if (temp->next == NULL){
			ll->head = NULL;
			ll->tail = NULL;
			printf("Contact was deleted. List is empty.");
			return;
		} else {
			ll->head = ll->head->next;
			printf("Contact was deleted.");
		}
		free(hold->ndPtr);
		hold->ndPtr=NULL;
		free(hold);
		hold=NULL;
		return;
	}

	//loop through linkedlist until either end is reached or contatct is found
	temp = ll->head;
	while (temp->next != NULL && strcmp(getNodeString(temp->next, type),str) != 0 ){
		temp = temp->next;
	}
 
	if (temp->next != NULL){
		if (strcmp(getNodeString(temp->next, type),str) == 0){
			NodePtr hold = temp->next;
			if(temp->next == ll->tail){
					ll->tail = temp;
					ll->tail->next = NULL;
			} else {
					temp->next = temp->next->next;
			}
			free(hold->ndPtr);
			hold->ndPtr=NULL;
			free(hold);
			hold=NULL;
			printf("Contact was deleted");
			return;
		}
	}
	printf("Contact was not found.");
	return;
}

//write file contents to linkedlist
void writeLL(LinkedList * ll){
	FILE * fp;
	fp = fopen("contactList.txt", "w+");
	if (fp == NULL){
		printf("File does not exist.\n");
		return; 
	} else {
		NodePtr	curr = ll->head;
		while (curr != NULL){
			fprintf(fp,"%s %s %s\n", curr->ndPtr->fname, curr->ndPtr->lname, curr->ndPtr->phoneNumber);
			curr = curr->next;
		}
		printf("File Saved.\n");
	}
	
	fclose(fp);
}


//free all of linkedlist contents
void freeLinkedList(NodePtr curr){
	while (curr != NULL){
		NodePtr temp = curr;
		curr = curr->next;
		free(temp->ndPtr);
		temp->ndPtr= NULL;
		free(temp);
		temp= NULL;
	}
}

