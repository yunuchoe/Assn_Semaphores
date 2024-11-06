#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semaphore; // general semaphore for read and deposit
sem_t withdraw; // used for withdraw as it is exclusive

float balance = 0;

void* read_balance(void *x){ //Check the current balance.
    sem_wait(&semaphore); // wait on semaphore
    int n = *(int *)x;
    printf("Opened by account number: %d\n", n);
    printf("Account balance is: $%.2f\n", balance);
    sem_post(&semaphore); //signal completion
} 

void* deposit_amount(void *x){ //Deposit a specified amount into the account.
    sem_wait(&semaphore); // wait on semaphore
    int n = *(int *)x;
    balance += n;
    sem_post(&semaphore); //signal completion
}

void* withdraw_amount(void *x){ //Withdraw a specified amount from the account.
    sem_wait(&withdraw); // wait on withdraw semaphore
    sem_wait(&semaphore); // wait on semaphore
    int n = *(int *)x;
    balance -= n;
    sem_post(&semaphore); // signal completion
    sem_post(&withdraw); // signal compeltion for withdraw
}

int main(int argc, char* argv[]) {

    // ready the semaphores
    sem_init(&semaphore, 0, 2); // two instances allowed
    sem_init(&withdraw, 0, 1); // only one instance allowed at a time

    // create all the threads
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;
    pthread_t thread5;
    pthread_t thread6;
    pthread_t thread7;
    pthread_t thread8;
    pthread_t thread9;

    // account instances
    int account1 = 1;
    int account2 = 2;
    int account3 = 3;

    // money instances
    int money1 = 5;
    int money2 = 1;
    int money3 = 100;
    int money4 = 5;
    int money5 = 1;
    int money6 = 75;

    // create all threads
    // work from account 1 to 3
    // read balance first, then deposit an amount, then withdraw an ammount
    pthread_create(&thread1, NULL, read_balance, &account1); // should read 0
    pthread_create(&thread2, NULL, deposit_amount, &money1); // deposit 5
    pthread_create(&thread3, NULL, withdraw_amount, &money2); // withdraw 1

    pthread_create(&thread4, NULL, read_balance, &account2); // should read 4
    pthread_create(&thread5, NULL, deposit_amount, &money3); // deposit 100
    pthread_create(&thread6, NULL, withdraw_amount, &money4); // withdraw 5

    pthread_create(&thread7, NULL, read_balance, &account3); // should read 99
    pthread_create(&thread8, NULL, deposit_amount, &money5); // deposit 1
    pthread_create(&thread9, NULL, withdraw_amount, &money6); // withdraw 75 (25 left)

    // join all threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);
    pthread_join(thread7, NULL);
    pthread_join(thread8, NULL);
    pthread_join(thread9, NULL);

    // destroy semaphores once complete
    sem_destroy(&semaphore);
    sem_destroy(&withdraw);
    
    return 0;
}
