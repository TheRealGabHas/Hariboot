#include <efi.h>
#include <efilib.h>
#include "graphics/drawing.h"
#include "memory/mmap.h"
#include "memory/mem.h"
#include "system/stall.h"
#include "filesystem/config.h"


int _fltused = 0;


static UINT8 g_8x16_font[4096] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7E, 0x81, 0xA5, 0x81, 0x81, 0xBD, 0x99, 0x81, 0x81, 0x7E, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7E, 0xFF, 0xDB, 0xFF, 0xFF, 0xC3, 0xE7, 0xFF, 0xFF, 0x7E, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x6C, 0xFE, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0xE7, 0xE7, 0xE7, 0x99, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x7E, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0x00, 0x00, 0x1E, 0x0E, 0x1A, 0x32, 0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3F, 0x33, 0x3F, 0x30, 0x30, 0x30, 0x30, 0x70, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7F, 0x63, 0x7F, 0x63, 0x63, 0x63, 0x63, 0x67, 0xE7, 0xE6, 0xC0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x18, 0x18, 0xDB, 0x3C, 0xE7, 0x3C, 0xDB, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFE, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x02, 0x06, 0x0E, 0x1E, 0x3E, 0xFE, 0x3E, 0x1E, 0x0E, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7F, 0xDB, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x7C, 0xC6, 0x60, 0x38, 0x6C, 0xC6, 0xC6, 0x6C, 0x38, 0x0C, 0xC6, 0x7C, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x7E, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x6C, 0xFE, 0x6C, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x38, 0x7C, 0x7C, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0x7C, 0x7C, 0x38, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x66, 0x66, 0x66, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 
	0x18, 0x18, 0x7C, 0xC6, 0xC2, 0xC0, 0x7C, 0x06, 0x86, 0xC6, 0x7C, 0x18, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xC2, 0xC6, 0x0C, 0x18, 0x30, 0x60, 0xC6, 0x86, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x38, 0x6C, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xCE, 0xD6, 0xD6, 0xE6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7C, 0xC6, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7C, 0xC6, 0x06, 0x06, 0x3C, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0C, 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xFE, 0xC0, 0xC0, 0xC0, 0xFC, 0x0E, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x38, 0x60, 0xC0, 0xC0, 0xFC, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xFE, 0xC6, 0x06, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0x06, 0x0C, 0x78, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7C, 0xC6, 0xC6, 0x0C, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xDE, 0xDE, 0xDE, 0xDC, 0xC0, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xC0, 0xC0, 0xC2, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xDE, 0xC6, 0xC6, 0x66, 0x3A, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xE6, 0x66, 0x6C, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xF0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC6, 0xE6, 0xF6, 0xFE, 0xDE, 0xCE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xDE, 0x7C, 0x0C, 0x0E, 0x00, 0x00, 
	0x00, 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7C, 0xC6, 0xC6, 0x60, 0x38, 0x0C, 0x06, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7E, 0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xD6, 0xFE, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC6, 0xC6, 0x6C, 0x6C, 0x38, 0x38, 0x6C, 0x6C, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFE, 0xC6, 0x86, 0x0C, 0x18, 0x30, 0x60, 0xC2, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C, 0x0E, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00, 0x00, 0x00, 0x00, 
	0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 
	0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xE0, 0x60, 0x60, 0x78, 0x6C, 0x66, 0x66, 0x66, 0x66, 0xDC, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC0, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x1C, 0x0C, 0x0C, 0x3C, 0x6C, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x38, 0x6C, 0x64, 0x60, 0xF0, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xCC, 0x78, 0x00,
	0x00, 0x00, 0xE0, 0x60, 0x60, 0x6C, 0x76, 0x66, 0x66, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x18, 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x06, 0x06, 0x00, 0x0E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, 0x00, 
	0x00, 0x00, 0xE0, 0x60, 0x60, 0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0xE6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xEC, 0xFE, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0x0C, 0x1E, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x76, 0x62, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0x60, 0x38, 0x0C, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x10, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x30, 0x30, 0x36, 0x1C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xD6, 0xD6, 0xFE, 0x6C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x6C, 0x38, 0x38, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x0C, 0xF8, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xCC, 0x18, 0x30, 0x60, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0E, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x18, 0x0E, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xC0, 0xC2, 0x66, 0x3C, 0x0C, 0x06, 0x7C, 0x00, 0x00, 
	0x00, 0x00, 0xCC, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0C, 0x18, 0x30, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x10, 0x38, 0x6C, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xCC, 0xCC, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x60, 0x30, 0x18, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x6C, 0x38, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x60, 0x60, 0x66, 0x3C, 0x0C, 0x06, 0x3C, 0x00, 0x00, 0x00, 
	0x00, 0x10, 0x38, 0x6C, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC6, 0xC6, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x60, 0x30, 0x18, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x66, 0x66, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x18, 0x3C, 0x66, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x60, 0x30, 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xC6, 0xC6, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, 
	0x38, 0x6C, 0x38, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x30, 0x60, 0x00, 0xFE, 0x66, 0x60, 0x7C, 0x60, 0x60, 0x66, 0xFE, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x76, 0x36, 0x7E, 0xD8, 0xD8, 0x6E, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3E, 0x6C, 0xCC, 0xCC, 0xFE, 0xCC, 0xCC, 0xCC, 0xCC, 0xCE, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x10, 0x38, 0x6C, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC6, 0xC6, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x60, 0x30, 0x18, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x30, 0x78, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x60, 0x30, 0x18, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x0C, 0x78, 0x00, 
	0x00, 0xC6, 0xC6, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x18, 0x18, 0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x38, 0x6C, 0x64, 0x60, 0xF0, 0x60, 0x60, 0x60, 0x60, 0xE6, 0xFC, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xF8, 0xCC, 0xCC, 0xF8, 0xC4, 0xCC, 0xDE, 0xCC, 0xCC, 0xCC, 0xC6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0E, 0x1B, 0x18, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0x70, 0x00, 0x00, 
	0x00, 0x18, 0x30, 0x60, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0C, 0x18, 0x30, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x18, 0x30, 0x60, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x18, 0x30, 0x60, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x76, 0xDC, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 
	0x76, 0xDC, 0x00, 0xC6, 0xE6, 0xF6, 0xFE, 0xDE, 0xCE, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x3C, 0x6C, 0x6C, 0x3E, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x38, 0x6C, 0x6C, 0x38, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x60, 0xC0, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xC0, 0xC0, 0xC2, 0xC6, 0xCC, 0x18, 0x30, 0x60, 0xCE, 0x93, 0x06, 0x0C, 0x1F, 0x00, 0x00, 
	0x00, 0xC0, 0xC0, 0xC2, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xCE, 0x9A, 0x3F, 0x06, 0x0F, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 
	0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 
	0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xF6, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x36, 0x36, 0x36, 0x36, 0x36, 0xF6, 0x06, 0xF6, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x06, 0xF6, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0xF6, 0x06, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x30, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x30, 0x37, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0xF7, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xF7, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0xF7, 0x00, 0xF7, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xFF, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xDC, 0xD8, 0xD8, 0xD8, 0xDC, 0x76, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xC6, 0xFC, 0xC6, 0xC6, 0xFC, 0xC0, 0xC0, 0xC0, 0x00, 0x00,
	0x00, 0x00, 0xFE, 0xC6, 0xC6, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x80, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFE, 0xC6, 0x60, 0x30, 0x18, 0x30, 0x60, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 0x70, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xC0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x76, 0xDC, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7E, 0x18, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x6C, 0x6C, 0x6C, 0xEE, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1E, 0x30, 0x18, 0x0C, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0xDB, 0xDB, 0xDB, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x06, 0x7E, 0xCF, 0xDB, 0xF3, 0x7E, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1C, 0x30, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x30, 0x1C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0xFE, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0xD8, 0x70, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xDC, 0x00, 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x0F, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0xEC, 0x6C, 0x6C, 0x3C, 0x1C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xD8, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x70, 0x98, 0x30, 0x60, 0xC8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

