#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

unsigned int ns[] = { 1000, 4269, 5000, 7500, 7551, 10000, 15155, 20000, 30000, 40000/* TODO: fill values which will be used as lists' sizes */ };

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

void print_bst(struct node *node)
{
    if (node != NULL){
        print_bst(node->left);
        printf("%d ", node->key);
        print_bst(node->right);
    }
}

// tree's beginning is called the root
struct node *root = NULL;

struct node **tree_search(struct node **candidate, int value) {
    if(value < (*candidate)->key){
        return tree_search(&((*candidate)->left), value);
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
                return new_node;
            }
            ptr = ptr->right;
        }
    }
}



struct node **tree_maximum(struct node **candidate) {
    if((*candidate)->right != NULL){
        return tree_maximum(&((*candidate)->right));
    } else {
        return *candidate;
    }
}

void tree_delete(int value) {
    struct node* to_del = root;
    struct node* prev = to_del;
    bool found = false;

    // znajdz rzecz to wyrzucenia
    while(to_del && !found){
        if(value < to_del->key){
            prev = to_del;
            to_del = to_del->left;
        } else if(value > to_del->key){
            prev = to_del;
            to_del = to_del->right;
        } else {
            found = true;
        }
    }

    //przypadek gdy root jest do wyrzucenia
    if(to_del == root){
        if(!(root->left) && !(root->right)){
            root = NULL;
            free(to_del);
            return;
        }
        struct node* ptr = to_del;
        if(to_del->left){
            ptr = to_del->left;
            if(ptr->right){
                struct node* ptr_prev = ptr;
                while(ptr->right){
                    ptr_prev = ptr;
                    ptr = ptr->right;
                }

                if(ptr->left){
                    ptr_prev->right = ptr->left;
                } else{
                    ptr_prev->right = NULL;
                }

                if(to_del->right){
                    ptr->right = to_del->right;
                } else{
                    ptr->right = NULL;
                }

                ptr->left = to_del->left;
            }
            else {
                if(to_del->right){
                    ptr->right = to_del->right;
                } else{
                    ptr->right = NULL;
                }
            }
        }
        else{
            if(to_del->right){
                ptr = to_del->right;
            } else {
                root = NULL;
                free(to_del);
                return;
            }
        }
        root = ptr;
        free(to_del);
        return;
    }
    else{
     if(!(to_del->left) && !(to_del->right)){
        if(to_del == prev->left){
            prev->left = NULL;
        } else{
            prev->right = NULL;
        }
        free(to_del);
        return;

    } else if(to_del->left && !(to_del->right)){
        if(to_del == prev->left){
            prev->left = to_del->left;
            free(to_del);
            return;
        } else{
            prev->right = to_del->left;
            free(to_del);
            return;
        }
    } else if(!(to_del->left) && to_del->right){
        if(to_del == prev->left){
            prev->left = to_del->right;
            free(to_del);
            return;
        } else{
            prev->right = to_del->right;
            free(to_del);
            return;
        }
    } else if(to_del->left && to_del->right){
        struct node* ptr = to_del->left;
        struct node* ptr_prev = ptr;

        if(ptr->right){

            while(ptr->right){
                ptr_prev = ptr;
                ptr = ptr->right;
            }

            if(ptr->left){
                ptr_prev->right = ptr->left;
            }
            else {
                ptr_prev->right = NULL;
            }
            ptr->right = to_del->right;
            ptr->left = to_del->left;
        }
        else{
            ptr->right = to_del->right;
        }

        if(to_del == prev->left){
            prev->left = ptr;
        } else{
            prev->right = ptr;
        }

        free(to_del);
        return;
        }
    }
}

unsigned int tree_size(struct node *element) {
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

void insert_binary_recur(int *t, int start, int end, int stop){
    if(start > end){
        return NULL;
    }
    int mid = (start + end)/2;
    if(mid != stop){
    tree_insert(t[mid]);
    insert_binary_recur(t, start, mid-1, stop);
    insert_binary_recur(t, mid+1, end, stop);
    }
}

void insert_binary(int *t, int n) {
    insert_binary_recur(t, 0, n, n);
}

char *insert_names[] = { "Increasing", "Random", "Binary" };
void (*insert_functions[])(int*, int) = { insert_increasing, insert_random, insert_binary };

int main(int argc, char **argv) {
    printf("Array size\tInsertion name\t  Insertion time  Search time\n");
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
            printf("%10d\t%10s\t%10f\t%10f\n",
                    n,
                    insert_names[i],
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);
        }
    }
    return 0;
}
