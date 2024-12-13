#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

typedef struct node
{
	int key;
	double weight;
	char *id;
	char *st_name;
	struct node *next;
}node;

typedef struct
{
	char *id;
	char *lat;
	char *lon;
	struct node *head;
}adjacency_list;

typedef struct
{
	int V;
	adjacency_list *arr;
}Graph;

typedef struct 
{	
	char *s;
	struct stack *next;
}stack;

static node *top;

