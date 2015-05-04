#include"hashmap_2.c" 
#include<pthread.h>
#define MAX_CACHE_SIZE 1024

typedef int (* cachevalueclone)(void **, const void *);
typedef int (* cachevaluefree)(void *);
typedef void * (* getfromothersource)(void **, const void *);
typedef int index_type;

struct RandomReplacementCache{
  hashmap_2 * hm; /* (K, V) = (void, int) */
  void ** items;
  node ** index_to_hashmap_2;
  int size;
  index_type current_pos;
  pthread_rwlock_t lock;
  cachevalueclone cache_value_clone;
  cachevaluefree cache_value_free;
  getfromothersource get_from_other;
};
typedef struct RandomReplacementCache cache;




cache * cache_create(int size,
		     hashcode key_type_hash_function,
		     cachevalueclone cache_value_clone_function,
		     cachevaluefree cache_value_free_function,
		     getfromothersource get_from_other_function);
void * cache_get(cache * ca,
		 void * key);
int  cache_set(cache * ca,
		void * key,
		void * value);
int cache_destroy(cache * ca);
