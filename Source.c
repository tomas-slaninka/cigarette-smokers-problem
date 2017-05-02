#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <unistd.h>

typedef struct {
	int isTobacco, isPaper, isMatch;
	sem_t * dataCopied;
	sem_t * agentSem;
	sem_t * tobacco;
	sem_t * paper;
	sem_t * match;
	sem_t * tobaccoSem;
	sem_t * paperSem;
	sem_t * matchSem;

	sem_t * mutex;
} Data;

int agentA(void *inputData) {
	Data data = *((Data*)inputData);
	
	assert(sem_post(data.dataCopied) == 0);
	
	while(1) {
		assert(sem_wait(data.agentSem) == 0);
		printf("Agent A working\n");
		assert(sem_post(data.tobacco) == 0);
		assert(sem_post(data.paper) == 0);
		  
	}
}

int agentB(void *inputData) {
	Data data = *((Data*)inputData);
  
	assert(sem_post(data.dataCopied) == 0);
	
	while(1) {
		assert(sem_wait(data.agentSem) == 0);
		printf("Agent B working\n");
		assert(sem_post(data.paper) == 0);
		assert(sem_post(data.match) == 0);
		  
	}
}

int agentC(void *inputData) {
	Data data = *((Data*)inputData);
  
	assert(sem_post(data.dataCopied) == 0);
	
	while(1) {
		assert(sem_wait(data.agentSem) == 0);
		printf("Agent C working\n");
		assert(sem_post(data.tobacco) == 0);
		assert(sem_post(data.match) == 0);
		  
	}
}

int pusherA(void *inputData) {
	Data data = *((Data*)inputData);
  
	assert(sem_post(data.dataCopied) == 0);
	
	while(1) {
		assert(sem_wait(data.tobacco) == 0);
		assert(sem_wait(data.mutex) == 0);
		
		if (data.isPaper == 1){
			data.isPaper = 0;
			assert(sem_post(data.matchSem) == 0);
		}
		else if (data.isMatch == 1){
			data.isMatch = 0;
			assert(sem_post(data.paperSem) == 0);
		}
		else
			data.isTobacco = 1;
		
		assert(sem_post(data.mutex) == 0);
		  
	}  
}

int pusherB(void *inputData) {
	Data data = *((Data*)inputData);
  
	assert(sem_post(data.dataCopied) == 0);
  
	while(1) {
		assert(sem_wait(data.paper) == 0);
		assert(sem_wait(data.mutex) == 0);
		
		if (data.isTobacco == 1){
			data.isTobacco = 0;
			assert(sem_post(data.matchSem) == 0);
		}
		else if (data.isMatch == 1){
			data.isMatch = 0;
			assert(sem_post(data.tobaccoSem) == 0);
		}
		else
			data.isPaper = 1;
		
		assert(sem_post(data.mutex) == 0);
		  
	}  
}

int pusherC(void *inputData) {
	Data data = *((Data*)inputData);
  
	assert(sem_post(data.dataCopied) == 0);
  
  while(1) {
  	assert(sem_wait(data.match) == 0);
  	assert(sem_wait(data.mutex) == 0);
  	
  	if (data.isTobacco == 1){
		data.isTobacco = 0;
		assert(sem_post(data.paperSem) == 0);
  	}
  	else if (data.isPaper == 1){
  		data.isPaper = 0;
  		assert(sem_post(data.tobaccoSem) == 0);
  	}
  	else
  		data.isMatch = 1;
  	
  	assert(sem_post(data.mutex) == 0);
  	  
  }  
}

//Smoker with tobacco
int smokerA(void *inputData) {
	Data data = *((Data*)inputData);
  
	assert(sem_post(data.dataCopied) == 0);
	
	while(1) {
		assert(sem_wait(data.tobaccoSem) == 0);
		usleep(rand() % 1000);
		printf("Smoker A - staring with tobacco\n");
		assert(sem_post(data.tobaccoSem) == 0);
	}
}

//Smoker with paper
int smokerB(void *inputData) {
	Data data = *((Data*)inputData);
  
	assert(sem_post(data.dataCopied) == 0);
	
	while(1) {
		assert(sem_wait(data.paperSem) == 0);
		usleep(rand() % 1000);
		printf("Smoker A - staring with paper\n");
		assert(sem_post(data.paperSem) == 0);
	}
}

//Smoker with match
int smokerC(void *inputData) {
	Data data = *((Data*)inputData);
  
	assert(sem_post(data.dataCopied) == 0);
	
	while(1) {
		assert(sem_wait(data.matchSem) == 0);
		usleep(rand() % 1000);
		printf("Smoker A - staring with match\n");
		assert(sem_post(data.matchSem) == 0);
	}
}

int main() {
	int i = 0;
	pthread_t id[9];

	Data data;
	sem_t dataCopied, mutex, agentSem, tobacco, paper, match, tobaccoSem, paperSem, matchSem;
  
	data.isPaper = 0;
	data.isTobacco = 0;
	data.isMatch = 0;

	assert(sem_init(&dataCopied, 0, 0) == 0);
	data.dataCopied = &dataCopied;
	assert(sem_init(&mutex, 0, 1) == 0);
	data.mutex = &mutex;
  
	assert(sem_init(&agentSem, 0, 1) == 0);
	data.agentSem = &agentSem;
  
	assert(sem_init(&tobacco, 0, 0) == 0);
	data.tobacco = &tobacco;
	assert(sem_init(&paper, 0, 0) == 0);
	data.paper = &paper;
	assert(sem_init(&match, 0, 0) == 0);
	data.match = &match;
  
	assert(sem_init(&tobaccoSem, 0, 0) == 0);
	data.tobaccoSem = &tobaccoSem;
	assert(sem_init(&paperSem, 0, 0) == 0);
	data.paperSem = &paperSem;
	assert(sem_init(&matchSem, 0, 0) == 0);
	data.matchSem = &matchSem;

	assert(pthread_create(&id[0], NULL, agentA, (void*)&data) == 0);
	assert(sem_wait(&dataCopied) == 0);
  
	assert(pthread_create(&id[1], NULL, agentB, (void*)&data) == 0);
	assert(sem_wait(&dataCopied) == 0);
  
	assert(pthread_create(&id[2], NULL, agentC, (void*)&data) == 0);
	assert(sem_wait(&dataCopied) == 0);
  
	assert(pthread_create(&id[3], NULL, pusherA, (void*)&data) == 0);
	assert(sem_wait(&dataCopied) == 0);
  
	assert(pthread_create(&id[4], NULL, pusherB, (void*)&data) == 0);
	assert(sem_wait(&dataCopied) == 0);
  
	assert(pthread_create(&id[5], NULL, pusherC, (void*)&data) == 0);
	assert(sem_wait(&dataCopied) == 0);
  
	assert(pthread_create(&id[6], NULL, smokerA, (void*)&data) == 0);
	assert(sem_wait(&dataCopied) == 0);
  
	assert(pthread_create(&id[7], NULL, smokerB, (void*)&data) == 0);
	assert(sem_wait(&dataCopied) == 0);
  
	assert(pthread_create(&id[8], NULL, smokerC, (void*)&data) == 0);
	assert(sem_wait(&dataCopied) == 0);

	for (i = 0; i < 9; i++) {
		assert(pthread_join(id[i], NULL) == 0);
	}

	return 0;
}
