#include <stdbool.h>
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

const int RUN = 32;

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

//begin functions for sorting
void insertionSort(int *arr, int left, int right){
	for(int i=left+1; i<=right; i++){
		int k = arr[i];
		int j = i-1;
		while(j >= left && arr[j] > key){
			arr[j+1] = arr[j];
			j--;
		}
		arr[j+1] = k;
	}
}

void merge(int *arr, int l, int m, int r){
	int l1 = m-l+1;
	int l2 = r-m;
	int* left = (int*)malloc(len1 * sizeof(int));
	int* right = (int*)malloc(len2 * sizeof(int));

	for (int i = 0; i<l1; i++){
		left[i] = arr[l+i];
	}
	for (int i = 0; i<l2; i++){
		right[i] = arr[m +1 +i];
	}

	int i=0, j=0, k=l;
	while (i < l1 && j<l2){
		if(left[i] <= right[j]){
			arr[k++] = left[i++];
		}
		else{
			arr[k++] = right[j++];
		}
	}

	while(i < l1){
		arr[k++] = left[i++];
	}
	while(j < l2){
		arr[k++] = right[j++];
	}

	free(left);
	free(right);
}

void timSort(int *arr, int size){
	for (int i = 0; i<size; i+=RUN){
		insertionSort(arr, i, min((i+RUN-1),(n-1)));
	}
	for (int temp = RUN; temp<size; temp = 2*temp){
		for(int left = 0; left<n ; left+=2*temp){
			int mid = left+size-1;
			int right = min((left+2*temp-1),(size-1));
			if(mid < right){
				merge(arr, left, mid, right);
			}
		}
	}
}

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

void sort_all(){
	for(int i=0; i<numnode; i++){
		timSort(nodes[i]->Edgelist, nodes[i]->edges);
	}
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

int community_degree(int i){
	community* com = communities[i];
	int degree=0;
	for(int i=0; i<com->count; i++){
		degree+=nodes[com->list[i]]->edges;
	}
	return degree;
}

bool is_connected(int com, int node){
	int left =0;
	int right = nodes[node]->size;
	int* arr = nodes[node]->list;
	while(left<=right){
		int mid = left + (right-left)/2;
		
		if(arr[mid]==com){
			return true;
		}		
		else if(arr[mid] < com){
			left = mid+1;
		}
		else{
			right = mid-1;
		}
	}
	return false;
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
		target->max*=2;
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
	if((target->max)>2*target->count){
		target->max/=2;
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
	
	sort_all();

}
