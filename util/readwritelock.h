#include<pthread.h>
#define MAX_READER_COUNT 30
#define LOCK_OK 0
#define LOCK_FAIL 1

typedef struct{
  pthread_mutex_t lock;
  int reader_count;
  int writer_count
}my_rwlock_t;

int my_rwlock_init(my_rwlock_t *);
int my_rwlock_rdlock(my_rwlock_t *);
int my_rwlock_wrlock(my_rwlock_t *);
int my_rwlock_destroy(my_rwlock_t *);


int my_rwlock_init(my_rwlock_t * lk){
  lk = malloc(sizeof(my_rwlock_t));
  if(lk == NULL){
    return LOCK_FAIL;
  }
  pthread_mutex_init(&(lk.lock), NULL);
  lk.reader_count = 0;
  lk.writer_count = 0;
  return LOCK_OK;
}

int my_rwlock_rdlock(my_rwlock_t * lk){
  pthread_mutex_lock(&(lk->lock));
  if(lk->writer_count > 0){
    pthread_mutex_unlock(&(lk->lock));
  }
  else{
    
  }
}
