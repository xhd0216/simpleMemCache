#include"hashmap_2.c"


struct stype{
  char c [30];
  int v;
  int no_use;
};
typedef struct stype ss;

int KeyTypeCopy(void * k1, void * k2){
  ss * s1 = (ss *) k1;
  ss * s2 = (ss *) k2;
  memcpy(k1, k2, sizeof(ss));
  return 1;

}

int KeyTypeEqual(void * k1, void * k2){
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

int KeyTypeHash(void * k){
  ss * s = (ss *) k;
  int r = s->v;
  int temp;
  for(int i = 0; i < 30; i++){
    if(s->c[i] == '\0') break;
    temp = ((1<<8)-1) & s->c[i];
    temp = temp << ((i%4)*8);
    r ^= temp;
  }
  return r;
}

struct vtype{
  int j;
};
typedef struct vtype vv;

int ValueTypeCopy(void * v1, void * v2){
  vv * j1 = (vv *) v1;
  vv * j2 = (vv *) v2;
  memcpy(v1, v2, sizeof(vv));
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
      tempk = (ss*)n->pKey;
      printf("(%s, %d, %d)\t", tempk->c, tempk->v, tempv->j);
      n = n->next;
    }
    printf("\n");
  }
}


int main(){
  hashmap * hm = hashmap_init(200, &KeyTypeHash, &KeyTypeEqual, sizeof(ss), sizeof(vv), &KeyTypeCopy, &ValueTypeCopy);
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
      vv * hh = hashmap_get(hm, &a);
      if(!hh) printf("did not find entry\n");
      else{
	printf("value is %d\n", hh->j);
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
