/* Tentativo di replicare la stampa degli alberi di OLJ.
 * Alla fine è venuta una cosa più complessa di quanto mi
 * aspettassi, sicuramente c'è un modo migliore dal punto
 * di vista di leggibilità del codice, complessità
 * computazionale, e uso della memoria, ma vabbè,
 * questo funziona decentemente.
 * La funzione da chiamare per stampare l'albero è
 * stampa_albero.
 */

/* Determiniamo la distanza dalla sinistra nella maniera seguente.
 *
 * L'elemento corrente ha un elemento a sinistra? Se la risposta è NO possiamo partire da sinistra.
 * In caso contrario, contiamo quanti elementi ci sono a destra di quello, e quella sarà la distanza
 * tra l'elemento radice e quello alla sua sinistra, andiamo avanti finché non troviamo un elemento
 * che non abbia nulla alla sua sinistra.
 * A questa idea generale applico qualche offeset per evitare che i figli si allontanino
 * troppo dai genitori e alla fine l'algoritmo che genera l'albero delle x è incompleto
 * e quindi "aggiusto l'albero" che può avere due difetti:
 * 1. L'albero contiene nodi che si sovrappongono quando un albero ha un figlio destro
 *    che ha figli sinistri.
 * 2. L'albero può avere nodi con X negativa.
 *
 * Questi difetti li individuiamo in fix_x_tree che poi sposta il pezzo di albero (o l'intero albero)
 * in modo tale che essi vengano risolti.
 */

#include "tree_int.h"
#include <stdlib.h>
#include <stdio.h>

const elem_width = 7; // larghezza di un elemento, facciamo ad esempio che diamo tre cifre, uno spazio prima e dopo e due parentesi
Node *populate_x_tree(Node *n, Node *x_tree, int offset);

int children(Node *n) {
	if (n == NULL || (n->right == NULL && n->left == NULL)) return 0;
	if (n->right != NULL) {
		if (n->left == NULL) {
			return 1 + children(n->right);
		}
		return 2 + children(n->right) + children(n->left);
	}
	else {
		return 1 + children(n->left);
	}
}

int calculate_x(Node *n) {
	if (n == NULL || n->left == NULL) return 0;
	
	return 1+ calculate_x(n->left) + children(n->left->right);

	
}

void print_char(int n, char c, int fix) {
	// stampa n*elem_width caratteri c

	/* il fix è un parametro che non viene
	 * moltiplicato per elem_width
	 * e serve per allineare le lineette
	 * di collegamento
	 */
	for (int i = 0; i < n*elem_width+fix; i++) putc(c, stdout);
}

void print_value(int n) {
	// stampa tra parentesi con spazi
	printf("( %3d )", n);
}

void print_linking_stuff(Node *n, Node *x_tree, int offset) {
	// stampiamo le lineette di collegamento per i figli
	// l'offset è da intendere come il punto in cui si trova il cursore all'inizio

	if (n->right == NULL && n->left == NULL) return;

	if (n->left != NULL) {
		print_char(x_tree->left->value - offset, ' ', -1);
		putc('+', stdout);
		print_char(x_tree->value - x_tree->left->value, '-', -1);

	}
	else {
		print_char(x_tree->value - offset, ' ', -1);
	}
	putc('+', stdout);

	if (n->right != NULL) {
		print_char(x_tree->right->value - x_tree->value, '-', -1);
		putc('+', stdout);
	}
}

void print_level(Node **nodes, Node **xs, unsigned int len) {

	// stampiamo i numeri
	for (unsigned int i = 0; i < len; i++) {
		if (i == 0) {
			print_char(xs[i]->value, ' ', 0);
			print_value(nodes[i]->value);
		}
		else {
			print_char(xs[i]->value - xs[i - 1]->value-1, ' ', 0);
			print_value(nodes[i]->value);
		}
	}

	putc('\n', stdout);
	// stampiamo le lineette di collegamento tra ogni nodo e i suoi figli
	fputs("    ", stdout);
	for (unsigned int i = 0; i < len; i++) {
		if (i == 0) {
			print_linking_stuff(nodes[i], xs[i], 0);
		}
		else {
			int start = 0;
			for(int j = i-1; j >= 0; j--) {
				if(xs[j]->right != NULL) {
					start = xs[j]->right->value;
					break;
				}
				if(xs[j]->left != NULL) {
					start = xs[j]->value;
					break;
				}
			}
			print_linking_stuff(nodes[i], xs[i], start);
		}
	}

	putc('\n', stdout);
}

