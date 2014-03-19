//
//  obfusc.
//
//  Created by Manu Martinez-Almeids on 19/03/14.
//
//  Simple tool to obfuscate texts in binary, so they can't be found by using tools such as "strings" or regex.
//  Anyway, more powerful reverse engineering tools could be used to discovered the secret message very easily.
//  Do not use as an encription method!!!

//  Reconstruct the string with this code\n"
//  char text[sizeof(k1)];
//  for(int i = 0; i < sizeof(k1); ++i)
//     text[i] = k1[i]^k2[i];
  
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
 
void randomBytes(char *buffer, size_t len) {
    FILE *file = fopen("/dev/random", "r");
    fread(buffer, len, 1, file);
    fclose(file);
}


void printArray(const char* name, char *array, size_t size, int perLine) {
    printf("    char %s[] = {", name);
    for(int i = 0; i < size; ++i) {
        if(i%perLine == perLine-1)
            printf("\n        ");

        printf("0x%02X, ", (unsigned char)array[i]);
    }
    
    printf("\b\b};\n");
}


void printHelp(void) {
    char help[] =
    "\n"
    "    // Reconstruct the string with this code\n"
    "    char text[sizeof(k1)];\n"
    "    for(int i = 0; i < sizeof(k1); ++i)\n"
    "        text[i] = k1[i]^k2[i];\n";
    
    puts(help);
}


void obfuscate(const char *original, char *k1, char *k2, size_t size) {
    // Generate keys
    randomBytes(k1, size);

    // XOR everything
    for(int i = 0; i < size; ++i)
        k2[i] = original[i] ^ k1[i];

}


int main(int argc, const char * argv[])
{
    // Check mandatory arguments
    if(argc <= 1) {
        fprintf(stderr, "Error: We need one argument\n");
        return -1;
    }

    // Check optional arguments
    int perLine = 10;
    if(argc == 3)
        perLine = atoi(argv[2]);

    // Get text to obfuscate
    const char *original = argv[1];
    size_t size = strlen(original)+1;

    // Allocate heap memory
    char *k1 = (char*)malloc(size);
    char *k2 = (char*)malloc(size);

    // Obfuscate original into k1 and k2
    obfuscate(original, k1, k2, size);

    // Print result
    printf("\n    // \"%s\"\n", original);
    printArray("k1", k1, size, perLine);
    printArray("k2", k2, size, perLine);
    printHelp();

    // Release memory
    free(k1);
    free(k2);
   
    return 0;
}
