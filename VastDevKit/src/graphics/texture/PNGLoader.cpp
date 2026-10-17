#include "PNGLoader.h"

#include "png.h"

/*
 * PNGLib interface
 */

png_bytep *row_pointers;
int rowbytes;

void info_callback(png_structp png_ptr, png_infop info)
{
	unsigned i;
	png_color_8p sig_bit;
	
	// Big endian to Little endian (not sure if needed) 
	//if (info->bit_depth == 16)
	//{
	//	png_set_swap(png_ptr);
	//}
	
	//if (png_get_sBIT(png_ptr, info, &sig_bit))
	//	png_set_shift(png_ptr, sig_bit);
	
	unsigned int w, h;
	png_byte channels;
	png_size_t rowbytes;

	png_read_update_info(png_ptr, info);

	w = png_get_image_width(png_ptr, info);
	h = png_get_image_height(png_ptr, info);
	channels = png_get_channels(png_ptr, info);
	rowbytes = png_get_rowbytes(png_ptr, info);
	
	// Allocate row pointer space.
	if (h > 0)
	{
		if (NULL == (row_pointers = (png_bytep *)calloc(1, sizeof(png_bytep) * h)))
		{
			hd::log::i("Malloc fail\n");
			return;
		}
		
		for (i = 0; i < h; ++i)
		{
			if (NULL == (row_pointers[i] = (png_byte *)calloc(1, sizeof(png_byte) * rowbytes)))
			{
				hd::log::i("Malloc fail\n");
				return;				
			}
			//memset(row_pointers[i], 0, sizeof(png_byte) * rowbytes);
		}
	}
	else
	{
		row_pointers = NULL;
	}
}


void row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass)
{
	png_progressive_combine_row(png_ptr, row_pointers[row_num], new_row);
}


void end_callback(png_structp png_ptr, png_infop info)
{
}


void LoadPNG( const char *filename, unsigned char **pic, unsigned short *width, unsigned short *height, unsigned short *bytes )
{
	png_structp png_ptr;
	
	png_byte header[8];
	png_byte *raw = NULL;
	png_infop png_info_ptr;
	png_infop png_end_info_ptr;
	
	void *error_ptr = nullptr;
	
	hd::File *pngFile;
	
	unsigned i;
	unsigned int w, h;
	png_byte channels;
	png_size_t rowbytes;
	int len;
	unsigned char *buffer;
	
	hd::log::i("[PNGLoader]: Attempting to open (%s) for reading\n", filename);
	
	row_pointers = NULL;
	
	pngFile = NULL;
	*pic = NULL;
	
	if (NULL == (pngFile = hd::FileSystem::OpenFile(filename)))
    {
		hd::log::i("[PNGLoader]: Could not open (%s) for reading. Couldn't find file\n", filename);
		goto PNGLOADFAILED;
    }
	
	// Check the png sig
	hd::FileSystem::ReadFile(header, 8, 1, pngFile);
	hd::FileSystem::FileSeek(pngFile, 0, SEEK_SET);
	if (0 != png_sig_cmp(header, 0, 8))
	{
		hd::log::i("[PNGLoader]: PNGLIB says file was not a png! %s\n", filename);
		goto PNGLOADFAILED;		
	}
	
	if (NULL == (png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr)))
	{
		hd::log::i("[PNGLoader]: PNGLIB could not create read struct. %s\n", filename);
		goto PNGLOADFAILED;	
	}
	
	if (NULL == (png_info_ptr = png_create_info_struct(png_ptr)))
	{
		hd::log::i("[PNGLoader]: PNGLIB could not create info struct. %s\n", filename);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		goto PNGLOADFAILED;	
	}
	
	if (NULL == (png_end_info_ptr = png_create_info_struct(png_ptr)))
	{
		hd::log::i("[PNGLoader]: PNGLIB could not create end info struct. %s\n", filename);
		png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
		goto PNGLOADFAILED;			
	}
	
	/*
	 * LOAD PNG FILE
	 */	
	raw = NULL;
	len = hd::FileSystem::GetFileSize(pngFile);
	if (NULL == (raw = (png_byte *)calloc(1, sizeof(png_byte) * len)))
	{
		png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
		goto PNGLOADFAILED;	
	}
	
	hd::FileSystem::ReadFile(raw, len, 1, pngFile);
	
	png_set_progressive_read_fn(png_ptr, NULL,
								info_callback, row_callback, end_callback);
	
	png_process_data(png_ptr, png_info_ptr, (png_bytep)raw, len);
	
	/*
	 * READ IMAGE DATA
	 */
	w = png_get_image_width(png_ptr, png_info_ptr);
	h = png_get_image_height(png_ptr, png_info_ptr);
	channels = png_get_channels(png_ptr, png_info_ptr);
	rowbytes = png_get_rowbytes(png_ptr, png_info_ptr);
	
	if (NULL == (buffer = (unsigned char *)calloc(1, h * rowbytes)))
	{
		hd::log::i("[PNGLoader]: MALLOC FAIL loading file %s\n", filename);
		goto PNGLOADFAILED;
	} 
	
	hdAssert(row_pointers != NULL);
	
	// Copy and clean the row pointers to local buffer.
	for (i = 0; i < h; ++i)
	{
		memcpy(buffer + (i * rowbytes), row_pointers[i], rowbytes);
		free(row_pointers[i]);
	}
	free(row_pointers);
	free(raw);
	row_pointers = NULL;
	
	/*
	 * CLEAN UP
	 */
	png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
	
	hd::FileSystem::CloseFile(pngFile);
	
	*pic = buffer;
	*width = w;
	*height = h;
	*bytes = channels;
	
	return;	

	
PNGLOADFAILED:
	if (raw)
		free(raw);
	
	if (pngFile)
		hd::FileSystem::CloseFile(pngFile);
	
	*pic = NULL;
	*width = 0;
	*height = 0;
	*bytes = 0;
}



bool WritePNG(const char *filename, uint8_t *pixels, uint32_t w, uint32_t h) {
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
        return false;

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, &info);
        return false;
    }

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        png_destroy_write_struct(&png, &info);
        return false;
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, w, h, 8 /* depth */, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_colorp palette = (png_colorp)png_malloc(png, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
    if (!palette) {
        fclose(fp);
        png_destroy_write_struct(&png, &info);
        return false;
    }
    png_set_PLTE(png, info, palette, PNG_MAX_PALETTE_LENGTH);
    png_write_info(png, info);
    png_set_packing(png);

    png_bytepp rows = (png_bytepp)png_malloc(png, h * sizeof(png_bytep));
    for (int i = 0; i < h; ++i)
        rows[i] = (png_bytep)(pixels + (h - i - 1) * w * 4);;

    png_write_image(png, rows);
    png_write_end(png, info);
    png_free(png, palette);
    png_destroy_write_struct(&png, &info);

    fclose(fp);
    delete[] rows;
    return true;
}
