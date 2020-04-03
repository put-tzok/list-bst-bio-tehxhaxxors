#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int ns[] = { 1000, 4269, 5000, 7500, 7551, 10000, 15155, 20000, 30000, 40000/* TODO: fill values which will be used as lists' sizes */ };

// each list node contains an integer key value and pointer to next list node
struct node {
    int key;
    struct node *next;
};

// list's beginning is called the head of list
struct node *head = NULL;


struct node* list_insert(int value) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->key = value;
    new_node->next = head;
    head = new_node;

    return new_node;
}

struct node* list_search(int value) {
    struct node* iter = head;
    while(iter != NULL && iter->key != value){
        iter = iter->next;
    }

    return iter;
}

void list_delete(int value) {
    struct node* to_del = head;
    if(to_del != NULL && to_del->key == value){
        head = head->next;
        free(to_del);
    }
    else {
        struct node* prev = head;
        while(to_del != NULL & to_del->key != value){
            prev = to_del;
            to_del = to_del->next;
        }
        if(to_del == NULL){
            return;
        }
        prev->next = to_del->next;
        free(to_del);
    }
    return;
}

unsigned int list_size() {
    unsigned int size = 0;
    struct node* iter = head;
    while(iter != NULL ){
        size++;
        iter = iter->next;
    }
    return size;
}

/*
 * Fill an array with increasing values.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void fill_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        t[i] = i;
    }
}

/*
 * Reorder array elements in a random way.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void shuffle(int *t, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % i;
        int temp = t[i];
        t[i] = t[j];
        t[j] = temp;
    }
}

int main() {
    // ZERO THE FILE - FOR DATA EXTRACTION
    FILE *data = fopen("data_list.dat", "w");
    fclose(data);

    fopen("data_list.dat", "a");
        if(data == NULL){
            fprintf(stderr, "\nProblem z otwarciem pliku data!\n");
            exit (1);
        };
        fprintf(data, "DATA FOR LIST\nArray size:\t    Shuffle\tInsertion time\tSearch time\n");
        fclose(data);

    printf("Array size:\t    Shuffle\tInsertion time\tSearch time\n");
    bool no_yes[] = { false, true };

    for (unsigned int i = 0; i < sizeof(no_yes) / sizeof(*no_yes); i++) {
        bool enable_shuffle = no_yes[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++) {
            unsigned int n = ns[j];

            // always create an array of size `n` and fill it with increasing values
            int *t = malloc(n * sizeof(*t));
            fill_increasing(t, n);

            // if true, reorder array elements randomly
            if (enable_shuffle) {
                shuffle(t, n);
            }

            // insert elements in the order present in array `t`
            clock_t insertion_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node *iter = list_insert(t[k]);
                assert(iter != NULL);       // inserted element cannot be NULL
                assert(iter->key == t[k]);  // inserted element must contain the expected value
            }
            insertion_time = clock() - insertion_time;

            // reorder array elements before searching
            shuffle(t, n);

            // search for every element in the order present in array `t`
            clock_t search_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node *iter = list_search(t[k]);
                assert(iter != NULL);       // found element cannot be NULL
                assert(iter->key == t[k]);  // found element must contain the expected value
            }
            search_time = clock() - search_time;

            // reorder array elements before deletion
            shuffle(t, n);

            // delete every element in the order present in array `t`
            for (unsigned int k = 0, l = n; k < n; k++, l--) {
                assert(list_size() == l);   // list size must be equal to the expected value
                list_delete(t[k]);
            }
            assert(list_size() == 0);       // after all deletions, the list size is zero
            assert(head == NULL);           // after all deletions, the list's head is NULL

            free(t);

            printf("%10d\t%10s\t%10f\t%10f\n", n, enable_shuffle ? "true" : "false",
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);

                    // APPEND DATA TO FILE

                FILE *data = fopen("data_list.dat", "a");
                if(data == NULL){
                    fprintf(stderr, "\nProblem z otwarciem pliku data!\n");
                    exit (1);
                };
                fprintf(data, "%10d\t%10s\t%10f\t%10f\n", n, enable_shuffle ? "true" : "false",
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);
                fclose(data);
        }
    }
    return 0;
}
