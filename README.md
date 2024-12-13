# OSM
## Graph-Based Pathfinding Program

This program parses OpenStreetMap (OSM) data to construct a graph representation of nodes and edges and implements Dijkstra's algorithm for shortest pathfinding between two nodes. It can be used to calculate distances and find optimal routes based on the geographic data.

## Features

Parses .osm files using the libxml2 library.

Constructs a graph from nodes and edges defined in the OSM data.

Implements Dijkstra's algorithm for shortest pathfinding.

Calculates distances between nodes using the Haversine formula.

Outputs results, including paths and distances, to the console.

## Prerequisites

Before running this program, ensure you have the following installed:

GCC (or another C compiler)

libxml2 library

A valid .osm file with geographic data

## Files

main.c: The main driver of the program.

fun.h and parser.h: Header files containing function prototypes.

types.h: Defines the structures used in the program.

map.osm: Example OSM file containing map data (not included; you need to supply your own).

nodes.txt: Intermediate file storing parsed nodes.

edges.txt: Intermediate file storing parsed edges.

## Compilation

Compile the program using GCC with the following command:

gcc main.c -I/usr/include/libxml2 -o main.out -lxml2 -lm

## Usage

Run the program with the following command:

./main.out <source_node_id> <destination_node_id>

## Example

./main.out 50633903 114148015

## How It Works

File Parsing:

Reads the OSM file (map.osm) and parses nodes and edges using the libxml2 library.

Writes the parsed nodes to nodes.txt and edges to edges.txt.

Graph Construction:

Constructs an adjacency list from the parsed data.

Shortest Path Calculation:

Uses Dijkstra's algorithm to compute the shortest path between the specified source and destination nodes.

Output:

Prints the shortest path, intermediate nodes, and total distance to the console.

## Functions

Key Functions

File Handling:

FILE *openFile(char *fileName, char *mode): Opens a file and exits on error.

Graph Management:

Graph *initializeGraph(int V, FILE *nodeInput): Initializes a graph from the node data.

void add_edge(Graph *graph, int start, int end, char *name): Adds an edge to the graph.

Pathfinding:

void dijkstra(Graph *g, char *source, char *dest): Implements Dijkstra's algorithm to find the shortest path.

Distance Calculation:

double haversine(double lat1, double lon1, double lat2, double lon2): Computes the distance between two geographic points.

## Error Handling

The program includes basic error handling for:

File I/O errors.

Memory allocation failures.

Invalid or missing node IDs.


## Contributing

Contributions are welcome! Please fork the repository and submit a pull request.

## License

This project is licensed under the MIT License.
