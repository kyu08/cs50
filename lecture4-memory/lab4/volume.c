#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
// Number of bytes to read once
const int PARTIAL_SIZE = 44;

int main(int argc, char *argv[]) {
  // Check command-line arguments
  if (argc != 4) {
    printf("Usage: ./volume input.wav output.wav factor\n");
    return 1;
  }

  // Open files and determine scaling factor
  FILE *input = fopen(argv[1], "r");
  if (input == NULL) {
    printf("Could not open file.\n");
    return 1;
  }

  FILE *output = fopen(argv[2], "w");
  if (output == NULL) {
    printf("Could not open file.\n");
    fclose(input);
    return 1;
  }

  float factor = atof(argv[3]);

  // Copy header from input file to output file
  uint8_t header[HEADER_SIZE];
  for (int i = 0; i < HEADER_SIZE; i++) {
    unsigned long read_size = fread(&header[i], sizeof(header[i]), 1, input);
    if (read_size != 1) {
      printf("read failed\n");
      return 1;
    }
  }

  for (int i = 0; i < HEADER_SIZE; i++) {
    unsigned long wrote_size = fwrite(&header[i], sizeof(header[i]), 1, output);
    if (wrote_size != 1) {
      printf("write failed\n");
      return 1;
    }
  }

  // Read samples from input file and write updated data to output file
  for (int i = 0; 1; i++) {
    // 2byteずつ読み出す
    int16_t partial;
    unsigned long read_size = fread(&partial, sizeof(int16_t), 1, input);
    partial *= factor;

    if (read_size != 1) {  // EOF or failed to read
      break;
    }

    if (fwrite(&partial, sizeof(int16_t), 1, output) != 1) {
      printf("write failed\n");
      return 1;
    }
  }

  // Close files
  fclose(input);
  fclose(output);
}
