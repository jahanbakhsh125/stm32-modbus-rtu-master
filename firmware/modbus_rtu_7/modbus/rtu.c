#include "rtu.h"

#include "stdio.h"

CRC16_Typedef CRC16_RTU;
D0_Typedef D0;

//struct for feedback message
res_02_Typedef res02;
res_03_Typedef res03;
res_0F_Typedef res0F;
res_10_Typedef res10;

//struct for send message
cmd_Read_Typedef cmd02;
cmd_Read_Typedef cmd03;
cmd_0F_Typedef cmd0F;
cmd_10_Typedef cmd10;



uint16_t CRC_RTU(uint8_t* data, uint8_t length){
	int j;
	unsigned int reg_crc=0Xffff;
	while(length--){
		reg_crc ^= *data++;
		for (j=0;j<8;j++){
			if(reg_crc & 0x01) reg_crc=(reg_crc>>1) ^ 0Xa001; /* LSB(b0)=1 */
			else reg_crc=reg_crc >>1;
		}
	}
	return reg_crc; // the value that sent back to the CRC register finally
}



void rtu_init(void){


	//cmd02 : read X0 to X7
	cmd02.SlaveAdd = 0x1;
	cmd02.CMD = 0x2;
	cmd02.Add_H = 0x4;
	cmd02.Add_L = 0x0;
	cmd02.NOP_H = 0x0;
	cmd02.NOP_L = 0x8;

	CRC16_RTU.u16 = CRC_RTU(&cmd02.SlaveAdd,6);

	cmd02.CRC_L = CRC16_RTU.u8[0];
	cmd02.CRC_H = CRC16_RTU.u8[1];






	//cmd03 : read C0
	cmd03.SlaveAdd = 0x1;
	cmd03.CMD = 0x3;
	cmd03.Add_H = 0xE;
	cmd03.Add_L = 0x0;
	cmd03.NOP_H = 0x0;
	cmd03.NOP_L = 0x1;

	CRC16_RTU.u16 = CRC_RTU(&cmd03.SlaveAdd,6);

	cmd03.CRC_L = CRC16_RTU.u8[0];
	cmd03.CRC_H = CRC16_RTU.u8[1];





	//cmd0F : set content of y1 to y5
	cmd0F.SlaveAdd = 0x1;
	cmd0F.CMD = 0xF;
	cmd0F.Add_H = 0x5;
	cmd0F.Add_L = 0x01;
	cmd0F.NOC_H = 0x0;
	cmd0F.NOC_L = 0x5;
	cmd0F.ByteCNT = 0x1;
	cmd0F.Data = 0x1F;

	CRC16_RTU.u16 = CRC_RTU(&cmd0F.SlaveAdd,8);

	cmd0F.CRC_L = CRC16_RTU.u8[0];
	cmd0F.CRC_H = CRC16_RTU.u8[1];




	//cmd10 : set content of D0
	cmd10.SlaveAdd = 0x1;
	cmd10.CMD = 0x10;
	cmd10.Add_H = 0x10;
	cmd10.Add_L = 0x0;
	cmd10.NOR_H = 0x0;
	cmd10.NOR_L = 0x2;
	cmd10.ByteCNT = 0x4;
	cmd10.Data_LH = D0.u8.LH;
	cmd10.Data_LL = D0.u8.LL;
	cmd10.Data_HH = D0.u8.HH;
	cmd10.Data_HL = D0.u8.HL;

	CRC16_RTU.u16 = CRC_RTU(&cmd10.SlaveAdd,11);

	cmd10.CRC_L = CRC16_RTU.u8[0];
	cmd10.CRC_H = CRC16_RTU.u8[1];




}


