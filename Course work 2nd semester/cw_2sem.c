#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <png.h>
#include <math.h>
#define SZ_OPTSTR 44
#define OPT_INFO 0
#define NUM_COORDS 2
#define SZ_OUTPUT 8
#define SZ_INPUT 8
#define NUM_FLAGS 17
#define LEN_HEADER 8
#define NUM_RGB 3
#define NUM_COLORS_RGB 0x1000000 // 256^3
#define IMG_READ 1
#define IMG_NOT_READ 0
#define CORRECT_COLOR 1
#define WRONG_COLOR 0
#define FUNC_11 0b11111111000000000000000000
#define FUNC_12 0b11111100000000000000000000 
#define FUNC_2 0b10100000111000000000000000 
#define FUNC_3 0b10000100000100000000000000
#define FUNC_4 0b10000000000011100000000000 
#define FUNC_5 0b10100100010000010000000000
#define FUNC_6 0b10000000000000001110000000
#define FUNC_7 0b10000001000000000001110000
#define FUNC_8 0b10000100000000000000011100
#define FUNC_9 0b10100000010000000000000010

enum error_types{
	ERR_OK = 0,
	ERR_MEM = 40,
	ERR_OPT,
	ERR_COLOR,
	ERR_SIGNATURE,
	ERR_COORDS,
	ERR_IMG_TYPE,
	ERR_INPUT_NAME,
	ERR_OUTPUT_NAME,
	ERR_EXCH_TYPE,
	ERR_IMG_READ
};

struct Png{ 
	int width, height; 
	png_byte color_type; 
 	png_byte bit_depth; 
	png_structp png_ptr; 
	png_infop info_ptr; 
 	int number_of_passes; 
	png_bytep* row_pointers; 
}; 

struct RGB{
	int R;
	int G;
	int B;
};

struct point{
    int x;
    int y;
};

struct contrast_data{
	double alpha;
	int beta;
};

struct square_data{
    struct point* left_up;
    int side_size;
    int thickness;
    struct RGB* color;
    bool fill;
    struct RGB* fill_color;
};

struct exchange_data{
    struct point* left_up;
    struct point* right_down;
    char* type;
};

struct change_color{
    struct RGB* new_color;
};

struct o_rect_data{
	struct point* left_up;
    struct point* right_down;
	struct RGB* color;
};

struct shift_data{
	int step;
	char* axis;
};

struct rhombus_data{
	struct point* upper_vertex;
	int size;
	struct RGB* fill_color;
};

struct circle_data{
	int size;
	struct RGB* color;
	struct RGB* circle_color;
};

struct mirror_data{
	struct point* left_up;
    struct point* right_down;
};

struct options_flags{
	unsigned iname_flag:1;

    unsigned sqr_flag:1;
    unsigned lft_up_flag:1;
    unsigned sd_sz_flag:1;
    unsigned thkns_flag:1;
    unsigned clr_flag:1;
    unsigned fill_flag:1;
    unsigned fill_clr_flag:1;
    unsigned exchg_flag:1;
    unsigned rt_dwn_flag:1;
    unsigned exchg_t_flag:1;
    unsigned fq_clr_flag:1;
	
	unsigned contr_flag:1;
	unsigned a_flag:1;
	unsigned b_flag:1;

	unsigned o_rect_flag:1;

	unsigned shift_flag:1;
	unsigned step_flag:1;
	unsigned axis_flag:1;

	unsigned rhombus_flag:1;
	unsigned up_vrtx_flag:1;
	unsigned size_flag:1;

	unsigned cp_flag:1;
	unsigned cir_clr_flag:1;

	unsigned diag_mir_flag:1;

	unsigned err:1;
};

union get_flags{
	struct options_flags inp_flags;
	struct{
		unsigned iname_bit:1;
		unsigned square_bit:1;
		unsigned lft_up_bit:1;
		unsigned sd_sz_bit:1;
		unsigned thkns_bit:1;
		unsigned clr_bit:1;
		unsigned fill_bit:1;
		unsigned fill_clr_bit:1;
		unsigned exchg_bit:1;
		unsigned rt_dwn_bit:1;
		unsigned exchg_t_bit:1;
		unsigned fq_clr_bit:1;
		unsigned contr_bit:1;
		unsigned a_bit:1;
		unsigned b_bit:1;
		unsigned o_rect_bit:1;
		unsigned shift_bit:1;
		unsigned step_bit:1;
		unsigned axis_bit:1;
		unsigned rhombus_bit:1;
	    unsigned up_vrtx_bit:1;
        unsigned size_bit:1;
		unsigned cp_bit:1;
        unsigned cir_clr_bit:1;
		unsigned diag_mir_bit:1;
		unsigned err_bit:1;
	}bits;
};

int check_input_data(struct options_flags flags){
	int checker = 0;

	int expected_func_11 = FUNC_11;
	int expected_func_12 = FUNC_12;
	int expected_func_2 = FUNC_2;
	int expected_func_3 = FUNC_3;
	int expected_func_4 = FUNC_4;
	int expected_func_5 = FUNC_5;
	int expected_func_6 = FUNC_6;
	int expected_func_7 = FUNC_7;
	int expected_func_8 = FUNC_8;
	int expected_func_9 = FUNC_9;

	union get_flags my_flags;
	my_flags.inp_flags = flags;

	unsigned int* bits = (unsigned int*)&my_flags.bits;
	int received_flags = 0;

	for (int i=0; i<26; i++)
		received_flags |= ((*bits >> i) & 1) << (25 - i);

	if (((received_flags & expected_func_11) == expected_func_11) || ((received_flags & expected_func_12) == expected_func_12))
	    checker = 1;
	else if ((received_flags & expected_func_2) == expected_func_2)
		checker = 2;
	else if ((received_flags & expected_func_3) == expected_func_3)
    	checker = 3;
	else if ((received_flags & expected_func_4) == expected_func_4)
		checker = 4;
	else if ((received_flags & expected_func_5) == expected_func_5)
        checker = 5;
	else if ((received_flags & expected_func_6) == expected_func_6)
        checker = 6;
	else if ((received_flags & expected_func_7) == expected_func_7)
        checker = 7;
	else if ((received_flags & expected_func_8) == expected_func_8)
        checker = 8;
	else if ((received_flags & expected_func_9) == expected_func_9)
        checker = 9;

	return checker;
}

