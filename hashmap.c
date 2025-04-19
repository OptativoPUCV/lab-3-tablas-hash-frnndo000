#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if (map == NULL || key == NULL) return ;

    long posicion = hash(key, map->capacity) ;
    long posOriginal = posicion ;

    do {
        if (map->buckets[posicion] == NULL || 
                    (map->buckets[posicion] != NULL && 
                    map->buckets[posicion]->key == NULL)) \
        {
            Pair* nuevo = createPair(strdup(key), value) ;
            map->buckets[posicion] = nuevo ;
            map->current = posicion ;
            map->size++ ;
            return ;
        }

        if (is_equal(map->buckets[posicion]->key, key)) return ;
        posicion = (posicion + 1) % map->capacity ;

    } while (posicion != posOriginal) ;

}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    HashMap* map = (HashMap*) malloc (sizeof(HashMap)) ;
    map->size = 0 ;
    map->capacity = capacity ;
    map->current = -1 ;

    map->buckets = (Pair**) malloc(sizeof(Pair*) * capacity) ;
    for (long i = 0 ; i < capacity ; i++) {
        map->buckets[i] = NULL ;
    }

    return map ;
}

void eraseMap(HashMap * map,  char * key) {    
    long posicion = hash(key, map->capacity) ;
    long posOriginal = posicion ;
    Pair* element = map->buckets[posicion] ;

    if (element != NULL && element->key != NULL && is_equal(element->key, key)) {
        map->buckets[posicion]->key = NULL ;
        map->size-- ;
        return ;
    } else {
        while(1) {
            posicion = (posicion + 1) % map->capacity ;
            Pair* temp = map->buckets[posicion] ;

            if (temp != NULL && temp->key != NULL && is_equal(temp->key, key)) {
                map->buckets[posicion]->key = NULL ;
                map->size-- ;
                return ;
            }
        if (posicion == posOriginal) break ;
        }
    }

    return ;
}

Pair * searchMap(HashMap * map,  char * key) {   
    long posicion = hash(key, map->capacity) ;
    Pair* element = map->buckets[posicion] ;

    if (element != NULL && element->key != NULL && is_equal(element->key, key)) {
        map->current = posicion ;
        return element ;
    } else {
        while(map->buckets[posicion] != NULL) {
            posicion = (posicion + 1) % map->capacity ;
            Pair* temp = map->buckets[posicion] ;

            if (temp != NULL && temp->key != NULL && is_equal(temp->key, key)) {
                map->current = posicion ;
                return temp ;
            }
        }
    }

    return NULL ;
}

Pair * firstMap(HashMap * map) {
    long index = 0 ;
    long indexOriginal = index ;
    Pair* temp = map->buckets[index] ;

    if (temp != NULL && temp->key != NULL && temp->value != NULL) {
        map->current = index ;
        return temp ;
    } else {
        while(1) {
            index = (index + 1) % map->capacity ;
            Pair* element = map->buckets[index] ;

            if (element != NULL && element->key != NULL && element->value != NULL) {
                map->current = index ;
                return element ;
            }
            if (index == indexOriginal) break ;
        }
    }

    return NULL ;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
