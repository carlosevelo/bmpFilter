#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

#define BAD_NUMBER_ARGS 1
#define FSEEK_ERROR 2
#define FREAD_ERROR 3
#define MALLOC_ERROR 4
#define FWRITE_ERROR 5

/**
 * Parses the command line.
 *
 * argc:      the number of items on the command line (and length of the
 *            argv array) including the executable
 * argv:      the array of arguments as strings (char* array)
 * grayscale: the integer value is set to TRUE if grayscale output indicated
 *            outherwise FALSE for threshold output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *isGrayscale) {
  if (argc > 2) {
    printf("Usage: %s [-g]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }
  
  if (argc == 2 && strcmp(argv[1], "-g") == 0) {
    *isGrayscale = TRUE;
  } else {
    *isGrayscale = FALSE;
  }

  return stdin;
}

unsigned getFileSizeInBytes(FILE* stream) {
  unsigned fileSizeInBytes = 0;
  
  rewind(stream);
  if (fseek(stream, 0L, SEEK_END) != 0) {
    exit(FSEEK_ERROR);
  }
  fileSizeInBytes = ftell(stream);

  return fileSizeInBytes;
}

void getBmpFileAsBytes(unsigned char* ptr, unsigned fileSizeInBytes, FILE* stream) {
  rewind(stream);
  if (fread(ptr, fileSizeInBytes, 1, stream) != 1) {
#ifdef DEBUG
    printf("feof() = %x\n", feof(stream));
    printf("ferror() = %x\n", ferror(stream));
#endif
    exit(FREAD_ERROR);
  }
}

unsigned char getAverageIntensity(unsigned char blue, unsigned char green, unsigned char red) {
  //#9 Compute the average intensity of a pixel
  //A value of zero indicates that the color is absent, a value of 255 indicates the maximum intensity for that color for the given pixel.
  
  return 0;
}

void applyGrayscaleToPixel(unsigned char* pixel) {
  //#8 Write the new value of the pixel using getAverageIntensity(blue, green, red)
  
}

void applyThresholdToPixel(unsigned char* pixel) {
  //#7 Write the new value of the pixel using getAverageIntensity(blue, green, red)
  
}

void applyFilterToPixel(unsigned char* pixel, int isGrayscale) {
  //#6 Filter each pixel to update it in the pixel array
  //if isGrayscale then applyGrayscaleToPixel() else applyThresholdToPixel()
  
}

void applyFilterToRow(unsigned char* row, int width, int isGrayscale) {
  //#5 Iterate over each pixel in a row calling applyFilterToPixel(pixel, isGrayscale) on each
  int pixelSize = 3;
  unsigned char* currentPixel = row;

  for (size_t i = 0; i < width; i++) {
    applyFilterToPixel(currentPixel, isGrayscale);
    currentPixel += pixelSize;
  }
#ifdef DEBUG
  printf("width in bytes = %u\n", widthInBytes);
  printf("padding in bytes = %u\n", padding);
  printf("row size in bytes = %u\n", rowSizeInBytes);
#endif
}

void applyFilterToPixelArray(unsigned char* pixelArray, int width, int height, int isGrayscale) {
  int pixelSize = 3;
  int widthInBytes = width * pixelSize;
  int rowSizeInBytes = widthInBytes;
  int padding = 0;
  unsigned char* currentRow = pixelArray;

  //Calculates how much padding to add to each row so that it is a multiple of 4
  if (widthInBytes % 4 != 0) {
    padding = 4 - (widthInBytes % 4);
  }
  rowSizeInBytes = widthInBytes + padding;

  //Iterates through each row calling applyFilterToRow()
  for (size_t i = 0; i < height; i++) {
    applyFilterToRow(currentRow, width, isGrayscale);
    currentRow += rowSizeInBytes;
  }
}

void parseHeaderAndApplyFilter(unsigned char* bmpFileAsBytes, int isGrayscale) {
  int offsetFirstBytePixelArray = 0;
  int width = 0;
  int height = 0;
  unsigned char* pixelArray = NULL;

  //#1 Use header.png to set offsetFirstBytePixelArray
  offsetFirstBytePixelArray =  *(int *)(bmpFileAsBytes + 10);

  //#2 Use DIBHeader.png to set width and height
  width = *(int *)(bmpFileAsBytes + 14 + 4);
  height = *(int *)(bmpFileAsBytes + 14 + 8);

  //#3 Use offsetFirstBytePixelArray to set the pixelArray to the start of the pixel array
  pixelArray = bmpFileAsBytes + offsetFirstBytePixelArray;

#ifdef DEBUG
  printf("offsetFirstBytePixelArray = %u\n", offsetFirstBytePixelArray);
  printf("width = %u\n", width);
  printf("height = %u\n", height);
  printf("pixelArray = %p\n", pixelArray);
  printf("isGrayscale = %u\n", isGrayscale);
#endif

  applyFilterToPixelArray(pixelArray, width, height, isGrayscale);
}

int main(int argc, char **argv) {
  int grayscale = FALSE;
  unsigned fileSizeInBytes = 0;
  unsigned char* bmpFileAsBytes = NULL;
  FILE *stream = NULL;
  
  stream = parseCommandLine(argc, argv, &grayscale);
  fileSizeInBytes = getFileSizeInBytes(stream);

#ifdef DEBUG
  printf("fileSizeInBytes = %u\n", fileSizeInBytes);
#endif

  bmpFileAsBytes = (unsigned char *)malloc(fileSizeInBytes);
  if (bmpFileAsBytes == NULL) {
    exit(MALLOC_ERROR);
  }
  getBmpFileAsBytes(bmpFileAsBytes, fileSizeInBytes, stream);

  parseHeaderAndApplyFilter(bmpFileAsBytes, grayscale);

#ifndef DEBUG
  if (fwrite(bmpFileAsBytes, fileSizeInBytes, 1, stdout) != 1) {
    exit(FWRITE_ERROR);
  }
#endif

  free(bmpFileAsBytes);
  return 0;
}
