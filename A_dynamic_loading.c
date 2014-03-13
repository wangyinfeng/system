/*====================================================================
 * Description: use dlopen/dlsym/dlclose/dlerror to dynamic load a lib
 * DATE: 2014/03/13
 * Modify:
 * Conclusion:
===================================================================*/
/* include */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dlfcn.h>

/* Macro definition */
#define false           0
#define true            1

int
main(int argc, char *argv[]) 
{
    void *handle;
    double (*cos_func)(double);
    char *error;
    /* loads the dynamic library file */
    handle = dlopen("libm.so", RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        /* returns a human readable string describing the most recent error 
         * that occurred from dlopen(), dlsym() or dlclose() since the last 
         * call to dlerror(). It returns NULL if no errors have occurred 
         * since initialization or since it was last called. */
        exit(EXIT_FAILURE);
    }
    dlerror(); /* need to call again to clear error message */

    /* dlsym() takes a "handle" of a dynamic library 
     * returned by dlopen() and the null-terminated symbol name, 
     * returning the address where that symbol is loaded into memory.
     * if not find, return NULL */
    cos_func = dlsym(handle, "cos");
    if (cos_func == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    printf("%f\n", (*cos_func)(2.0));
    /* dlclose() decrements the reference count on the dynamic library 
     * handle handle. If the reference count drops to zero and no 
     * other loaded libraries use symbols in it, then the dynamic 
     * library is unloaded. Returns 0 on success, and nonzero on error. */
    dlclose(handle);
    
    exit(EXIT_SUCCESS);
}

