#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

struct Seg {
    double (*f) (double x);  
    double value;
    int idx;
};

pthread_mutex_t total_mutex = PTHREAD_MUTEX_INITIALIZER;
static double a = 1.;
static double b = 4.;
static double total = 0.0;
static int n = 100000000;
static double eps = 1e-15;
const double value1 = 15.;
const double value2 = 1035/4;
const double value3 = 246.;
const double value4 = 105/2;
const double value5 = 9.;

static int p;

double f1(double x) {
    double res;
    res = x * x - 2 * x + 3;
    return res;
}

double f2(double x) {
    double res;
    res = 3*x*x*x + 2*x*x - 3*x + 16;
    return res;
}

double f3(double x) {
    double res;
    res = 4*x*x*x + 4*x -13;
    return res;
}

double f4(double x) {
    double res;
    res = 7*x;
    return res;
}

double f5() {
    double res;
    res = 3;
    return res;
}


double integrate(double a, double b, int n, double (*f) (double x)) {

    double h = (b - a) / n;
    double sum = 0;

    double x0 = a;
    double x1 = a + h;

    for (int i = 0; i < n; i++) {
        sum += f(x0) + 4 * f(x0 + h / 2) + f(x1);

        x0 += h;
        x1 += h;
    }

    return (h / 6) * sum;
}

void* process_function(void* pa)     {
    //int my_rank = *((int*)pa);
    struct Seg* segment = (struct Seg*)pa;
    int my_rank = segment -> idx;
    double len = (b - a) / p;
    int local_n = n / p;
    double local_a = a + my_rank * len;
    double local_b = local_a + len;
    double integral;
    integral = integrate(local_a, local_b, local_n, segment -> f);
    pthread_mutex_lock(&total_mutex);
    total += integral;
    pthread_mutex_unlock(&total_mutex);
    return 0;
}

int main(int argc, char* argv[]) {
    pthread_t* threads;
	int i;
    double res[5];
    struct Seg segment[5];

    segment[0].f = f1;
    segment[0].value = value1;
    segment[0].idx = 0;
    segment[1].f = f2;
    segment[1].value = value2;
    segment[1].idx = 0;
    segment[2].f = f3;
    segment[2].value = value3;
    segment[2].idx = 0;
    segment[3].f = f4;
    segment[3].value = value4;
    segment[3].idx = 0;
    segment[4].f = f5;
    segment[4].value = value5;
    segment[4].idx = 0;


    if (argc != 2) {
        printf("Usage: %s <instances>\n", argv[0]);
        return 1;
    }
    char* end;
    p = (int)strtol(argv[1], &end, 10);

    for(int j = 0; j < 5; j++) {

        total = 0.0;

        if (!(threads = (pthread_t*)malloc(p * sizeof(pthread_t)))) {
        fprintf(stderr, "Not enough memory!\n");
        return 1;
    }

        for (i = 0; i < p; i++) {
            segment[j].idx = i;
            if (pthread_create(threads + i, 0, process_function, (void*)&segment[j])) {
                fprintf(stderr, "cannot create thread #%d!\n", i);
                return 2;
            }
        }

        for (i = 0; i < p; i++) {
            if (pthread_join(threads[i], 0)) {
                fprintf(stderr, "cannot wait thread #%d!\n", i);
            }
        }

        free(threads);
        res[j] = total;

        printf("measurement error in test %lf\n", res[j]);
    }

    return 0;
}