//Zoltan Sojtory
//2442602s
//SP Exercise 1a
//This is my own work as defined in the Academic Ethics agreement I have signed.

#include "tldlist.h"
#include "date.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

struct tldlist{
	//representing BST with root node, begin and end dates, and size for count() function
	struct tldnode *root;
	struct date *begin;
	struct date *end;
	long size;
};

struct tldnode{
	//BST node with left, right pointers, TLD string and count for count function
	struct tldnode *left;
	struct tldnode *right;
	char *value;
	long count;
};

struct tlditerator{
	//array which acts as a stack storing all the elements of tldlist
	int top;
	unsigned size;
	struct tldnode *array;
};

void free_postorder(TLDNode *node);
TLDNode *insert(TLDNode *root, char* key);
void iter_create_recurs(TLDIterator *iter, TLDNode *node);
void push(TLDIterator *iter, TLDNode *data);
TLDNode *pop(TLDIterator *iter);
TLDNode *new_node(char* key);

TLDList *tldlist_create(Date *begin, Date *end){
	struct tldlist *tree;

	if((tree = (struct tldlist *)malloc(sizeof(struct tldlist))) != NULL){
		tree->root = NULL;
		tree->begin = begin;
		tree->end = end;
		tree->size = 0;
		return tree;
	}
	else{
		free(tree);
		return NULL;
	}
}

void tldlist_destroy(TLDList *tld){
	//free all nodes separately, then free list itself
	free_postorder(tld->root);
	free(tld);
}

void free_postorder(TLDNode *node){
	if(node == NULL){
		return;
	}
	free_postorder(node->left);
	free_postorder(node->right);
	free(node);
}

int tldlist_add(TLDList *tld, char *hostname, Date *d){
	//check if date is in range
	if(date_compare(tld->begin, d) <= 0 && date_compare(tld->end, d) >= 0){
		char *token, *tld_string;
		token = strtok(hostname, ".");
		//find last substring of hostname with delimiter "." being the TLD
		while(token != NULL){
			tld_string = token;
			token = strtok(NULL, ".");
		}
		//lowercase TLD
		for(int i =0; tld_string[i]; i++){
			tld_string[i] = tolower(tld_string[i]);
		}
		//set root if list is empty
		if(tld->root == NULL){
			tld->root = insert(tld->root, tld_string);
			tld->size = tld->size + 1;
			return 1;
		}
		if (insert(tld->root, tld_string) != NULL){
			tld->size = tld->size + 1;
			return 1;
		}
	}
	return 0;
}

TLDNode *insert(TLDNode *node, char* key){
	//if node not in list, create new node
	if(node == NULL){
		return new_node(key);
	}
	//if in list, increase its count
	if(strcmp(key, node->value) == 0){
		node->count = node->count + 1;
		return node;
	}
	if(strcmp(key, node->value) < 0){
		node->left = insert(node->left, key);
	}
	else{
		node->right = insert(node->right, key);
	}
	return node;
}

TLDNode *new_node(char* key){
	struct tldnode *tempnode = (struct tldnode *)malloc(sizeof(struct tldnode));
	if(tempnode != NULL){
		tempnode->left = NULL;
		tempnode->right = NULL;
		tempnode->value = strdup(key);
		tempnode->count = 1;
		return tempnode;
	}
	else{
		free(tempnode);
		return NULL;
	}
}

long tldlist_count(TLDList *tld){
	return tld->size;
}

TLDIterator *tldlist_iter_create(TLDList *tld){
	//malloc iterator and array in it (size=count() as it will never have to be larger
	struct tlditerator *iter = (struct tlditerator *)malloc(sizeof(struct tlditerator));
	iter->size = (int)tldlist_count(tld);
	iter->top = -1;
	iter->array = (struct tldnode *)malloc(iter->size * sizeof(struct tldnode));
	if(iter != NULL){
		//traverse BST and add all elements to iter
		iter_create_recurs(iter, tld->root);
		return iter;
	}
	else{
		tldlist_iter_destroy(iter);
		return NULL;
	}
}

void iter_create_recurs(TLDIterator *iter, TLDNode *node){
	if(node == NULL){
		return;
	}
	iter_create_recurs(iter, node->left);
	iter_create_recurs(iter, node->right);
	push(iter, node);
}

void push(TLDIterator *iter, TLDNode *data){
	iter->top = iter->top + 1;
	iter->array[iter->top] = *data;
}

void tldlist_iter_destroy(TLDIterator *iter){
	free(iter->array);
	free(iter);	
}

TLDNode *tldlist_iter_next(TLDIterator *iter){
	return pop(iter);
}

TLDNode *pop(TLDIterator *iter){
	if(iter->top == -1){
		return NULL;
	}	
	struct tldnode *result = &(iter->array[iter->top]);
	if(result != NULL){
		iter->top = iter->top - 1;
		return result;
	}
	else{
		free(result);
		return NULL;
	}
}

char *tldnode_tldname(TLDNode *node){
	return node->value;
}

long tldnode_count(TLDNode *node){
	return node->count;
}
