// preso da http://www2.ing.unipi.it/~a008149/corsi/osor/materiale/Semafori.pdf

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define LUN 20
#define CICLI 1
#define DELAY 100000
struct 
{
	char scritta[LUN+1];
	/* Variabili per la  gestione del buffer */
	int primo, ultimo; 
	/* Variabili semaforiche */
	sem_t
	mutex, piene, vuote;
} shared;

void *scrittore1(void *); 
void *scrittore2(void *);
void *lettore(void *);

int
main(void) {
pthread_t s1TID, s2TID, lTID;
int res, i;
shared.primo = shared.ultimo = 0;
sem_init(&shared.mutex, 0, 1); 
sem_init(&shared.piene, 0, 0);
sem_init(&shared.vuote, 0, LUN);
pthread_create(&lTID, 
NULL, lettore, NULL);
pthread_create(&s1TID, NULL, scrittore1, NULL);
pthread_create(&s2TID, NULL, scrittore2, NULL);
pthread_join(s1TID, NULL);
pthread_join(s2TID, NULL);
pthread_join(lTID, NULL);
printf("E' finito l'esperimento ....\n");
}


void
*scrittore1(void
*in) {
int
i, j, k;
for
(i=0; i<CICLI; i++) {
for(k=0; k<LUN; k++) {
sem_wait(&shared.vuote);
/* Controllo che il buffer non sia pieno */
sem_wait(&shared.mutex);
/* Acquisisco la mutua esclusione */
shared.scritta[shared.ultimo] = '-';       /* Operazioni sui dati */
shared.ultimo = (shared.ultimo+1)%(LUN);
sem_post(&shared.mutex);
/* Libero il mutex
*/
sem_post(&shared.piene);
/* Segnalo l’aggiunta di un carattere */
for(j=0; j<DELAY; j++);
/* ... perdo un po’
di tempo */
}
}
return NULL;
}
void
*scrittore2(void
*in) {
int
i, j, k;
for
(i=0; i<CICLI; i++) {
for(k=0; k<LUN; k++) {
sem_wait(&shared.vuote);
/* Controllo che il buffer non sia pieno */
sem_wait(&shared.mutex);
/* Acquisisco la mutua esclusione */
shared.scritta[shared.ultimo] = '+';       /* Operazioni sui dati */
shared.ultimo = (shared.ultimo+1)%(LUN);
sem_post(&shared.mutex);
/* Libero il mutex
*/
sem_post(&shared.piene);
/* Segnalo l’aggiunta di un carattere */
for(j=0; j<DELAY; j++);
/* ... perdo un po’ di tempo */
}
}
return NULL;
}


void
*lettore(void
*in) {
int
i, j, k; char
local[LUN+1]; local[LUN] = 0;      /* Buffer locale */
for
(i=0; i<2*CICLI; i++) {
for(k=0; k<LUN; k++) {
sem_wait(&shared.piene);
/* Controllo che il buffer non sia vuoto */
sem_wait(&shared.mutex);
/* Acquisisco la mutua esclusione */
local[k] = shared.scritta[shared.primo];   /* Operazioni sui dati */
shared.primo = (shared.primo+1)%(LUN);
sem_post(&shared.mutex);
/* Libero il mutex
*/
sem_post(&shared.vuote);
/* Segnalo che ho letto un carattere */
for(j=0; j<DELAY; j++);              /* ... perdo un pò
di tempo */
}
printf("Stringa = %s\n", local);
}
return NULL;
}
