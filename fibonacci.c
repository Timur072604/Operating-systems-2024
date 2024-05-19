#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "function.h"
int p[2];  // Массив для хранения дескрипторов канала для чтения и записи
int o[2];  // Массив для хранения дескрипторов канала для чтения и записи
void run_fib(){
    size_t m = 5;
    close(p[1]);  // Закрыть дескриптор канала для записи
    size_t sz = read(p[0], &m, sizeof(m));  // Прочитать из канала значение m
    close(p[0]);  // Закрыть дескриптор канала для чтения
    if (sz!=sizeof(m)){
        printf("Cannot read all the data\n");
        _exit(1);  // Выйти из процесса с кодом 1
    }
    size_t res = fibonacci(5);  // Вычислить значение res
    printf("(SUBPROCESS) Fibonacci number %ld is %ld\n", m, res);  // Вывести значение m и res
    close(o[0]);  // Закрыть дескриптор канала для чтения
    write(o[1], &res, sizeof(res));  // Записать в канал значение res
    close(o[1]);  // Закрыть дескриптор канала для записи
}
size_t n = 5;
int main(){
    printf("Hello, World!\n");
    size_t cpid;
    pipe(p);  // Создать канал для чтения и записи
    pipe(o);  // Создать канал для чтения и записи
    if ((cpid = fork()) == 0){  // Создать новый процесс
        run_fib();  // Выполнить функцию run_fib в новом процессе
    }
    else {
        close(p[0]);  // Закрыть дескриптор канала для чтения
        write(p[1], &n, sizeof(n));  // Записать в канал значение n
        close(p[1]);  // Закрыть дескриптор канала для записи
        size_t rc;
        close(o[1]);  // Закрыть дескриптор канала для записи
        read(o[0], &rc, sizeof(rc);  // Прочитать из канала значение rc
        close(o[0]);  // Закрыть дескриптор канала для чтения
        printf("Fibonacci number %ld is %ld, which was taken from child process)\n", n, rc);  // Вывести значение n и rc
    }
    if (cpid!=0){
        int rc;
        waitpid(cpid, &rc, 0);  // Дождаться завершения нового процесса
        printf("A Child process killed.\n");  // Вывести сообщение
    }
    return 0;
}
