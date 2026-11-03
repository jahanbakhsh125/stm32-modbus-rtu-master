#pragma once



/******************************************************************************
 ******     Function Pointer Types for CallBack Function
******************************************************************************/

 // Typedef for RTC interrupt function pointers


 // Typedef for TIM2to4 interrupt function pointers
 typedef struct{
 	void (*UP)  (void);
 	void (*CC1) (void);
 	void (*CC2) (void);
 	void (*CC3) (void);
 	void (*CC4) (void);
 	void (*TRG) (void);
 } TIM2_CallBack_Typedef;





 /******************************************************************************
  ******     extern definition of Function Pointers for CallBack Functions
 ******************************************************************************/


 extern TIM2_CallBack_Typedef  X_CallBack_TIM4;

