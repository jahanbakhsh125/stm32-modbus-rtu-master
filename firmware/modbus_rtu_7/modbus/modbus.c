#include "modbus.h"
#include "rtu.h"
#include "stdio.h"

#define __X0_MASK 0x1
#define __X1_MASK 0x2

#define __UART_TX 	GPIOB->BSRR = GPIO_BSRR_BS1
#define __UART_RX 	GPIOB->BSRR = GPIO_BSRR_BR1

#define __LED_X1_OFF 	GPIOB->BSRR = GPIO_BSRR_BR9
#define __LED_X1_ON 	GPIOB->BSRR = GPIO_BSRR_BS9

#define __LED_X0_OFF 	GPIOB->BSRR = GPIO_BSRR_BR5
#define __LED_X0_ON 	GPIOB->BSRR = GPIO_BSRR_BS5

#define __GLED_OFF 	GPIOC->BSRR = GPIO_BSRR_BS13
#define __GLED_ON 	GPIOC->BSRR = GPIO_BSRR_BR13

typedef enum{
	cmd0F_0 = 0,
	cmd10_1 = 1,
	cmd02_2 = 2,
	cmd03_3 = 3
}modbus_stage_Typedef;

modbus_stage_Typedef modbus_stage;

extern UART_HandleTypeDef huart2;


Coil_Typedef mycoil;
uint16_t myc0;

void TIM4_Update_callback(void);


void modbus_init(void){

	//PB1 : MAX485 RX/TX
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRL &= ~(GPIO_CRL_MODE1|GPIO_CRL_CNF1);
	GPIOB->CRL |= GPIO_CRL_MODE1_1;

	//PC13 : Checksum LED
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH &= ~(GPIO_CRH_MODE13|GPIO_CRH_CNF13);
	GPIOC->CRH |= GPIO_CRH_MODE13_1;

	//PB5 : LED GPIO Output
	GPIOB->CRL &= ~(GPIO_CRL_MODE5|GPIO_CRL_CNF5);
	GPIOB->CRL |= GPIO_CRL_MODE5_1;

	//PB9 : LED GPIO Output
	GPIOB->CRH &= ~(GPIO_CRH_MODE9|GPIO_CRH_CNF9);
	GPIOB->CRH |= GPIO_CRH_MODE9_1;

	//PB14 : DIP GPIO Input
	GPIOB->CRH &= ~(GPIO_CRH_MODE14|GPIO_CRH_CNF14);
	GPIOB->CRH |= GPIO_CRH_CNF14_1;
	GPIOB->ODR &= ~GPIO_ODR_ODR14;

	//PB15 : DIP GPIO Input
	GPIOB->CRH &= ~(GPIO_CRH_MODE15|GPIO_CRH_CNF15);
	GPIOB->CRH |= GPIO_CRH_CNF15_1;
	GPIOB->ODR &= ~GPIO_ODR_ODR15;

	//PB12 & PB13 EXTI Rising
	GPIOB->CRH &= ~(GPIO_CRH_MODE12|GPIO_CRH_CNF12);
	GPIOB->CRH |= GPIO_CRH_CNF12_0;

	GPIOB->CRH &= ~(GPIO_CRH_MODE13|GPIO_CRH_CNF13);
	GPIOB->CRH |= GPIO_CRH_CNF13_0;

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	AFIO->EXTICR[3] &= ~AFIO_EXTICR4_EXTI12;
	AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI12_PB;

	AFIO->EXTICR[3] &= ~AFIO_EXTICR4_EXTI13;
	AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PB;

	EXTI->RTSR |= (EXTI_RTSR_TR12|EXTI_RTSR_TR13);
	EXTI->FTSR &= ~(EXTI_FTSR_TR12|EXTI_FTSR_TR13);
	EXTI->IMR |= (EXTI_IMR_MR12|EXTI_IMR_MR13);
	EXTI->PR = EXTI_PR_PR12;
	EXTI->PR = EXTI_PR_PR13;


	//ADC1 CH1 PA0
	RCC->APB2ENR |= (RCC_APB2ENR_ADC1EN|RCC_APB2ENR_IOPAEN);
	GPIOA->CRL &= ~(GPIO_CRL_MODE0|GPIO_CRL_CNF0);

	ADC1->SQR1 &= ~ADC_SQR1_L;
	ADC1->SQR3 &= ~ADC_SQR3_SQ1;
	ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->SMPR2 |= ADC_SMPR2_SMP8;

	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_ADON;

	ADC1->CR2 |= ADC_CR2_CAL;


	//tim4
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	TIM4->CR1 = 0;
	TIM4->CR1 |= TIM_CR1_OPM;

	TIM4->ARR = (uint16_t)4999;
	TIM4->PSC = (uint16_t)71;

	TIM4->SR = ~TIM_SR_UIF;
	TIM4->DIER |= TIM_DIER_UIE;

	X_CallBack_TIM4.UP = TIM4_Update_callback;

	uint32_t PG=NVIC_GetPriorityGrouping();
	NVIC_SetPriority(TIM4_IRQn, NVIC_EncodePriority(PG, 6, 0));
	NVIC_EnableIRQ(TIM4_IRQn);




}


uint8_t cmd0F_MakeData(void){

	mycoil.bit.y2 = (GPIOB->IDR & GPIO_IDR_IDR14) != 0x0 ;
	mycoil.bit.y3 = (GPIOB->IDR & GPIO_IDR_IDR15) != 0x0 ;

	if(EXTI->PR & EXTI_PR_PR12){
		EXTI->PR = EXTI_PR_PR12;
		mycoil.bit.y1 =1;
	}
	if(EXTI->PR & EXTI_PR_PR13){
		EXTI->PR = EXTI_PR_PR13;
		mycoil.bit.y1 =0;
	}

	return mycoil.byte;

}