int check_color(int R, int G, int B){
	return ((R>=0 && R<256) && (G>=0 && G<256) && (B>=0 && B<256)) ? CORRECT_COLOR : WRONG_COLOR;
}

void print_help(){
	printf("\n\tОбщие опции:\n");
	printf("--input -i имя входного изображения\n");
	printf("--output -o переопределить имя выходного изображения (по умолчанию out.png)\n");
	printf("--info информация об изображении\n");
	printf("--help -h справка\n\n");
	printf("\tФлаги функций:\n");
	printf("--square -S рисование квадрата\n");
	printf("--left_up -l координаты левого верхнего угла\n");
	printf("--side_size -s размер стороны\n");
	printf("--thickness -t толщина линий\n");
	printf("--color -C цвет линий / цвет для изменения текущего\n");
	printf("--fill -f заливка\n");
	printf("--fill_color -c цвет заливки\n");
	printf("--exchange -e меняет местами 4 куска заданной области\n");
	printf("--left_up -l координаты левого верхнего угла области\n");
	printf("--right_down -r координаты правого нижнего угла области\n");
	printf("--exchange_type -T способ обмена частей\n");
	printf("--freq_color -F поиск наиболее часто встречаемого цвета и замена его на другой\n\n");
}

void print_png_info(struct Png* img){
	printf("Информация об изображении: \n\n");
	printf("Ширина: %d пикселей\n", img->width);
	printf("Высота: %d пикселей\n", img->height);
	printf("Цветовая модель: ");

	switch(img->color_type){
		case PNG_COLOR_TYPE_GRAY:
			printf("grayscale (1 канал)\n");
			break;
		case PNG_COLOR_TYPE_RGB:
			printf("RGB (3 канала)\n");
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			printf("grayscale + alpha (2 канала)\n");
			break;
		case PNG_COLOR_TYPE_RGBA:
			printf("RGBA (4 канала)\n");
			break;
		default:
			printf("неизвестный тип\n");
			break;
	}
	
	printf("Глубина цвета: %d бит/пиксель\n", img->bit_depth);
	printf("Количество проходов для полной загрузки: %d\n", img->number_of_passes);
}

int read_image(struct Png* image, char* filename, enum error_types* err){
	FILE* file = fopen(filename, "rb");
	if (file){
		png_byte header[LEN_HEADER] = {0};
		fread(header, 1, 8, file);

		if (!png_sig_cmp(header, 0, 8)){
			image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			image->info_ptr = png_create_info_struct(image->png_ptr);
			
			if (image->png_ptr && image->info_ptr){
				png_init_io(image->png_ptr, file);
				png_set_sig_bytes(image->png_ptr, LEN_HEADER);
				png_read_info(image->png_ptr, image->info_ptr);
				image->width = png_get_image_width(image->png_ptr, image->info_ptr);
				image->height = png_get_image_height(image->png_ptr, image->info_ptr);
				image->color_type = png_get_color_type(image->png_ptr,image->info_ptr);
				image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);
				image->number_of_passes = png_set_interlace_handling(image->png_ptr);
				png_read_update_info(image->png_ptr, image->info_ptr);

				image->row_pointers = (png_bytep*)malloc(sizeof(png_bytep)*image->height);
				if (image->row_pointers){
					for (int y = 0; y < image->height; y++){
						image->row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));
						if (image->row_pointers[y] == NULL)
							*err = ERR_MEM;
					}
					if (*err == ERR_OK)
						png_read_image(image->png_ptr, image->row_pointers);
					else *err = ERR_IMG_READ;

				} else {
					printf("Ошибка памяти\n");
					*err = ERR_MEM;
				}

			} else {
				printf("Ошибка выделения памяти для структуры на чтение PNG\n");
				*err = ERR_MEM;
			}

			png_destroy_read_struct(&(image->png_ptr), &(image->info_ptr), NULL);
			png_destroy_info_struct(image->png_ptr, &(image->info_ptr));

		} else {
			printf("Входное изображение не соответсвует PNG формату\n");
			*err = ERR_SIGNATURE;
		}
		
		fclose(file);

	} else {
		printf("Входной файл отсутствует в текущей директории\n");
		*err = ERR_MEM;	
	}

	return (*err == ERR_OK) ? IMG_READ : IMG_NOT_READ;
}

