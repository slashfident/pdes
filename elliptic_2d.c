#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define X(i, h) ((i)*(h))
#define Y(i, h) ((i)*(h))

int main(int argc, char **argv) {
    char const * const options = "o:n:i:v";
    char* filename = 0;
    size_t n = 0; // # of lattice points per dimension, including boundary
    size_t iter = 0; // # of iterations
    bool verbose = 0;

    int c;
    while ((c = getopt(argc, argv, options)) != -1) {
        switch (c) {
            case 'o':
                filename = optarg;
                break;
            case 'n':
                n = atoi(optarg);
                if (!n) {
                    printf("Invalid argument to -n.\n");
                    abort();
                }
                break;
            case 'i':
                iter = atoi(optarg);
                if (!iter) {
                    printf("Invalid argument to -i.\n");
                    abort();
                }
                break;
            case 'v':
                verbose = 1;
                break;
            case '?':
                if (optopt == 'o' || optopt == 'n' || optopt == 'i')
                    printf("-%c expected argument.\n", optopt);
                else
                    printf("Unknown option -%c.\n", optopt);
                break;
            default:
                abort();
        }
    }

    FILE * const file = fopen(filename, "w");
    float f[n*n]; // right-hand side
    float u[n*n]; // (eventual) solution
    float const h = 1.0/(n - 1.0);
    float const h_ = 1.0/(h*h);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (!i) {
                u[i*n+j] = -sin(M_PI*Y(j,h)); // western boundary condition
                f[i*n+j] = 0.0;
            } else if (i == n-1) {
                u[i*n+j] = -sin(M_PI*Y(j,h)); // eastern boundary condition
                f[i*n+j] = 0.0;
            } else if (!j) {
                u[i*n+j] = sin(M_PI*X(i,h)); // southern boundary condition
                f[i*n+j] = 0.0;
            } else if (j == n-1) {
                u[i*n+j] = sin(M_PI*X(i,h)); // northern boundary condition
                f[i*n+j] = 0.0;
            } else {
                u[i*n+j] = 0.0; // initial guess
                f[i*n+j] = 1.0; // right-hand side
            }
        }
    }

    for (size_t it = 0; it < iter; ++it) {
        if (verbose) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j)
                    fprintf(file, "%.10f,", u[i*n+j]);
				fseek(file, -1, SEEK_CUR);
				fprintf(file, "\n");
			}
		}

        for (size_t i = 1; i < n-1; ++i) {
            for (size_t j = 1; j < n-1; ++j)
                u[i*n+j] = (f[i*n+j] + h_*(u[i*n+j-1] + u[i*n+j+1] + u[(i-1)*n+j] + u[(i+1)*n+j]))/(4.0*h_);
		}
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j)
            fprintf(file, "%.10f,", u[i*n+j]);
		fseek(file, -1, SEEK_CUR);
		fprintf(file, "\n");
	}

    return 0;
}
