#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <malloc.h>
#include <string.h>

#define RUNS 32768
#define NAME "Some.Dude.On.The.Internet"
#define DOMAIN "Some.Domain.On.The.Internet"

using namespace std;

void test_c_function();
void test_cpp_function();
void test_lesscrap_c_function();

char* compose_c(const char *name, const char *domain);
string compose_cpp(string& name, string& domain);
char* compose_c_lesscrap(const char *name, const char *domain);

long get_time_diff(timeval *start, timeval *end);

int main (int argc, char *argv[]) {
    test_cpp_function();
    test_c_function();
    test_lesscrap_c_function();
    return 0;
}

void test_c_function() {
    struct timeval start;
    gettimeofday(&start, NULL);
    struct timeval end;

    for (long i = 0; i < RUNS; i++) {
        char* result = compose_c(NAME, DOMAIN);
        free(result);
    }

    gettimeofday(&end, NULL);
    long ms = get_time_diff(&start, &end);
    printf("C function: %d runs took %d ms\n", RUNS, (int)ms);
}

void test_lesscrap_c_function() {
    struct timeval start;
    gettimeofday(&start, NULL);
    struct timeval end;

    for (long i = 0; i < RUNS; i++) {
        char* result = compose_c_lesscrap(NAME, DOMAIN);
        free(result);
    }

    gettimeofday(&end, NULL);
    long ms = get_time_diff(&start, &end);
    printf("C function (sprintf): %d runs took %d ms\n", RUNS, (int)ms);
}

void test_cpp_function() {
    struct timeval start;
    gettimeofday(&start, NULL);
    struct timeval end;

    // do C++ a favour: let the automagic conversion of char* 
    //  to string only happen once, right here
    string name = NAME;
    string domain = DOMAIN;
    for (long i = 0; i < RUNS; i++) {
        string result = compose_cpp(name, domain);
    }

    gettimeofday(&end, NULL);
    long ms = get_time_diff(&start, &end);
    printf("C++ function: %d runs took %d ms\n", RUNS, (int)ms);
}

string compose_cpp(string& name, string& domain) {
    return name + "@" + domain;
}

char* compose_c(const char* name, const char* domain) {
     char* res = (char*)malloc(strlen(name)+strlen(domain)+2); // space for strings, '@', and 0
     char* p = strcpy(res,name);
     p += strlen(name);
     *p = '@';
     strcpy(p+1,domain);
     return res;
}

char* compose_c_lesscrap(const char *name, const char *domain) {
    char* result = (char*)malloc(strlen(name) + strlen(domain) + 2);
    sprintf(result, "%s@%s", name, domain);
    return result;
}

long get_time_diff(timeval *start, timeval *end) {
    long totalStartMs = (start->tv_sec * 1000) + start->tv_usec;
    long totalEndMs = (end->tv_sec * 1000) + end->tv_usec;
    return totalEndMs - totalStartMs;
}
