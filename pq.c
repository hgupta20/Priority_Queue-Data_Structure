#include "pq.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



typedef struct heap {
	int id;
  double priority;
	int index;
} heapStruct;
struct pq_struct {
  // YOU DECIDE WHAT TO PUT IN HERE TO ENCAPSULATE
  //   A PRIORITY QUEUE!
  heapStruct* heap;
	heapStruct** poin_id;
	int size;
	int capacity;
	int min_heap;

};

// YOU IMPLEMENT ALL OF THE FUNCTIONS SPECIFIED IN
//   pq.h BELOW!


PQ* pq_create(int capacity, int min_heap){
  PQ* newpq = malloc(sizeof(PQ));
  newpq->heap = (heapStruct*)malloc(sizeof(heapStruct)*(capacity+1));
  newpq->poin_id = malloc(sizeof(heapStruct*)*(capacity));
  newpq->capacity= capacity;
  newpq->size= 0;
  newpq->min_heap=  min_heap;

  for(int i=0; i< capacity; i++){
    newpq->poin_id[i] = NULL;
  }
	//printf("Helllo \n"); // Debug
  return newpq;
}

void pq_free(PQ* pq){
  if(pq->heap != NULL){
    free(pq->heap);
  }
  if(pq->poin_id != NULL){
    free(pq->poin_id);
  }
  if(pq != NULL){
    free(pq);
  }
}
void swap(PQ* pq, int i1, int i2) {
	int id1 = pq->heap[i1].id;
  int id2 = pq->heap[i2].id;
	double priority1 = pq->heap[i1].priority;
	double priority2 = pq->heap[i2].priority;

	pq->heap[i1].id = id2;
	pq->heap[i1].priority = priority2;

	pq->heap[i2].id = id1;
	pq->heap[i2].priority = priority1;

	pq->poin_id[id1] = &(pq->heap[i2]);
	pq->poin_id[id2] = &(pq->heap[i1]);
}
void percolate_up_min(PQ *pq, int index){
  heapStruct temp = pq->heap[index];
  int parent;
  parent = index/2;
  while(parent >=1 && temp.priority <pq->heap[parent].priority){
    swap(pq,index,parent);
    index=parent;
    parent=index/2;
  }
}
void percolate_up_max(PQ *pq, int index){
  heapStruct temp = pq->heap[index];
  int parent;
  parent = index/2;
  while(parent >=1 && temp.priority>pq->heap[parent].priority)
  {
    swap(pq,index,parent);
    index=parent;
    parent=index/2;
  }  
}
void percolate_down_max(PQ* pq, int index){
  int max =0;
  int min =0;
  int lindex=2*index;
  int rindex= 1+lindex;

  while(!min && lindex <= pq->size)
  {
    max = lindex;
    if(rindex <= pq->size){
      if(pq->heap[lindex].priority < pq->heap[rindex].priority){
          max = rindex;
      }

    }
    if(pq->heap[index].priority < pq->heap[max].priority){
        swap(pq,max,index);
        index = max;
    }
    else{
      min =1;
    }
    lindex = 2*index;
    rindex=1+lindex;
  }
}
void percolate_down_min(PQ* pq, int index){
  int min =0;
  int max =0;
  int lindex=2*index;
  int rindex= 1+lindex;

  while(!max && lindex <= pq->size){
    min = lindex;
    if(rindex <= pq->size){
      if(pq->heap[lindex].priority > pq->heap[rindex].priority){
        min = rindex;
      }
    }
    if(pq->heap[index].priority > pq->heap[min].priority)
    {
      swap(pq,min,index);
      index = min;
    }
    else{
      max =1;
    }
    lindex = 2*index;
    rindex=1+lindex;
  }
}

