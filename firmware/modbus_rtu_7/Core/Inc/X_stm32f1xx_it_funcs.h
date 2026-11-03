#pragma once


#undef NULL  /* others (e.g. <stdio.h>) also define */
#define NULL 0
#define __CALL( CallBack_ptr ) ((CallBack_ptr != NULL ) ? CallBack_ptr() : (void)0U)



static inline void X_TIM4_IRQHandler (void){

 	if( (TIM4->DIER & TIM_DIER_UIE) && (TIM4->SR & TIM_SR_UIF) ){
 		TIM4->SR = ~TIM_SR_UIF;
 		__CALL(X_CallBack_TIM4.UP);}

 	if( (TIM4->DIER & TIM_DIER_CC1IE) && (TIM4->SR & TIM_SR_CC1IF) ){
 		TIM4->SR = ~TIM_SR_CC1IF;
 		__CALL(X_CallBack_TIM4.CC1);}

 	if( (TIM4->DIER & TIM_DIER_CC2IE) && (TIM4->SR & TIM_SR_CC2IF) ){
 		TIM4->SR = ~TIM_SR_CC2IF;
 		__CALL(X_CallBack_TIM4.CC2);}

 	if( (TIM4->DIER & TIM_DIER_CC3IE) && (TIM4->SR & TIM_SR_CC3IF) ){
 		TIM4->SR = ~TIM_SR_CC3IF;
 		__CALL(X_CallBack_TIM4.CC3);}

 	if( (TIM4->DIER & TIM_DIER_CC4IE) && (TIM4->SR & TIM_SR_CC4IF) ){
 		TIM4->SR = ~TIM_SR_CC4IF;
 		__CALL(X_CallBack_TIM4.CC4);}

 	if( (TIM4->DIER & TIM_DIER_TIE) && (TIM4->SR & TIM_SR_TIF) ){
 		TIM4->SR = ~TIM_SR_TIF;
 		__CALL(X_CallBack_TIM4.TRG);}
}

