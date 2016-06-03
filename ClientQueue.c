#include <stdio.h>
#include <stdlib.h>
#include "ClientQueue.h"

//  Adds nodes to the end of the list
void add_qnode(qnode **n, int x) {
  if (*n == NULL) {
    *n = malloc(sizeof(qnode));
    (*n)->client_id = x;
    (*n)->next = NULL;
    return;
  }
  qnode *temp = *n;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = malloc(sizeof(qnode));
  temp->next->client_id = x;
  temp->next->next = NULL;
}

//  Returns the client client_id of the first
//  added client and removes the qnode from the list
int get_qnode(qnode **n) {
  int temp_id;
  qnode *temp;

  if ((*n)->next == NULL) {
    temp_id = (*n)->client_id;
    free(*n);
    *n = NULL;
    return temp_id;
  }

  qnode *current = *n;
  while (current->next != NULL) {
    if (current->next->next == NULL) {
      temp = current->next;
      current->next = temp->next;
      temp_id = temp->client_id;
      free(temp);
      return temp_id;
    }
    current = current->next;
  }

  return 0;
}

//  Frees the list allocated memory
void destroy_qlist(qnode **n) {
  qnode *current = *n;
  qnode *temp;
  while (current != NULL) {
    temp = current->next;
    free(current);
    current = temp;
  }
  *n = NULL;
}

//  Returns true if there is a qnode with a given client_id
int find_qnode(qnode *n, int x) {
  qnode *current = n;

  while (current != NULL) {
      if (current->client_id == x) {
        return 1;
      }
      current = current->next;
  }
  return 0;
}

//  Prints the whole list
void print_qlist(qnode *n) {
    qnode *current = n;

    while (current != NULL) {
        printf("Client ID: %d\n", current->client_id);
        current = current->next;
    }
}
