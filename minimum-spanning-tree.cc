/**
 *  \file minimum-spanning-tree.cc.
 *  Sudharshan Arutselvan
 *  Student ID: 89979164
 * 	Ramkumar Rajabaskaran
 *  Student ID: 85241493
 *
 *  The file briefs the functions to create the adjacency list and the priority queue using heaps
 *  and also to create the minimum spanning tree using the Prim's algorithm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <cstring>
#include <fstream>
#include <limits.h>
#include <float.h>
#include "MST.hh"
using namespace std;

// Structure to create the adjacency list
struct adjList{
	long int dest;
	float weight;
	struct adjList* next;
};
// Structure to store the head and tail of the adjacency list
struct listHeadTail{
	struct adjList* head;
	struct adjList* tail;
};
// Structure to store array of all vertices to link the nodes
struct graph{
	struct listHeadTail* array;
};
// Structure to store details of each node in queue
struct heap{
    long int v;
    float priority;
    long int parent;
};
// Structure to maintain the size of queue and create queue for vertices
struct priorityQueue{
    long int size;
    struct heap* array;
};
struct graph* undirectedgraph;  // Object for creating array of vertices for adjacency list
struct priorityQueue* queue;    // Object for creating the priority queue using heap
struct priorityQueue* MST;      // Object to store the vertices and weights of minimum spanning tree
struct heap* tempheap=(struct heap*)malloc(sizeof(struct heap));    // Temporary object to swap heap nodes

// Function to initialize the vertices with the edges and initialize the queue
void creategraph(long int vertices){
	if(!undirectedgraph){
		undirectedgraph = (struct graph*)malloc(sizeof(struct graph));
		undirectedgraph->array = (struct listHeadTail*)malloc(vertices*sizeof(struct listHeadTail));
	}
    if(!queue){
        queue = (struct priorityQueue*)malloc(sizeof(struct priorityQueue));
        queue->array = (struct heap*)malloc(vertices*sizeof(struct heap));
        queue->size = vertices;
        MST = (struct priorityQueue*)malloc(sizeof(struct priorityQueue));
        MST->array = (struct heap*)malloc(vertices*sizeof(struct heap));
    }
	for(long int i=0;i<vertices;++i){
        undirectedgraph->array[i].head = NULL;
        undirectedgraph->array[i].tail = NULL;
        queue->array[i].priority=FLT_MAX;
        if(i==1){
            queue->array[i].priority=0;
        }
        queue->array[i].v=i;
        queue->array[i].parent=-1;
    }
}

// Function to add an edge to the adjacency list.
void addEdge(long int v1,long int v2, float w){
	struct adjList* newNode = (struct adjList*)malloc(sizeof(struct adjList));
    newNode->dest = v2;
    newNode->weight = w;
    newNode->next = NULL;
    if(undirectedgraph->array[v1].head==NULL){
        undirectedgraph->array[v1].head = newNode;
        undirectedgraph->array[v1].tail = newNode;
    }
    else{
        undirectedgraph->array[v1].tail->next = newNode;
        undirectedgraph->array[v1].tail = newNode;
    }
}

// Function to check if queue is empty or not
bool queuenotempty(){
    if(queue->size>0){
        return true;
    }
    else{
        return false;
    }
}

// Function to swap two nodes in queue
void swap(long int a, long int b){
    tempheap->v=queue->array[a].v;
    tempheap->priority=queue->array[a].priority;
    tempheap->parent=queue->array[a].parent;
    queue->array[a].v=queue->array[b].v;
    queue->array[a].priority=queue->array[b].priority;
    queue->array[a].parent=queue->array[b].parent;
    queue->array[b].v=tempheap->v;
    queue->array[b].priority=tempheap->priority;
    queue->array[b].parent=tempheap->parent;
    return;
}

// Min-heapify the queue
void min_heapify(long int key){
    long int left=key*2+1,right=key*2+2,small=key;
    if(left<queue->size && queue->array[left].priority<queue->array[small].priority){
        small=left;
    }
    if(right<queue->size && queue->array[right].priority<queue->array[small].priority){
        small=right;
    }
    if(small!=key){
        swap(small,key);
        min_heapify(small);
    }
    return;
}

// Build the minimum heap
void build_min_heap(){
    for(long int i=(queue->size/2)-1;i>=0;--i){
        min_heapify(i);
    }
    return;
}

// Extract the vertex with minimum edge
long int extract_min(){
    if(queue->size<=0){ return -1; }
    long int min=queue->array[0].v;
    swap(0,queue->size-1);
    queue->size=queue->size-1;
    min_heapify(0);
    return min;
}

// Get the index of a vertex
long int indexOf(long int v){
    for(int i=0;i<queue->size;i++){
        if(queue->array[i].v==v){
            return i;
        }
    }
    return -1;
}

// Update the queue based on the weights
void decrease_key(long int v){
    long int p;
    if(v%2==0){
        p=(v/2)-1;
    }
    else{
        p=((v+1)/2)-1;
    }
    if(queue->array[p].priority>queue->array[v].priority && v>0){
        swap(v,p);
        decrease_key(p);
    }
    return;
}

//Function to create the minimum spanning tree
void mst (string fileName){
	string temp;
	long int vertices,u,v,index,uindex,vert1,vert2,mst_count=0,count=0;
    float wt,weight,totalwt=0;
	int a,b;

    fstream readFile;
    readFile.open(fileName.c_str());
    if(!readFile){
        fprintf(stderr, "The file could not be accessed!\n");
        return;
    }
	void init();       // Initialize the list and queue

	//Create the adjacency list
	while(!readFile.eof()){ 
        getline(readFile,temp);     // Read a line from the input file
        if(temp!=""||temp!=" "||!temp.empty()){ 
            a=temp.find(" ");
            if(a<temp.length()&&a>0){
                b=temp.substr(a+1).find(" ");
                if(a+b<temp.length()){ 
                    vert1=atoi(temp.substr(0,a).c_str());
                    vert2=atoi(temp.substr(a+1,a+b-1).c_str());
                    weight=atof(temp.substr(a+b+2).c_str());
                    addEdge(vert1,vert2,weight);    // Include both the vertices in the adjacency list of the other vertex.
                    addEdge(vert2,vert1,weight);
                } 
            }
            else if(!temp.empty()){
            	vertices=atoi(temp.c_str());
            	creategraph(vertices);
            }
        } 
  
    } 
	readFile.close();
    build_min_heap();   // Creating minimum priority heap from the heap created

    // MST using PRIM
    while(queuenotempty()){
        u=extract_min();        // Extract minimum edge from the queue
        // Check if the edge has a parent to ensure the vertex is connected
        while(undirectedgraph->array[u].head){
            v = undirectedgraph->array[u].head->dest;
            wt = undirectedgraph->array[u].head->weight;
            index=indexOf(v);
            // Change the weight of the vertex if the weight between u and v is less than weight in queue
            if(index>-1 && wt<queue->array[index].priority){
                queue->array[index].priority=wt;
                queue->array[index].parent=u;
                decrease_key(index);
            }
            undirectedgraph->array[u].head=undirectedgraph->array[u].head->next;
        }
        if(queue->array[queue->size].parent>-1){
            MST->array[mst_count].v=u;
            MST->array[mst_count].priority=queue->array[queue->size].priority;
            MST->array[mst_count].parent=queue->array[queue->size].parent;
            mst_count++;
        }
    }

    // Create an output file to write the edges in the minimum spanning tree
    fileName=fileName.substr(0,fileName.find("."))+"_output.txt";
    ofstream writeFile(fileName.c_str());
    if(!writeFile){
        fprintf(stderr, "The file could not be created!\n");
        return;
    }
    for(long int i=0;i<mst_count;++i){
        if(MST->array[i].parent>-1){
            writeFile<<MST->array[i].parent<<" "<<MST->array[i].v<<" "<<MST->array[i].priority<<"\n";
            totalwt=totalwt+MST->array[i].priority;
            count=count+1;
        }
    }
    fprintf(stderr, "Total MST weight-%.4f\nTotal pairs-%ld\n", totalwt, count);
	return;
}

/* eof */
