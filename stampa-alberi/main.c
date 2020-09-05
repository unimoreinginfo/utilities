#include "tree_int.h"
#include "stampa_albero.h"
#include <stdlib.h>


Node* BstInsert(const ElemType *e, Node *n) {
    if(n == NULL) {
        Node *ret = malloc(sizeof(Node));
        ret->value = ElemCopy(e);
        ret->left = NULL;
        ret->right = NULL;
        return ret;
    }

    Node *tree = n;
    
    while(1) {
        if(tree->value > *e) {
            if(tree->left == NULL) {
                tree->left = malloc(sizeof(Node));
                tree->left->left = NULL;
                tree->left->right = NULL;
                tree->left->value = ElemCopy(e);
                return n;
            }
            tree = tree->left;
        } else {
            if(tree->right == NULL) {
                tree->right = malloc(sizeof(Node));
                tree->right->left = NULL;
                tree->right->right = NULL;
                tree->right->value = ElemCopy(e);
                return n;
            }
            tree = tree->right;
        }
    
    }
}

int main(int argc, char **argv) {


	int *a = malloc((argc-1)*sizeof(int));
	for(unsigned int i = 1; i < argc; i++) {
		a[i-1] = atoi(argv[i]);
	}

	Node *n = NULL;

	for(int i = 0; i < argc-1; i++) {
		n = BstInsert(a+i, n);
	}

	stampa_albero(n);
}