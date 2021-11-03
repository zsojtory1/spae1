//Zoltan Sojtory
//2442602s
//SP Exercise 1a
//This is my own work as defined in the Academic Ethics agreement I have signed.

#include "tldlist.h"
#include "date.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct stacknode StackNode;

struct tldlist{
	struct tldnode *root;
	struct date *begin;
	struct date *end;
	long size;
};

struct tldnode{
	struct tldnode *left;
	struct tldnode *right;
	char *value;
	long count;
};

struct tlditerator{
	struct stacknode *top;
};

struct stacknode{
	struct tldnode *data;
	struct stacknode *next;
};

void free_postorder(TLDNode *node);
TLDNode *insert(TLDNode *root, char* key);
void push(TLDIterator *iter, TLDNode *data);
StackNode *new_stack_node(TLDNode *data);
TLDNode *pop(TLDIterator *iter);
void iter_create_recurs(TLDIterator *iter, TLDNode *node);

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
	if(date_compare(tld->begin, d) < 0 && date_compare(tld->end, d) > 0){
		char *token, *tld_string;
		token = strtok(hostname, ".");
		while(token != NULL){
			tld_string = token;
			token = strtok(NULL, ".");
		}
		struct tldnode *temp = insert(tld->root, tld_string);
		if (temp != NULL){
			tld->size = tld->size + 1;
			return 1;
		}
	}
	return 0;
}

TLDNode *insert(TLDNode *root, char* key){
	if(root == NULL){
		struct tldnode *node = (struct tldnode *)malloc(sizeof(struct tldnode));
		node->left = node->right = NULL;
		node->value = key;
		node->count = 0;
		return node;
	}

	if(strcmp(key, root->value) == 0){
		root->count = root->count + 1;
		return root;
	}

	if(strcmp(key, root->value) < 0){
		root->left = insert(root->left, key);
	}
	else{
		root->right = insert(root->right, key);
	}
	return root;
}

long tldlist_count(TLDList *tld){
	return tld->size;
}

TLDIterator *tldlist_iter_create(TLDList *tld){
	struct tlditerator *iter;
	iter = (struct tlditerator *)malloc(sizeof(struct tlditerator));
	if(iter != NULL){
		iter_create_recurs(iter, tld->root);
		return iter;
	}
	else{
		free(iter);
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
	struct stacknode *sn = new_stack_node(data);
	sn->next = iter->top;
	iter->top = sn;
}

StackNode *new_stack_node(TLDNode *data){
	struct stacknode *sn = (struct stacknode *)malloc(sizeof(struct stacknode));
	sn->data = data;
	sn->next = NULL;
	return sn;
}

void tldlist_iter_destroy(TLDIterator *iter){
	struct stacknode *sn = iter->top;
	struct stacknode *temp;
	while(sn != NULL){
		temp = sn->next;
		free(sn);
		sn = temp;
	}
	free(iter);	
}

TLDNode *tldlist_iter_next(TLDIterator *iter){
	return pop(iter);
}

TLDNode *pop(TLDIterator *iter){
	if(iter->top == NULL){
		return NULL;
	}	
	struct stacknode *temp = iter->top;
	iter->top = iter->top->next;
	struct tldnode *result = temp->data;
	free(temp);
	return result;
}

char *tldnode_tldname(TLDNode *node){
	return node->value;
}

long tldnode_count(TLDNode *node){
	return node->count;
}
