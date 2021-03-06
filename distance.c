// Outputs the distance between 2 nodes u and v on an undirected graph, by first doing a BFS starting from u, and then reporting v's distance number.

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
  int dist; // Distance from u (a.k.a. S).
} vertex;

// Stub supporting functions.
void addDirectedLink (vertex* vertices, int fromIndex, int toIndex);
bool dfsUnwind(vertex* vertices, int index);
// bool explore(vertex* v, vertex* vertices, int cc);

int main(int argc, char **argv) {
  // Generic registers.
  int i, ii, x, y, z;
  char* s;

  // Program-specific vars.
  int n, m; // Number of vertices/edges, respectively.
  int u, v; // Target vertices.
  char cl[15]; // Current line. max index is 6-char (100000); 2 nodes + space + newline + null.

  // Parse 1st line (n & m).
  if (fgets(cl, 15, stdin)) {}; // Assume input is not malformed.
  *(s = strchr(cl, ' ')) = 0; // Record where the space is, then change it to a null terminator.
  n = atoi(cl);
  m = atoi(s+1); // Rely on atoi to disregard the trailing \n.

  fprintf(stderr, "n: %d, m: %d\n", n, m);

  // Parse the main-body list of edges, construct adjacency-list. Assume number of lines plays nicely and is m or m+1.
  vertex vertices[n]; // Allocate vertices.
  vertex blankVertex = {NULL, 0, 0, NULL, 0, 0, -1};
  for (i = 0; i < n; i++) {vertices[i] = blankVertex;} // Zero them out. -1 for dist is analagous to infinity.
  for (i = 0; i < m; i++) {
    if (fgets(cl, 15, stdin)) {};
    *(s = strchr(cl, ' ')) = 0; // Record where the space is, then change it to a null terminator.
    x = atoi(cl) - 1; // -1 because of conversion from 1 to 0-indexing.
    y = atoi(s+1) - 1;

    addDirectedLink(vertices, x, y); // Some redundancy with seperate out/in structure, but good so that we can easily change it to support directed graphs.
    addDirectedLink(vertices, y, x); // Comment this out if it's a directed rather than undirected graph.
  }

  // Parse last line (u & v).
  if (fgets(cl, 15, stdin)) {}; // Assume input is not malformed.
  *(s = strchr(cl, ' ')) = 0; // Record where the space is, then change it to a null terminator.
  u = atoi(cl) - 1; // -1 because of conversion from 1 to 0-indexing.
  v = atoi(s+1) - 1; // Rely on atoi to disregard the trailing \n.

  fprintf(stderr, "u: %d, v: %d\n", u + 1, v + 1);

  int queue[n];
  queue[0] = u;
  vertices[queue[0]].dist = 0;
  int qCurr = 0;
  int qTotal = 1;

  while (qCurr < qTotal) {
    for (i = 0; i < vertices[queue[qCurr]].outCount; i++) {
      if (vertices[vertices[queue[qCurr]].outs[i]].dist == -1) {
        vertices[vertices[queue[qCurr]].outs[i]].dist = vertices[queue[qCurr]].dist + 1; // Assign distance if new.
        queue[qTotal++] = vertices[queue[qCurr]].outs[i];
      }
    }
    qCurr++;
  }

  // Logging: Print adjacency list to stderr.
  fprintf(stderr, "Printing adjacency list (1-indexed)...\n");
  fprintf(stderr, "[i] >outs... (used/alloc) dist\n");
  for (i = 0; i < n; i++) {
    fprintf(stderr, "[%d] ", i+1);
    for (ii = 0; ii < vertices[i].outCount; ii++) {fprintf(stderr, ">%d ", vertices[i].outs[ii]+1);}
    fprintf(stderr, "(%d/%d) %d\n", vertices[i].outCount, vertices[i].outCapacity, vertices[i].dist);
  }

  printf("%d\n", vertices[v].dist);

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
