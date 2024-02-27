#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_on_file(char *str) {
	FILE *f;
	f = fopen("domain.c", "w");
	fprintf(f, str);
	fclose(f);
}

int main() {
	system("touch domain.c");
	char token[100];
	while (scanf("%s", token) != EOF) {
		if (strcmp(token, "(:predicates") == 0) {
			char buff[100], taux[100];
			scanf("\t%*c%s", token);
			int acc = 0;
			while (scanf("%s", taux) && strcmp(taux, "-") != 0) acc++;
			sprintf(buff, "printf 'int %s' >> domain.c", token);
			system(buff);
			for (int i = 0; i < acc; i++)
				system("printf '[]' >> domain.c");
			system("echo ';' >> domain.c");
		}
	}
	return 0;
}
