#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "fun.h"
#include "parser.h"

// gcc main.c -I/usr/include/libxml2 -o main.out -lxml2 -lm
// ./main.out 50633903 114148015

int main( int argc , char **argv ) {
	
	char osmFilename[] = "map.osm"; 
    uint counterNodes = 0;
    
    FILE *nodeOutput = openFile( "nodes.txt" , "w+" );
    FILE *edgeOutput = openFile( "edges.txt" , "w+" );



	xmlDoc *docEdges = xmlReadFile(osmFilename, "UTF-8", 0);
	if (docEdges == NULL) {
		fprintf(stderr, "Failed to parse %s\n", osmFilename);
		return -1;
	}
	xmlNode *rootEdges = xmlDocGetRootElement(docEdges);
	writeEdges(edgeOutput , rootEdges);
	xmlFreeDoc(docEdges);
	xmlCleanupParser();



	xmlDoc *docNodes = xmlReadFile(osmFilename, "UTF-8", 0);
	if (docNodes == NULL) {
		fprintf(stderr, "Failed to parse %s\n", osmFilename);
		return -1;
	}
	xmlNode *rootNodes = xmlDocGetRootElement(docNodes);
	writeNodes(nodeOutput , rootNodes , &counterNodes , edgeOutput );
	xmlFreeDoc(docNodes);
	xmlCleanupParser();



    Graph *g = NULL;
    g = createGraph( edgeOutput , nodeOutput , counterNodes );
	dijkstra( g, *( argv + 1 ) , *( argv + 2 ) );


    fclose( edgeOutput );
    fclose( nodeOutput );
    free( g );

	return 0;
}
