// Produces 1 possible topological sort from a directed acyclic graph. If graph is found to contain a cycle, prints 'non-acyclic' to stdout and exits with code 2.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Define a vertex.
typedef struct {
  int* outs; // Array of indices of vertices pointed to by this vertex.
  int outCount;
  int outCapacity;
  int* ins; // Array of indices of vertices pointing to this vertex.
  int inCount;
  int inCapacity;
  bool isExplored; // Guard to check for cyclicity. Exploration should never encounter a previously-explored node again unless there's a cycle.
  bool isOrdered; // Whether the vertex has already been allocated a place in the sort queue.
} vertex;

// Stub supporting functions.
void addDirectedLink (vertex* vertices, int fromIndex, int toIndex);
bool dfsUnwind(vertex* vertices, int index);
// bool explore(vertex* v, vertex* vertices, int cc);

int* sort;
int sI;

int main(int argc, char **argv) {
  // Generic registers.
  int i, ii, x, y, z;
  char* s;
  // char buffer[8];

  // Program-specific vars.
  int n, m; // Number of vertices/edges, respectively.
  char cl[15]; // Current line. max index is 6-char (100000); 2 nodes + space + newline + null.

  // Parse 1st line (n & m).
  if (fgets(cl, 15, stdin)) {}; // Assume input is not malformed.
  *(s = strchr(cl, ' ')) = 0; // Record where the space is, then change it to a null terminator.
  n = atoi(cl);
  m = atoi(s+1); // Rely on atoi to disregard the trailing \n.

  fprintf(stderr, "n: %d, m: %d\n", n, m);

  // Parse the main-body list of edges, construct adjacency-list. Assume number of lines plays nicely and is m or m+1.
  vertex vertices[n]; // Allocate vertices.
  sI = n - 1;
  vertex blankVertex = {NULL, 0, 0, NULL, 0, 0, false, false};
  for (i = 0; i < n; i++) {vertices[i] = blankVertex;} // Zero them out.
  for (i = 0; i < m; i++) {
    if (fgets(cl, 15, stdin)) {};
    *(s = strchr(cl, ' ')) = 0; // Record where the space is, then change it to a null terminator.
    x = atoi(cl) - 1; // -1 because of conversion from 1 to 0-indexing.
    y = atoi(s+1) - 1;

    addDirectedLink(vertices, x, y);
  }

  // Logging: Print adjacency list to stderr.
  fprintf(stderr, "Printing adjacency list (1-indexed)...\n");
  fprintf(stderr, "[i] >outs... (used/alloc) ][ <ins...  (used/alloc)\n");
  for (i = 0; i < n; i++) {
    fprintf(stderr, "[%d] ", i+1);
    for (ii = 0; ii < vertices[i].outCount; ii++) {fprintf(stderr, ">%d ", vertices[i].outs[ii]+1);}
    fprintf(stderr, "(%d/%d) ][ ", vertices[i].outCount, vertices[i].outCapacity);
    for (ii = 0; ii < vertices[i].inCount; ii++) {fprintf(stderr, "<%d ", vertices[i].ins[ii]+1);}
    fprintf(stderr, "(%d/%d)\n", vertices[i].inCount, vertices[i].inCapacity);
  }

  sort = malloc(n*sizeof(int));
  for (i = 0; i < n; i++) {
    if (vertices[i].isOrdered) continue; // Should be skipping almost all the time. Together with for loop, forms comprehensiveness.
    if (!dfsUnwind(vertices, i)) { // Start exploring. If false was returned, output error about cyclicity.
      printf("non-acyclic\n");
      return 2;
    }
  }

  for (i = 0; i < n; i++) {
    printf("%d ", sort[i]+1); // Convert 0 to 1-index.
  }
  printf("\n");
  return 0;
}

vertex* from; // registers.
vertex* to;
void addDirectedLink (vertex* vertices, int fromIndex, int toIndex) {
  from = vertices+fromIndex;
  to = vertices+toIndex;

  if (!from->outs) { // Initialize if needed.
    from->outs = malloc(sizeof(int));
    from->outCapacity = 1;
  } else if (from->outCapacity <= from->outCount) { // Reallocate by doubling if out of space.
    from->outs = realloc(from->outs, 2*from->outCapacity*sizeof(int));
    from->outCapacity = from->outCapacity * 2;
  }
  from->outs[from->outCount] = toIndex;
  from->outCount++;

  if (!to->ins) { // Initialize if needed.
    to->ins = malloc(sizeof(int));
    to->inCapacity = 1;
  } else if (to->inCapacity <= to->inCount) { // Reallocate by doubling if out of space.
    to->ins = realloc(to->ins, 2*to->inCapacity*sizeof(int));
    to->inCapacity = to->inCapacity * 2;
  }
  to->ins[to->inCount] = fromIndex;
  to->inCount++;

  return;
}

// Should usually return true, but return false if it hits a cycle, so the program knows to halt.
bool dfsUnwind(vertex* vertices, int index) {
  int i, ii;
  if (vertices[index].isExplored) return false;
  vertices[index].isExplored = true;

  for (i = 0; i < vertices[index].outCount; i++) { // Recursively explore for all outlinks.
    if (vertices[index].outs[i] == -1) continue; // Skip if burnt link.
    if (!dfsUnwind(vertices, vertices[index].outs[i])) return false;
  }

  // At this point, it's a sink.
  fprintf(stderr, "sink %d.\n", index+1);
  for (i = 0; i < vertices[index].inCount; i++) { // Burn links to this sink from its incoming neighbours.
    for (ii = 0; ii < vertices[vertices[index].ins[i]].outCount; ii++) {
      if (vertices[vertices[index].ins[i]].outs[ii] == index) {vertices[vertices[index].ins[i]].outs[ii] = -1; fprintf(stderr, "Burnt %d->%d.\n", vertices[index].ins[i]+1, index+1);} // Link matches, burn it.
    }
  }
  sort[sI] = index;
  sI--;
  vertices[index].isOrdered = true;
  return true;
}
