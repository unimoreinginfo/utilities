#include <stdlib.h>
#include <stdio.h>
#include "minheap_int.h"
#define roba int* array, size_t array_size
#define robissima int* array, int first, int last

void selection_sort(roba);
void bubble_sort(roba);
void insertion_sort(roba);
void quicksort(robissima);
void mergesort(roba);
void heapsort(roba);
Heap* heapify(roba);

int main() {

	int array[5] = { 4, 30, -5, 16, 9 };
	// selection_sort(array, 5);
	// bubble_sort(array, 5);
	// insertion_sort(array, 5);
	// quicksort(array, 0, 5 - 1);
	// mergesort(array, 5);
	heapsort(array, 5);

	// ma grazie carzaccolo

	for (int i = 0; i < 5; i++) {

		printf("%d ", array[i]);

	}

	return 0;

}

void heapsort(roba) {

	/*
	
		Concetto dell'heapsort: heap a sx e array ordinato a dx
			1) creiamo un heap dall'array dato
			2) ad ogni iterazione scambiamo la testa dell'heap con la coda dell'heap
			3) si noti che, essendo questo un min heap, questo equivale a dire di scambiare l'elemento più piccolo (testa)
				con la coda dell'heap, pertanto, ad ogni iterazione, avremo l'elemento più piccolo in fondo all'heap. 
				In questo modo andremo a formare l'array ordinato a destra dell'heap.
			4) dopo aver fatto questo scambio riduciamo la dimensione dell'heap di 1, in questo modo garantiamo
				che l'elemento più piccolo che abbiamo appena scambiato non rimanga intaccato
			5) l'elemento che abbiamo messo in testa, sicuramente, non rispetta la proprietà heap, pertanto lo muoviamo in basso per tornare ad avere un min heap
		ad esempio,

		dato l'array [ 4, 30, -5, 16, 9 ]
		heapify: [ -5, 9, 4, 16, 30 ] (min heap)
		heapsort:
			[ 30, 9, 4, 16, | -5] ( "|" indica la separazione fra "array dell'heap" e "array ordinato")
			[ 4, 9, 30, 16 | -5 ]

			[ 16, 9, 30, | 4, -5 ]
			[ 9, 16, 30, | 4, -5 ]

			[ 30, 16, | 9, 4, -5 ]
			[ 16, 30, | 9, 4, -5 ]

			[ 30, | 16, 9, 4, -5 ]
			[ 30, 16, 9, 4, -5 ]

			N.B: se si volesse ordinare un array in ordine CRESCENTE, occorrerebbe un MAX-HEAP
			


	*/

	Heap* heap_array = heapify(array, array_size);
	int backup = array_size;
	for (; heap_array->size > 0;) {

		ElemSwap(&heap_array->data[0], &heap_array->data[--heap_array->size]);
		MoveDownMinHeap(heap_array, 0);

	}

	heap_array->size = backup;
	array = heap_array->data;

}

Heap* heapify(roba) {

	if (!array || !array_size)
		return;

	Heap* h = malloc(sizeof(Heap));
	h->data = array;
	h->size = array_size;

	for (int i = (array_size / 2) - 1; i >= 0; i--) {

		MoveDownMinHeap(h, i);

	}

	return h;
}

void mergesort(roba) {
	if (array_size == 1) return; // un elemento solo è un vettore ordinato
	mergesort(array, array_size / 2); // ordina la prima metà
	mergesort(array + array_size / 2, array_size - (array_size / 2)); // ordina la seconda metà

	/* La fase "merge" funziona così: abbiamo due vettori che
	 * supponiamo siano ordinati, uno va da 0 a array_size/2-1 e l'altro
	 * da array_size/2 alla fine del nostro vettore. Per unirli in un vettore
	 * ordinato lungo array_size confrontiamo il primo el. del primo vec
	 * col primo del secondo vec e mettiamo il minore nella prima posizione,
	 * la posizione da studiare nel vettore da cui abbiamo preso quell'elemento
	 * viene incrementata, si incrementa l'indice a cui inserire l'elemento
	 * nel vec temporaneo e si va avanti fino a quando non abbiamo usato tutti
	 * gli elementi in entrambi i vettori per formare un vettore unito (vmerge)
	 * lungo array_size, che quindi va nelle posizioni da 0 a array_size del vettore di partenza
	 */
	int* vmerge = malloc(array_size * sizeof(int));
	size_t index = 0;  // indice da usare per vmerge
	size_t i = 0, j = array_size / 2; // i è l'indice del vettore da 0 a array_size/2-1, j per quello da array_size/2 a array_size-1
	while (i < array_size / 2 && j < array_size) {  // prendiamo i più piccoli tra  i due vettori come spiegato nel commento grande
		if (array[i] < array[j]) vmerge[index++] = array[i++];
		else vmerge[index++] = array[j++];
	}
	/* Dobbiamo usare tutti i dati rimasti prima o poi,
	 * e può accadere che in uno dei due ne sono rimasti parecchi,
	 * e in ogni caso li prendiamo in ordine visto che uno dei vettori
	 * già abbiamo finito di utilizzarlo (come da cond del while di sopra)
	 * e quindi dobbiamo utilizzare tutto quello che è rimasto nell'altro
	 */
	while (index < array_size)
		if (i < array_size / 2) vmerge[index++] = array[i++];
		else if (j < array_size) vmerge[index++] = array[j++];

	for (size_t i = 0; i < array_size; i++)
		array[i] = vmerge[i]; // copiamo il vettore merged nel vettore serio
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
		int val = array[pivot];
		do {

			while (array[i] > val) i++;
			while (array[j] < val) j--;
			if (i <= j) {
				int tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
				i++;
				j--;
			}
		} while (i <= j);

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
	int k;
	for (int i = 1; i < array_size; i++) {

		int tmp = array[i];

		for (k = i - 1; (k >= 0) && (array[k] < tmp); k--) {

			array[k + 1] = array[k];

		}

		array[k + 1] = tmp;

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
