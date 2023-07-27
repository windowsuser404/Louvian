#include <stdbool.h>
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

void addEdge(int src, int dest){
	Node* temp = nodes[src];
	temp->edges++;
	if(temp->edges==1){
		temp->Edgelist = (int*)malloc(temp->edges*sizeof(int));
	}
	else{
		temp->Edgelist = realloc(temp->Edgelist, temp->edges*sizeof(int));
	}
	temp->Edgelist[temp->edges-1] = dest;
	//
	temp = nodes[dest];
	temp->edges++;
	if(temp->edges==1){
		temp->Edgelist = (int*)malloc(temp->edges*sizeof(int));
	}
	else{
		temp->Edgelist = realloc(temp->Edgelist, temp->edges*sizeof(int));
	}
	temp->Edgelist[temp->edges-1] = src;
	num_edges+=1; //in modularity formula we should use num_edges/2, hence i am increasing only by one even though two edges are added
}

community* create_community(int i){
	community* temp = (community*)malloc(sizeof(community));
	temp->count=1;
//	temp->degree = nodes[i]->edges;
	temp->max=10;
	temp->list = (int*)malloc(temp->max*sizeof(int));
	temp->list[0] = i;
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
	return degree;
}

int edges_connected(int node, int neighbour){//neighbour is basically the target community member
	community* nei_com = communities[neighbour];
	int edge_count = 0;
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
	community* target = communities[com];
	if(target->count == target->max){
		target->max+=100;
		target->list = realloc(target->list, target->max*sizeof(int));
	}
	target->list[target->count] = i;
	target->count++;
}

void remove_frm_com(int i, int com){
	community* target = communities[com];
	int index=0;
	while(index<target->count){
		if(target->list[index] == i){
			break;
		}
		index++;
	}
	while(index<target->count-1){
		target->list[index] = target->list[index+1];
	}
	target->count--;
	if((target->max-target->count)>100){
		target->max-=100;
		target->list = realloc(target->list, target->max*sizeof(int));
	}
}

void Louvian(){
	bool improv = true;
	while(improv){
		improv = false;
		int* old_com = (int*)malloc(num_node*sizeof(int));
		bool* change_community = (bool*)calloc(num_node, sizeof(bool));//to keep track if a node was changed, if changed we will have to switch communities
		for(int i=0; i<num_node; i++){
			Node* node = nodes[i];
			old_com[i] = node->community;
			int max_com = node->community;
			int delQ = 0;
			for(int j=0; j<node->edges; j++){
				int newQ = change_modularity(i, node->Edgelist[j]);			
				if(newQ>delQ){
					delQ = newQ;
					max_com = node->Edgelist[j];
					improv = true;
					change_community[i] = true;
				}
			}
			node->community = max_com;
		}
		for(int i=0; i<num_node; i++){
			if(change_community[i]){
				remove_frm_com(i, old_com[i]);
				insert_in_com(i, nodes[i]->community);
			}
		}
		free(change_community);
		free(old_com);
	}
}

int main(int argc, char *argv[]){
        FILE* file = fopen(argv[1],"r");
        num_node = atoi(argv[2])+1;
        if(file==NULL){
                printf("File handling error");
                exit(0);
        }
        nodes = (Node**)malloc(num_node*sizeof(Node*));
        for (int i = 0; i < num_node; i++) {
            nodes[i] = createNode(i);
        }
   
        int src,dest;
   
        while(fscanf(file, "%d %d", &src, &dest) == 2){
            addEdge(src, dest);
        }


}
