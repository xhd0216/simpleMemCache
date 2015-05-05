#include"hashmap_2.c" 
#include<pthread.h>
#define MAX_CACHE_SIZE 1024


/*
 * value_clone_function: clones value to heap
 * typical use:
 * struct ValueType{
 *   char c [30];
 *   int i;
 * };
 * int ValueClone(void ** dest, const void * src){
 *   *dest = malloc(sizeof(ValueType)); //return a ptr to ValueType
 *   memcpy(*dest, src, sizeof(ValueType));
 *   return *dest;
 * }
 *
 * typical use 2:
 * struct ValueType{
 *   char * c;
 *   int i;
 * };
 * int ValueClone(void ** dest, const void * src){
 *   //allocate memory
 *   *dest = malloc(sizeof(ValueType)); 
 *   (*dest)->c = malloc(strlen(src->c) + 1);
 *   strncpy((*dest)->c, src->c, strlen(src->c) + 1);
 *   (*dest)->i = src->i;
 *   return *dest;
 * }
 */
typedef void * (* cachevalueclone)(const void *);
typedef int (* cachevaluefree)(void *);
//typedef void * (* getfromothersource)(void **, const void *);
typedef void * (* getfromothersource)(const void *);
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
typedef cache cache_t;

pthread_mutex_t hardlock;


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
