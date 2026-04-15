#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "You aren't using a cross compiler"
#endif


#if !defined(__i386__)
#error "compile this with a ix86-elf compiler"
#endif

enum vga_color {
		VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
		};

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
  return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
  return (uint16_t) uc | (uint16_t) color << 8;    
}

size_t strlen(const char* str){
  size_t len = 0;
  while(str[len])
    len++;
  return len;
}

size_t term_row;
size_t term_col;
uint8_t term_color;
uint16_t* term_buffer = (uint16_t*) VGA_MEMORY;

char term_data[VGA_WIDTH][VGA_HEIGHT];

void term_init(){
  term_row = 0;
  term_col = 0;
  term_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t idx = y * VGA_WIDTH + x;
      term_buffer[idx] = vga_entry(' ', term_color);
    }
  }
}

void term_setcolor(uint8_t color){
  term_color = color;
}

void term_putentry(char c, uint8_t color, size_t x, size_t y){
  const size_t idx = y * VGA_WIDTH + x;
  term_buffer[idx] = vga_entry(c, color);
  
}
void term_redraw(void);
void term_putchar(char c ){

  if(c == '\n'){
    term_col = 0;
      term_row++;
      return;
  }
  term_data[(term_col%VGA_WIDTH)][(term_row%VGA_HEIGHT)] = c;
  term_putentry(c, term_color, term_col, term_row);

  if(++term_col == VGA_WIDTH){
    term_col = 0;
    if(++term_row == VGA_HEIGHT){
      term_row = 0;
      term_redraw();
    }
  }
}

void term_write(const char* data, size_t size)
{
  for (size_t i = 0; i < size; ++i) {
    term_putchar(data[i]);
  }
}

void term_writestr(const char* data){
  term_write(data, strlen(data));
}
void term_redraw(void)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t idx = y * VGA_WIDTH + x;
            term_buffer[idx] = vga_entry(' ', term_color);
        }
    }
    
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            char c = term_data[x][y];
            if (c != '\0') {
                const size_t idx = y * VGA_WIDTH + x;
                term_buffer[idx] = vga_entry(c, term_color);
            }
        }
    }
}
void kernel_main(void)
{
  term_init();

}
