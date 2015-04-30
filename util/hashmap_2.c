#include"hashmap.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


/*
 * return a integer value in [0, 2^length-1]
 * input:
 * length: log(size_of_hash_map)
 */
int get_bucket(int i, int length){
  if(length < 1 || length > 31) return 0;
  //int m = 32 / length;
  int r = 0;
  while(i > 0){
    r ^= i & ((1<<length)-1);
    i = i >> length;
  }
  return r & ((1<<length)-1);
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
hashmap * hashmap_init(int size, hashcode hc, equals eq, int ks, int vs){
  hashmap * hm = (hashmap *) malloc(sizeof(hashmap));
  if(!hm) return hm;
  size = 1<<(MAX_LENGTH_IN_BIT-1);
  hm->buckets = (node **)malloc(sizeof(node *) * size);
  if(hm->buckets == NULL){
    free(hm);
    return NULL;//fail
  }
  for(int i = 0; i < size; i++){
    hm->buckets[i] = NULL;
  }
  hm->size = size;
  hm->hash_func = hc;
  hm->eq_func = eq;
  //hm->copy_key = ck;
  //hm->copy_value = cv;
  hm->key_size = ks;
  hm->value_size = vs;
  return hm;
}


void * hashmap_get(hashmap * hm, void * key){
  if(!hm) return NULL;
  int h = hm->hash_func(key);
  node * temp = hm->buckets[get_bucket(h, MAX_LENGTH_IN_BIT)];
  while(temp){
    
    if(hm->eq_func(key, temp->pKey) == 1){
      return temp->pVal;
    }
    temp = temp->next;
  }
  return NULL;
}




int hashmap_insert(hashmap * hm, void * key, void * value){
  if(!hm) return 0;
  int h = hm->hash_func(key);
  int t = get_bucket(h, MAX_LENGTH_IN_BIT);
  printf("go to bucket %d\n", t);
  node * temp = hm->buckets[t];
  node * p = NULL;
  while(temp){
    if(hm->eq_func(temp->pKey, key)==1){
      /* update */
      //free(temp->pVal); no need to free
      //temp->pVal = value;
      memcpy(temp->pVal, value, hm->value_size);
      return 1;//hm->cv(value, &(temp->pVal));
    }
    p = temp;
    temp = temp->next;
  }
  node * n = (node *) malloc(sizeof(node));
  if(!n) return 0;
  //n->pVal = value;
  //n->pKey = key;
  memcpy(n->pVal, value, hm->value_size);//use memcpy seems not good
  memcpy(n->pKey, key, hm->key_size);//e.g., key is struct{char *; int}
  //if key = {"ABC", 20}, memcpy will only copy the first pointer of "ABC"
  n->next = NULL;
  n->prev = NULL;
  if(!p)  hm->buckets[t] = n;
  else{
    p->next = n;
    n->prev = p;
  }
  return 1;
  
}

int hashmap_delete(hashmap * hm, void * key){
  if(!hm) return 0;
  int t = get_bucket(hash_func(key), MAX_LENGTH_IN_BIT);
  node * temp = hm->buckets[t];
  node * p = NULL;
  node * n = NULL;
  int result = 0;
  while(temp){
    if(hm->eq_func(temp->pKey, key) == 1){
      /* found, delete it */
      //free(temp->pVal);
      //free(temp->pKey);
      n = temp->next;
      free(temp);
      result = 1;
      break;
    }
    p = temp;
    temp = temp->next;

  }
  if(result == 0) return result;
  if(!p) hm->buckets[t] = n;
  else{
    p->next = n;
    if(n) n->next = p;
  }
  return result;
}