void write_image(struct Png* image, char* filename, enum error_types* err){
	FILE* file = fopen(filename, "wb");
	if (file){
		image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		image->info_ptr = png_create_info_struct(image->png_ptr);

		if (image->png_ptr && image->info_ptr){
			png_init_io(image->png_ptr, file);
			png_set_IHDR(image->png_ptr, 
			image->info_ptr, 
			image->width, 
			image->height, 
			image->bit_depth, 
			image->color_type, 
			PNG_INTERLACE_NONE, 
			PNG_COMPRESSION_TYPE_BASE, 
			PNG_FILTER_TYPE_BASE);
     		png_write_info(image->png_ptr, image->info_ptr);
			png_write_image(image->png_ptr, image->row_pointers);
			png_write_end(image->png_ptr, NULL);

			for (int y = 0; y < image->height; y++)
				free(image->row_pointers[y]);
      			free(image->row_pointers);
      			fclose(file);
		} else {
			printf("Ошибка выделения памяти для структуры на запись PNG\n");
			*err = ERR_MEM;
		}
		
		png_destroy_info_struct(image->png_ptr, &(image->info_ptr));
		png_destroy_write_struct(&(image->png_ptr), &(image->info_ptr));

	} else {
		printf("Ошибка выходного файла\n");
		*err = ERR_MEM;
	}
}

void draw_line(int x, int y, int side, int thick, struct RGB* color, struct Png* image){
	for (int i=y; i<y+thick; i++){
                png_byte* row = image->row_pointers[i];
                for (int j=x; j<x+side; j++){
                    png_byte* pixel = &(row[j*3]);
			
			if ((j >= 0 && j < image->width) && (i >= 0 && i < image->height)){  
                pixel[0] = color->R;
                pixel[1] = color->G;
                pixel[2] = color->B;
			}
        }
	}	
}

void fill_square(int x, int y, int side_x, int side_y, struct RGB* fill_color, struct Png* image){
	for (int i=y; i<y+side_y; i++){
		png_byte* row = image->row_pointers[i];
        for (int j=x; j<x+side_x; j++){
            png_byte* pixel = &(row[j*3]);
        		
			if ((j >= 0 && j < image->width) && (i >= 0 && i < image->height)){
	            pixel[0] = fill_color->R;
        	    pixel[1] = fill_color->G;
                pixel[2] = fill_color->B;
			}
        }
	}
}

void draw_square(struct square_data* my_square, struct Png* image){
	int x = my_square->left_up->x;
	int y = my_square->left_up->y;
	int thick = my_square->thickness;
	int side = my_square->side_size;
	
	draw_line(x-(thick/2), y-(thick/2), side+thick, thick, my_square->color, image);
	draw_line(x-(thick/2), y-(thick/2)+side, side+thick, thick, my_square->color, image);
	draw_line(x-(thick/2), y-(thick/2), thick, side+thick, my_square->color, image);
	draw_line(x-(thick/2)+side, y-(thick/2), thick, side+thick, my_square->color, image);
	
	if (my_square->fill)
		fill_square(x+(thick/2)+(thick%2), y+(thick/2)+(thick%2), side-thick, side-thick, my_square->fill_color, image);	
}

void get_piece(int len_x, int len_y, int img_x, int img_y, png_bytep* piece, struct Png* image, enum error_types* err){
	for (int y=0; y<len_y; y++){
		piece[y] = (png_byte*)calloc(len_x*3, sizeof(png_byte));
		
		if (piece[y]){
			for (int x=0; x<len_x; x++){
				png_byte* dst_pix = &(piece[y][x*3]);
				png_byte* src_pix = &(image->row_pointers[img_y+y][(img_x+x)*3]);

				if ((img_x+x >= 0 && img_x+x < image->width) && (img_y+y >= 0 && img_y+y < image->height)){
					dst_pix[0] = src_pix[0];
					dst_pix[1] = src_pix[1];
					dst_pix[2] = src_pix[2];
				}
			}
		
		} else {
			printf("Ошибка памяти\n");
			*err = ERR_MEM;
		}
	}
}

void paste_piece(int x, int y, int len_x, int len_y, png_bytep* piece, struct Png* image){
	for (int i=0; i<len_y; i++){
		for (int j=0; j<len_x; j++){
			png_byte* dst_pix = &(image->row_pointers[y+i][(x+j)*3]);
			png_byte* src_pix = &(piece[i][j*3]);
			
			if ((x+j >= 0 && x+j < image->width) && (y+i >= 0 && y+i < image->height)){
				dst_pix[0] = src_pix[0];
                dst_pix[1] = src_pix[1];
                dst_pix[2] = src_pix[2];
			}	
		}
	}
}

