#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct piatto {

	int tipologia;
	int kcal;
	char* nome;

};

struct nodo {
	int valore;
	struct nodo* prev;
	struct nodo* next;
};

typedef struct nodo Nodo;
typedef struct piatto Piatto;

void _print(Piatto*, int);
bool _valid(Piatto*, int, Piatto);
void backtrack(Piatto*, Piatto*, int, int, int, int);
void es2(Nodo*);
int is_bst(Node*);
void visit(Node*, int*);

int main() {
	
	// esercizio 1
	printf("Output esercizio 1\n");
	Piatto a = { .tipologia = 3, .kcal = 100, .nome = "zebra affumicata" };
	Piatto b = { .tipologia = 1, .kcal = 200, .nome = "penne super giganti" };
	Piatto c = { .tipologia = 2, .kcal = 400, .nome = "pate' con fagioli cannellini" };
	Piatto d = { .tipologia = 1, .kcal = 350, .nome = "zuppa di cangle" };
	Piatto e = { .tipologia = 2, .kcal = 100, .nome = "zuuuz" };
	Piatto f = { .tipologia = 3, .kcal = 200, .nome = "faffino buonissimo" };

	Piatto piatti[6] = { a,b,c,d,e,f };
	Piatto *solution = calloc(3, sizeof(Piatto));
	backtrack(piatti, solution, 6, 0, 0, 0, 0);
	printf("\n\n");

	// esercizio 2
	
	Nodo node = { .valore = 1, .prev = NULL, .next = NULL };
	Nodo node1 = { .valore = 3, .prev = NULL, .next = NULL };
	Nodo node2 = { .valore = 2, .prev = NULL, .next = NULL };
	
	node.next = &node1;
	node1.next = &node2;
	node1.prev = &node;
	node2.prev = &node1;
	node2.next = NULL;

	es2(&node);

	// esercizio 3 solo definito

}

// esercizio 3

int is_bst(Node* tree) {

	int is_it = 1;
	visit(tree, &is_it);
	return is_it;
}

void visit(Node* tree, int* is_it) {

	if (!tree || !(*is_it))
		return;

	if (!tree->left && tree->right) {
		if (tree->right->value < tree->value)
			(*is_it) = 0;
		return;
	}

	if (tree->left && !tree->right) {
		if (tree->left->value > tree->value)
			(*is_it) = 0;
		return;
	}

	if (tree->left && tree->right) {
		if (tree->right->value < tree->value)
			(*is_it) = 0;

		if (tree->left->value > tree->value)
			(*is_it) = 0;
		return;
	}

	visit(tree->left, is_it);
	visit(tree->right, is_it);

}

// esercizio 2

void es2(Nodo* head) {

	Nodo* h = head;

	while (head) {

		if (head->prev && head->next) {
			int sum = head->prev->valore + head->next->valore;
			if (sum == head->valore) {

				head->prev->next = head->next;
				head->next->prev = head->prev;
				// free(head) 
				// free non messo perché non ho allocato i nodi con malloc, ma teoricamente dovreste farlo

			}
		}

		head = head->next;

	}

	head = h;

}

// esercizio 1

void backtrack(Piatto* available, Piatto* solution, int size, int index, int sol_index, int kcal, int count) {

		if (count == 3 && kcal == 700) {
			_print(solution, 3);
			return;
		}

		if (index == size)
			return;
			

	Piatto current_dish = available[index];
	if (_valid(solution, index, current_dish) && count < 3 && (kcal + current_dish.kcal <= 700)) {

		solution[index] = current_dish;
		backtrack(available, solution, size, index + 1, sol_index + 1, kcal + current_dish.kcal, count + 1);

	}
	backtrack(available, solution, size, index + 1, sol_index, kcal, count);

}

void _print(Piatto* s, int size) {

	for (int i = 0; i < size; i++)
		printf("%s: %d kcal\n", s[i].nome, s[i].kcal);

	printf("\n==\n");
}

bool _valid(Piatto* s, int size, Piatto current_dish) {

	int type = current_dish.tipologia;
	for (int i = 0; i < size; i++) {
		if (type == s[i].tipologia)
			return false;
	}
	return true;

}