#include "ntree.h"
#include <string.h>

Ntree *create_tree(int n) 
{
	Ntree *new_tree = malloc(sizeof(Ntree));
	new_tree->n = n;
	new_tree->adj = calloc(n, sizeof(Node*));
	return new_tree;
}

Node *insert(Node *list, Data d) 
{
	Node *new_node = malloc(sizeof(Node));
	new_node->d = d;
	new_node->next = list;
	list = new_node;
	return list;
}

void add_edge(Ntree *t, Data d1, Data d2)
{
	t->adj[d1.index] = insert(t->adj[d1.index], d2);
	t->adj[d2.index] = insert(t->adj[d2.index], d1);
}

void free_tree(Ntree *t)
{
    for (int i = 0; i < t->n; i++) {
        Node *aux = t->adj[i];
        while (aux) {
            Node *toremove = aux;
            aux = aux->next;
            free(toremove);
        }
    }
    free(t->adj);
    free(t);
}

void print_tree(Ntree *t)
{
    for (int i = 0; i < t->n; i++) {
        Node *aux = t->adj[i];
        printf("Node %d:", i);
        while (aux) {
            printf(" %d", aux->d.index);
            aux = aux->next;
        }
        printf("\n");
    }
}

int main(void)
{
	Ntree *precon = create_tree(10);
	
	Data d0;	
	d0.index = 0;
	strcpy(d0.id, "and");
	d0.t = AND;
	insert(*precon->adj, d0);

	Data d1;	
	d1.index = 1;
	strcpy(d1.id, "headsnake");
	d1.t = PARAMETER;
	insert(*precon->adj, d1);
	add_edge(precon, d0, d1);

	Data d2;	
	d2.index = 2;
	strcpy(d2.id, "ISADJACENT");
	d2.t = PARAMETER;
	insert(*precon->adj, d2);
	add_edge(precon, d0, d2);

	Data d3;	
	d3.index = 3;
	strcpy(d3.id, "not");
	d3.t = NOT;
	insert(*precon->adj, d3);
	add_edge(precon, d0, d3);

	Data d4;	
	d4.index = 4;
	strcpy(d4.id, "blocked");
	d4.t = PARAMETER;
	insert(*precon->adj, d4);
	add_edge(precon, d3, d4);

	Data d5;	
	d5.index = 5;
	strcpy(d5.id, "ispoint");
	d5.t = PARAMETER;
	insert(*precon->adj, d5);
	add_edge(precon, d0, d5);

	Data d6;	
	d6.index = 6;
	strcpy(d6.id, "spawn");
	d6.t = PARAMETER;
	insert(*precon->adj, d6);
	add_edge(precon, d0, d6);

	print_tree(precon);
	free_tree(precon);
	return 0;
}
