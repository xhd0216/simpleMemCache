#include"cache.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*return a random index from 0 to size -1*/
/*don't use rand() for multithreads*/
int get_rand_index(int size){
  return 0;
  
}

int copy_int(void ** dest, const void * src){
  index_type * spp = (index_type *)src;
  index_type * r = malloc(sizeof(index_type));
  if(!r) return 0;
  *r = *spp;
  //memcpy(r, spp, sizeof(index_type));
  *dest = r;
  return 1;
}
int free_int(void * ptr){
  free(ptr);
  return 1;
}


cache * cache_create(int size,
		     hashcode key_type_hash_function,
		     cachevalueclone cache_value_clone_function,
		     cachevaluefree cache_value_free_function,
		     getfromothersource get_from_other_function){

  cache * ca;
  ca = malloc(sizeof(cache));
  if(ca == NULL) return ca;
  ca->items = malloc(sizeof(void *) * size);
  if(ca->items == NULL){
    free(ca);
    return NULL;
  }
  ca->index_to_hashmap_2 = malloc(sizeof(node *) * size);
  if(ca->index_to_hashmap_2 == NULL){
    free(ca->items);
    free(ca);
    return NULL;
  }
  ca->hm = hashmap_create(0,/*use default initial value*/
			  key_type_hash_function,
			  &copy_int,
			  &free_int);
  if(ca->hm == NULL){
    free(ca->items);
    free(ca->index_to_hashmap_2);
    free(ca);
    return NULL;
  }
  for(int i = 0; i < size; i++){
    ca->items[i] = NULL;
    ca->index_to_hashmap_2[i] = NULL;
  }
  ca->size = size;
  ca->current_pos = 0;
  pthread_rwlock_init(&(ca->lock), NULL);
  ca->cache_value_clone = cache_value_clone_function;
  ca->cache_value_free = cache_value_free_function;
  ca->get_from_other = get_from_other_function;
  return ca;
}
void * cache_get(cache * ca,
		 void * key)
{
  void ** res = malloc(sizeof(void *));
  if(res == NULL){
    printf("system run out of memory\n");
    return NULL;
  }
  index_type * i;
  pthread_rwlock_rdlock(&(ca->lock));
  node * ip = hashmap_get(ca->hm, key);
  if(ip != NULL){
    printf("hash code: %s\n", ip->hc.hc);
    i = (index_type *) (ip->pVal);
    printf("index in array: %d\n", *i); 
    //ca->cache_value_clone(res, ca->items[*i]);
  }
  pthread_rwlock_unlock(&(ca->lock));
  if(ip != NULL){
    printf("cache hit in index %d!\n", *i);
    //return *res;
    return ca->items[*i];
  }
  else{
    printf("cache miss.\n");
    void * r = ca->get_from_other(res, key);
    if(r == NULL){/*failed*/
      return NULL;
    }
    else{
      pthread_rwlock_wrlock(&(ca->lock));
      /* 
       * if fail to insert,
       * still return *res, the value return by get_from_other() 
       */
      cache_set(ca, key, *res);
      pthread_rwlock_unlock(&(ca->lock));
      return *res;
    }
  }
}
int  cache_set(cache * ca,
		void * key,
		void * value){
  /*create new entry*/
  index_type ti = ca->current_pos;
  node * new_entry = hashmap_insert(ca->hm, key, &ti);
  if(new_entry == NULL){
    printf("failed to insert new entry!\n");
    return 0;
  }
  else{
    printf("new entry inserted into hashmap\n");
    printf("node info: %s, %d\n", new_entry->hc.hc, *((int *)new_entry->pVal));
    /*delete old entry*/
    node * tmp = ca->index_to_hashmap_2[ca->current_pos];
    if(tmp != NULL){
      tmp->prev->next = tmp->next;
      if(tmp->next != NULL) tmp->next->prev = tmp->prev;
      ca->hm->free_value(tmp->pVal);
      free(tmp);
    }
    ca->index_to_hashmap_2[ca->current_pos] = new_entry;
    void * tu;
    int rr =  ca->cache_value_clone(&tu, value);
    if(rr == 0) return 0;
    ca->items[ca->current_pos] = tu;
    ca->current_pos++;
    if(ca->current_pos >=ca->size) ca->current_pos = 0;
  }
  return 1;
}
int cache_destroy(cache * ca){
  hashmap_destroy(ca->hm);
  free(ca->items);
  free(ca->index_to_hashmap_2);
  free(ca);
  return 1;

}