int pq_insert(PQ * pq, int id, double priority){
  if(id < 0 || id >= pq->capacity){
    printf("Error: Id is not in the Range\n");
    return 0;
  }
  if (pq->size >= pq->capacity){
    printf("Error: Queue is already Full\n");
    return 0;
  }
  if (pq->poin_id[id] != NULL){
    printf("Error: Id is already in use\n");
    return 0;
  }
  int num = 1+pq->size; // n+1
	pq->heap[num].id = id;
  pq->heap[num].priority = priority;
	pq->heap[num].index = num;
	pq->poin_id[id] = &(pq->heap[num]);
	pq->size++;
  if(pq->min_heap==1){
    percolate_up_min(pq,pq->size);
    }
  else{
    percolate_up_max(pq,pq->size);
  }


  return 1;		
}
int pq_change_priority(PQ * pq, int id, double new_priority){
  if(id < 0 || id >= pq->capacity){
    printf("Error: Id is not in the Range\n");
    return 0;
  }
  if (pq->poin_id[id] == NULL){
    printf("Error: Id is not being used\n");
    return 0;
  }
  pq->poin_id[id]->priority= new_priority;
  int index = pq->poin_id[id]->index;
  if(pq->min_heap == 1){
      percolate_up_min(pq,index);
      percolate_down_min(pq,index);
  }
  else{
      percolate_up_max(pq,index);
      percolate_down_max(pq,index);
  }
  return 1;
}
int pq_remove_by_id(PQ * pq, int id){
  if(id < 0 || id >= pq->capacity || pq->size == 0) {
		printf("Error: ID is not in the range.\n");
		return 0;
	}

	if(pq->poin_id[id] == NULL) {
		printf("Error: ID is not in use.\n");
		return 0;
	}
  if(pq->size == 1 || pq->poin_id[id]->index == pq->size)
    {
      pq->poin_id[id] = NULL;
      pq->size--;
      return 1;
    }
  int idEnd = pq->heap[pq->size].id;
  double priorityEnd = pq->heap[pq->size].priority;
  pq->poin_id[id]->id = idEnd;
  pq->poin_id[id]->priority = priorityEnd;
  pq->poin_id[idEnd] = pq->poin_id[id];
  pq->poin_id[id] = NULL;
  pq->size--;
  int index = pq->poin_id[idEnd]->index;
  if(pq->min_heap == 1){
      percolate_up_min(pq,index);
      percolate_down_min(pq,index);
  }
  else{
      percolate_up_max(pq,index);
      percolate_down_max(pq,index);
  }
	return 1;
 
}
int pq_get_priority(PQ * pq, int id, double *priority){
	if(id < 0 ||id >= pq->capacity || pq->size == 0) {
		printf("Error: ID is not in the range.\n");
		return 0;
	}

	if(pq->poin_id[id] == NULL) {
		printf("Error: ID is not in use.\n");
		return 0;
	}

	*priority = pq->poin_id[id]->priority;

	return 1;
}

int pq_delete_top(PQ * pq, int *id, double *priority){
	if(pq->size == 0) {
		printf("Error: The priority queue is empty.\n");
		return 0;
	}

	*id = pq->heap[1].id;
	*priority = pq->heap[1].priority;

	if(pq->size > 1) {
		int newid = pq->heap[pq->size].id;
		double newpriority = pq->heap[pq->size].priority;
		pq->heap[1].id = newid;
		pq->heap[1].priority = newpriority;
		pq->poin_id[newid] = &(pq->heap[1]);		
	}

	pq->poin_id[*id] = NULL;
	pq->size--;

	if(pq->size > 1){
		if(pq->min_heap==1){
			percolate_down_min(pq, 1);
		}
		else{
			percolate_down_max(pq, 1);
		}
	}
		

	return 1;
}
int pq_peek_top(PQ * pq, int *id, double *priority){
	
	if(pq->size == 0) {
	  printf("Error: The priority queue is empty.\n");
		return 0;
	}
	else{
		*id = pq->heap[1].id;
		*priority = pq->heap[1].priority;
		return 1;

	}

}
int pq_capacity(PQ * pq){
	return pq->capacity;
}

int pq_size(PQ * pq){
	return pq->size;
}


