#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tools.h"

#define STBI_FAILURE_USERMSG
#define STBIW_FAILURE_USERMSG

#define STBI_ASSERT(x) ASSERT((x))
#define STBIW_ASSERT(x) ASSERT((x))

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef enum {
  Ext_ERR,
  Ext_PNG,
  Ext_BMP,
  Ext_TGA,
  Ext_JPG,
  Ext_HDR
} File_Ext;

File_Ext get_file_extension(char* name);
unsigned char *tograyscale(int w, int h, int comp, unsigned char* in);
int write_output(char *name, File_Ext ext, int w, int h, int comp, unsigned char* data);

int main(int argc, char **argv)
{
  COND_EXIT(argc != 3, "Usage: %s <input> <output.format>\n", *argv);

  // parse command args
  char* input = argv[1];
  char* output = argv[2];
  File_Ext output_ext = get_file_extension(output);

  COND_EXIT(!output_ext, "Unsoported output file format '%s'\n", output);
  
  int w, h, comp;
  unsigned char *data = stbi_load(input, &w, &h, &comp, 0);
  
  COND_EXIT(!data, "Error: Unable to load '%s'. Reason: %s \n", input, stbi_failure_reason());

  if (comp > 1) {
	unsigned char *out = tograyscale(w, h, comp, data);
	COND_EXIT(!out, "Error: while converting to grayscale\n");
	COND_EXIT(!write_output(output, output_ext, w, h, 1, out), 
			  "Error: Unable to write to '%s'\n", output);
	free(out);
  }

  stbi_image_free(data);
}


File_Ext get_file_extension(char* name)
{
  int len = strlen(name);
  char *end = &name[len - 1];

  while (*end != '.')
	end--;

  end++;
  if (strcmp(end, "png") == 0) return Ext_PNG;
  if (strcmp(end, "bmp") == 0) return Ext_BMP;
  if (strcmp(end, "tga") == 0) return Ext_TGA;
  if (strcmp(end, "jpg") == 0) return Ext_JPG;
  /* if (strcmp(end, "hdr") == 0) return Ext_HDR; */
  if (strcmp(end, "hdr") == 0) TODOF("Implement HDR output\n");

  return Ext_ERR;
}

unsigned char *tograyscale(int w, int h, int comp, unsigned char* in)
{
  ASSERTF(comp > 1 && comp <= 4, "Invalid component amount: %d\n", comp);
  int size = w * h;
  unsigned char* out = (unsigned char*)calloc(sizeof(unsigned char), size);

  if (comp == 2) { // gray, aplha
	for (int i = 0; i < size; i++) 
	  out[i] = in[i * 2];
	
  } else if (comp == 3) { // rgb
	int q;
	for (int i = 0; i < size; i++) {
	  q = i * 3;
	  out[i] = (in[q] + in[q + 1] + in[q + 2]) / 3.0f; // mean
	}
	
  } else if (comp == 4) { // rgba
	int q;
	for (int i = 0; i < size; i++) {
	  q = i * 4;
	  out[i] = (in[q] + in[q + 1] + in[q + 2]) * in[q + 3] / (3.0f * 0xFF); // mean times alpha
	}
  }
	
  return out;
}

int write_output(char *name, File_Ext ext, int w, int h, int comp, unsigned char* data)
{
  switch (ext) {
  case Ext_PNG: return stbi_write_png(name, w, h, comp, data, w * comp);
  case Ext_BMP: return stbi_write_bmp(name, w, h, comp, data);
  case Ext_TGA: return stbi_write_tga(name, w, h, comp, data);
  case Ext_JPG: return stbi_write_jpg(name, w, h, comp, data, 50);
	/* case Ext_HDR: return stbi_write_hdr(name, w, h, comp, data); */
  case Ext_HDR: TODOF("Implement HDR output\n");
  default:      return false;
  }
}
