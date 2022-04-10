#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>
#define N 10

int in_circle_Points = 0;

sem_t sem;
pthread_mutex_t mutex;

double my_random(int min,int max){
    double e = rand() / (double) RAND_MAX;
    return min + e*(max - min);
}

void* count_points_in_circle(void* arg){
    int nPoints = *((int*)arg);
    
    for(int i = 0; i < nPoints; i++){
        float x = my_random(-1,1);
        float y = my_random(-1,1);
        if(x*x + y*y <= 1) {
            sem_wait(&sem);
            pthread_mutex_lock(&mutex);
            circle_Points++; 
            pthread_mutex_unlock(&mutex);
            sem_post(&sem);
        }
    
    }
    pthread_exit(&in_circle_Points);
}

int main(int argc,char* argv[]){
    srand((int)time(0));
    pthread_mutex_init(&mutex,NULL);
    sem_init(&sem,0,1);
    int nPoints = atoi(argv[1]);
    int arg_nPoints = nPoints / N;
    pthread_t tid_List[N];
    for(int i = 0; i < N; i++){
        pthread_create(&(tid_List[i]),NULL,count_points_in_circle,&arg_nPoints);
    }
    for(int i = 0; i < N; i++){
        pthread_join(tid_List[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem);
    double pi = 4.0 *in_circle_Points / nPoints;
    printf("%f\n",pi);
    return 0;
}
