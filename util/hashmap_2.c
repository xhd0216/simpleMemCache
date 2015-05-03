#include"hashmap_2.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


hashmap * hashmap_resize(hashmap * hm){
  if(hm->size >= MAX_BUCKETS) return 0;
  int size = hm->size * RESIZE_FACTOR;
  size = hm->size > MAX_BUCKETS?MAX_BUCKETS:hm->size;
  hashmap  * new_hashmap = hashmap_create(size, hm->hash_func, hm->copy_value, hm->free_value);
  if(new_hashmap == NULL){
    return NULL;
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
      if(new_hashmap->buckets[t] == NULL){
	n->prev = NULL;
	n->next = NULL;
      }
      else{
	n->next = new_hashmap->buckets[t];
	new_hashmap->buckets[t]->prev = n;
      }
      new_hashmap->buckets[t] = n;
      n = temp;
    }
  }
  free(hm->buckets);
  free(hm);
  return new_hashmap;
}

/*
 * return a integer value in [0, length]
 * input:
 * length: log(size_of_hash_map)
 */
int get_bucket(hash_code_t * h, int length){
  int r = 0;
  for(int i = 0; i < HASH_CODE_SIZE; i++){
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
 * hashmap h;
 * if(hashmap_init(&h, size) == 0){ ...deal with failure...}
 */
hashmap * hashmap_create(int size, hashcode hc, copyvalue cv, freevalue fv){
  hashmap * hm = malloc(sizeof(hashmap));
  if(!hm) return hm;
  hm->buckets = malloc(sizeof(node *) * INIT_NUM_BUCKETS);
  if(hm->buckets == NULL){
    free(hm);
    return NULL;//fail
  }
  hm->size = INIT_NUM_BUCKETS;
  for(int i = 0; i < hm->size; i++){
    hm->buckets[i] = NULL;
  }
  hm->hash_func = hc;
  hm->copy_value = cv;
  hm->free_value = fv;
  return hm;
}


node * hashmap_get(hashmap * hm, void * key){
  if(!hm) return NULL;
  hash_code_t h = hm->hash_func(key);
  int t = get_bucket(&h, hm->size);
  printf("hash code: %s, bucket %d\n", h.hc, t);
  node * temp = hm->buckets[t];
  printf("error here?\n");
  while(temp){
    printf("we enter here!\n");
    if(memcmp(key, &(temp->hc), sizeof(node)) == 0){
      return temp;
    }
    temp = temp->next;
  }
  printf("seems no error here\n");
  return NULL;
}




int hashmap_insert(hashmap * hm, void * key, void * value){
  node * tmp = hashmap_get(hm, key);
  printf("we got to insert\n");
  if(tmp != NULL){
    printf("found matched key...\n");
    hm->free_value(tmp->pVal);
    return hm->copy_value(&(tmp->pVal), value);
  }
  printf("didn't find matched key, create new one\n");
  printf("we are happy here!\n");
  node * n = malloc(sizeof(node));
  if(!n) return 0;
  int res = hm->copy_value(&(n->pVal), value);
  if(res != 1){
    free(n);
    return 0;
  }
  n->prev = NULL;
  n->hc = hm->hash_func(key);
  printf("we've copied the value!\n");
  int t = get_bucket(&(n->hc), hm->size);
  if(hm->buckets[t] == NULL){
    hm->buckets[t] = n;
  }
  else{
    n->next = hm->buckets[t];
    hm->buckets[t]->prev = n;
    hm->buckets[t] = n;
  }
  return 1;
  
}

int hashmap_delete(hashmap * hm, void * key){
  node * tmp = hashmap_get(hm, key);
  if(tmp == NULL){
    printf("no match item");
    return 0;
  }
  
  if(tmp->prev == NULL){//head of that bucket
    hash_code_t h = hm->hash_func(key);
    int t = get_bucket(&h, hm->size);
    hm->buckets[t] = tmp->next;
    if(tmp->next != NULL) tmp->next->prev = NULL;
  }
  else{
    if(tmp->next != NULL) tmp->next->prev = tmp->prev;
    tmp->prev->next = tmp->next;
  }
  hm->free_value(tmp->pVal);
  free(tmp);
  return 1;
}


/* not necessarily to call this when quitting program  */
int hashmap_destroy(hashmap * hm){
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
