#include <stdio.h>
#include <string.h>
#include <math.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "types.h"

#define INF 9999

FILE *openFile( char *fileName , char *mode )
{
	FILE *file;

	file = fopen( fileName , mode );
	if( file == NULL )
	{
		printf("Error with %s\n", fileName );
		exit(1);
	};
}

Graph *initializeGraph( int V , FILE *nodeInput)
{
	rewind(nodeInput);

	Graph *g = ( Graph * )malloc( sizeof( Graph ) );
	if( g == NULL )
	{
		printf( "Memory allocation failed for Graph\n");
		exit(1);
	}
	g->V = V;
	g->arr = ( adjacency_list * )malloc( sizeof( adjacency_list ) * V );
	if( g->arr == NULL ) exit(1);

	

	int i;
	char id[50];
	char lat[50];
	char lon[50];

	for( i = 0 ; i < V ; i++ )
	{
		fscanf( nodeInput, "%s %s %s\n" , id , lat , lon );
		g->arr[i].head = NULL;

		g->arr[i].id = ( char * )malloc( sizeof( char ) * 50 );
		g->arr[i].lat = ( char * )malloc( sizeof( char ) * 50 );
		g->arr[i].lon = ( char * )malloc( sizeof( char ) * 50 );

		strcpy(g->arr[i].id , id );
		strcpy(g->arr[i].lat , lat );
		strcpy(g->arr[i].lon , lon);
	}
	
	return g;
}

node *createNode( int key , char *id , double weight , char *name ) 
{

	node *newNode = ( node* ) malloc(sizeof( node ));


	newNode->st_name = ( char * )malloc( sizeof( char ) * 50 );
	newNode->id = ( char * )malloc( sizeof( char ) * 50 );

	strcpy( newNode->st_name , name );
	strcpy( newNode->id , id );

	newNode->weight = weight;
	newNode->key = key;
	newNode->next = NULL; 

	return newNode;
}

double haversine(double lat1, double lon1, double lat2, double lon2)
{
	double dLat = (lat2 - lat1) * M_PI / 180.0;
	double dLon = (lon2 - lon1) * M_PI / 180.0;

	lat1 = (lat1) * M_PI / 180.0;
	lat2 = (lat2) * M_PI / 180.0;

	double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
	double rad = 6371;
	double c = 2 * asin(sqrt(a));

	return ( 1000 * rad * c );
}

double distance( Graph *g , char *id1 , char *id2  )
{
	double k1 , k2;
	double k3 , k4;
	double res;

	char lat1[50];
	char lon1[50];
	char lat2[50];
	char lon2[50];

	int indicator = 0;
	int i;

	for( i = 0 ; i < g->V ; i++ )
	{
		if( !strcmp( g->arr[i].id , id1 ) )
		{
			strcpy( lat1 , g->arr[i].lat );
			strcpy( lon1 , g->arr[i].lon );
			indicator += 1;
		}
		if( !strcmp( g->arr[i].id , id2 ) ) 
		{
			strcpy( lat2 , g->arr[i].lat );
			strcpy( lon2 , g->arr[i].lon );
			indicator += 1;
		}

	}

	if( indicator != 2 )
	{
		printf("Wrong id ----");
		printf("%s ==== %s \n" , id1 , id2 );
		exit(1);
	}

	k1 = atof( lat1 );
	k2 = atof( lon1 );	
	k3 = atof( lat2 );
	k4 = atof( lon2 );

	res = haversine( k1 , k2 , k3 , k4 );

	return res;
}

void addEdge( Graph *g , int start , int end , char *name )
{
	int i;
	double weight = distance( g , g->arr[start].id , g->arr[end].id );


	node *newNode = createNode( end , g->arr[end].id, weight , name );
 
	newNode->next = g->arr[start].head;
	g->arr[start].head = newNode;


	newNode = createNode( start , g->arr[start].id , weight , name );

	newNode->next = g->arr[end].head;
	g->arr[end].head = newNode;
}

