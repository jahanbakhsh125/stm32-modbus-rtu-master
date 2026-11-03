#pragma once

#include <main.h>



//sent message length
#define __CMD02_L 8
#define __CMD03_L 8
#define __CMD0F_L 10
#define __CMD10_L 13

//sent message CRC bytes
#define __CMD02_BYTES_CRC 6
#define __CMD03_BYTES_CRC 6
#define __CMD0F_BYTES_CRC 8
#define __CMD10_BYTES_CRC 11


//feedback message length
#define __RES02_L 6
#define __RES03_L 7
#define __RES0F_L 8
#define __RES10_L 8

//feedback message CRC bytes
#define __RES02_BYTES_CRC 4
#define __RES03_BYTES_CRC 5
#define __RES0F_BYTES_CRC 6
#define __RES10_BYTES_CRC 6

//Structure type for sent massage
typedef struct{
	uint8_t SlaveAdd;
	uint8_t CMD;
	uint8_t Add_H;
	uint8_t Add_L;
	uint8_t NOP_H;
	uint8_t NOP_L;
	uint8_t CRC_L;
	uint8_t CRC_H;
}cmd_Read_Typedef;

typedef struct{
	uint8_t SlaveAdd;
	uint8_t CMD;
	uint8_t Add_H;
	uint8_t Add_L;
	uint8_t NOC_H;
	uint8_t NOC_L;
	uint8_t ByteCNT;
	uint8_t Data;
	uint8_t CRC_L;
	uint8_t CRC_H;
}cmd_0F_Typedef;

typedef struct{
	uint8_t SlaveAdd;
	uint8_t CMD;
	uint8_t Add_H;
	uint8_t Add_L;
	uint8_t NOR_H;
	uint8_t NOR_L;
	uint8_t ByteCNT;
	uint8_t Data_LH;
	uint8_t Data_LL;
	uint8_t Data_HH;
	uint8_t Data_HL;
	uint8_t CRC_L;
	uint8_t CRC_H;
}cmd_10_Typedef;

//sturct type for feedback message

typedef struct{
	uint8_t SlaveAdd;
	uint8_t CMD;
	uint8_t ByteCNT;
	uint8_t Data;
	uint8_t CRC_L;
	uint8_t CRC_H;
}res_02_Typedef;


typedef struct{
	uint8_t SlaveAdd;
	uint8_t CMD;
	uint8_t ByteCNT;
	uint8_t Data_H;
	uint8_t Data_L;
	uint8_t CRC_L;
	uint8_t CRC_H;
}res_03_Typedef;


typedef struct{
	uint8_t SlaveAdd;
	uint8_t CMD;
	uint8_t Add_H;
	uint8_t Add_L;
	uint8_t NOC_H;
	uint8_t NOC_L;
	uint8_t CRC_L;
	uint8_t CRC_H;
}res_0F_Typedef;


typedef struct{
	uint8_t SlaveAdd;
	uint8_t CMD;
	uint8_t Add_H;
	uint8_t Add_L;
	uint8_t NOR_H;
	uint8_t NOR_L;
	uint8_t CRC_L;
	uint8_t CRC_H;
}res_10_Typedef;

//extern struct for feedback message
extern res_02_Typedef res02;
extern res_03_Typedef res03;
extern res_0F_Typedef res0F;
extern res_10_Typedef res10;

//extern struct for sent message
extern cmd_0F_Typedef cmd0F;
extern cmd_10_Typedef cmd10;
extern cmd_Read_Typedef cmd02;
extern cmd_Read_Typedef cmd03;


//Coil type
typedef union{
	uint8_t byte;
	struct{
		uint8_t y1:1;
		uint8_t y2:1;
		uint8_t y3:1;
		uint8_t y4:1;
		uint8_t y5:1;
		uint8_t   :3;
	}bit;
}Coil_Typedef;


typedef union{
	int32_t i32;
	struct{
		uint8_t LL;
		uint8_t LH;
		uint8_t HL;
		uint8_t HH;
	}u8;
}D0_Typedef;

typedef union{
	uint16_t u16;
	uint8_t u8[2];
}CRC16_Typedef;

extern CRC16_Typedef CRC16_RTU;
extern D0_Typedef D0;

void rtu_init(void);
uint16_t CRC_RTU(uint8_t* data, uint8_t length);

