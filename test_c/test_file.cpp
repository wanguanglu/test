#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: %s infile outfile.\n", argv[0]);
        return -1;
    }

    FILE* fp_in = fopen(argv[1], "rb");
    FILE* fp_out = fopen(argv[2], "wb");
    char line[1024];

    while (fgets(line, 1024, fp_in)) {
        if (line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }
        printf("%s\n", line);
        fprintf(fp_out, "%s\n", line);
    }


    fclose(fp_in);
    fclose(fp_out);
    return 0;
}
