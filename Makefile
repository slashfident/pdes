default: elliptic_1d elliptic_2d parabolic_2d

elliptic_1d:
	gcc -o elliptic_1d.elf src/elliptic_1d.c -g -lm

elliptic_2d:
	gcc -o elliptic_2d.elf src/elliptic_2d.c -g -lm

parabolic_2d:
	gcc -o parabolic_2d.elf src/parabolic_2d.c -g -lm

