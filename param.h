#pragma once
#include "BigInt.h"

struct paramset{
	 BigInt p, a, b, e, d, m, q, x, y, u, v;
};

struct paramset SetA = {
	.p = vector<uint8_t> {0x97,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	      				  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},

	.a = vector<uint8_t> {0x35,0x73,0x7E,0x27,0x6F,0x65,0x2C,0xB2,0x33,0xAA,0x95,0xBF,0x13,0x20,0x5E,0xE2,
		  				  0x7C,0xA2,0x35,0x30,0xC2,0x92,0x48,0xAF,0x73,0x16,0x98,0x13,0x15,0x3F,0x17,0xC2},

	.b = vector<uint8_t> {0x13,0x95,0xAE,0xF8,0xA6,0x37,0x93,0xBA,0xF7,0x7B,0xE1,0x08,0x91,0xCD,0xFC,0x22,
		  				  0x1A,0xD4,0xA9,0x59,0xC3,0xE7,0x20,0xCC,0x9C,0xED,0x28,0x74,0xAE,0x9B,0x5F,0x29},

	.e = vector<uint8_t> {0x01},

	.d = vector<uint8_t> {0xFB,0xBF,0xC7,0x6D,0x2D,0xC3,0x22,0xE5,0xF7,0x9A,0x00,0x97,0x28,0xF6,0x9D,0x2B,
		 				  0x13,0x18,0xD5,0xFA,0x9C,0xC3,0x8B,0x57,0x81,0xFA,0x83,0xC1,0xB7,0xF6,0x05,0x06},

	.m = vector<uint8_t> {0x9C,0x31,0xD8,0xB0,0x55,0xBD,0x56,0x04,0xD7,0x98,0xED,0x21,0x7F,0x37,0x63,0x3F,
		 				  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		 				  0x01},

	.q = vector<uint8_t> {0x67,0x0C,0x36,0x6C,0x55,0xAF,0x15,0xC1,0x35,0x66,0x7B,0xC8,0xDF,0xCD,0xD8,0x0F,
		 				  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40},

	.x = vector<uint8_t> {0x28,0xAA,0x2D,0x74,0xFE,0x82,0x25,0x8B,0xC7,0x02,0x2E,0x93,0x96,0x91,0x8B,0x65,
		  				  0xBB,0xB2,0x12,0x57,0x42,0x23,0x09,0x88,0x0D,0x2C,0xE8,0xA5,0x43,0x84,0xE3,0x91},

	.y = vector<uint8_t> {0x5C,0x2E,0x32,0x32,0xDB,0x8A,0x26,0xAF,0x40,0x67,0x76,0x44,0x53,0x0B,0xDE,0x5F,
		  				  0x56,0xE9,0x46,0xBB,0xC4,0x86,0x57,0x89,0x75,0x03,0x1A,0xAB,0x23,0x94,0x87,0x32},	

	.u = vector<uint8_t> {0x0D},

	.v = vector<uint8_t> {0xE7,0x00,0xA3,0xDB,0x92,0x25,0x2B,0xF9,0x81,0x2E,0xF2,0x87,0xBE,0x8D,0xEF,0xE7,
		  				  0x9C,0x64,0x07,0xAB,0x8F,0xC3,0x88,0x84,0x34,0x5B,0x47,0xAA,0x32,0x1E,0xCA,0x60}  
};

struct paramset SetC = {
	.p = vector<uint8_t> {0xC7,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
						  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
						  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
						  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},

	.a = vector<uint8_t> {0xd3,0x9b,0x68,0x39,0x6f,0x54,0xb6,0x2e,0x2a,0x9f,0xda,0x1f,0x95,0x7f,0xd9,0x2a,
					      0xff,0x50,0xcf,0x6f,0xf4,0x71,0xde,0x2a,0xd9,0xed,0xc9,0xe2,0xc0,0x61,0xe8,0x46,
					      0x45,0x06,0x43,0xe1,0x68,0x1c,0xe4,0x4d,0x64,0x66,0xb8,0x0e,0x98,0xc8,0x7b,0x18,
					      0xfb,0x22,0xc7,0xd2,0x29,0xa5,0x85,0x54,0x87,0x21,0xa7,0x14,0xe5,0x03,0x92,0xdc},

