// Given a list of edges, constructs an adjacency list. If there is an extra line in the list, outputs whether those 2 vertices are connected (1 if true, 0 if false). Otherwise, outputs the number of connected components in the graph..

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Define a vertex.
typedef struct {
  int* neighbours; // An array of neighbours.
  int nCount; // total number of neighbours.
  int nCapacity; // Allocated space available for neighbours array.
  bool isExplored;
  int cc;
} vertex;

// Stub supporting functions.
void addToNeighbours(vertex* x, int y);
bool explore(vertex* v, vertex* vertices, int cc);

int main(int argc, char **argv) {
  // Generic registers.
  int i, ii, x, y, z;
  char buffer[8];

  // Program-specific vars.
  int n, m; // Number of vertices/edges, respectively.
  int u, v; // Target vertices to check.
  char cl[11]; // Current line. max index for u/v is 4-char (1000), + space + newline + null.

  // Parse 1st line (n & m).
  if (fgets(cl, 11, stdin)) {}; // Assume input is not malformed.
  strncpy(buffer, cl, z = strchr(cl, ' ')-cl); // z as temp.
  buffer[z] = 0; // Null-terminate.
  n = atoi(buffer); // z as temp. Assume no starting whitespace.
  m = atoi(cl+z+1); // Rely on atoi to disregard the trailing \n.

  // Parse the main-body list of edges, construct adjacency-list. Assume number of lines plays nicely and is m or m+1.
  vertex vertices[n]; // Allocate vertices.
  vertex blankVertex = {NULL, 0, 0, false, 0}; // Zero them out.
  for (i = 0; i < n; i++) {vertices[i] = blankVertex;}
  for (i = 0; i < m; i++) {
    if (fgets(cl, 11, stdin)) {};
    strncpy(buffer, cl, z = strchr(cl, ' ')-cl); // z as temp.
    buffer[z] = 0; // Null-terminate.
    x = atoi(buffer) - 1; // -1 because of conversion from 1 to 0-indexing.
    y = atoi(cl+z+1) - 1;

    addToNeighbours(&vertices[x], y);
    addToNeighbours(&vertices[y], x);
  }

  // Exploration algorithm.
  z = 1; // Current island.
  for (i = 0; i < n; i++) {
    if (explore(&vertices[i], vertices, z)) {z++;} // Recursively explore, increasing the island count after fruitful (not already explored) explorations.
  }

  // Logging: Print adjacency list to stderr.
  fprintf(stderr, "Printing adjacency list (1-indexed)...\n");
  fprintf(stderr, "(i) neighbours... [used/alloc] (cc no.)\n");
  for (i = 0; i < n; i++) {
    fprintf(stderr, "(%d) ", i+1);
    for (ii = 0; ii < vertices[i].nCount; ii++) {fprintf(stderr, "%d ", vertices[i].neighbours[ii]+1);}
    fprintf(stderr, "[%d/%d] (cc %d)\n", vertices[i].nCount, vertices[i].nCapacity, vertices[i].cc);
  }

  // Return output. If there's 1 remaining line, that's u and v, and we should return whether a path exists specifically between them or not. In the absence of a line, just return the number of connected components (cc).
  if (fgets(cl, 11, stdin)) {
    strncpy(buffer, cl, z = strchr(cl, ' ')-cl); // z as temp.
    buffer[z] = 0; // Null-terminate.
    u = atoi(buffer) - 1; // -1 because of conversion from 1 to 0-indexing.
    v = atoi(cl+z+1) - 1;
    printf("%d\n", vertices[u].cc == vertices[v].cc);
  } else {
    x = 0;
    for (i = 0; i < n; i++) {
      if (vertices[i].cc > x) x = vertices[i].cc;
    }
    printf("%d\n", x);
  }

  return 0;
}

// Add y to x's neighbours.
void addToNeighbours(vertex* x, int y) {
  if (!x->neighbours) { // Initialize if needed.
    x->neighbours = malloc(sizeof(int)); // length of 1 neighbour.
    x->nCount = 0;
    x->nCapacity = 1;
  } else if (x->nCapacity <= x->nCount) { // Reallocate neighbours array if out of space.
    x->neighbours = realloc(x->neighbours, 2*x->nCapacity*sizeof(int));
    x->nCapacity = x->nCapacity * 2;
  }
  x->neighbours[x->nCount] = y;
  x->nCount++;
  return;
}

// Recursive exploration marking algorithm. Return indicates whether non-empty exploration was done or not.
bool explore(vertex* v, vertex* vertices, int cc) {
  int i;
  if (v->isExplored) return false;  // Skip if already explored.
  v->isExplored = true;
  v->cc = cc;
  for (i = 0; i < v->nCount; i++) {
    explore(&vertices[v->neighbours[i]], vertices, cc);
  }
  return true;
}
