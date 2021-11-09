#include "DataSet.h"

#pragma warning(disable: 4996)

#define rotateInt( A ) ((((A) & 0x000000FF) << (24)) | (((A) & 0x0000FF00) << (8)) | (((A) & 0x00FF0000) >> (8)) | (((A) & 0xFF000000) >> (24)))

#define TRUE_IMAGES_MAGIC_NUMBER 0x00000803
#define TRUE_LABELS_MAGIC_NUMBER 0x00000801

#define IMAGE_SIZE 28


// from http://yann.lecun.com/exdb/mnist/
/*
TRAINING SET LABEL FILE (train-labels-idx1-ubyte):
[offset] [type]          [value]          [description]
0000     32 bit integer  0x00000801(2049) magic number (MSB first)
0004     32 bit integer  60000            number of items
0008     unsigned byte   ??               label
0009     unsigned byte   ??               label
........
xxxx     unsigned byte   ??               label
The labels values are 0 to 9.

TRAINING SET IMAGE FILE (train-images-idx3-ubyte):
[offset] [type]          [value]          [description]
0000     32 bit integer  0x00000803(2051) magic number
0004     32 bit integer  60000            number of images
0008     32 bit integer  28               number of rows
0012     32 bit integer  28               number of columns
0016     unsigned byte   ??               pixel
0017     unsigned byte   ??               pixel
........
xxxx     unsigned byte   ??               pixel
Pixels are organized row-wise. Pixel values are 0 to 255. 0 means background (white), 255 means foreground (black).
*/


DataSet<IMAGE_SIZE * IMAGE_SIZE> LoadMNIST(std::string images, std::string labels)
{
  FILE *ImagesFile, *LabelsFile; // AS2 can say anything, but C-style files are much faster
  
  DataSet<IMAGE_SIZE * IMAGE_SIZE> result;

  if ((ImagesFile = fopen(images.c_str(), "rb")) == NULL)
    throw std::exception("Images file cannot be opened");

  if ((LabelsFile = fopen(labels.c_str(), "rb")) == NULL)
  {
    fclose(ImagesFile);
    throw std::exception("Labeles file cannot be opened");
  }

  // magic number
  int32_t magicNumberImages = 0, magicNumberLables = 0;

  fread(&magicNumberImages, sizeof(int32_t), 1, ImagesFile);
  fread(&magicNumberLables, sizeof(int32_t), 1, LabelsFile);

  magicNumberImages = rotateInt(magicNumberImages);
  magicNumberLables = rotateInt(magicNumberLables);

  if (magicNumberImages != TRUE_IMAGES_MAGIC_NUMBER || magicNumberLables != TRUE_LABELS_MAGIC_NUMBER)
  {
    fclose(ImagesFile);
    fclose(LabelsFile);
    throw std::exception("Magic number is not correct");
  }


  // amount
  int32_t imagesAmount = 0, labelsAmount = 0;

  fread(&imagesAmount, sizeof(int32_t), 1, ImagesFile);
  fread(&labelsAmount, sizeof(int32_t), 1, LabelsFile);

  imagesAmount = rotateInt(imagesAmount);
  labelsAmount = rotateInt(labelsAmount);

  if (imagesAmount != labelsAmount)
  {
    fclose(ImagesFile);
    fclose(LabelsFile);
    throw std::exception("labels amount are not equal images amount");
  }

  result.Reserve(imagesAmount);

  // Width + Height
  int32_t imagesW = 0, imagesH = 0;

  fread(&imagesW, sizeof(int32_t), 1, ImagesFile);
  fread(&imagesH, sizeof(int32_t), 1, ImagesFile);

  imagesW = rotateInt(imagesW);
  imagesH = rotateInt(imagesH);

  if (imagesW != IMAGE_SIZE || imagesH != IMAGE_SIZE)
  {
    fclose(ImagesFile);
    fclose(LabelsFile);
    throw std::exception("incorrect image size");
  }

  // Lets load everything
  for (int sample = 0; sample < imagesAmount; sample++)
  {
    DataElement<IMAGE_SIZE * IMAGE_SIZE> &D = result.CreateNewAndReturnIt();
    unsigned char buf[IMAGE_SIZE * IMAGE_SIZE] = {0};

    fread(buf, sizeof(unsigned char), IMAGE_SIZE * IMAGE_SIZE, ImagesFile);
    for (int i = 0; i < IMAGE_SIZE * IMAGE_SIZE; i++)
      D.input.vect[i] = 1.0 * buf[i] / 255;

    fread(&D.target, sizeof(unsigned char), 1, LabelsFile);
  }

  fclose(ImagesFile);
  fclose(LabelsFile);

  return result;
}