EFI_PHYSICAL_ADDRESS *allocBuffer(EFI_SYSTEM_TABLE *SystemTable, Vec2 ScreenSize, UINT64 Pitch) {

	EFI_PHYSICAL_ADDRESS *Buffer; 
	
	Buffer = (EFI_PHYSICAL_ADDRESS *)allocPool(SystemTable, EfiLoaderData, (ScreenSize.x * ScreenSize.y * Pitch));
	return Buffer;
}


static inline void drawChar(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, CHAR8 character, Vec2 position, UINT32 foregroundColor) {

	Vec3 Colors;
	INT32 mask[8] = {128, 64, 32, 16, 8, 4, 2, 1};
	UINT8 *glyph = g_8x16_font + (INT32)character * 16;

	for (UINT8 cy = 0; cy < 16; cy++) {
		for (UINT8 cx = 0; cx < 8; cx++) {
			Colors = getPixelValue(Gop, (Vec2){position.x + cx, position.y + cy});
			drawPoint_32bpp(Gop, (Vec2){(position.x + cx), ((position.y + cy))}, glyph[cy]&mask[cx]?foregroundColor:((Colors.red << 16) | (Colors.green << 8) | Colors.blue));
		}
	}
}

void drawPoint_32bpp(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, Vec2 position, UINT32 color) {

    *((UINT32*)(gop->Mode->FrameBufferBase + (4 * gop->Mode->Info->PixelsPerScanLine * position.y) + (4 * position.x))) = color;
}

void fillScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, Vec3 backgroundColor) {

	Vec3 Colors;

	for (UINTN i = 0; i < Gop->Mode->Info->HorizontalResolution; i++) {
        for (UINTN j = 0; j < Gop->Mode->Info->VerticalResolution; j++) {

			switch (backgroundColor.red) {
				case X_HORIZONTAL:
					Colors.red = (UINT32)(MIN((i / (Gop->Mode->Info->HorizontalResolution / 256)), 255));
					break;
				case Y_HORIZONTAL:
					Colors.red = (UINT32)(MIN((j / (Gop->Mode->Info->HorizontalResolution / 256)), 255));
					break;
				case X_VERTICAL:
					Colors.red = (UINT32)(MIN((i / (Gop->Mode->Info->VerticalResolution / 256)), 255));
					break;
				case Y_VERTICAL:
					Colors.red = (UINT32)(MIN((j / (Gop->Mode->Info->VerticalResolution / 256)), 255));
					break;
				default:
					Colors.red = backgroundColor.red;
					break;
			}

			switch (backgroundColor.green) {
				case X_HORIZONTAL:
					Colors.green = (UINT32)(MIN((i / (Gop->Mode->Info->HorizontalResolution / 256)), 255));
					break;
				case Y_HORIZONTAL:
					Colors.green = (UINT32)(MIN((j / (Gop->Mode->Info->HorizontalResolution / 256)), 255));
					break;
				case X_VERTICAL:
					Colors.green = (UINT32)(MIN((i / (Gop->Mode->Info->VerticalResolution / 256)), 255));
					break;
				case Y_VERTICAL:
					Colors.green = (UINT32)(MIN((j / (Gop->Mode->Info->VerticalResolution / 256)), 255));
					break;
				default:
					Colors.green = backgroundColor.green;
					break;
			}

			switch (backgroundColor.blue) {
				case X_HORIZONTAL:
					Colors.blue = (UINT32)(MIN((i / (Gop->Mode->Info->HorizontalResolution / 256)), 255));
					break;
				case Y_HORIZONTAL:
					Colors.blue = (UINT32)(MIN((j / (Gop->Mode->Info->HorizontalResolution / 256)), 255));
					break;
				case X_VERTICAL:
					Colors.blue = (UINT32)(MIN((i / (Gop->Mode->Info->VerticalResolution / 256)), 255));
					break;
				case Y_VERTICAL:
					Colors.blue = (UINT32)(MIN((j / (Gop->Mode->Info->VerticalResolution / 256)), 255));
					break;
				default:
					Colors.blue = backgroundColor.blue;
					break;
			}
            drawPoint_32bpp(Gop, (Vec2){i, j}, ((Colors.red << 16) | (Colors.green << 8) | Colors.blue));
        }
    }
}

