#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AccountList.h"

//  ToDo Arxikopoihsh th lista me ta posa metaforas kai o,ti allo exei sxesh me th lista

//  Adds nodes to the end of the list
void add_node(node **n, int init_amount, char *x) {
  if (*n == NULL) {
    *n = malloc(sizeof(node));
    (*n)->account_id = malloc(strlen(x+1));
    strcpy((*n)->account_id,x);
    (*n)->account_balance = init_amount;
    (*n)->next = NULL;
    (*n)->transactions = NULL;
    return;
  }
  node *temp = *n;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = malloc(sizeof(node));
  temp->next->account_id = malloc(strlen(x+1));
  strcpy(temp->next->account_id,x);
  temp->next->account_balance = init_amount;
  temp->next->next = NULL;
  temp->next->transactions = NULL;
}

//  Removes nodes from the list
void delete_node(node **n, char *x) {
  node *temp;

  if (strcmp((*n)->account_id,x) == 0) {
    temp = (*n)->next;
    free((*n)->account_id);
    free(*n);
    *n = temp;
    return;
  }

  node *current = *n;
  while (current->next != NULL) {
    if (strcmp(current->next->account_id,x) == 0) {
      temp = current->next;
      current->next = temp->next;
      //  Delete tnode
      free(temp->account_id);
      free(temp);
      break;
    }
    current = current->next;
  }
}

//  Frees the list allocated memory
void destroy_list(node **n) {
  node *current = *n;
  node *temp;
  while (current != NULL) {
    temp = current->next;
    free(current->account_id);
    // graph_delete(current->bank_account); //  Destroy the list
    free(current);
    current = temp;
  }
  *n = NULL;
}

//  Returns true if there is a node with a given id
int find_node(node *n, char *x) {
  node *current = n;

  while (current != NULL) {
    if (strcmp(current->account_id,x) == 0) {
      return 1;
    }
    current = current->next;
  }
  return 0;
}

//  Returns the node with a given id
tnode *get_node(node *n, char *x) {
  node *current = n;

  while (current != NULL) {
    if (strcmp(current->account_id,x) == 0) {
      return current->transactions;
    }
    current = current->next;
  }
  return NULL;
}

//  Prints the whole list
void print_list(node *n) {
    node *current = n;

    while (current != NULL) {
      // tnode_print(current->bank_account);
      current = current->next;
    }
}
