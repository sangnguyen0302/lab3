#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#define N 10

int circle_Points = 0;
pthread_mutex_t mutex;

double my_random(int min,int max){
    double e = rand() / (double) RAND_MAX;
    return min + e*(max - min);
}

void* points_in_circle(void* arg){
    int nPoints = *((int*)arg);
    
    for(int i = 0; i < nPoints; i++){
        float x = my_random(-1,1);
        float y = my_random(-1,1);
        if(x*x + y*y <= 1) {
            pthread_mutex_lock(&mutex);
            circle_Points++; 
            pthread_mutex_unlock(&mutex);
        }
    
    }
    pthread_exit(&circle_Points);
}

int main(int argc,char* argv[]){
    srand((int)time(NULL));
    pthread_mutex_init(&mutex,NULL);
    int nPoints = atoi(argv[1]);
    int arg_nPoints = nPoints / N;
    pthread_t tid_List[N];
    for(int i = 0; i < N; i++){
        pthread_create(&(tid_List[i]),NULL,points_in_circle,&arg_nPoints);
    }
    for(int i = 0; i < N; i++){
        pthread_join(tid_List[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    double pi = 4.0 *circle_Points / nPoints;
    printf("%lf\n",pi);
    return 0;
}