void fillScreenDarkAndLightMode(EFI_SYSTEM_TABLE *SystemTable, EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT8 mode) {

    Vec3 Colors;

	if (mode == 0x1) {

		for (UINT32 i = 0; i < Gop->Mode->Info->HorizontalResolution; i++) {
        	for (UINT32 j = 0; j < Gop->Mode->Info->VerticalResolution; j++) { 

    	        Colors.red = (UINT32)(MIN((i / (Gop->Mode->Info->HorizontalResolution / 256)), 255));
	            Colors.green = 0x80;//(UINT32)(MIN((j / (Gop->Mode->Info->VerticalResolution / 256)), 0));
            	Colors.blue = (UINT32)(MIN((j / (Gop->Mode->Info->VerticalResolution / 256)), 255));

            	drawPoint_32bpp(Gop, (Vec2){i, j}, ((Colors.red << 16) | (Colors.green << 8) | Colors.blue));
        	}
    	}
	} else if (mode == 0x0) {

		for (UINT32 i = 0; i < Gop->Mode->Info->HorizontalResolution; i++) {
        	for (UINT32 j = 0; j < Gop->Mode->Info->VerticalResolution; j++) { 

        	    Colors.red = (UINT32)(MIN((i / (Gop->Mode->Info->HorizontalResolution / 256)), 255));
    	        Colors.green = 0x40;//(UINT32)(MIN((j / (Gop->Mode->Info->VerticalResolution / 256)), 0));
	            Colors.blue = (UINT32)(MIN((j / (Gop->Mode->Info->VerticalResolution / 256)), 255));

            	drawPoint_32bpp(Gop, (Vec2){i, j}, ((Colors.red << 16) | (Colors.green << 8) | Colors.blue));
        	}
    	}
	} else {
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Uncompatible mode.\n");
	}
}

void drawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, Vec2 posA, Vec2 posB, Vec3 pixel) {
    UINT32 dx = posB.x - posA.x;
    UINT32 dy = posB.y - posA.y;

    if (posA.x != posB.x) {
        for (UINT32 x = posA.x; x < posB.x; ++x) {

            UINT32 y = posA.y + dy * (x - posA.x) / dx;
            drawPoint_32bpp(Gop, (Vec2){x, y}, ((pixel.red << 16) | (pixel.green << 8) | pixel.blue));
        }
    }
    else {
        for (UINT32 y = posA.y; y < posB.y; ++y) {

            UINT32 x = posA.x + dx * (y - posA.y) / dy;
            drawPoint_32bpp(Gop, (Vec2){x, y}, ((pixel.red << 16) | (pixel.green << 8) | pixel.blue));
        }
    }
}

void drawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, Vec2 posUpperLeft, Vec2 posDownRight, Vec3 pixel, BOOLEAN rectType) {

    Vec3 ColorsRead, ColorsWrite;

    if (rectType == FALSE) {
        UINT32 dx = posDownRight.x - posUpperLeft.x;
        UINT32 dy = posDownRight.y - posUpperLeft.y;

        drawLine(gop, (Vec2){posUpperLeft.x, posUpperLeft.y}, (Vec2){posUpperLeft.x + dx, posUpperLeft.y}, pixel); // Up
        drawLine(gop, (Vec2){posUpperLeft.x, posUpperLeft.y + dy}, (Vec2){posUpperLeft.x + dx, posUpperLeft.y + dy}, pixel); // Down
        drawLine(gop, (Vec2){posUpperLeft.x, posUpperLeft.y}, (Vec2){posUpperLeft.x, posUpperLeft.y + dy}, pixel); // Left
        drawLine(gop, (Vec2){posUpperLeft.x + dx, posUpperLeft.y}, (Vec2){posUpperLeft.x + dx, posUpperLeft.y + dy}, pixel); // Right
    // Outlined
    } else {

        
        for (UINT32 y = posUpperLeft.y; y < posDownRight.y; ++y) {
            for (UINT32 x = posUpperLeft.x; x < posDownRight.x; ++x) {

				ColorsRead = getPixelValue(gop, (Vec2){x, y});
                ColorsWrite.red = (ColorsRead.red * pixel.alpha + pixel.red * (255 - pixel.alpha)) / 255;
                ColorsWrite.green = (ColorsRead.green * pixel.alpha + pixel.green * (255 - pixel.alpha)) / 255;
                ColorsWrite.blue = (ColorsRead.blue * pixel.alpha + pixel.blue * (255 - pixel.alpha)) / 255;
                drawPoint_32bpp(gop, (Vec2){x, y}, ((ColorsWrite.red << 16) + (ColorsWrite.green << 8) + ColorsWrite.blue));
            }
        }
    }
    // filled
}

