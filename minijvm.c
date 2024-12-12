#include <stdio.h>
#include <stdlib.h>
#include "minijvm.h"
#include "stack.h"
#include <string.h>
#include <unistd.h>

void usage(void);
void initialize_mini_jvm(void);
void run_program(const char* filename);
void free_memory(void);
/*
 * Reads the bytes form the specified file into an array
 * Filename - name of the .mclass file from which to read
 * Return - A pointer to the array of bytecode
 */

char* jvm_read(const char* filename) {

    char target_filename[256];

    // Check if filename already has .mclass at the end (has some issues with this so kept it in)
    if (strstr(filename, ".mclass") != NULL)
    {
        strcpy(target_filename, filename); // If yes, copy the filename as is
    }

    //If it does not have .mclass on the end, add it
    else
    {
        sprintf(target_filename, "%s.mclass", filename);
    }
   printf("made it");
        FILE* file = fopen(target_filename, "r"); // open the file
printf("made it out");
        if (file != NULL) {
	   
            // Dynamically allocate an array of bytes
            char* bytecode = malloc(MAX_CLASS_SIZE * sizeof(char)*2);
	   
            if (bytecode == NULL) {
                printf("Error: Memory allocation failed.\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }

            // Read it byte-by-byte into the array until EOF is reached
            size_t i = 0;
            while (fread(&bytecode[i], sizeof(char), 1, file) == 1 && i < MAX_CLASS_SIZE) {
               i++;
            }

            // Close the file
            fclose(file);

            return bytecode;
        } else {
            printf("Error: File could not be opened.\n");
            exit(EXIT_FAILURE);
    }
}
void jvm_run(minijvm* jvm)
{
    while (1) //run all the time
    {
        unsigned char bytecode = *((*jvm).pc++);
        switch (bytecode)
        {

            case INST_ICONST0: //push 0 onto the operand stack
                stack_push((*jvm).operands, 0);
                break;

            case INST_POP: //pop the top item off of the stack
                item_type temp;
                stack_pop((*jvm).operands, &temp);

                break;

            case INST_DUP: //duplicate top item
                item_type top;
                stack_peek((*jvm).operands, &top);
                stack_push((*jvm).operands, top);
                break;

            case INST_IADD: //pops and adds together the top two items
                item_type a, b;
                stack_pop((*jvm).operands, &a);
                stack_pop((*jvm).operands, &b);
                stack_push((*jvm).operands, a + b);
                break;

            case INST_ISUB: //pops and subtracts the top two items
                stack_pop((*jvm).operands, &a);
                stack_pop((*jvm).operands, &b);
                stack_push((*jvm).operands, a - b);
		break;

            case INST_IMUL: //pop and multiply the top two items
                stack_pop((*jvm).operands, &a);
                stack_pop((*jvm).operands, &b);
                stack_push((*jvm).operands, a * b);
		break;

            case INST_IDIV: //pop and integer divide the top two
                stack_pop((*jvm).operands, &a);
                stack_pop((*jvm).operands, &b);
		if (b == 0)
                {
                    printf("Error: Division by zero\n"); //divide by 0
                    exit(EXIT_FAILURE);
                }
                stack_push((*jvm).operands, a/ b);
                break;

            case INST_IREM: //pop the top two and compute the remainder
                stack_pop((*jvm).operands, &a);
                stack_pop((*jvm).operands, &b);
		if (b == 0)
                {
                    printf("Error: division by zero\n"); //divide by 0
                    exit(EXIT_FAILURE);
                }
                stack_push((*jvm).operands, a % b);
                break;

            case INST_ISHR: //pop top two items and shift right
                stack_pop((*jvm).operands, &a);
                stack_pop((*jvm).operands, &b);
                stack_push((*jvm).operands, b >> a);
		break;

            case INST_RETURN: //ends the program
                (*jvm).return_value = 0;
		stack_free((*jvm).operands);
		return;


            case INST_PRINT: //prints the top item of the stack
                stack_peek((*jvm).operands, &top);
                printf("Top item: %d\n", top);
                break;

            case INST_BIPUSH: //pushes signed byte onto stack
                char n = *((*jvm).pc++);
                stack_push((*jvm).operands, n);
		break;

           case INST_ILOAD: //push local variable at index n onto stack
                n = *((*jvm).pc++);
                stack_push((*jvm).operands, (*jvm).locals[n]);
                break;

           case INST_ISTORE: //Pop top of stack and storein loval var at index n
                n = *((*jvm).pc++);
                stack_pop((*jvm).operands, &a);
                (*jvm).locals[n] = a;
                break;

           case INST_IINC: //increment local var at index n by d
                n = *((*jvm).pc++);
                char d = *((*jvm).pc++);
                (*jvm).locals[n] += d;
                break;

           case INST_IFEQ: // bransh to offset if 0
                short offset = *((*jvm).pc) << 8 | *((*jvm).pc + 1);
                (*jvm).pc += 2;
                stack_pop((*jvm).operands, &a);
                if (a == 0) {
                    (*jvm).pc += offset;
                }
		break;

           case INST_GOTO: //branch to offset
                offset = ((*jvm).pc[0] << 8) | ((*jvm).pc[1]);
                (*jvm).pc += offset -2;
                break;

            default:
                printf("Unknown bytecode\n");
                exit(EXIT_FAILURE);
		(*jvm).return_value = 1;
		stack_free((*jvm).operands);
		return;
        }}


}

/*
 * Initializes a new minijvm struct
 * filename - name of the .mclass file from which to read from
 * return -a pointer to the initialized minijvm
 */
minijvm* jvm_init(const char* filename)
{
    minijvm* jvm = malloc(sizeof(minijvm));//allocates memory
    if (jvm == NULL)
    {
        printf("Memory Allocation Error\n");
        exit(EXIT_FAILURE);
    }
    printf("init: %s\n",filename);
    (*jvm).bytecode = jvm_read(filename);
    (*jvm).pc = 0;
    (*jvm).operands = stack_create();

    printf("fuck");
    return jvm;


      
}

/*
 * Frees all memory allocated for the minijvm
 * jvm - the minijvm struct to deallocate
 */
void jvm_free(minijvm* jvm) 
{
    if (jvm != NULL) 
    {
        stack_free((*jvm).operands);
	free(jvm);
}

}

void jvm_usage()
{
    fprintf(stderr, "Usage: mjvm FILENAME\n");
    exit(EXIT_FAILURE);
}
    

/*
 * The main entry point to minijvm
 * argc - number of parameters passed to the program
 * argv - Array of parameters passed to the program
 * returns 0 on successful termination of .mclass file 1 if fail
 */
int main(int argc, char** argv) {
    // Check number of arguments
    
    printf("%d\n", argc);
    if (argc != 2) {
        jvm_usage();
        return 1;
    }

    // Initialize MiniJVM
    printf("main: %s\n",argv[1]);
    minijvm* jvm = jvm_init(argv[1]);

    printf("before run");
    // Run the program
    jvm_run(jvm);
    printf("past run");
    // Free dynamically allocated memory
    jvm_free(jvm);
    printf("DEMOCRACY");
    return 0;
}
