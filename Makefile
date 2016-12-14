pam_test: src/main.c
	gcc -o pam_test src/main.c -lpam -lpam_misc