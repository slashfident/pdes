#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *file = fopen(argv[1], "w");
    size_t const n = atoi(argv[2]); // # of lattice points, including boundary
    double const h = 1.0 / (1.0 + n);
    double const h_ = 1.0 / (h*h);
    double f[n]; // right-hand side
    double u[n]; // solution
    u[0] = 0; // boundary condition
    u[n-1] = 0; // boundary condition
    f[0] = h_*u[0];
    f[n-1] = h_*u[n-1];
    for (size_t i = 1; i < n-1; ++i) {
        f[i] = 1.0; // right-hand-side
        u[i] = 0.0; // initial guess
    }
    for(size_t it = 0; it < atoi(argv[3]); ++it)
        for (size_t i = 1; i < n-1; ++i)
            u[i] = (f[i] + h_*u[i-1] + h_*u[i+1]) / (2.0*h_);
    for (size_t i = 0; i < n; ++i)
        fprintf(file, "%.10f,", u[i]);
    fseek(file, -1, SEEK_CUR), fprintf(file, "\n");
    fclose(file);
    return 0;
}