void exchange_places(struct exchange_data* space, struct Png* image, enum error_types* err){
	int lu_x = space->left_up->x;
	int lu_y = space->left_up->y;
	int rd_x = space->right_down->x;
	int rd_y = space->right_down->y;
	
	if (lu_y>rd_y){
		lu_y = space->right_down->y;
		rd_y = space->left_up->y;
	}
	
	if (lu_x>rd_x){
		lu_x = space->right_down->x;
		rd_x = space->left_up->x;
	}

	int width_rec = rd_x-lu_x;
	int height_rec = rd_y-lu_y;

	png_bytep* left_up_piece = (png_bytep*)calloc(height_rec/2, sizeof(png_bytep));
    png_bytep* right_up_piece = (png_bytep*)calloc(height_rec/2, sizeof(png_bytep));
	png_bytep* left_down_piece = (png_bytep*)calloc(height_rec/2, sizeof(png_bytep));
	png_bytep* right_down_piece = (png_bytep*)calloc(height_rec/2, sizeof(png_bytep));
	
	if (left_up_piece && right_up_piece && left_down_piece && right_down_piece){
		get_piece(width_rec/2, height_rec/2, lu_x, lu_y, left_up_piece, image, err);
        get_piece(width_rec/2, height_rec/2, lu_x + width_rec/2, lu_y, right_up_piece, image, err);
		get_piece(width_rec/2, height_rec/2, lu_x, lu_y + height_rec/2, left_down_piece, image, err);	
		get_piece(width_rec/2, height_rec/2, lu_x + width_rec/2, lu_y + height_rec/2, right_down_piece, image, err);

		if (!strcmp(space->type, "clockwise")){
			paste_piece(lu_x, lu_y, width_rec/2, height_rec/2, left_down_piece, image);
			paste_piece(lu_x + width_rec/2, lu_y, width_rec/2, height_rec/2, left_up_piece, image);
			paste_piece(lu_x, lu_y + height_rec/2, width_rec/2, height_rec/2, right_down_piece, image);
			paste_piece(lu_x + width_rec/2, lu_y + height_rec/2, width_rec/2, height_rec/2, right_up_piece, image);
	
		} else if (!strcmp(space->type, "counterclockwise")){
			paste_piece(lu_x, lu_y, width_rec/2, height_rec/2, right_up_piece, image);
            paste_piece(lu_x + width_rec/2, lu_y, width_rec/2, height_rec/2, right_down_piece, image);
	        paste_piece(lu_x, lu_y + height_rec/2, width_rec/2, height_rec/2, left_up_piece, image);
        	paste_piece(lu_x + width_rec/2, lu_y + height_rec/2, width_rec/2, height_rec/2, left_down_piece, image);

		} else if (!strcmp(space->type, "diagonals")){
			paste_piece(lu_x, lu_y, width_rec/2, height_rec/2, right_down_piece, image);
            paste_piece(lu_x + width_rec/2, lu_y, width_rec/2, height_rec/2, left_down_piece, image);
	        paste_piece(lu_x, lu_y + height_rec/2, width_rec/2, height_rec/2, right_up_piece, image);
        	paste_piece(lu_x + width_rec/2, lu_y + height_rec/2, width_rec/2, height_rec/2, left_up_piece, image);

		}

		for (int i=0; i<height_rec/2; i++){
            free(left_up_piece[i]);
            free(right_up_piece[i]);
            free(left_down_piece[i]);
            free(right_down_piece[i]);
        }

	} else *err = ERR_MEM;  
	
	if (left_up_piece)
		free(left_up_piece);
	if (right_up_piece)
		free(right_up_piece);
	if (left_down_piece)
		free(left_down_piece);
	if(right_down_piece)
		free(right_down_piece);	
}

void replace_freq_color(struct change_color* my_colors, struct Png* image, enum error_types* err){
	if (image->color_type == PNG_COLOR_TYPE_RGB){
		struct RGB* freq_color = (struct RGB*)calloc(1, sizeof(struct RGB));
	
		int* clr_cnt = (int*)calloc(NUM_COLORS_RGB, sizeof(int));
		int max_cnt = -1;
		
		if (clr_cnt && freq_color){
			for (int y=0; y < image->height; y++){
				png_byte* row = image->row_pointers[y];
				for (int x=0; x<image->width; x++){
					png_byte* pixel = &(row[x*3]);
			
					int idx = (pixel[0] << 16) | (pixel[1] << 8) | (pixel[2]);
					clr_cnt[idx]++;

					if (clr_cnt[idx] > max_cnt){
						max_cnt = clr_cnt[idx];
						freq_color->R = pixel[0];
						freq_color->G = pixel[1];
						freq_color->B = pixel[2];
					}
				}
			}
		} else {
			printf("Ошибка памяти\n");
			*err = ERR_MEM;
		}

		for (int y=0; y < image->height; y++){
			png_byte* line = image->row_pointers[y];
            for (int x=0; x < image->width; x++){
				png_byte* cur_pixel = &(line[x*3]);

				if (cur_pixel[0] == freq_color->R && cur_pixel[1] == freq_color->G && cur_pixel[2] == freq_color->B){
					cur_pixel[0] = my_colors->new_color->R;
					cur_pixel[1] = my_colors->new_color->G;
					cur_pixel[2] = my_colors->new_color->B;
				}
			}
		}
		
		free(freq_color);
		free(clr_cnt);
	} else {
		printf("Формат изображения не RGB\n");
		*err = ERR_IMG_TYPE;
	}
}

void make_contrast(struct contrast_data* c_data, struct Png* image){
	for (int y=0; y<image->height; y++){
		png_byte* row = image->row_pointers[y];
		for (int x=0; x<image->width; x++){
			png_byte* pix = &(row[x*3]);

			int red = (c_data->alpha * pix[0] + c_data->beta)/1;
			int green = (c_data->alpha * pix[1] + c_data->beta)/1;
			int blue = (c_data->alpha * pix[2] + c_data->beta)/1;

			if (red <= 255)
				pix[0] = red;
			else pix[0] = 255;

			if (green <= 255)
                pix[1] = green;
            else pix[1] = 255;
			
			if (blue <= 255)
                pix[2] = blue;
            else pix[2] = 255;
		}
	}
}

void fill_outside(struct o_rect_data* area, struct Png* image, enum error_types* err){
	int area_width = area->right_down->x - area->left_up->x; 
	int area_height = area->right_down->y - area->left_up->y;

	png_bytep* piece = (png_bytep*)calloc(area_height, sizeof(png_bytep));
	get_piece(area_width, area_height, area->left_up->x, area->left_up->y, piece, image, err);

	fill_square(0, 0, image->width, image->height, area->color, image);
	
	paste_piece(area->left_up->x, area->left_up->y, area_width, area_height, piece, image);

	if (piece){
		for (int i=0; i<area_height; i++)
			free(piece[i]);
		free(piece);
	}
}

