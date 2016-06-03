#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AccountList.h"

//  node functions

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

//  Adds an amount of money to account x
//  and updates its transactions list with y
int add_amount(node *n, int amount, char *x, char *y) {
  node *current = n;

  while (current != NULL) {
    if (strcmp(current->account_id,x) == 0) {
      current->account_balance += amount;
      add_transaction(&(current->transactions), amount, y);
      return 1;
    }
    current = current->next;
  }

  return -1;
}

//  Decreases the amount of money from account x by amount
int decrease_amount(node *n, int amount, char *x) {
  node *current = n;

  while (current != NULL) {
    if (strcmp(current->account_id,x) == 0) {
      current->account_balance -= amount;
      return 1;
    }
    current = current->next;
  }

  return -1;
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
      destroy_tlist(&temp->transactions);
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
    destroy_tlist(&temp->transactions);
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

//  Returns the balance from the requested
//  bank_account x or -1 if it doens't exist
int get_balance(node *n, char *x) {
  node *current = n;

  while (current != NULL) {
    if (strcmp(current->account_id,x) == 0) {
      return current->account_balance;
    }
    current = current->next;
  }

  return -1;
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

//  tnode functions

//  Adds tnodes with transactions to the end of the list
//  If there is an existing transaction with this account, it adds the amount
void add_transaction(tnode **n, int amount, char *x) {
  if (*n == NULL) {
    *n = malloc(sizeof(tnode));
    (*n)->account_id = malloc(strlen(x+1));
    strcpy((*n)->account_id,x);
    (*n)->transfered_amount = amount;
    (*n)->next = NULL;
    return;
  }
  tnode *temp = *n;
  while (temp->next != NULL) {
    if (strcmp(temp->account_id,x) == 0) {
      temp->transfered_amount += amount;
      return;
    }
    temp = temp->next;
  }
  if (strcmp(temp->account_id,x) == 0) {
    temp->transfered_amount += amount;
    return;
  }
  else {
    temp->next = malloc(sizeof(tnode));
    temp->next->account_id = malloc(strlen(x+1));
    strcpy(temp->next->account_id,x);
    temp->next->transfered_amount = amount;
    temp->next->next = NULL;
  }
}

//  Removes tnodes from the list
void delete_tnode(tnode **n, char *x) {
  tnode *temp;

  if (strcmp((*n)->account_id,x) == 0) {
    temp = (*n)->next;
    free((*n)->account_id);
    free(*n);
    *n = temp;
    return;
  }

  tnode *current = *n;
  while (current->next != NULL) {
    if (strcmp(current->next->account_id,x) == 0) {
      temp = current->next;
      current->next = temp->next;
      free(temp->account_id);
      free(temp);
      break;
    }
    current = current->next;
  }
}

//  Frees the list allocated memory
void destroy_tlist(tnode **n) {
  tnode *current = *n;
  tnode *temp;
  while (current != NULL) {
    temp = current->next;
    free(current->account_id);
    free(current);
    current = temp;
  }
  *n = NULL;
}

//  Returns the transfered ammount from
//  the bank_account x or -1 if it doens't exist
int find_transaction(tnode *n, char *x) {
  tnode *current = n;

  while (current != NULL) {
    if (strcmp(current->account_id,x) == 0) {
      return current->transfered_amount;
    }
    current = current->next;
  }

  return -1;
}