int findID( Graph *g , char *id )
{
	for( int i = 0 ; i < g->V ; i++ )
	{
		if( !strcmp( g->arr[i].id , id ) ) return i;
	}

	return -1;
}

Graph *createGraph( FILE *edgeInput , FILE *nodesInput , int counterNodes )
{
	Graph *g = NULL;

	g = initializeGraph( counterNodes , nodesInput );

	char name1[50];
	char name2[50];
	char stName[50];
	char id[50];
	char prev_id[50] = {0};
	char character;

	double rastojanje;

	int indicator = 1;
	int start;
	int end;

	rewind( edgeInput);

	while( indicator )
	{
		indicator = EOF != fscanf( edgeInput , "%s %s" , id , name1 );

		if( indicator )
		{
		if( (character = fgetc( edgeInput ) ) != '\n' )
		{
			fscanf( edgeInput , "%s" , name2 );
			strcat( name1 ," " );
			strcat( name1 , name2 );
			if( (character = fgetc( edgeInput ) ) != '\n' )
			{
				fscanf( edgeInput , "%s" , name2 );
				strcat( name1 ," " );
				strcat( name1 , name2 );
			}
		}
			if( !strcmp( id , "name" ) )
			{
				strcpy( stName , name1 );
				*( prev_id + 0 ) = 0;
				continue;
			}
			if( !*( prev_id + 0 ) )
			{
				strcpy( prev_id , name1 );
			}
			else
			{
				start = findID( g , prev_id );
				end = findID( g , name1 );

				addEdge( g , start , end , stName );

				strcpy( prev_id , name1 ); 
			}
		}
	}

	return g;

	
}

int minDistance( int* distance, int* visited, int dim )
{
	int max = INT_MAX;
	int min;

	for( int i = 0 ; i < dim ; i++ )
		if( !( *( visited + i ) ) && *( distance + i ) <= max )
		{
			min = i;
		    max = *( distance + i );
		}	       

	return min;		
}

void printPath( Graph *g , int* parent, int i )
{
	if( *( parent + i ) == -1 )
		return;

	printPath( g , parent, *( parent + i ) );

	if (g->arr[i].head != NULL) 
	{
    printf("\t--> %s\t<===>\tid : %s\n ", g->arr[i].head->st_name, g->arr[i].head->id);
	}
}

void dijkstra( Graph* g, char *source , char *dest )
{
	int *visited = NULL;
	int *distance = NULL;
	int *parent = NULL;
	int end = findID( g , dest );
	int s = findID( g , source );

	visited = ( int * ) calloc( g->V , sizeof( int ) );
	if( visited == NULL ) exit( EXIT_FAILURE );

	distance = ( int * ) malloc( sizeof( int ) * g->V );
	if( distance == NULL ) exit( EXIT_FAILURE );

	parent = ( int * ) malloc( sizeof( int ) * g->V );
	if( parent == NULL ) exit( EXIT_FAILURE );

	for( int i = 0 ; i < g->V ; i++ )
	{
		*( distance + i ) = INF;
		*( parent + i ) = -1;
	}

	*( distance + s ) = 0;

	for( int i = 0 ; i < g->V - 1 ; i++ )
	{
		int u = minDistance( distance, visited, g->V );
		
		*( visited + u ) = 1;

		node* p = g->arr[u].head;
		
		while( p != NULL )
		{
			int x = p->key;

			if( !( *( visited + x ) ) && *( distance + u ) + p->weight < *( distance + x ) )
			{
				*( distance + x ) = *( distance + u ) + p->weight;
				*( parent + x ) = u;
			}
			p = p->next;
		}
	}

	printf( "Starting node [%s] -> Destination node [%s]\n", source, dest );
	printf("Distance : %d [m]\n", *( distance + end ));
	printPath( g , parent, end );
	printf( "\n" );

	free( visited );
	free( parent );
	free( distance );
}

