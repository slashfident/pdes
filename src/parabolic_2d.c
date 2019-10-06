#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define arr(a, n, i, j) (*((a) + (((i)*(n)) + (j))))
#define x(i,h) ((i)*(h))
#define y(j,h) ((j)*(h))

/**
 * IMPORTANT: this doesn't work since it uses the Gauß-Jordan Algorithm
 * which is insufficient in the case of non positive definite matrices
 * and unfortunately parabolic pdes rarely result in those
 */

int main(int argc, char **argv) {
    char const * const options = "o:n:i:t:m:k:";
    char *filename = 0;
    size_t iter = 0;

    double theta = 1.0;
    double k = 1.0; // u_t + k*u_xx = f | f independent of t!

    size_t n = 0; // # of lattice points per dimension, including boundary
    size_t t = 0; // # of time steps

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
            case 't':
                t = atoi(optarg);
                if (!t) {
                    printf("Invalid argument to -t.\n");
                    abort();
                }
                break;
            case 'm':
                theta = atof(optarg);
                if (theta < 0.0 || theta > 1.0) {
                    printf("Invalid argument to -m.\n");
                    abort();
                }
                break;
            case 'k':
                k = atof(optarg);
                if (!k) {
                    printf("Invalid argument to -k.\n");
                    abort();
                }
                break;
            case '?':
                if (optopt == 'f' || optopt == 'n' || optopt == 'i' || optopt == 't' || optopt == 'm' || optopt == 'k')
                    printf("-%c expected argument.\n", optopt);
                else
                    printf("Unknown option -%c.\n", optopt);
                break;
            default:
                abort();
        }
    }

    if (!filename || !iter || !n || !t) abort();

    double const h = 1.0/(n - 1.0);
    double const h_ = 1.0/(h*h);
    double const dt = 1.0/t;

    double const z = dt*(1.0 - theta)*k*h_;
    double const w = dt*theta*k*h_;

    FILE * const file = fopen(filename, "w");
	double f[n*n]; // right-hand side
	double f_[n*n]; // right-hand side after time step
	double u[n*n]; // solution
	double g[n*n]; // right-hand side after transformation

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (!i || i == n-1 || !j || j == n-1) {
                arr(u,n,i,j) = 0.0; // boundary condition
                arr(f,n,i,j) = 0.0;
            } else {
                arr(f,n,i,j) = 0.0; // right-hand side
                if (i >= n/4 && i <= 3*n/4 && j >= n/4 && j <= 3*n/4)
                    arr(u,n,i,j) = 1.0; // initial condition
                else
                    arr(u,n,i,j) = 0.0; // initial condition
            }
        }
    }

    memcpy(f_, f, n*n*sizeof *f);

    for (size_t t_ = 0; t_ < t; ++t_) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j)
                fprintf(file, "%.10f,", arr(u,n,i,j));
			fseek(file, -1, SEEK_CUR);
			fprintf(file, "\n");
		}

        /* update f, f_ here if time dependent */

        for (size_t i = 1; i < n-1; ++i) { // right-hand-side of linear system for this timestep
            for (size_t j = 1; j < n-1; ++j) {
                arr(g,n,i,j) = (-z)*(arr(u,n,i-1,j) + arr(u,n,i+1,j) + arr(u,n,i,j-1) + arr(u,n,i,j+1))
                               + (1.0 + 4.0*z)*arr(u,n,i,j) + dt*(theta*arr(f_,n,i,j) + (1.0 - theta)*arr(f,n,i,j));
			}
		}

        for (size_t it = 0; it < iter; ++it) { // Gauß-Jordan NOTE: Gauß-Jordan is insufficient since the matrix is not positive definite
            for (size_t i = 1; i < n-1; ++i)
                for (size_t j = 1; j < n-1; ++j) {
                    arr(u,n,i,j) = (arr(g,n,i,j) - w*(arr(u,n,i-1,j) + arr(u,n,i+1,j) + arr(u,n,i,j-1)
                                   + arr(u,n,i,j+1)))/(1.0 - 4.0*w);
				}
		}
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j)
            fprintf(file, "%.10f,", arr(u,n,i,j));
		fseek(file, -1, SEEK_CUR);
		fprintf(file, "\n");
	}

    fclose(file);
    return 0;
}
