// define the treenode
typedef struct treeNode{
	NodePtr data;
    struct treeNode *left, *right;
}TreeNode,*TreeNodePtr;

// define the BinaryTree 
typedef struct{
	TreeNodePtr root;
	int type;
}BinaryTree;


//create tree node for binary tree containing contact info 
TreeNodePtr makeTreeNode(NodePtr d){
    TreeNodePtr node = (TreeNodePtr) malloc(sizeof(TreeNode));
	if (node == NULL){
		printf("Dynamic memory allocation error!\n");
		exit(1);
	}
    node->data = d;
    node->left = NULL;
	node->right = NULL;
    return node;
}

//get the correct string from the contact to compare user input with
char * getTreeNodeString(TreeNodePtr curr, int type){
	if (type == 0){
		return curr->data->ndPtr->fname;
	} else if (type == 1){
		return curr->data->ndPtr->lname;
	} else {
		return curr->data->ndPtr->phoneNumber;
	}	
}

//add contact to binary tree 
TreeNodePtr addbt(NodePtr llptr, TreeNodePtr root, int type){
	
	//type is used to determine which string we are comparing 
	char * str = "";
	if (type == 0)str = llptr->ndPtr->fname;
	else if (type == 1)str = llptr->ndPtr->lname;
	else str = llptr->ndPtr->phoneNumber;

    if (root == NULL){
		return makeTreeNode(llptr);
	}
	else if (strcmp(str, getTreeNodeString(root, type)) < 0) {
		root->left = addbt(llptr, root->left, type);
	}
	else if (strcmp(str, getTreeNodeString(root,type)) > 0) {
		root->right = addbt(llptr, root->right, type);
	}
  	return root;
}

//retrieve contact information from respective binary tree
TreeNodePtr retrievebt(char *str, TreeNodePtr root, int type){
	if (root == NULL){
		return NULL;
	}
	if (strcmp(str, getTreeNodeString(root, type)) == 0) return root;
	else if (strcmp(str, getTreeNodeString(root, type)) < 0) return retrievebt(str, root->left, type);
   	else if (strcmp(str, getTreeNodeString(root, type)) > 0) return retrievebt(str, root->right, type);
}


//print sorted list from binary tree
void printInorder(TreeNodePtr root){
	
	if (root == NULL){
		return;
	}
	printInorder(root->left);
	printf("\nFirst name: %s\nLast Name: %s\nPhone Number: %s\n", 
							root->data->ndPtr->fname, root->data->ndPtr->lname, root->data->ndPtr->phoneNumber);
	printInorder(root->right);
}

//funct to return true if input is valid
bool isValidInput(char tmpfname[30],char tmplname[30], char tmpphoneNumber[30]){
	return strcmp(tmpfname, " ") != 0 && strcmp(tmplname, " ") != 0 && strcmp(tmpphoneNumber, " ") != 0;
}

//free dynamically allocated memory
void freeTreeNodePtr(TreeNodePtr curr){
	free(curr);
	curr= NULL;
}

//delete binary tree through post order traversal
void deletePostOrder(TreeNodePtr root){
	
	if (root == NULL){
		return;
	}
	deletePostOrder(root->left);
	deletePostOrder(root->right);
	freeTreeNodePtr(root);
}

//write binary tree contents to file through inorder Traversal
void printInordertoFile(FILE * fp, TreeNodePtr root){
	
	if (root == NULL){
		return;
	}
	printInordertoFile(fp, root->left);
	fprintf(fp, "%s %s %s\n", root->data->ndPtr->fname, root->data->ndPtr->lname, root->data->ndPtr->phoneNumber);
	printInordertoFile(fp, root->right);
}

//write binary tree contents to file contactList
void writeBt(BinaryTree bt){
	FILE * fp;
	fp = fopen("contactList.txt", "w+");
	if (fp == NULL){
		printf("File does not exist.\n");
		return; 
	} 
	
	printInordertoFile(fp, bt.root);
	printf("File Saved.\n");

	
	fclose(fp);

}
