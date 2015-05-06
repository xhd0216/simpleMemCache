#include"cache.c"
#include<pthread.h>
#define NUM_THREADS 20
#define LOOP_TIMES 5000

pthread_mutex_t lk;

struct KeyType{
  //char name [30];
  char c;
};
typedef struct KeyType key_t;

struct ValueType{
  //char name [15];
  char c;
};
typedef struct ValueType value_t;

hash_code_t KeyTypeHash(const void * k){
  key_t * s = (key_t *) k;
  hash_code_t r;
  memset(r.hc, '\0', HASH_CODE_SIZE+1);
  //int le = strlen(s->name);
  //le = le<HASH_CODE_SIZE?le:HASH_CODE_SIZE;
  //strncpy(r.hc, &(s->c), 1);
  r.hc[0] = s->c;
  r.hc[1] = s->c-32;
  return r;
}

/*
int ValueTypeCopy(void ** v1, const void * v2){
  value_t ** j1 = (value_t **) v1;
  value_t * j3 = malloc(sizeof(value_t));
  if(j3 == NULL) return 0;
  value_t * j2 = (value_t *) v2;
  memcpy(j3, j2, sizeof(value_t));
  *j1 = j3;
  return 1;
}
*/
void * ValueTypeCopy(const void * v2){
  //value_t ** j1 = (value_t **) v1;
  value_t * j3 = malloc(sizeof(value_t));
  if(j3 == NULL) return 0;
  value_t * j2 = (value_t *) v2;
  memcpy(j3, j2, sizeof(value_t));
  //*j1 = j3;
  return j3;
}

int ValueTypeFree(void * v){
  value_t * s = (value_t *) v;
  free(s);
  return 1;
}

typedef struct{
  int thread_num;
  cache_t * caa;
  key_t key;
}arg_t;

void * GetOther( const void * key){
  value_t * r = malloc(sizeof(value_t));
  if(!r) return NULL;
  key_t * k = (key_t *) key;
  r->c = k->c-32;//change to upper case letters
  printf("from other source: %c\n", r->c);
  return r;
}
void * GetCache(void * avoid){
  arg_t * a = (arg_t *)avoid;
  key_t key;
  memcpy( &key, &(a->key), sizeof(key_t));
  int index = a->thread_num;
  void * r;
  value_t * t;
  printf("enter thread %d, key = %c\n", index, key.c);
  for(int j = 0; j < LOOP_TIMES; j++){
    //pthread_mutex_lock(&lk);
    r = cache_get(a->caa, &key);
    //pthread_mutex_unlock(&lk);
    t = (value_t *) r;
    if(t != NULL){
      printf("thread %d, times=%d, key %c, value %c\n", index, j, key.c, t->c);
    }
    key.c = 'a' + (key.c-'a'+index * 3 + 1)%26;
  }
  pthread_exit(NULL);

}

int main(){
  cache * ca = cache_create(20, &KeyTypeHash, &ValueTypeCopy, &ValueTypeFree, &GetOther);
  if(!ca){
    printf("initialization failed\n");
    return 0;
  }
  pthread_mutex_init(&lk, NULL);
  pthread_t tarray[NUM_THREADS];
  for(int i = 0; i < NUM_THREADS; i++){
    arg_t * a = malloc(sizeof(arg_t));
    a->thread_num = i;
    a->key.c = 'a'+i;
    a->caa = ca;
    pthread_create(&tarray[i], NULL, &GetCache, (void *)a);
    printf("thread %d created\n", i);
  }
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(tarray[i], NULL);
  }
  cache_destroy(ca);
  printf("finish excution\n");
}