void cyclic_shift(struct shift_data* shift, struct Png* image){
	png_bytep* new_image = (png_bytep*)calloc(image->height, sizeof(png_bytep));
	
	for (int y=0; y<image->height; y++)
		new_image[y] = calloc(image->width*3, sizeof(png_byte));
	
	if ((shift->step <= image->width) && (shift->step <= image->height)){
		for (int y=0; y<image->height; y++){
			png_byte* src_row = image->row_pointers[y];
			png_byte* dst_row = new_image[y];

			int new_y = y+shift->step;
	
			if (strcmp(shift->axis, "y") == 0 || strcmp(shift->axis, "xy") == 0){
                if (new_y >= image->height)
                    new_y = (y+shift->step) - image->height;
				dst_row = new_image[new_y];
			}

			for (int x=0; x<image->width; x++){
				png_byte* src_pix = &(src_row[x*3]);
				png_byte* dst_pix = &(dst_row[x*3]);
				
				int new_x = x+shift->step;

				if (strcmp(shift->axis, "x") == 0 || strcmp(shift->axis, "xy") == 0){
					if (new_x >= image->width)
						new_x = (x+shift->step) - image->width;
					dst_pix = &(dst_row[new_x*3]);
				}
	
				dst_pix[0] = src_pix[0];
				dst_pix[1] = src_pix[1];
				dst_pix[2] = src_pix[2];
			}
		}
	
		paste_piece(0, 0, image->width, image->height, new_image, image);
	}

	if (new_image){
		for (int i=0; i<image->height; i++)
			free(new_image[i]);
		free(new_image);
	}
}

void draw_rhombus(struct rhombus_data* rhomb, struct Png* image){
	int upper_x = rhomb->upper_vertex->x;
	int upper_y = rhomb->upper_vertex->y;
	int size = rhomb->size;

	for (int dy=0; dy < 2*size; dy++){
		int y = upper_y + dy;
		if (y >= 0 && y<image->height){
			int dx = size - abs(size - dy);
			int x_start = upper_x - dx;
			int x_end = upper_x + dx;

			if (x_end >= 0 && x_start < image->width){
				if (x_start<0) 
					x_start = 0;
				if (x_end >= image->width)
					x_end = image->width-1;

				for (int x=x_start; x<=x_end; x++){
					png_byte* pix = &(image->row_pointers[y][x*3]);
					pix[0] = rhomb->fill_color->R;
                    pix[1] = rhomb->fill_color->G;
                    pix[2] = rhomb->fill_color->B;
                }
			}
		}
	}
}

void circle_pixel(struct circle_data* cpix, struct Png* image) {
    struct RGB* target_color = cpix->color; // Цвет, который нужно обвести
    struct RGB* outline_color = cpix->circle_color; // Цвет рамки
    int size = cpix->size;

    for (int y = 0; y < image->height; y++) {
        png_byte* row = image->row_pointers[y];
        for (int x = 0; x < image->width; x++) {
            png_byte* pixel = &(row[x * 3]);
            // Проверяем, совпадает ли текущий пиксель с целевым цветом
            if (pixel[0] == target_color->R && pixel[1] == target_color->G && pixel[2] == target_color->B) {
                // Обводим вокруг этого пикселя
                for (int i = -size; i <= size; i++) {
                    for (int j = -size; j <= size; j++) {
                        // Проверяем границы
                        int new_y = y + i;
                        int new_x = x + j;
                        if (new_y >= 0 && new_y < image->height && new_x >= 0 && new_x < image->width) {
                            png_byte* outline_pixel = &(image->row_pointers[new_y][new_x * 3]);
                            // Проверяем, не является ли пиксель целевым цветом
                            if (!(outline_pixel[0] == target_color->R && outline_pixel[1] == target_color->G && outline_pixel[2] == target_color->B)) {
                                outline_pixel[0] = outline_color->R;
                                outline_pixel[1] = outline_color->G;
                                outline_pixel[2] = outline_color->B;
                            }
                        }
                    }
                }
            }
        }
    }
}

void swap_pixels(png_byte* pix1, png_byte* pix2){
	png_byte* extra_pix = pix1;

	pix1[0] = pix2[0];
	pix1[1] = pix2[1];
	pix1[2] = pix2[2];

	pix2[0] = extra_pix[0];
	pix2[1] = extra_pix[1];
	pix2[2] = extra_pix[2];
}

void diag_mirror(struct mirror_data* area, struct Png* image){
	int w_size = area->right_down->x - area->left_up->x;
	int h_size = area->right_down->y - area->left_up->y;
	int height = image->height;
	int width = image->width;
	int size;
	if (w_size < h_size)
		size = w_size;
	else size = h_size;

	for (int i=0; i<size; i++){
		for (int j=i+1; j<size; j++){
			int x = area->left_up->x + i;
			int y = area->left_up->y + j;

			int mirror_x = area->left_up->x + j;
            int mirror_y = area->left_up->y + i;

			if (x>=0 && x<width && y>=0 && y<height && mirror_x>=0 && mirror_x<width && mirror_y>=0 && mirror_y<height)
				swap_pixels(&(image->row_pointers[y][x*3]), &(image->row_pointers[mirror_y][mirror_x*3]));
		}
	}
}