	.b = vector<uint8_t> {0xe1,0x57,0x25,0x31,0xa5,0x19,0x23,0x8d,0x3c,0x0a,0xbf,0xf5,0xa5,0xc7,0x8c,0x2b,
					      0xb5,0xf3,0xfe,0x8b,0x4b,0x28,0xe0,0x8d,0xc1,0xd2,0x19,0xf7,0xff,0xc2,0xcb,0x38,
					      0xe0,0xad,0xe5,0x0d,0x4f,0x2e,0xda,0xff,0x57,0x4b,0x9f,0xf6,0xa9,0xb6,0xef,0xc7,
					      0x6a,0xf1,0x37,0xcf,0x52,0x29,0xc1,0x8a,0x2c,0x6c,0xbc,0xce,0x28,0xee,0xc4,0xb4},

	.e = vector<uint8_t> {0x01},

	.d = vector<uint8_t> {0x50,0x75,0xee,0x33,0xbb,0x2d,0x30,0xca,0xb4,0x22,0x2a,0xbc,0xc2,0xcf,0xa0,0x91,
					      0x9e,0x36,0x9e,0xe7,0x43,0xce,0xe2,0x04,0xb3,0x06,0x5c,0x51,0x0a,0x9e,0xb3,0xa6,
					      0x9c,0x43,0x50,0x10,0x62,0xa0,0x28,0xde,0xeb,0x31,0x8e,0x19,0x54,0x2d,0x40,0xab,
					      0x4d,0xfe,0x5b,0xdf,0x3c,0xcf,0xa5,0x13,0x9f,0x8d,0x7d,0x01,0x8c,0x5d,0x4f,0x9e},

	.m = vector<uint8_t> {0xb4,0x8f,0xc0,0x1f,0xbd,0xf3,0x88,0x51,0x9a,0x84,0xa6,0x9d,0x9e,0xa7,0xb6,0x23,
						  0xb3,0x40,0x1d,0x45,0xfd,0xa7,0xce,0x30,0x01,0xac,0x1a,0x94,0x91,0x6e,0x33,0x26,
						  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
						  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},

	.q = vector<uint8_t> {0xed,0x23,0xf0,0x47,0xef,0x3c,0x62,0x94,0x26,0xa1,0x69,0xa7,0xe7,0xa9,0xed,0xc8,
					      0x2c,0x50,0x47,0x51,0xff,0xa9,0x33,0x4c,0x00,0xab,0x06,0x65,0xa4,0xdb,0x8c,0xc9, 
					      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
					      0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x3f},

	.x = vector<uint8_t> {0x48,0x01,0x95,0xc1,0x28,0x79,0xbc,0xc5,0xaa,0x97,0xae,0x7e,0x48,0x85,0xfb,0xc6,
					      0x10,0x36,0xed,0xb9,0x3d,0x03,0xb9,0xa7,0xf2,0x2b,0x60,0xae,0xa7,0x72,0x72,0xa2,
					      0x3a,0x04,0xea,0x4c,0x07,0xf7,0x85,0xd3,0x21,0xf0,0xae,0xcb,0xa9,0xb7,0x95,0x22,
					      0xde,0xf5,0x3e,0x59,0xce,0x41,0xe2,0xeb,0xbd,0xe7,0x3d,0xc2,0xdf,0x1e,0xe3,0xe2},

	.y = vector<uint8_t> {0x0f,0xc4,0xdd,0x9a,0x9a,0x6e,0x39,0xd0,0x07,0xae,0x4b,0x85,0xaa,0x26,0xf7,0x04,
					      0x63,0x3b,0x42,0x22,0x58,0xd8,0x32,0xef,0xd2,0x1e,0x02,0xe3,0x33,0x2d,0x8e,0xe1,
					      0x9b,0xff,0x90,0x20,0x3d,0x8c,0x10,0x8c,0x8b,0x37,0x27,0x65,0x4d,0x80,0x39,0x79,
					      0x57,0xb8,0x1c,0x91,0xf5,0xcf,0xbc,0xab,0x99,0xb8,0x5e,0x5b,0xd9,0x40,0xce,0xf5},

	.u = vector<uint8_t> {0x12},

	.v = vector<uint8_t> {0x3d,0x1a,0x00,0x73,0xee,0x03,0x03,0x60,0xe7,0xaa,0x2b,0x4b,0xc0,0x22,0x56,0x90,
					      0xf0,0x32,0x97,0x13,0x5d,0x8c,0x06,0xbf,0xee,0x89,0xf7,0x0c,0x65,0x4b,0xdd,0x22,
					      0x95,0x66,0x38,0x7b,0x7f,0x11,0x56,0x9a,0x8d,0x36,0x94,0x17,0xd0,0xb0,0xdf,0x0f,
					      0x2a,0x1e,0xa0,0x77,0x2b,0x59,0x99,0x6b,0xe1,0xf5,0xb1,0x1f,0x9d,0xf7,0x9a,0x46}
};