uint8_t test[20];

void delete_parity(uint8_t * data,uint8_t num){

	for(int i =0 ;i<num;i++){
		data[i] &= (uint8_t)0x7F;
	}
}

void led_change(uint8_t data){

	if(data & __X0_MASK){
		__LED_X0_ON;
	}else{
		__LED_X0_OFF;
	}

	if(data & __X1_MASK){
		__LED_X1_ON;
	}else{
		__LED_X1_OFF;
	}




}


void modbus_start(void){

	// 0F  : set content of y1 to y5
	cmd0F.Data = cmd0F_MakeData();

	CRC16_RTU.u16 = CRC_RTU(&cmd0F.SlaveAdd, __CMD0F_BYTES_CRC);
	cmd0F.CRC_H = CRC16_RTU.u8[1];
	cmd0F.CRC_L = CRC16_RTU.u8[0];

	modbus_stage = cmd0F_0;
	__UART_TX;

	TIM4->CR1 |= TIM_CR1_CEN;

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){

	__UART_RX;
	switch(modbus_stage){

	case cmd0F_0 :

		HAL_UART_Receive_DMA(&huart2, &res0F.SlaveAdd , __RES0F_L);


		break;

	case cmd10_1 :
		HAL_UART_Receive_DMA(&huart2, &res10.SlaveAdd , __RES10_L);


		break;

	case cmd02_2 :
		HAL_UART_Receive_DMA(&huart2, &res02.SlaveAdd , __RES02_L);


		break;

	case cmd03_3 :
	   HAL_UART_Receive_DMA(&huart2, &res03.SlaveAdd , __RES03_L);



		break;

	default:
		break;
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	__UART_TX;
	switch(modbus_stage){

		case cmd0F_0 :
			CRC16_RTU.u8[1] = res0F.CRC_H;
			CRC16_RTU.u8[0] = res0F.CRC_L;
			if( CRC16_RTU.u16 == CRC_RTU(&res0F.SlaveAdd,__RES0F_BYTES_CRC)){
				__GLED_ON;
			}else{__GLED_OFF;}


			// 10 : set content of D0
			modbus_stage = cmd10_1;

			cmd10.Data_HH = 0x0;
			cmd10.Data_HL = 0x0;
			cmd10.Data_LH = (uint8_t)(ADC1->DR >> 8);
			cmd10.Data_LL = (uint8_t)ADC1->DR;

			CRC16_RTU.u16 = CRC_RTU(&cmd10.SlaveAdd, __CMD10_BYTES_CRC);
			cmd10.CRC_H = CRC16_RTU.u8[1];
			cmd10.CRC_L = CRC16_RTU.u8[0];




			break;

		case cmd10_1 :
			CRC16_RTU.u8[1] = res10.CRC_H;
			CRC16_RTU.u8[0] = res10.CRC_L;
			if( CRC16_RTU.u16 == CRC_RTU(&res10.SlaveAdd,__RES10_BYTES_CRC)){
				__GLED_ON;
			}else{__GLED_OFF;}

			// 02  : read X0 to X7
			modbus_stage = cmd02_2;



			break;

		case cmd02_2 :
			CRC16_RTU.u8[1] = res02.CRC_H;
			CRC16_RTU.u8[0] = res02.CRC_L;
			if( CRC16_RTU.u16 == CRC_RTU(&res02.SlaveAdd,__RES02_BYTES_CRC)){
				__GLED_ON;
				led_change(res02.Data);
			}else{__GLED_OFF;}

			// 03  : read C0
			modbus_stage = cmd03_3;




			break;

		case cmd03_3 :
			CRC16_RTU.u8[1] = res03.CRC_H;
			CRC16_RTU.u8[0] = res03.CRC_L;
			if( CRC16_RTU.u16 == CRC_RTU(&res03.SlaveAdd,__RES03_BYTES_CRC)){
				__GLED_ON;
				myc0 = (uint16_t)res03.Data_H << 8 | res03.Data_L ;
			}else{__GLED_OFF;}


			// 0F  : set content of y1 to y5
			modbus_stage = cmd0F_0;

			cmd0F.Data = cmd0F_MakeData();

			CRC16_RTU.u16 = CRC_RTU(&cmd0F.SlaveAdd, __CMD0F_BYTES_CRC);
			cmd0F.CRC_H = CRC16_RTU.u8[1];
			cmd0F.CRC_L = CRC16_RTU.u8[0];







			break;

		default:
			break;
		}

	TIM4->CR1 |= TIM_CR1_CEN;

}


void TIM4_Update_callback(void){

	switch(modbus_stage){

		case cmd0F_0 :

			HAL_UART_Transmit_DMA(&huart2, &cmd0F.SlaveAdd , __CMD0F_L);

			break;

		case cmd10_1 :

			HAL_UART_Transmit_DMA(&huart2, &cmd10.SlaveAdd, __CMD10_L);

			break;

		case cmd02_2 :

			HAL_UART_Transmit_DMA(&huart2, &cmd02.SlaveAdd, __CMD02_L);

			break;

		case cmd03_3 :

			HAL_UART_Transmit_DMA(&huart2, &cmd03.SlaveAdd, __CMD03_L);

			break;

		default:
			break;
		}

}
