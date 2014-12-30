Pagerank computation by power iteration method
========

### Task:

Given a webgraph (such as one [here](https://github.com/vineethshankar/pagerank/blob/master/togit/submission/data0.dat) or [here](https://github.com/vineethshankar/pagerank/blob/master/togit/submission/data1.dat)), compute the pagerank of each webpage.

### Basics:

A webpage can be considered important and of a higher rank, if many other pages point (link) towards it. Therefore, it is useful to think of a network as a directed graph with the webpages being its nodes or vertices and the links being its edges. See this [page](http://www.math.cornell.edu/~mec/Winter2009/RalucaRemus/Lecture3/lecture3.html) from Cornell for more information and understanding.

Now, matrices can be powerful representations of graphs, allowing for further investigation. Central to the computation of pagerank is the development of a **weighted adjacency matrix**.

From a useful [link](http://www.utdallas.edu/~jwz120030/Teaching/PastCoursesUMBC/M221HS06/ProjectFiles/Adjacency.pdf) from UT Dallas,
>The adjacency matrix for a graph with n vertices is an n×n matrix whose (i,j) entry is 1,if the ith vertex and jth vertex are connected, and 0 if they are not. In our case, instead of the entry 1 for each edge, the entry will be 1/k where k is the number of outward links from vertex i.

Since, a vertex will be connected to only few other vertices, most of the entries in the weighted adjaceny matrix will be zero. The resulting matrix will be very **sparse**. Also, a sizeable webgraph (see [here](http://snap.stanford.edu/data/) for examples) will have tens of thousands of nodes. Hence we will need a technique which can efficiently store these large sparse matrices without running out of memory.

Common [ways](http://netlib.org/linalg/html_templates/node90.html) for storing large sparse matrices are:
1. Compressed row storage (CRS)
2. Compressed column storage (CCS)

The **CCS** storage technique has been used in this implementation and it consists of creation of three arrays:
*val - Holds all non zero entries
*rowind - Contains the corresponding row indices of the elements in val
*colptr - Contains the indices of val array of thos elements which start a new column
