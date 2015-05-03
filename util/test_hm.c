#include"hashmap_2.c"


struct stype{
  char c [30];
  int v;
  int no_use;
};
typedef struct stype ss;

int KeyTypeCopy(void * k1, const void * k2){
  ss * s1 = (ss *) k1;
  ss * s2 = (ss *) k2;
  memcpy(k1, k2, sizeof(ss));
  return 1;

}

int KeyTypeEqual(const void * k1, const void * k2){
  ss * s1 = (ss *)k1;
  ss * s2 = (ss *)k2;
  if(s1->v != s2->v) return 0;
  char * c1 = s1->c;
  char * c2 = s2->c;
  int i = 0;
  while(c1 && c2 && i < 30){
    if(*c1 != *c2) return 0;
    if(*c1 == '\0') return 1;
    c1 = c1 + 1;
    c2 = c2 + 1;
    i = i+1;
  }
  return 1;
}

hash_code_t KeyTypeHash(const void * k){
  ss * s = (ss *) k;
  hash_code_t r;
  memcpy(r.hc, s->c, HASH_CODE_SIZE - sizeof(int));
  memcpy(r.hc + HASH_CODE_SIZE - sizeof(int), &(s->v), sizeof(int));
  r.hc[HASH_CODE_SIZE] = '\0';
  return r;
}

struct vtype{
  int j;
};
typedef struct vtype vv;

int ValueTypeCopy(void ** v1, const void * v2){
  printf("enter Value Type Copy Functino!\n");
  vv ** j1 = (vv **) v1;
  vv * j3 = malloc(sizeof(vv *));
  if(j3 == NULL) return 0;
  vv * j2 = (vv *) v2;
  printf("before mem copy...\n");
  memcpy((j3), j2, sizeof(vv));
  printf("is this ok??\n");
  *j1 = j3;
  return 1;
}

void iterate(hashmap * h){
  if(!h) return;
  node * n;
  vv * tempv;
  ss * tempk;
  for(int i = 0; i < h->size; i++){
    printf("entry %d:\t", i);
    n = h->buckets[i];
    while(n){
      tempv = (vv*)n->pVal;
      printf("(%s, %d)\t", n->hc.hc, tempv->j);
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


int main(){
  hashmap * hm = hashmap_create(200, &KeyTypeHash, &ValueTypeCopy, &FreeValue);
  if(!hm){
    printf("initialization failed\n");
    return 0;

  }
  printf("initialized\n");
  
  int choice = 0;
  while(choice != 5){
    printf("choose an option:\n1. insert\n2. get\n3. delete\n4. print\n5. quit\n");
    scanf("%d", &choice);
    if(choice == 1){
      ss a;
      vv b;
      printf("input key name:");
      scanf("%29s", a.c);
      printf("input key data:");
      scanf("%d", &a.v);
      printf("input value data:");
      scanf("%d", &b.j);
      int r = hashmap_insert(hm, &a, &b);
      if(r == 1) printf("success!\n");
      else printf("failure!\n");
    }
    else if(choice == 2){
      ss a;
      printf("input key name:");
      scanf("%29s", a.c);
      printf("input key data:");
      scanf("%d", &a.v);
      node * hh = hashmap_get(hm, &a);
      if(!hh) printf("did not find entry\n");
      else{
	vv * shh = (vv *)hh->pVal;
	printf("value is %d\n", shh->j);
      }
    }
    else if(choice == 3){

      ss a;
      printf("input key name:");
      scanf("%29s", a.c);
      printf("input key data:");
      scanf("%d", &a.v);
      int r = hashmap_delete(hm, &a);
      if(r == 1) printf("deleted!\n");
      else{
	printf("did not find element or delete fail\n");
      }
    }
    else if(choice == 4){
      iterate(hm);
    }
    else if(choice == 5){
      break;
    }
    else printf("invalid input\n");
  }
  hashmap_destroy(hm);
  printf("hash map destroyed\n");
  return 0;

}
