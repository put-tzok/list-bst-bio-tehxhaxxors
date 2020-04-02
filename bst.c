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
    //printf("SEARCHING FOR: %d\n", value);

    if(value < (*candidate)->key){
        return tree_search(&((*candidate)->left), value);
    }
    if(value > (*candidate)->key){
        return tree_search(&((*candidate)->right), value);
    }
    return candidate;
}

struct node* tree_insert(int value) {
    printf("INSERTING: %d\n", value);
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
        //printf("\nno siema\n");
    //printf("Candidate right: %d\n", (*candidate)->right);

    if((*candidate)->right != NULL){
    printf("Candidate key: %d\n", (*candidate)->key);

        return tree_maximum(&((*candidate)->right));
    } else {
            printf("Candidate key: %d\n", (*candidate)->key);

        return *candidate;
    }
}

void tree_delete(int value) {
    printf("\n\nDELETING: %d\n\n", value);
    struct node* to_del = root;
    struct node* prev = to_del;
    struct node* to_del_prev = prev;
    bool found = false;
    // znajdz rzecz to wyrzucenia
    while(to_del && !found){
        if(value < to_del->key){
            to_del_prev = prev;
            prev = to_del;
            to_del = to_del->left;
        } else if(value > to_del->key){
            to_del_prev = prev;
            prev = to_del;
            to_del = to_del->right;
        } else {
            found = true;
        }
    }
    //przypadek gdy root jest do wyrzucenia
    if(to_del == root){
        //printf("\nZNALAZLEM ROOT\n");
        if(!(root->left) && !(root->right)){
            root = NULL;
            free(to_del);
            return;
        }
        if(to_del->left){
            prev = tree_maximum(&(to_del->left));
            prev->right = to_del->right;
            root = prev;
            // trzeba usunac polaczenie miedzy poprzednim przed prev a prev
            if(to_del->left->right){
                struct node* fix_connection = to_del->left->right;
                //znajdz poprzedni przed prev
                while(fix_connection->right != prev){
                    fix_connection = fix_connection->right;
                }
                // ustaw odpowiednie right
                fix_connection->right = NULL;
                // teraz trzeba polaczyc minimum z poddrzewa prev->left z to_del->left
                if(prev->left){
                   fix_connection = prev;
                    while(fix_connection->left){
                        fix_connection = fix_connection->left;
                    }
                    fix_connection->left = to_del->left;
                }
            }
            free(to_del);
            return;
        } else {
            root = prev->right;
            free(to_del);
            return;
        }
        //printf("ROOT key: %d\n\n", root->key);
    }

    else{
    printf("nie robie ifa\n");
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
            //TUTAJ TRZEBA BEDZIE POSORTOWAC DRZEWO
            prev->right = to_del->left;
            free(to_del);
            return;
        }
    } else if(!(to_del->left) && to_del->right){
        //printf("Robie to\n");
        if(to_del == prev->left){
            prev->left = to_del->right;
            free(to_del);
            return;
        } else{
            //TUTAJ TRZEBA BEDZIE POSORTOWAC DRZEWO
            prev->right = to_del->right;
            free(to_del);
            return;
        }
    } else if(to_del->left && to_del->right){
        if(to_del->left){
            prev = tree_maximum(&(to_del->left));
            //printf("PREV key: %d\n", prev->key);

            printf("\nto_del key: %d\n", to_del->key);
            printf("to_del_prev key: %d\n", to_del_prev->key);
            //costam = prev
            if(to_del_prev->key > prev->key){
            // TU ZNIKAJA RZECZY!!!
printf("\nLISTA:\n");
print_bst(root);
printf("\n");
                to_del_prev->left = prev;
            } else{
                to_del_prev->right = prev;
            }
printf("\nLISTA:\n");
print_bst(root);
printf("\n");
            // trzeba usunac polaczenie miedzy poprzednim przed prev a prev
            if(to_del->left->right){
                struct node* fix_connection = to_del->left->right;
                //znajdz poprzedni przed prev

                //printf("fix connection key: %d\n", fix_connection->key);
                while(fix_connection->right != prev){
                //printf("fix connection key: %d\n", fix_connection->key);

                    fix_connection = fix_connection->right;

                }
                //printf("\n\nsiema\n\n");
                //printf("fix connection ->right key: %d\n", fix_connection->right->key);

                // ustaw odpowiednie right
                fix_connection->right = NULL;

//printf("PREV left key: %d\n", prev->left->key);
                // teraz trzeba polaczyc minimum z poddrzewa prev->left z to_del->left
                if(prev->left){
                   fix_connection = prev;
                    while(fix_connection->left){
                        //printf("fix_connection->left->key: %d\n", fix_connection->left->key);
                        fix_connection = fix_connection->left;
                    }
                    //printf("fix_connection->left->key: %d\n", to_del->left->key);
                    //printf("fix_connection->left->left->key: %d\n", to_del->left->left->key);
                    //fix_connection->left = to_del->left;
                }
            }
            free(to_del);
            return;
        } else {
            root = prev->right;
            free(to_del);
            return;
        }
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
    printf("Counter: %d\n", counter);
    print_bst(root);
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
    printf("\n\nno jestem tutaj\n\n");
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
                printf("m = %d\n", m);
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