void calc_levels_rec(Node *n, Node *x_tree, Node ****node_levels, Node ****x_levels, int **level_len, int *levels_number, int curlevel) {
	/* funzione ricorsiva per dividere i nodi in livelli.
	 * fastidiosissima cosa che [] abbia precedenza su *,
	 * obbliga ad usare tutte quelle parentesi, ma immagino
	 * ci siano più use case in cui è utile che le cose siano
	 * come sono nel C.
	 */
	if (n == NULL) return;
	if (curlevel >= *levels_number) {
		*levels_number = curlevel + 1;
		*level_len = realloc(*level_len, sizeof(int) * (curlevel + 1));
		(*level_len)[curlevel] = 0;
		*node_levels = realloc(*node_levels, sizeof(Node**) * (curlevel + 1));
		(*node_levels)[curlevel] = NULL;
		*x_levels = realloc(*x_levels, sizeof(Node**) * (curlevel + 1));
		(*x_levels)[curlevel] = NULL;
	}
	(*level_len)[curlevel]++;
	(*node_levels)[curlevel] = realloc((*node_levels)[curlevel], sizeof(Node*) * (*level_len)[curlevel]);
	(*x_levels)[curlevel] = realloc((*x_levels)[curlevel], sizeof(Node*) * (*level_len)[curlevel]);
	(*node_levels)[curlevel][(*level_len)[curlevel] - 1] = n;
	(*x_levels)[curlevel][(*level_len)[curlevel] - 1] = x_tree;

	calc_levels_rec(n->left, x_tree->left, node_levels, x_levels, level_len, levels_number, curlevel + 1);
	calc_levels_rec(n->right, x_tree->right, node_levels, x_levels, level_len, levels_number, curlevel + 1);
}

void print_with_dist(Node *n, Node *x_tree) {
	// Funzione ricorsiva per stampare l'albero

	/* Penso che l'approccio migliore sia dividere l'albero in livelli
	 * in modo tale da poter andare a capo quando si può e non serve
	 * più tornare su una certa riga, poi boh sicuramente c'è una
	 * soluzione più intelligente, questa mi sembra l'unica senza
	 * dover usare delle stringhe come buffer e impazzire
	 * dietro a quelle.
	 * In pratica facciamo due vettori di nodi, uno per le X dei nodi
	 * e uno per i nodi veri e propri.
	 */
	Node *tree = n;

	
	
	Node ***node_levels = NULL; // matrice di nodi per livello
	Node ***x_levels = NULL; // matrice di x di nodi per livello
	int levels = 0;
	int *level_len = NULL;
	// passo rif. perch� facciamo le realloc e quindi cambia anche il puntatoer
	calc_levels_rec(n, x_tree, &node_levels, &x_levels, &level_len, &levels, 0);

	for (int i = 0; i < levels; i++) {
		print_level(node_levels[i], x_levels[i], level_len[i]);
		free(node_levels[i]); free(x_levels[i]);
	}

	free(node_levels); free(x_levels); free(level_len);
}

Node *populate_left_child(Node *n, Node *x_tree, int par_x) {
	if (n != NULL) {
		x_tree = malloc(sizeof(Node));
		x_tree->left = NULL;
		x_tree->right = NULL;
		x_tree->value = par_x - ((n->right != NULL) ? (1+children(n->right)) : 0)-1;
		x_tree->left = populate_left_child(n->left, x_tree->left, x_tree->value);
		x_tree->right = populate_x_tree(n->right, x_tree->right, x_tree->value + 1);

		return x_tree;
	}
	return NULL;
}


Node *populate_x_tree(Node *n, Node *x_tree, int offset) {
	// l'offset funge da baseline, essendo 1+X_genitore
	
	if (n != NULL) {
		x_tree = malloc(sizeof(Node));
		x_tree->left = NULL;
		x_tree->right = NULL;
		x_tree->value = calculate_x(n) + offset;
		x_tree->left = populate_left_child(n->left, x_tree->left, x_tree->value);
		x_tree->right = populate_x_tree(n->right, x_tree->right, x_tree->value+1);

		return x_tree;
	}
	return NULL;
}

void increase_by_1_all_the_way_down(Node *n) {
	if (n == NULL) return;
	n->value = n->value + 1;
	increase_by_1_all_the_way_down(n->left);
	increase_by_1_all_the_way_down(n->right);
}

void compare_and_fix(Node *ancestor, Node *n) {

	int min = ancestor->value;

	while (n != NULL) {
		min = n->value < min ? n->value : min;
		if (n->value <= ancestor->value) {
			increase_by_1_all_the_way_down(ancestor->right);
			continue; // magari forse a volte serve farlo più di una volta sullo stesso nodo
		}
		n = n->left;
	}
}

void fix_x_tree(Node *n) {
	/* Dobbiamo confrontare ogni nodo con i figli
	 * del nodo destro andando verso sinistra
	 * perché si rischia di finire con qualche nodo
	 * alla stessa X di un antenato. Il viceversa non
	 * dovrebbe succedere perché fa parte di come
	 * calcoliamo le X.
	 */

	Node *nc = n;

	while (n != NULL) {
		compare_and_fix(n, n->right);

		n = n->right;
	}

	n = nc;

	// Dobbiamo anche spostare l'albero a destra se ci sono nodi negativi

	while(n != NULL) {
		if(n->value < 0) {
			increase_by_1_all_the_way_down(nc);
			continue;
		}
		n = n->left;
	}


}

void stampa_albero(Node *n) {
	/* In una funzione ci calcoliamo le distanze dalla sinistra
	 * e poi in questa stampiamo l'albero in base a quelle
	 */
	if (n == NULL) return;
	Node *x_tree = NULL;

	x_tree = populate_x_tree(n, x_tree, 0);
	fix_x_tree(x_tree);

	print_with_dist(n, x_tree);
	// print_with_dist(x_tree, x_tree); // per verificare le X, strumento di debugging
}