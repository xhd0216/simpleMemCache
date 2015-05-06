#include"hashmap_2.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int hc_compare(hash_code_t * n1, hash_code_t * n2){
  if(strlen(n1->hc) != strlen(n2->hc)) return 1;
  return strncmp(n1->hc, n2->hc, strlen(n1->hc));

}

hashmap_2 * hashmap_resize(hashmap_2 * hm){
  if(hm->size >= MAX_BUCKETS) return 0;
  int size = hm->size * RESIZE_FACTOR;
  size = size > MAX_BUCKETS?MAX_BUCKETS:size;
  printf("new size should be %d\n", size);
  hashmap_2  * new_hashmap_2 = hashmap_create(size,
					  hm->hash_func,
					  hm->copy_value,
					  hm->free_value);
  printf("new_hashmap_2 size is %d\n", new_hashmap_2->size);
  if(new_hashmap_2 == NULL){
    printf("resize failed\n");
    return hm;
  }
  node * n;
  node * temp;
  node * temp_in_new;
  int t;
  for(int i = 0; i < hm->size; i++){
    n = hm->buckets[i];
    while(n){
      t = get_bucket(&(n->hc), size);//new bucket index in new_hashmap
      temp = n->next;
      if(new_hashmap_2->buckets[t] == NULL){
	n->prev = NULL;
	n->next = NULL;
      }
      else{
	n->next = new_hashmap_2->buckets[t];
	new_hashmap_2->buckets[t]->prev = n;
      }
      new_hashmap_2->buckets[t] = n;
      n = temp;
    }
  }
  free(hm->buckets);
  free(hm);
  return new_hashmap_2;
}

/*
 * return a integer value in [0, length]
 * input:
 * length: log(size_of_hash_map)
 */
int get_bucket( hash_code_t * h,  int length){
  int r = 0;
  for(int i = 0; i < strlen(h->hc); i++){
    r ^= (h->hc[i]<<i);
  }
  return r % length;
}

/*
 * initialize hash map
 * input:
 * size: size of hash map
 * hm: hash map to be initialized
 * output:
 * 0: fail
 * 1: success
 * usage:
 * hashmap_2 h;
 * if(hashmap_init(&h, size) == 0){ ...deal with failure...}
 */
hashmap_2 * hashmap_create(int size, hashcode hc, copyvalue cv, freevalue fv){
  hashmap_2 * hm2 = malloc(sizeof(hashmap_2));
  if(!hm2) return hm2;
  if(size > MAX_BUCKETS || size <= 0) size = INIT_NUM_BUCKETS;
  hm2->buckets = malloc(sizeof(node *) * (size+1));
  if(hm2->buckets == NULL){
    free(hm2);
    return NULL;//fail
  }
  hm2->size = size;
  printf("hash map size in create: %d\n", hm2->size);
  for(int i = 0; i < (hm2->size+1); i++){
    hm2->buckets[i] = malloc(sizeof(node *));
    hm2->buckets[i]->next = NULL;
    hm2->buckets[i]->prev = NULL;
    if(hm2->buckets[i] != NULL)    printf("bucket %d initialized, ", i);
    if(hm2->buckets[i]->next == NULL) printf("correctly\n");
  }
  hm2->hash_func = hc;
  hm2->copy_value = cv;
  hm2->free_value = fv;
  return hm2;
}


node * hashmap_get(hashmap_2 * hm, const void * key){
  if(!hm) return NULL;
  hash_code_t h = hm->hash_func(key);
  int t = get_bucket(&h, hm->size);
  //printf("hashmap_get: hash code: %s, bucket %d\n", h.hc, t);
  node * temp = hm->buckets[t]->next;
  while(temp){
    if(hc_compare(&h, &(temp->hc)) == 0){
      return temp;
    }
    temp = temp->next;
  }
  return NULL;
}




node * hashmap_insert(hashmap_2 * hm, const void * key, void * value){
  node * tmp = hashmap_get(hm, key);
  if(tmp != NULL){
    hm->free_value(tmp->pVal);
    tmp->pVal = hm->copy_value(value);
    //int a = hm->copy_value(&(tmp->pVal), value);
    //if(a == 0) printf("failed to allocate mem\n");
    return tmp;
  }
  node * n = malloc(sizeof(node));
  if(!n) return NULL;
  n->pVal = hm->copy_value(value);
  if(n->pVal == NULL){
    free(n);
    return NULL;
  }
  n->hc = hm->hash_func(key);
  int t = get_bucket(&(n->hc), hm->size);
  node * th = hm->buckets[t]->next;
  n->next = th;
  hm->buckets[t]->next = n;
  n->prev = hm->buckets[t];
  if(th!=NULL) th->prev = n;
  return n;  
}

int hashmap_delete(hashmap_2 * hm, const void * key){
  node * tmp = hashmap_get(hm, key);
  if(tmp == NULL){
    printf("no match item");
    return 0;
  }
  tmp->prev->next = tmp->next;
  if(tmp->next != NULL) tmp->next->prev = tmp->prev;
  hm->free_value(tmp->pVal);
  free(tmp);
  return 1;
}


/* not necessarily to call this when quitting program  */
int hashmap_destroy(hashmap_2 * hm){
  if(!hm) return 0;
  node * n;
  node * temp;
  void * tempv;
  void * tempk;
  for(int i = 0; i < hm->size; i++){
    n = hm->buckets[i];
    while(n){
      hm->free_value(n->pVal);
      temp = n->next;
      free(n);
      n = temp;
    }
  }
  free(hm->buckets);
  free(hm);
  return 1;
}
