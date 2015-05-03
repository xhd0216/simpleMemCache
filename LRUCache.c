#include"LRUCache.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int set_buckets(int);
int get_bucket(hash_code_t);
int cmp_hash_code(hash_code_t *, hash_code_t *);

cache * cache_init(int size_of_cache, int num_of_buckets, key_hash_func khf, value_clone_func vcf)
{
  cache * r = malloc(sizeof(cache));
  if(r == NULL){
    printf("fail to allocate memory for cache.\n");
    return r;
  }  
  r->start = malloc(sizeof(linked_list_node));
  if(r->start == NULL){
    printf("fail to allocate memory for linked list\n");
    free(r);
    return NULL;
  }
  r->map = malloc(sizeof(hash_map_t));
  if(r->map == NULL){
    printf("fail to allocate memory for hash map.\n");
    free(r->start);
    free(r);
    return NULL;
  }
  r->map->num_buckets = set_buckets(num_of_buckets);
  r->map->buckets = malloc(sizeof(hash_map_node *) * r->map->num_buckets);
  if(r->map->buckets == NULL){
    free(r->map);
    free(r->start);
    free(r);
    return NULL;
  }
  memset(r->map->buckets, NULL, r->map->num_buckets);
  r->end = r->start;
  r->current_size = 0;
  r->max_size = size_of_cache;
  r->khf = khf;
  r->vcf = vcf;
  return r;
}

void * cache_get(cache * ca, const void * key){
  hash_code_t hc;
  char * tmp = ca->khf(key);
  memcpy(hc.c, tmp, HASH_CODE_BYTES);
  hc.c[HASH_CODE_BYTES] = '\0';
  int i = get_bucket(hc);
  void ** r = malloc(sizeof(void *));
  int found = 0;
  //place read lock here.
  hash_map_node t = ca->map[i];
  while(t){
    if(cmp_hash_code(&hc, t) == 0){
      ca->vcf(r, t->ptrNode->value);
      //no need to catch exception here
      //if value clone function fails and returns NULL here, return NULL.
      found = 1;
      break;
    }
    t = t->next;
  }
  //release read lock here.
  if(found == 0) return NULL;
  //place write lock here
  

  //release write lock
  
}
int cache_insert(cache * , const void * key, const void * value)
int cache_destroy(cache *);
