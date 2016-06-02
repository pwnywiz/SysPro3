#ifndef ACCOUNTLIST_H_
#define ACCOUNTLIST_H_

struct node {
  char *account_id;
  int account_balance;
  struct node *next;
  struct tnode *transactions;
};

struct tnode {
  char *account_id;
  int transfered_amount;
  struct tnode *next;
};

typedef struct node node;
typedef struct tnode tnode;

//  node functions
void add_node(node **n, int init_amount, char *x);
void delete_node(node **n, char *x);
void destroy_list(node **n);
int find_node(node *n, char *x);
tnode *get_node(node *n, char *x);
void print_list(node *n);

#endif /* ACCOUNTLIST_H_ */
