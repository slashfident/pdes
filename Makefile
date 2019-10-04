default:
	gcc -o elliptic_1d.elf elliptic_1d.c -g -lm
	gcc -o elliptic_2d.elf elliptic_2d.c -g -lm
	gcc -o parabolic_2d.elf parabolic_2d.c -g -lm
