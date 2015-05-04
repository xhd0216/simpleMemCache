#include"cache.c"


struct stype{
  char c [30];
  //int v;
  //int no_use;
};
typedef struct stype ss;

int KeyTypeCopy(void * k1, const void * k2){
  ss * s1 = (ss *) k1;
  ss * s2 = (ss *) k2;
  memcpy(k1, k2, sizeof(ss));
  return 1;

}


hash_code_t KeyTypeHash(const void * k){
  ss * s = (ss *) k;
  hash_code_t r;
  memset(r.hc, '\0', HASH_CODE_SIZE+1);
  int le = strlen(s->c);
  //int te = s->v;
  le = le<HASH_CODE_SIZE?le:HASH_CODE_SIZE;
  strncpy(r.hc, s->c, le);
  //r.hc[HASH_CODE_SIZE] = '\0';
  return r;
}

struct vtype{
  char name [15];
};
typedef struct vtype vv;

int ValueTypeCopy(void ** v1, const void * v2){
  printf("enter Value Type Copy Functinon!\n");
  vv ** j1 = (vv **) v1;
  vv * j3 = malloc(sizeof(vv));
  if(j3 == NULL){
    printf("failed to allocate mem\n");
    return 0;
  }
  vv * j2 = (vv *) v2;
  printf("first initial %c\n", j2->name[0]);
  memcpy((j3), j2, sizeof(vv));
  printf("temporarily ok?\n");
  *j1 = j3;
  printf("exit Value Type Copy Functinon!\n");

  return 1;
}

void iterate(hashmap_2 * h){
  if(!h) return;
  node * n;
  int * index;
  printf("hash map size %d\n", h->size);
  for(int i = 0; i < h->size; i++){
    printf("entry %d:\t", i);
    n = h->buckets[i]->next;
    while(n){
      index = (index_type *)n->pVal;
      printf("(%s, %d)\t", n->hc.hc, *index);
      n = n->next;
    }
    printf("\n");
  }
}

int FreeValue(void * v){
  vv * s = (vv *) v;
  free(s);
  return 1;
}

void * GetOther(void ** dest, const void * key){
  vv * r = malloc(sizeof(vv));
  if(!r) return NULL;
  if(r != NULL){
    printf("type in value:");
    scanf("%14s", r->name);
  }
  *dest = (void *)r;

  return *dest;
}

int main(){
  cache * ca = cache_create(20, &KeyTypeHash, &ValueTypeCopy, &FreeValue, &GetOther);
  if(!ca){
    printf("initialization failed\n");
    return 0;
  }
  ss a;
  vv * b;
  void * c;
  printf("initialized\n");
  int choice = 0;
  while(choice != 1){
    printf("1. quit\n2. get\n3. print\n");
    scanf("%d", &choice);
    if(choice == 1) break;
    else if(choice == 2){
      printf("input key:");
      scanf("%29s", a.c);
      c = cache_get(ca, &a);
      b = (vv *) c;
      printf("value we want: %s\n", b->name);
    }
    else if(choice == 3){
      iterate(ca->hm);
    }
    
  }
  cache_destroy(ca);
  return 1;

}
