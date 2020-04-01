#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

unsigned int ns[] = { 10, /* TODO: fill values which will be used as lists' sizes */ };

// each tree node contains an integer key and pointers to left and right children nodes
struct node {
    int key;
    struct node *left;
    struct node *right;
};

struct node* newNode(int key){
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->key = key;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
};

// tree's beginning is called the root
struct node *root = NULL;

struct node **tree_search(struct node **candidate, int value) {
    //printf("JESTEM TU\n");

    if(value < (*candidate)->key){
        return tree_search((*candidate)->left, value);
    }
    if(value > (*candidate)->key){
        return tree_search(&((*candidate)->right), value);
    }
    return candidate;
}

struct node* tree_insert(int value) {
    struct node* new_node = newNode(value);
    if(root == NULL){
        root = new_node;
        return new_node;
    }
    struct node* ptr = root;
    while(1){
        if(value == ptr->key){
            return NULL;
        }
        if(value < ptr->key){
            if(ptr->left == NULL){
                ptr->left = new_node;
                return new_node;
            }
            ptr = ptr->left;
        } else {
            if(ptr->right == NULL){
                ptr->right = new_node;

                /*
                //PRINT LISTA
                ptr = root;
                printf("LISTA:\n");
                while(ptr){
                    printf("%d\t", ptr->key);
                    ptr = ptr->right;
                }
                */

                return new_node;
            }
            ptr = ptr->right;
        }
    }
}



struct node **tree_maximum(struct node **candidate) {
    if((*candidate)->right){
        return tree_maximum((*candidate)->right);
    }
    return candidate;
}

void zero_node(struct node* node){
    node->key = NULL;
    node->left = NULL;
    node->right = NULL;
}

void tree_delete(int value) {
    struct node* to_del = root;
    to_del = root;
    bool found = false;
    while(to_del && !found){
        if(value < to_del->key){
            to_del = to_del->left;
        } else if(value > to_del->key){
            to_del = to_del->right;
        } else {
            found = true;
        }
    }
    // return current -> delete
    if(!(to_del->left) && !(to_del->right)){

        printf("\n\nno siema\n\n");
        free(to_del);

        //PRINT LISTA
                struct node* ptr = root;
                printf("LISTA:\n");
                while(ptr){
                    printf("%d\t", ptr->key);
                    ptr = ptr->right;
                }
    } else if(to_del->left && to_del->right == NULL){
        to_del = to_del->left;
    } else if(to_del->left == NULL && to_del->right){
        to_del = to_del->right;
    } else {

        struct node* max_node = tree_maximum(to_del->left);
        printf("Max_node: %d", max_node->key);
        to_del->key = max_node->key;
        max_node = max_node->left;
        printf("Max_node: %d", max_node->key);
    }
}

unsigned int tree_size(struct node *element) {
    //PRINT LISTA
                struct node* ptr = root;
                printf("LISTA:\n");
                while(ptr){
                    printf("%d\t", ptr->key);
                    ptr = ptr->right;
                }
    if(!root){
        return 0;
    }
    int counter = 0;
    if(element->left){
        counter += 1;
        counter += tree_size_rec(element->left);
    }
    if(element->right){
        counter += 1;
        counter+= tree_size_rec(element->right);
    }
    //dolicz za root
    counter++;
    printf("Counter: %d", counter);
    return counter;
}

int tree_size_rec(struct node* element){
    int counter = 0;
    if(element->left){
        counter += 1;
        counter += tree_size_rec(element->left);
    }
    if(element->right){
        counter += 1;
        counter+= tree_size_rec(element->right);
    }
    return counter;
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

/*
 * Check if tree is a valid BST.
 *
 * Parameters:
 *      struct node *element:   pointer to node to be checked
 *
 * Returns:
 *      bool:                   true if subtree rooted in "element" is a BST
 */
bool is_bst(struct node *element) {
    // empty tree is a valid BST
    if (element == NULL) {
        return true;
    }
    // leaf node is valid
    if (element->left == NULL && element->right == NULL) {
        return true;
    }
    // only right subnode? check it recursively
    if (element->left == NULL && element->right != NULL) {
        printf("CHECKING: %d\n", element->key);
        return (element->key < element->right->key) && is_bst(element->right);
    }
    // only left subnode? check it recursively
    if (element->left != NULL && element->right == NULL) {
        return (element->key > element->left->key) && is_bst(element->left);
    }
    // both subnodes present? check both recursively
    return (element->key > element->left->key)
        && (element->key < element->right->key)
        && is_bst(element->left)
        && is_bst(element->right);
}

void insert_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        tree_insert(t[i]);
    }
}

void insert_random(int *t, int n) {
    shuffle(t, n);
    for (int i = 0; i < n; i++) {
        tree_insert(t[i]);
    }
}

void insert_binary(int *t, int n) {
    // TODO: implement
}

char *insert_names[] = { "Increasing", "Random", "Binary" };
void (*insert_functions[])(int*, int) = { insert_increasing, insert_random, insert_binary };

int main(int argc, char **argv) {
    for (unsigned int i = 0; i < sizeof(insert_functions) / sizeof(*insert_functions); i++) {
        void (*insert)(int*, int) = insert_functions[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++) {
            unsigned int n = ns[j];

            // crate an array of increasing integers: 0, 1, 2, ...
            int *t = malloc(n * sizeof(*t));
            fill_increasing(t, n);

            // insert data using one of methods
            clock_t insertion_time = clock();
            insert(t, n);
            insertion_time = clock() - insertion_time;

            assert(tree_size(root) == n);       // after all insertions, tree size must be `n`
            assert(is_bst(root));               // after all insertions, tree must be valid BST

            // reorder array elements before searching
            shuffle(t, n);

            // search for every element in the order present in array `t`
            clock_t search_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node **pnode = tree_search(&root, t[k]);
                struct node *iter = *pnode;
                assert(iter != NULL);           // found element cannot be NULL
                assert(iter->key == t[k]);      // found element must contain the expected value
            }
            search_time = clock() - search_time;

            // reorder array elements before deletion
            shuffle(t, n);

            // delete every element in the order present in array `t`
            for (unsigned int l = 0, m = n; l < n; l++, m--) {
                assert(tree_size(root) == m);   // tree size must be equal to the expected value
                tree_delete(t[l]);
                assert(is_bst(root));           // after deletion, tree must still be valid BST
            }
            assert(tree_size(root) == 0);       // after all deletions, tree has size zero

            free(root);
            free(t);

            printf("%d\t%s\t%f\t%f\n",
                    n,
                    insert_names[i],
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);
        }
    }
    return 0;
}
