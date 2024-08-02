#include <stdio.h>
#include <stdbool.h>

#define MEM_SIZE 30000

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Syntax:\n\tbf <file>\n\t Provide any 2nd argument to trigger the debug mode.");
        return 1;
    }
    FILE *fptr = fopen(argv[1], "r");
    if(fptr == NULL) {
        printf("Failed to open the file \"%s\"", argv[1]);
        return 1;
    }
    bool debug_mode = false;
    if(argc >= 3) debug_mode = true;


    int mem[MEM_SIZE];
    for(unsigned int i = 0; i < MEM_SIZE; i++) mem[i] = 0;
    char current_char = fgetc(fptr);
    int loop_depth = 0;
    unsigned int mem_loc = 0;
    while(current_char != EOF) {
        if(debug_mode) printf("[DEBUG] %d: %c\n", ftell(fptr) - 1, current_char);
        switch(current_char) {
            case '+':
                mem[mem_loc]++;
                break;
            case '-':
                mem[mem_loc]--;
                break;
            case '>':
                mem_loc++;
                break;
            case '<':
                mem_loc--;
                break;
            case '[': break;
            case ']':
                if(mem[mem_loc] == 0) break;
                loop_depth = 1;
                fseek(fptr, -1, SEEK_CUR);
                while(loop_depth != 0) {
                    if(fseek(fptr, -1, SEEK_CUR) != 0) {
                        printf("Fatal: fseek() exceeded file bounds! (Getting position %d)", ftell(fptr) - 1);
                        return -1;
                    }
                    current_char = fgetc(fptr);
                    fseek(fptr, -1, SEEK_CUR);
                    if(current_char == '[') loop_depth--;
                    else if(current_char == ']') loop_depth++;
                }
                break;
            case '.':
                printf("%c", mem[mem_loc]);
                break;
            case ',':
                fflush(stdin);
                char in[] = "\0";
                fgets(in, sizeof(in), stdin);
                mem[mem_loc] = (char) in[0];
                break;
            default: break;
        }
        current_char = fgetc(fptr);
    }
    fclose(fptr);
    return 0;
}