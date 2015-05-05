#include"cache.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*return a random index from 0 to size -1*/
/*don't use rand() for multithreads*/

/*
int copy_int(void ** dest, const void * src){
  index_type * spp = (index_type *)src;
  index_type * r = malloc(sizeof(index_type));
  if(!r) return 0;
  // *r = *spp;
  memcpy(r, spp, sizeof(index_type));
  *dest = r;
  return 1;
  }
*/
void * copy_int(const void * src){
  index_type * spp = (index_type *)src;
  index_type * r = malloc(sizeof(index_type));
  if(!r) return NULL;
  *r = *spp;
  //memcpy(r, spp, sizeof(index_type));
  return r;
}
int free_int(void * ptr){
  index_type * s = (index_type *)ptr;
  printf("going to free value!\n");
  free(s);
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
  printf("cache size: %d\n", ca->size);
  pthread_mutex_init(&hardlock, NULL);
  return ca;
}
void * cache_get(cache * ca,
		 void * key)
{
  if(ca == NULL){
    printf("cache is empty\n");
    return NULL;
  }
  void * res;
  index_type * i;
  int found = 0;
  pthread_rwlock_rdlock(&(ca->lock));
  node * ip = hashmap_get(ca->hm, key);
  if(ip != NULL){
    i = (index_type *) (ip->pVal);
    res = ca->cache_value_clone(ca->items[*i]);
    found = 1;
  }
  pthread_rwlock_unlock(&(ca->lock));
  if(found == 1){
    printf("cache hit in cache array, index %d!\n", *i);
    return res;
  }
  else{
    printf("cache miss.\n");
    
    void * r = ca->get_from_other(key);
    printf("got from others\n");
    if(r == NULL){/*failed*/
      return NULL;
    }
    else{
      //pthread_rwlock_wrlock(&(ca->lock));
      pthread_mutex_lock(&hardlock);
      /* 
       * if fail to insert,
       * still return *res, the value return by get_from_other() 
       */
      printf("going to set cache_value\n");
      cache_set(ca, key, r);
      //pthread_rwlock_unlock(&(ca->lock));
      pthread_mutex_unlock(&hardlock);
      return r;
    }
  }
}
int  cache_set(cache * ca,
		void * key,
		void * value){
  /*create new entry*/
  index_type * ti = malloc(sizeof(index_type));
  *ti = ca->current_pos;
  node * new_entry = hashmap_insert(ca->hm, key, (void *)ti);
  if(new_entry == NULL){
    printf("failed to insert new entry!\n");
    return 0;
  }
  else{
    /*delete old entry*/
    printf("current position is %d\n", ca->current_pos);
    node * tmp = ca->index_to_hashmap_2[*ti];
    if(tmp == new_entry){
      /*this means new_entry = hashmap_insert was an update*/
      /*no need to delete old one*/
      printf("it was an update\n");
      return 1;
    }
    if(tmp != NULL){
      tmp->prev->next = tmp->next;
      if(tmp->next != NULL) tmp->next->prev = tmp->prev;
      index_type * aa = (index_type *)(tmp->pVal);
      printf("to delete this entry: %d, current position: %d\n", *aa, *ti);
      if(*aa == *ti && *ti == ca->current_pos){
	if(tmp->pVal != NULL){
	  //free_int((tmp->pVal));
	  ca->hm->free_value(tmp->pVal);
	}
	free(tmp);
      }
      else{
	printf("error here! different pos %d, %d, %d\n", *aa, *ti, ca->current_pos);

      }
    }
    ca->index_to_hashmap_2[ca->current_pos] = new_entry;
    void * tu =  ca->cache_value_clone( value);
    if(tu == NULL){
      printf("failed to copy cache value\n");
      return 0;
    }
    ca->items[ca->current_pos] = tu;
    if(ca->current_pos != *(index_type *)new_entry->pVal){
      printf("problems here!\n");
    }
    ca->current_pos++;
    if(ca->current_pos >= ca->size){
      ca->current_pos = 0;
    }
  }
  return 1;
}
int cache_destroy(cache * ca){
  hashmap_destroy(ca->hm);
  for(int i = 0; i < ca->size; i++){
    ca->cache_value_free(ca->items[i]);
  }
  free(ca->items);
  free(ca->index_to_hashmap_2);
  free(ca);
  return 1;

}
