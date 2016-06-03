#ifndef CLIENTQUEUE_H_
#define CLIENTQUEUE_H_

struct qnode {
  int client_id;
  struct qnode *next;
};

typedef struct qnode qnode;

void add_qnode(qnode **n, int x);
int get_qnode(qnode **n);
void destroy_qlist(qnode **n);
int find_qnode(qnode *n, int x);
void print_qlist(qnode *n);

#endif /* CLIENTQUEUE_H_ */
