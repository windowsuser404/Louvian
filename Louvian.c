#include <bool.h>
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef struct{
	int* Edgelist;
	int edges;
	int community;
}Node;

typedef struct{
	int* list;
//	int degree;
	int count;
	int max;
}community;

int num_edges=0;
int num_node;
Node** nodes;
community** communities;

Node* createNode(int i){
	Node* temp = (Node*)malloc(sizeof(Node));
	temp->Edgelist = NULL;
	temp->edges = 0;
	temp->community = i;
	return temp;
}

void addEdge(src, dest){
	Node* temp = nodes[src];
	temp->edges++;
	if(temp->edges==1){
		temp->Edgelist = (int*)malloc(edges*sizeof(int))
	}
	else{
		temp->Edgelist = realloc(temp->Edgelist, temp->edges*sizeof(int));
	}
	temp->Edgelist[size-1] = dest;
	//
	Node* temp = nodes[dest];
	temp->edges++;
	if(temp->edges==1){
		temp->Edgelist = (int*)malloc(edges*sizeof(int))
	}
	else{
		temp->Edgelist = realloc(temp->Edgelist, temp->edges*sizeof(int));
	}
	temp->Edgelist[size-1] = src;
	num_edges+=1; //in modularity formula we should use num_edges/2, hence i am increasing only by one even though two edges are added
}

community* create_community(int i){
	community* temp = (community*)malloc(sizeof(community));
	temp->count=1;
//	temp->degree = nodes[i]->edges;
	temp->max=10;
	temp->list = (int*)malloc(max*sizeof(int));
	temp->list[0] = i
	return temp;
}

void initialise_Louvian(){
	communities = (community**)malloc(num_node*sizeof(community*));
	for(int i=0;i<num_node;i++){
		communities[i] = create_community(i);
	}
}

bool is_connected(int frst, int scnd){
	for(int i=0; i<nodes[frst]->edges; i++){
		if(nodes[frst]->Edgelist[i] == scnd){
			return true;
		}
	}
	return false;
}

int community_degree(int i){
	community* com = communities[i];
	int degree=0;
	for(int i=0; i<com->count; i++){
		degree+=nodes[com->list[i]]->edges;
	}
	return degree
}

int edges_connected(int node, int neighbour){//neighbour is basically the target community member
	community* nei_com = communities[neighbour];
	edge_count = 0;
	for(int i=0;i<nei_com->count;i++){
		if(is_connected(nei_com->list[i], node)){ //no removal of i assuming no self edges as of now
			edge_count++;
		}
	}
	return edge_count;
}

int change_modularity(int node, int neighbour){
	int delQ;
	delQ = (edges_connected(node, neighbour)-edges_connected(node, node))/(num_edges)+(nodes[node]->edges)*(community_degree(node)-community_degree(neighbour)-2*(nodes[node]->edges))/(2*num_edges*num_edges);
	return delQ;
}

void insert_in_com(int i, int com){
}

void remove_frm_com(int i, int com){
}

void Louvian(){
	bool improv = true;
	while(improv){
		improv = false;
		for(int i=0; i<num_node; i++){
			int max_com = i;
			int delQ = 0;
			Node* node = nodes[i];
			for(int j=0; j<node->edges; j++){
				int newQ = change_modularity(i, node->Edgelist[j]);			
				if(newQ>delQ){
					delQ = newQ;
					max_com = node->Edgelist[j];
					improv = true;
				}
				
			}
			if(max_com!=i){
				
			}
		}
	}
}

int main(){
        FILE* file = fopen(argv[1],"r");
        num_node = atoi(argv[2])+1;
        if(file==NULL){
                printf("File handling error");
                exit(0);
        }
        nodes = (Node**)malloc(num_node*sizeof(Mode*))
        for (int i = 0; i < num_node; i++) {
            nodes[i] = createNode(i);
        }
   
        int src,dest;
   
        while(fscanf(file, "%d %d", &src, &dest) == 2){
            addEdge(src, dest);
        }


}
