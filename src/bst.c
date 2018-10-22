#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "bst.h"

struct bst_node {
  char *key;
  bst left;
  bst right;
};

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
  if(t->key != NULL) {
    fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
            t->key, t->key, t->frequency,
            (RBT == tree_type && RED == t->colour) ? "red":"black");
  }
  if(t->left != NULL) {
    tree_output_dot_aux(t->left, out);
    fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
  }
  if(t->right != NULL) {
    tree_output_dot_aux(t->right, out);
    fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
  }
}

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
  fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
  tree_output_dot_aux(t, out);
  fprintf(out, "}\n");
}

bst bst_delete(bst b, char *str) {
  /*If finding key to delete doesn't exist return tree un modified*/
  if(0 == bst_search(b, str)) {
    return b;
  } else if (strcmp(str, b->key) == 0){
    /*if string exists in the current node then splice it out */
    /*three cases where we splice out a node*/
    if( b->left == NULL && b->right == NULL) { /*1. node is a leaf:*/
       free(b->key); /*free the key*/
       free(b);      /*free the node*/
       b = NULL;     /*set the node to NULL*/
    } else if (b->left == NULL && b->right != NULL ) {
      free(b->key);
      free(b);
      b = b->right;
    } else if (b->left != NULL && b->right == NULL ) {
      free(b->key);
      free(b);
      b = b->left;
    } else { /*Node has two children so we find success in right left most subtree*/
      bst successor = b->right;
      while (successor->left != NULL) {
        successor = successor->left;
      }
      b->key = successor->key; /*swap keys*/
      b = bst_delete(b, successor->key); /*delete key from right subtree*/
    }
  } else if (strcmp(str, b->key) < 0) {
    b->left = bst_delete(b->left, str);
  } else if (strcmp(str, b->key) > 0) {
    b->right = bst_delete(b->right, str);
  }
  return b;
}

void bst_inorder(bst b, void f(char *str)) {
  if(NULL == b){ /*Base case to stop recursion*/
    return;
  }
  bst_inorder(b->left, f);  /*inorder traverse left subtree*/
  f(b->key);                /*apply the function to the key*/
  bst_inorder(b->right, f); /*inorder traverse right subtree*/
}

bst bst_insert(bst b, char *str) {
  if(b == NULL){
    b = emalloc(sizeof *b);
    b->key = emalloc((strlen(str) + 1) * sizeof  str[0]);
    strcpy(b->key, str);
    return b;
  }
  else if(strcmp(str, b->key) == 0) { /*if key is same then do nothing as duplicates aren't aloud*/
    return NULL;
  }
  else if(strcmp(str, b->key) < 0) { /*if inserting key is smaller then insert into left subtree.*/
    b->left = bst_insert(b->left, str);
  }
  else if(strcmp(str, b->key) > 0) {
    b->right = bst_insert(b->right, str);
  }
  return b;
}

bst bst_new() {
  return NULL;
}

void bst_preorder(bst b, void f(char *str)) {
  if (NULL == b) {
    return;
  }
  f(b->key);
  bst_preorder(b->left, f);
  bst_preorder(b->right, f);
}

int bst_search(bst b, char *str) {
  /*if key tree is null return 0*/
  if (NULL == b) {
    return 0;
  }
  /*if the key in the current node is our key then return 1*/
  if (strcmp(b->key, str) == 0) {
    return 1;
  }
  /*if nodes key is to big return result of searching left subtree*/
  if(strcmp(str, b->key) < 0) {
    return bst_search(b->left, str);
  }
  /*if nodes key is to small return result of searching righ subtree*/
  if(strcmp(str, b->key) > 0) {
    return bst_search(b->right, str);
  }
  return 0;
}
