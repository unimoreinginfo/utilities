#include <stdlib.h>
#include <stdio.h>
#define roba int* array, size_t array_size
#define robissima int* array, int first, int last

void selection_sort(roba);
void bubble_sort(roba);
void insertion_sort(roba);
void quicksort(robissima);
void mergesort(roba);

int main() {

	int array[5] = { 4, 30, -5, 16, 9 };
	// selection_sort(array, 5);
	// bubble_sort(array, 5);
	// insertion_sort(array, 5);
	// quicksort(array, 0, 5 - 1);
	mergesort(array, 5);
	
	// ma grazie carzaccolo
	
	for (int i = 0; i < 5; i++) {

		printf("%d ", array[i]);

	}

	return 0;

}

void mergesort(roba) {
	if (len == 1) return; // un elemento solo è un vettore ordinato
	mergesort(vec, len / 2); // ordina la prima metà
	mergesort(vec + len/2, len - (len/2)); // ordina la seconda metà

	/* La fase "merge" funziona così: abbiamo due vettori che
	 * supponiamo siano ordinati, uno va da 0 a len/2-1 e l'altro
	 * da len/2 alla fine del nostro vettore. Per unirli in un vettore
	 * ordinato lungo len confrontiamo il primo el. del primo vec
	 * col primo del secondo vec e mettiamo il minore nella prima posizione,
	 * la posizione da studiare nel vettore da cui abbiamo preso quell'elemento
	 * viene incrementata, si incrementa l'indice a cui inserire l'elemento
	 * nel vec temporaneo e si va avanti fino a quando non abbiamo usato tutti
	 * gli elementi in entrambi i vettori per formare un vettore unito (vmerge)
	 * lungo len, che quindi va nelle posizioni da 0 a len del vettore di partenza
	 */
	int *vmerge = malloc(len * sizeof(int));
	size_t index = 0;  // indice da usare per vmerge
	size_t i = 0, j = len/2; // i è l'indice del vettore da 0 a len/2-1, j per quello da len/2 a len-1
	while (i < len / 2 && j < len) {  // prendiamo i più piccoli tra  i due vettori come spiegato nel commento grande
		if (vec[i] < vec[j]) vmerge[index++] = vec[i++];
		else vmerge[index++] = vec[j++];
	}
	/* Dobbiamo usare tutti i dati rimasti prima o poi,
	 * e può accadere che in uno dei due ne sono rimasti parecchi,
	 * e in ogni caso li prendiamo in ordine visto che uno dei vettori
	 * già abbiamo finito di utilizzarlo (come da cond del while di sopra)
	 * e quindi dobbiamo utilizzare tutto quello che è rimasto nell'altro
	 */
	while(index < len) 
		if (i < len / 2) vmerge[index++] = vec[i++];
		else if (j < len) vmerge[index++] = vec[j++];

	for (size_t i = 0; i < len; i++)
		vec[i] = vmerge[i]; // copiamo il vettore merged nel vettore serio
	free(vmerge); // era solo una cosa temporanea
}
void quicksort(robissima) {

	/* 
		https://upload.wikimedia.org/wikipedia/commons/9/9c/Quicksort-example.gif

		le slide sono abbastanza chiare in questo.
		sostanzialmente quello che fa questo algoritmo è quello di:
			1) trovare un pivot (punto medio dell'array)
			2) isolare a destra gli elementi più piccoli, a sinistra quelli più grandi tramite il do - while (questo perché voglio ordinare in ordine decrescente, nel caso contrario basterebbe fare l'opposto)
			3) richiamo quicksort nelle due parti in cui ho spezzato il vettore (a destra e a sinistra del pivot)
			4) tramite ricorsione l'algoritmo funge
	*/

	int i, j, pivot;

	if (first < last) {

		i = first;
		j = last;
		pivot = (i + j) / 2;
		do {

			while (array[i] > array[pivot]) i++;
			while (array[j] < array[pivot]) j--;
			if (i <= j) {
				int tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
				i++;
				j--;
			}
		}
		while (i <= j);

		quicksort(array, first, j);
		quicksort(array, i, last);
		
	}

}

void insertion_sort(roba) {

	/*
	
        https://upload.wikimedia.org/wikipedia/commons/9/9c/Insertion-sort-example.gif

		questo, secondo me è il più conveniente anche da imparare:
		analizzo, per ogni elemento dell'array, tutti gli elementi che sono dopo di esso.
		suppongo quindi che a sinistra dell'elemento che sto analizzando, l'array sia ordinato.

		se trovo che l'elemento è più grande di quello in questione, vorrà dire che non sono in ordine, pertanto vanno scambiati.
		il metodo funziona perché array[i] conterrà il massimo trovato fino a quel momento

	*/

	for (int i = 0; i < array_size; i++) {

		for (int k = i; k < array_size; k++) {

			if (array[i] < array[k]) {

				int tmp = array[i];
				array[i] = array[k];
				array[k] = tmp;

			}

		}

	}

}

void bubble_sort(roba) {

	/*
		
        https://upload.wikimedia.org/wikipedia/commons/0/06/Bubble-sort.gif

		ad ogni passata swappo gli elementi che sono in ordine errato
		se trovo che l'elemento corrente è più piccolo di quello alla sua destra, allora devo swapparlo
		in questo modo sono sicuro che l'elemento più piccolo si troverà sempre in fondo all'array che sto prendendo in esame
	
	*/

	int ordered = 0;

	while (array_size > 1 && !ordered) {

		ordered = 1;
		for (int i = 0; i < array_size; i++) {
			if (i + 1 < array_size) {
				if (array[i] < array[i + 1]) {

					int tmp = array[i];
					array[i] = array[i + 1];
					array[i + 1] = tmp;
					ordered = 0;
				}
			}
		}

		array_size--;

	}

}

void selection_sort(roba) {

	unsigned int starting_index = 0;

	/*
		
        https://i2.wp.com/algorithms.tutorialhorizon.com/files/2019/01/Selection-Sort-Gif.gif?ssl=1

		ad ogni passata metto l'elemento più grande dell'array in testa alla porzione che sto analizzando
		(al primo giro l'array va da 0 fino ad array_size, al secondo da 1 fino ad array_size, ...., (array_size - 1) fino ad array_size)
		in questo modo sono certo che la parte sinistra dell'array sia sempre ordinata

	*/

	while (starting_index < array_size) {

		int max = starting_index;

		for (int i = starting_index; i < array_size; i++) {

			if (array[i] > array[max])
				max = i;

		}

		int tmp = array[starting_index];
		array[starting_index] = array[max];
		array[max] = tmp;
		
		starting_index++;

	}

}