void drawRountedMenu(EFI_SYSTEM_TABLE *SystemTable, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, Vec2 posUpperLeft, Vec2 posDownRight, Vec3 pixel, UINT32 radius) {

    Vec2 pointReference, firstPoint, secondPoint, pointToDraw;
	Vec3 ColorsRead, ColorsWrite;
	Vec3 *ColorsArray = (Vec3 *)allocPool(SystemTable, EfiLoaderData, sizeof(Vec3) * radius * 1001);

	posUpperLeft.x += radius;
	posDownRight.x -= radius;

	drawRect(
		gop,
		(Vec2){posUpperLeft.x, posUpperLeft.y}, (Vec2){posDownRight.x, posDownRight.y},
		pixel,
		TRUE);

	pointReference = (Vec2){posUpperLeft.x, posUpperLeft.y};
	secondPoint = (Vec2){posUpperLeft.x - radius, posUpperLeft.y};
	firstPoint = (Vec2){posUpperLeft.x - radius, posUpperLeft.y + radius - 1};

	for (UINT32 i = 0; i < radius; i++) {
		for (float t = 0; t < 1.001; t+=0.001) {

    	    pointToDraw = (Vec2){((1 - t) * (1 - t) * pointReference.x + 2 * (1 - t) * (secondPoint.x - i) * t + t * t * (firstPoint.x + i)),
			((1 - t) * (1 - t) * (pointReference.y  + i) + 2 * (1 - t) * (secondPoint.y + i) * t + t * t * firstPoint.y)};

			ColorsRead = getPixelValue(gop, pointToDraw);
			ColorsArray[i].red = ColorsRead.red;
			ColorsArray[i].green = ColorsRead.green;
			ColorsArray[i].blue = ColorsRead.blue;
        }
    }

    for (UINT32 i = 0; i < radius; i++) {
		for (float t = 0; t < 1.001; t+=0.001) {

    	    pointToDraw = (Vec2){((1 - t) * (1 - t) * pointReference.x + 2 * (1 - t) * (secondPoint.x - i) * t + t * t * (firstPoint.x + i)),
			((1 - t) * (1 - t) * (pointReference.y  + i) + 2 * (1 - t) * (secondPoint.y + i) * t + t * t * firstPoint.y)};

			ColorsWrite.red = (ColorsArray[i].red * pixel.alpha + pixel.red * (255 - pixel.alpha)) / 255;
            ColorsWrite.green = (ColorsArray[i].green * pixel.alpha + pixel.green * (255 - pixel.alpha)) / 255;
            ColorsWrite.blue = (ColorsArray[i].blue * pixel.alpha + pixel.blue * (255 - pixel.alpha)) / 255;
			drawPoint_32bpp(gop, pointToDraw, ((ColorsWrite.red << 16) | (ColorsWrite.green << 8) | ColorsWrite.blue));

        }
    }

	firstPoint = (Vec2){posUpperLeft.x - radius, posDownRight.y - radius + 1};
	secondPoint = (Vec2){posUpperLeft.x - radius, posDownRight.y};
	pointReference = (Vec2){posUpperLeft.x, posDownRight.y};

	for (UINT32 i = 0; i < radius; i++) {
		for (float t = 0; t < 1.001; t+=0.001) {

    	    pointToDraw = (Vec2){((1 - t) * (1 - t) * pointReference.x + 2 * (1 - t) * (secondPoint.x + i) * t + t * t * (firstPoint.x + i)),
			((1 - t) * (1 - t) * (pointReference.y - i) + 2 * (1 - t) * (secondPoint.y - i) * t + t * t * firstPoint.y)};

			ColorsRead = getPixelValue(gop, pointToDraw);
			ColorsArray[i].red = ColorsRead.red;
			ColorsArray[i].green = ColorsRead.green;
			ColorsArray[i].blue = ColorsRead.blue;
        }
    }

	for (UINT32 i = 0; i < radius; i++) {
		for (float t = 0; t < 1.001; t+=0.001) {

    	    pointToDraw = (Vec2){((1 - t) * (1 - t) * pointReference.x + 2 * (1 - t) * (secondPoint.x + i) * t + t * t * (firstPoint.x + i)),
			((1 - t) * (1 - t) * (pointReference.y - i) + 2 * (1 - t) * (secondPoint.y - i) * t + t * t * firstPoint.y)};

			ColorsWrite.red = (ColorsArray[i].red * pixel.alpha + pixel.red * (255 - pixel.alpha)) / 255;
            ColorsWrite.green = (ColorsArray[i].green * pixel.alpha + pixel.green * (255 - pixel.alpha)) / 255;
            ColorsWrite.blue = (ColorsArray[i].blue * pixel.alpha + pixel.blue * (255 - pixel.alpha)) / 255;
			drawPoint_32bpp(gop, pointToDraw, ((ColorsWrite.red << 16) | (ColorsWrite.green << 8) | ColorsWrite.blue));
        }
    }

	drawRect(
		gop,
		(Vec2){posUpperLeft.x - radius, posUpperLeft.y + radius}, (Vec2){posUpperLeft.x, posDownRight.y - radius},
		pixel,
		TRUE);

    pointReference = (Vec2){posDownRight.x, posUpperLeft.y};
    firstPoint = (Vec2){posDownRight.x + radius, posUpperLeft.y + radius - 0.5};
    secondPoint = (Vec2){posDownRight.x + radius, posUpperLeft.y};

    for (UINT32 i = 0; i < radius; i++) {

        for (float t = 0; t < 1.001; t+=0.001) {

            pointToDraw = (Vec2){((1 - t) * (1 - t) * pointReference.x + 2 * (1 - t) * (secondPoint.x - i) * t + t * t * (firstPoint.x - i)),
			((1 - t) * (1 - t) * (pointReference.y + i) + 2 * (1 - t) * (secondPoint.y + i) * t + t * t * firstPoint.y)};
            
			ColorsRead = getPixelValue(gop, pointToDraw);
			ColorsArray[i].red = ColorsRead.red;
			ColorsArray[i].green = ColorsRead.green;
			ColorsArray[i].blue = ColorsRead.blue;
        }
    }

	for (UINT32 i = 0; i < radius; i++) {

        for (float t = 0; t < 1.001; t+=0.001) {

            pointToDraw = (Vec2){((1 - t) * (1 - t) * pointReference.x + 2 * (1 - t) * (secondPoint.x - i) * t + t * t * (firstPoint.x - i)),
			((1 - t) * (1 - t) * (pointReference.y + i) + 2 * (1 - t) * (secondPoint.y + i) * t + t * t * firstPoint.y)};

			ColorsWrite.red = (ColorsArray[i].red * pixel.alpha + pixel.red * (255 - pixel.alpha)) / 255;
            ColorsWrite.green = (ColorsArray[i].green * pixel.alpha + pixel.green * (255 - pixel.alpha)) / 255;
            ColorsWrite.blue = (ColorsArray[i].blue * pixel.alpha + pixel.blue * (255 - pixel.alpha)) / 255;
			drawPoint_32bpp(gop, pointToDraw, ((ColorsWrite.red << 16) | (ColorsWrite.green << 8) | ColorsWrite.blue));
        }
    }

    pointReference = (Vec2){posDownRight.x, posDownRight.y};
	secondPoint = (Vec2){posDownRight.x + radius, posDownRight.y};
	firstPoint = (Vec2){posDownRight.x + radius, posDownRight.y - radius + 1};

	for (UINT32 i = 0; i < radius; i++) {

        for (float t = 0; t < 1.001; t+=0.001) {

            pointToDraw = (Vec2){((1 - t) * (1 - t) * pointReference.x + 2 * (1 - t) * (secondPoint.x - i) * t + t * t * (firstPoint.x - i)),
			((1 - t) * (1 - t) * (pointReference.y - i) + 2 * (1 - t) * (secondPoint.y - i) * t + t * t * firstPoint.y)};

			ColorsRead = getPixelValue(gop, pointToDraw);
			ColorsArray[i].red = ColorsRead.red;
			ColorsArray[i].green = ColorsRead.green;
			ColorsArray[i].blue = ColorsRead.blue;
        }
    }

    for (UINT32 i = 0; i < radius; i++) {

        for (float t = 0; t < 1.001; t+=0.001) {

            pointToDraw = (Vec2){((1 - t) * (1 - t) * pointReference.x + 2 * (1 - t) * (secondPoint.x - i) * t + t * t * (firstPoint.x - i)),
			((1 - t) * (1 - t) * (pointReference.y - i) + 2 * (1 - t) * (secondPoint.y - i) * t + t * t * firstPoint.y)};
            
			ColorsWrite.red = (ColorsArray[i].red * pixel.alpha + pixel.red * (255 - pixel.alpha)) / 255;
            ColorsWrite.green = (ColorsArray[i].green * pixel.alpha + pixel.green * (255 - pixel.alpha)) / 255;
            ColorsWrite.blue = (ColorsArray[i].blue * pixel.alpha + pixel.blue * (255 - pixel.alpha)) / 255;
			drawPoint_32bpp(gop, pointToDraw, ((ColorsWrite.red << 16) | (ColorsWrite.green << 8) | ColorsWrite.blue));
        }
    }

	drawRect(
		gop,
		(Vec2){posDownRight.x, posUpperLeft.y + radius}, (Vec2){posDownRight.x + radius, posDownRight.y - radius},
		pixel,
		TRUE);

	freePool(SystemTable, ColorsArray);
}

void drawString(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, CHAR8 *string, Vec2 position, Vec3 color) {
	
	for (UINT16 i = 0; i < strlen(string); i++) {
		drawChar(Gop, string[i], position, ((color.red << 16) + (color.green << 8) + color.blue));
		position.x += 8;
	}
}

Vec3 getPixelValue(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, Vec2 position) {

    UINT32 color;
    Vec3 Colors;
    color = *((UINT32*)(Gop->Mode->FrameBufferBase + (4 * Gop->Mode->Info->PixelsPerScanLine * position.y) + (4 * position.x)));

    Colors.red = (color >> 16) & 0xFF;
    Colors.green = (color >> 8) & 0xFF;
    Colors.blue = color & 0xFF;
    
    return Colors;
}
