#include "treeMT.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TNode* makeNode(int val) {
    TNode* t = (TNode*)malloc(sizeof(TNode));
    t->left = NULL;
    t->right = NULL;
    t->val = val;
    pthread_mutex_init(&t->lock, NULL);
    return t;
}

Tree* makeEmptyTree() {
    Tree* t = (Tree*)malloc(sizeof(Tree));
    t->root = NULL;
    pthread_mutex_init(&t->lock, NULL);
    return t;
}

void insertIntoTree(Tree* t, int val) {
    pthread_mutex_lock(&t->lock);
    pthread_mutex_t* prevLock = &t->lock;

    if (!t->root) {
        t->root = makeNode(val);
        pthread_mutex_unlock(prevLock);
        return;
    }
    pthread_mutex_unlock(prevLock);

    TNode* cur = t->root;
    TNode* prev = t->root;

    while (cur) {
        prev = cur;

        pthread_mutex_lock(&cur->lock);
        if (cur->val > val) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
        pthread_mutex_unlock(prevLock);
        prevLock = &prev->lock;
    }

    if (prev->val > val) {
        prev->left = makeNode(val);
    } else {
        prev->right = makeNode(val);
    }

    pthread_mutex_unlock(prevLock);
}

void printTreeAux(TNode* root) {
    if (root == NULL)
        return;
    else {
        printf("(");
        printTreeAux(root->left);
        printf(" %d ", root->val);
        printTreeAux(root->right);
        printf(")");
    }
}

void printTree(Tree* t) {
    printTreeAux(t->root);
}

void destroyTreeAux(TNode* root) {
    if (root != NULL) {
        destroyTreeAux(root->left);
        destroyTreeAux(root->right);
        pthread_mutex_destroy(&root->lock);
        free(root);
    }
}

void destroyTree(Tree* t) {
    destroyTreeAux(t->root);
    pthread_mutex_destroy(&t->lock);
    free(t);
}
