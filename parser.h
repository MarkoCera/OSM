#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

void push( char *value )
{
	node *newNode = ( node *)malloc( sizeof( node ) );
	if( newNode == NULL ) exit(1);

	newNode->st_name = value;
	newNode->next = top;

	top = newNode;
}

int is_empty()
{
	return top == NULL;
}

char *pop()
{
	if(is_empty() ) exit(1);

	char *i = top->st_name;
	node *old = top;
	top = top->next;

	free( old );

	return i;

}

void make_empty( void )
{
	if( is_empty() )return;
	node *temp;
	temp = top;
	top = top->next;
	free( temp );

	make_empty();
}

void writeNodes( FILE *nodeOutput , xmlNode *rootNodes , uint *counterNodes , FILE *edgeInput ) {
	xmlNode *cur_node = NULL;
	char name1[50];
	char id2[50];
	int indicator = 1;
	char character;
	char name2[50];


	for (cur_node = rootNodes; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if (!strcmp(cur_node->name, "node")) {
				xmlChar *id = xmlGetProp(cur_node, "id");
                xmlChar *lat = xmlGetProp(cur_node, "lat");
                xmlChar *lon = xmlGetProp(cur_node, "lon");
                xmlChar *vers = xmlGetProp(cur_node, "version");

				if (id != NULL) {
					indicator = 1;

					rewind(edgeInput);

					while( indicator)
					{
						indicator = EOF != fscanf( edgeInput , "%s" , name1  );
						if( !strcmp( name1 , "name") )
						{
							if( (character = fgetc( edgeInput ) ) != '\n' )
							{
								fscanf( edgeInput , "%s" , name2 );

								if( (character = fgetc( edgeInput ) ) != '\n' )
								{
									fscanf( edgeInput , "%s" , name2 );
									if( (character = fgetc( edgeInput ) ) != '\n' )
									{
									fscanf( edgeInput , "%s" , name2 );
									}
								}
							}
						}
						else
						{
							fscanf( edgeInput , "%s" , id2 );
						}
						if( indicator )
						{
							if( !strcmp( id , id2 ) )
							{
								fprintf(nodeOutput,"%s %s %s\n",id,lat,lon);

                    			(*counterNodes)++;

								free(id);
								indicator = 0;
							}
						}
					}
					
				}

			}
		}
		
		xmlChar *k = xmlGetProp(cur_node , "k" );

		writeNodes(nodeOutput,cur_node->children,counterNodes , edgeInput );
	}
	
}

void writeEdges( FILE *edgeOutput , xmlNode *rootEdges) {
	xmlNode *cur_node = NULL;
	int indicator = 0;
	char *s;

	for (cur_node = rootEdges; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {


			if (!strcmp(cur_node->name, "way")) {
				xmlChar *id = xmlGetProp(cur_node, "id");
				if (id != NULL ) {
					
					xmlNode *childNode = cur_node->children;
					for (; childNode; childNode = childNode->next) {
						if (!strcmp("tag", childNode->name)) {
							xmlChar *key = xmlGetProp(childNode, "k");
							xmlChar *value = xmlGetProp(childNode, "v");

							if( !strcmp( key , "highway" ) && ( !strcmp( value , "residential") || !strcmp( value , "secondary") ) ) indicator += 1;


																
							if (key != NULL && value != NULL && ( !strcmp( key , "name" ) ) && indicator ) {
								
								fprintf(edgeOutput,"%s %s\n", key, value);


								while( !is_empty() )
								{
									fprintf(edgeOutput,"id %s\n", pop() );
								}



								free(key);
								free(value);
							}
						}
						if (!strcmp("nd", childNode->name)) {
							xmlChar *node_id = xmlGetProp(childNode, "ref");


							if (node_id != NULL ) {
								push( node_id );

							}
						}
					}

					make_empty();

					indicator = 0;

					free(id);
				}

			}
		}

		writeEdges(edgeOutput,cur_node->children);
	}
}