int main(int argc, char** argv){

	struct options_flags flags = {0};

	struct option options[] = {
		{"input", required_argument, NULL, 'i'},
		{"output", required_argument, NULL, 'o'},
		{"info", no_argument, NULL, OPT_INFO},	
		{"help", no_argument, NULL, 'h'},
		{"contrast", no_argument, NULL, 'R'},
		{"outside_rect", no_argument, NULL, 'O'},
		{"shift", no_argument, NULL, 'H'},
		{"square_rhombus", no_argument, NULL, 'U'},
		{"circle_pixel", no_argument, NULL, 'I'},
		{"diag_mirror", no_argument, NULL, 'M'},

		{"square", no_argument, NULL, 'S'},
		{"left_up", required_argument, NULL, 'l'},
		{"side_size", required_argument, NULL, 's'},
		{"thickness", required_argument, NULL, 't'},
		{"color", required_argument, NULL, 'C'},
		{"fill", no_argument, NULL, 'f'},
		{"fill_color", required_argument, NULL, 'c'},
		{"exchange", no_argument, NULL, 'E'},
		{"right_down", required_argument, NULL, 'r'},
		{"exchange_type", required_argument, NULL, 'T'},
		{"freq_color", no_argument, NULL, 'F'},
		{"alpha", required_argument, NULL, 'a'},
		{"beta", required_argument, NULL, 'b'},
		{"step", required_argument, NULL, 'p'},
		{"axis", required_argument, NULL, 'x'},
		{"upper_vertex", required_argument, NULL, 'v'},
		{"size", required_argument, NULL, 'z'},
		{"circle_color", required_argument, NULL, 'e'},
		{0, 0, 0, 0}
	};

	char optstring[SZ_OPTSTR] = "i:o:hROHUIMSl:s:t:C:fc:e:r:T:Fa:b:p:x:v:z:e:";
	int optidx = 0;
	int opt;

	enum error_types err = 0;

	int checker = 0;
	int help_opt = 0;
	int info_opt = 0;

	char* img_iname = (char*)calloc(SZ_INPUT, sizeof(char));
	char* img_oname = (char*)calloc(SZ_OUTPUT, sizeof(char));
	
	if (img_oname)
		strcpy(img_oname, "out.png");
	else err = ERR_MEM;
	
	char* axis = calloc(2, sizeof(char));

	struct point* l_point = (struct point*)calloc(1, sizeof(struct point));
	struct point* r_point = (struct point*)calloc(1, sizeof(struct point));
	struct RGB* color = (struct RGB*)calloc(1, sizeof(struct RGB));
	struct RGB* fill_color = (struct RGB*)calloc(1, sizeof(struct RGB));
	struct RGB* circle_color = (struct RGB*)calloc(1, sizeof(struct RGB));

	struct square_data* my_square = (struct square_data*)calloc(1, sizeof(struct square_data));
	struct exchange_data* exchange_data = (struct exchange_data*)calloc(1, sizeof(struct exchange_data));
	struct change_color* my_freq_color = (struct change_color*)calloc(1, sizeof(struct change_color));
	struct contrast_data* my_contrast = (struct contrast_data*)calloc(1, sizeof(struct contrast_data));
	struct mirror_data* my_mirror = (struct mirror_data*)calloc(1, sizeof(struct mirror_data));

	struct o_rect_data* my_rect = (struct o_rect_data*)calloc(1, sizeof(struct o_rect_data));
	struct shift_data* my_shift = (struct shift_data*) calloc(1, sizeof(struct shift_data));
	struct rhombus_data* my_rhomb = (struct rhombus_data*)calloc(1, sizeof(struct rhombus_data));
	struct circle_data* my_cir_p = (struct circle_data*)calloc(1, sizeof(struct circle_data));

	struct Png* my_image = (struct Png*)calloc(1, sizeof(struct Png));

	while ((opt = getopt_long(argc, argv, optstring, options, &optidx)) != -1){
		switch(opt){
			case 'i':
				if (optind < argc && argv[optind][0] != '-'){
                    printf("Опция --input принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					img_iname = realloc(img_iname, strlen(optarg));
					
					if (img_iname){
						strcpy(img_iname, optarg);

						if (read_image(my_image, img_iname, &err))
							flags.iname_flag = 1;
						else {
							printf("Ошибка чтения изображения\n");
							err = ERR_IMG_READ;
						}

					} else err = ERR_MEM;
				}
				break;
			case 'o':
				 if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --output принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					img_oname = realloc(img_oname, strlen(optarg)+1);
	
					if (img_oname){
						if (optarg[0] != '-')
							strcpy(img_oname, optarg);
						else {
							printf("Не введено имя выходного файла\n");
							err = ERR_OPT;
						}
					} else {
						printf("Ошибка памяти\n");
						err = ERR_MEM;
					}
				}
                                break;
			case 'h':
			    if ((optind < argc && argv[optind][0] == '-') || optind == argc || optind == argc-1){
					help_opt = 1; 
					printf("Course work for option 4.16, created by Alina Kiseleva\n");
					print_help();
				} else {
					printf("Опция --help не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
					err = ERR_OPT;
				}
				break;
			case 'R':
				if (my_contrast)
					flags.contr_flag = 1;
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --contrast не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                }
                break;
			case 'O':
				if (my_rect)
					flags.o_rect_flag = 1;
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --outside_rect не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                }
                break;
			case 'H':
				if (my_shift)
					flags.shift_flag = 1;
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --shift не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                }
                break;
			case 'U':
				if (my_rhomb)
					flags.rhombus_flag = 1;
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --square_rhombus не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                }
                break;
			case 'I':
				if (my_cir_p)
					flags.cp_flag = 1;
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --circle_pixel не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                }
                break;
			case 'M':
                if (my_mirror)
                    flags.diag_mir_flag = 1;
                if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --diag_mirror не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                }
                break;
			case 'S': 
				if (my_square)
					flags.sqr_flag = 1;
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --square не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                }
				break;
			case 'E':
				if (exchange_data)
					flags.exchg_flag = 1;
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --exchange не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                }
				break;
			case 'F':
				if (my_freq_color)
					flags.fq_clr_flag = 1;
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --freq_color не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
					err = ERR_OPT;
				}
				break;
			case 'e':
                if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --circle_color принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
                    if (circle_color){
                        int checker = sscanf(optarg, "%d.%d.%d", &circle_color->R, &circle_color->G, &circle_color->B);
	                    if ((checker == 3) && check_color(circle_color->R, circle_color->G, circle_color->B)){
	                        if (my_cir_p){
	                            my_cir_p->circle_color = circle_color;
	                            flags.cir_clr_flag = 1;
	                        } else if (!my_cir_p){
	                            printf("Ошибка памяти\n");
	                            err = ERR_MEM;
	                        }
	                    } else {
	                		printf("Цвет заливки задан неверно\n");
	                        err = ERR_COLOR;
	                    }
                	} else {
                        printf("Ошибка памяти\n");
                        err = ERR_MEM;
                    }
                }
                break;
			case 'v':
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --upper_vertex принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
                    if (l_point && my_rhomb){
                        checker = sscanf(optarg, "%d.%d", &l_point->x, &l_point->y);
                        if (checker == 2){
                            flags.up_vrtx_flag = 1;
							my_rhomb->upper_vertex = l_point;
						} else {
                            printf("Координаты верхней вершины ромба заданы неверно.\n");
                            err = ERR_COORDS;
                        }
                    } else err = ERR_MEM;
                }
                break;
			case 'z':
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --size принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
                    if (my_rhomb && my_cir_p){
                        checker = sscanf(optarg, "%d", &my_rhomb->size);
						checker *= sscanf(optarg, "%d", &my_cir_p->size);
                        if (checker == 1 && my_rhomb->size > 0 && my_cir_p->size > 0){
                            flags.size_flag = 1;
                        } else {
                            printf("Cторона задана неверно\n");
                            err = ERR_OPT;
                        }
                    } else err = ERR_MEM;
                }
                break;
			case 'p':
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --step принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					if (my_shift){
						checker = sscanf(optarg, "%d", &my_shift->step);
                        if (checker == 1 && my_shift->step > 0)
                            flags.step_flag = 1;
						else {
							printf("Значение циклического сдвига задано неверно.\n");
							err = ERR_COORDS;
						}
					} else err = ERR_MEM;
				}
				break;
			case 'x':
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --axis принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
                    if (my_shift){
                        checker = sscanf(optarg, "%s", axis);
                        if (checker == 1){
							my_shift->axis = axis;
                            flags.axis_flag = 1;
						} else {
                            printf("Значение оси сдвига задано неверно.\n");
                            err = ERR_COORDS;
                        }
                    } else err = ERR_MEM;
                }
                break;
			case 'a':
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --alpha принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					if (my_contrast){
						checker = sscanf(optarg, "%lf", &my_contrast->alpha);
						if (checker == 1 && my_contrast->alpha > 0)
							flags.a_flag = 1;
						else {
							printf("Коэффициент изменения контрастности alpha задан неверно\n");
							err = ERR_COORDS;
						}
					} else err = ERR_MEM;
				}
				break;
			case 'b':
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --beta принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					if (my_contrast){
                        checker = sscanf(optarg, "%d", &my_contrast->beta);
                        if (checker == 1)
                            flags.b_flag = 1;
                        else {
                            printf("Сдвиг контрастности beta задан неверно\n");
                            err = ERR_COORDS;
                        }
					} else err = ERR_MEM;
                }
                break;
			case 'l':
			    if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --left_up принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					if (l_point && my_square && exchange_data && my_rect && my_mirror){
						checker = sscanf(optarg, "%d.%d", &l_point->x, &l_point->y);
						if (checker == 2){
							my_square->left_up = l_point;
							exchange_data->left_up = l_point;
							my_rect->left_up = l_point;
							my_mirror->left_up = l_point;
							flags.lft_up_flag = 1;
						} else {
							printf("Координаты левой верхней точки заданы неверно\n");
							err = ERR_COORDS;
						}
					} else {
						printf("Ошибка памяти\n");
						err = ERR_MEM;
					}
				}
				break;
			case 's':
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --side_size принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					if (my_square){
						checker = sscanf(optarg, "%d", &my_square->side_size);
						if (checker == 1 && my_square->side_size > 0){
							flags.sd_sz_flag = 1;
						} else {
							printf("Cторона задана неверно\n");
							err = ERR_OPT;
						}
					} else {
						printf("Ошибка памяти\n");
						err = ERR_MEM;
					}
				}
				break;
			case 't':
				 if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --thickness принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					if (my_square){
						checker = sscanf(optarg, "%d", &my_square->thickness);
						if (checker == 1 && my_square->thickness >= 0){
							flags.thkns_flag = 1;
						} else {
							printf("Толщина задана неверно\n");
							err = ERR_OPT;
						}
					} else {
						printf("Ошибка памяти\n");
						err = ERR_MEM;
        	        }
				}
                break;
			case 'C':
				 if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --color принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					if (color && my_square && my_freq_color && my_rect && my_cir_p){
						checker = sscanf(optarg, "%d.%d.%d", &color->R, &color->G, &color->B);
						if ((checker == 3) && check_color(color->R, color->G, color->B)){
							flags.clr_flag = 1;
							my_square->color = color;
							my_freq_color->new_color = color;
							my_rect->color = color;
							my_cir_p->color = color;
						} else {
							printf("Цвет задан неверно\n");
							err = ERR_COLOR;
						}
					} else {
						printf("Ошибка памяти\n");
						err = ERR_MEM;
					}
				}
                break;
			case 'f':
				if (my_square){
					flags.fill_flag = 1;
					my_square->fill = (bool)1;
				} else {
					printf("Ошибка памяти\n");
                    err = ERR_MEM;
				}
				break;
			case 'c': 
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --fill_color принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					if (fill_color){
						int checker = sscanf(optarg, "%d.%d.%d", &fill_color->R, &fill_color->G, &fill_color->B);
						if ((checker == 3) && check_color(fill_color->R, fill_color->G, fill_color->B)){
							if (my_square && my_rhomb){
								my_square->fill_color = fill_color;
								my_rhomb->fill_color = fill_color;
								flags.fill_clr_flag = 1;
							} else if (!my_square){
								printf("Ошибка памяти\n");
								err = ERR_MEM;
							}
						} else {
							printf("Цвет заливки задан неверно\n");
							err = ERR_COLOR;
						}
					} else {
						printf("Ошибка памяти\n");
						err = ERR_MEM;
					}
				}
				break;
			case 'r':
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --right_down принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					if (r_point){
						int checker = sscanf(optarg, "%d.%d", &r_point->x, &r_point->y);
						if (checker == 2){
							if (exchange_data && my_rect && my_mirror){
								exchange_data->right_down = r_point;
								my_rect->right_down = r_point;
								my_mirror->right_down = r_point;
								flags.rt_dwn_flag = 1;
							} else {
								printf("Ошибка памяти\n");
								err = ERR_MEM;
							}
						} else {
							printf("Координаты правой нижней точки заданы неверно\n");
							err = ERR_COORDS;
						}
					} else {
						printf("Ошибка памяти\n");
						err = ERR_MEM;
					}
				}			
				break;
			case 'T':
				if ((optind < argc && argv[optind][0] != '-') && optind != argc-1){
                    printf("Опция --exchange_type принимает только один аргумент.\n%s и следующие аргументы будут проигнорированы.\n", argv[optind]);
                    err = ERR_OPT;
                } else {
					if (exchange_data){
						if (!strcmp(optarg, "clockwise") || !strcmp(optarg, "counterclockwise") || !strcmp(optarg, "diagonals")){
							exchange_data->type = optarg;
							flags.exchg_t_flag = 1;
						} else {
							printf("Введенного способа обмена частей не существует\n");
							err = ERR_EXCH_TYPE;
						}
					} else {
						printf("Ошибка памяти\n");
						err = ERR_MEM;
					}
				}
				break;
			case OPT_INFO:
				if ((optind < argc && argv[optind][0] == '-') || optind == argc-1){
					info_opt = 1;
				} else {
                    printf("Опция --info не принимает аргументов.\n%s и последующие аргументы будут проигнорированы.\n", argv[optind]);
					err = ERR_OPT;
				}
				break;
			case '?':
			default:
				printf("Введенного флага не существует.\n");
				err = ERR_OPT;
				break;
		}
	}

	/* Проверка на то, что последним подано имя входного файла */
	if (!flags.iname_flag || !img_iname){

		if (argv[argc-1][0] != '-'){ 
			img_iname = realloc(img_iname, strlen(argv[argc-1])); 

			if (img_iname){
				strcpy(img_iname, argv[argc-1]);
				if (read_image(my_image, img_iname, &err))
					flags.iname_flag = 1;
			} else err = ERR_MEM;

		} else	if (!help_opt)
			err = ERR_INPUT_NAME;
	}	

	if (img_iname != NULL && strcmp(img_iname, img_oname) == 0){
                printf("Имена входного и выходного файлов совпадают\n");
                err = ERR_OUTPUT_NAME;
        }
	
	if (!flags.err && !help_opt) 
		printf("Course work for option 4.16, created by Alina Kiseleva\n");

	if (info_opt)
		print_png_info(my_image);

	if (err) flags.err = 1;

	int is_valid = check_input_data(flags);

	if (is_valid){
		if (is_valid == 1)
			draw_square(my_square, my_image);
		else if (is_valid == 2)
			exchange_places(exchange_data, my_image, &err);
		else if (is_valid == 3)
			replace_freq_color(my_freq_color, my_image, &err);
		else if (is_valid == 4)
			make_contrast(my_contrast, my_image);
		else if (is_valid == 5)
			fill_outside(my_rect, my_image, &err);
		else if (is_valid == 6)
			cyclic_shift(my_shift, my_image);
		else if (is_valid == 7)
			draw_rhombus(my_rhomb, my_image);
		else if (is_valid == 8)
			circle_pixel(my_cir_p, my_image);
		else if (is_valid == 9)
                        diag_mirror(my_mirror, my_image);
		write_image(my_image, img_oname, &err);
	}
	
	if (img_iname)
		free(img_iname);
	if (img_oname)
		free(img_oname);
	if (l_point)	
		free(l_point);
	if (r_point)
		free(r_point);
	if (color)
		free(color);
	if (fill_color)	
		free(fill_color);
	if (my_square)	
		free(my_square);
	if (exchange_data)	
		free(exchange_data);
	if (my_freq_color)
		free(my_freq_color);
	if (my_image)	
		free(my_image);
	if (my_contrast)	
		free(my_contrast);
	if (my_rect)
		free(my_rect);
	if (my_shift)
		free(my_shift);
	if (axis)
		free(axis);
	if (my_rhomb)
		free(my_rhomb);
	if (my_cir_p)
		free(my_cir_p);
	if (circle_color)
		free(circle_color);

	return err;
}
