#include "stm8s.h"
#include "stm8s_it.h"
#include "defines.h"

#include "filters.h"
#include "data_buffer.h"
#include "Screen.h"


void Initialize(void);

#define INTERRUPT_BASED 1
#define EXPORT_RAW		0

/*
 * filter headers:
 */
int16_t stage1_fir_filter_advance(int8_t input, char output_cycle);
int32_t stage2_fir_filter_advance(int16_t input, char output_cycle);

int32_t stage3_fir_filter_advance(int32_t input, char output_cycle, int32_t *_filter_memory);
int32_t stage4_fir_filter_advance(int32_t input, char output_cycle, int32_t *_filter_memory);

int32_t generic_fir_rom_opt_advance(uint32_t input, char output_cycle, int32_t *filter_memory, int32_t *filter_coeff, uint8_t filter_order);

int32_t generic_2ord_iir_advance(int32_t input, int32_t *filter_memory, int32_t *coeff);

int32_t stage8_fir_filter_advance(int32_t input, int32_t *_filter_memory);

/*
 * This value needs to be between 2500 - (nr of adc clock cycles) and (255 * max adc multiplication factor for feedback)
 * this is thus 2388 and 2040 (for 112 and 8 respectively)
 * Choosing this value higher makes more time for timer 2 interrupt to stop the timer after a cycle but less time for the ADC EOC interrupt to start timer 2
 *
 */

void Timer1_configuration(void);
void Timer2_configuration(void);
void ADC1_configuration(void);

volatile uint32_t input = 0;
volatile uint32_t output = 0;

#define STAGE3_ORDER	6
#define STAGE4_ORDER	8
#define STAGE5_ORDER	12
#define STAGE6_ORDER	18
#define STAGE7_ORDER	70


static const int32_t stage4_coeff[STAGE4_ORDER + 1] = {-21671168, -57446912, 86228736, 515199488, 786390528, 515199488, 86228736, -57446912, -21671168};
static const int32_t stage5_coeff[STAGE5_ORDER + 1] = {4534784, 10265344, -26586880, -84064768, 65555456, 500018688, 765431552, 500018688, 65555456, -84064768, -26586880, 10265344, 4534784};
static const int32_t stage6_coeff[STAGE6_ORDER + 1] = {-560384, -2084096, 1233920, 15193088, 11409408, -49801472, -85861120, 95791104, 472342016, 678930944, 472342016, 95791104, -85861120, -49801472, 11409408, 15193088, 1233920, -2084096, -560384};
static const int32_t stage7_coeff[STAGE7_ORDER + 1] = {-1024, -5376, -11776, -8704, 20224, 72192, 96256, 8960, -211456, -414720, -304128, 302336, 1116928, 1331200, 195328, -2025728, -3620096, -2405888, 2115328, 7168512, 7792128, 891904, -10530304, -17341440, -10492416, 9925120, 30503168, 31349760, 1985792, -44643328, -72928000, -44145408, 55686400, 199139328, 326368768, 377164800, 326368768, 199139328, 55686400, -44145408, -72928000, -44643328, 1985792, 31349760, 30503168, 9925120, -10492416, -17341440, -10530304, 891904, 7792128, 7168512, 2115328, -2405888, -3620096, -2025728, 195328, 1331200, 1116928, 302336, -304128, -414720, -211456, 8960, 96256, 72192, 20224, -8704, -11776, -5376, -1024};


FILTER_MEMORY_MEM_SAVE(stage3_ir, int32_t, STAGE3_ORDER);
FILTER_MEMORY_MEM_SAVE(stage3_r, int32_t, STAGE3_ORDER);

FILTER_MEMORY_MEM_SAVE(stage4_ir, int32_t, STAGE4_ORDER);
FILTER_MEMORY_MEM_SAVE(stage4_r, int32_t, STAGE4_ORDER);

FILTER_MEMORY_MEM_SAVE(stage5_ir, int32_t, STAGE5_ORDER);
FILTER_MEMORY_MEM_SAVE(stage5_r, int32_t, STAGE5_ORDER);

FILTER_MEMORY_MEM_SAVE(stage6_ir, int32_t, STAGE6_ORDER);
FILTER_MEMORY_MEM_SAVE(stage6_r, int32_t, STAGE6_ORDER);

FILTER_MEMORY_MEM_SAVE(stage7_ir, int32_t, STAGE7_ORDER);
FILTER_MEMORY_MEM_SAVE(stage7_r, int32_t, STAGE7_ORDER);



/* gating filter function, implements decimation of frequency */
#define STAGE1_GATE								(0x00U)
#define STAGE2_GATE								(0x01U)

#define STAGE3_GATE								(0x00U)
#define STAGE4_GATE								(0x01U)
#define STAGE5_GATE								(0x03U)
#define STAGE6_GATE								(0x04U)
#define STAGE7_GATE								(0x05U)

#define GET_STAGE_GATE_BIT(state, index)		((state >> index) & 0x1U)
#define TOGGLE_STAGE_GATE_BIT(state, index)		(state ^= (1U << index))

#define STAGE1_GATE_BIT(state)					((state >> 0) & 0x1U)
#define STAGE2_GATE_BIT(state)					((state >> 1) & 0x1U)

volatile uint8_t int_gate_state = 0;
uint8_t main_gate_state = 0;

const int32_t mixing_table[20] = {1394679063, 1831030810, 2088148503, 2140863672, 1984016188, 1632959376, 1122057123, 501320101, -168489625, -821806413, -1394679063, -1831030810, -2088148503, -2140863672, -1984016188, -1632959376, -1122057123, -501320101, 168489625, 821806413};

const uint8_t mixing_table_size = sizeof(mixing_table)/sizeof(mixing_table[0]);
uint8_t mixing_table_r_index = 5;
uint8_t mixing_table_ir_index = 0;

#define UART1_TX_RED_INDICATOR		(1U << 7)
#define UART1_TX_IR_INDICATOR		(0U << 7)

int8_t uart_tx_reg[10] = {0};
volatile uint8_t uart1_tx_index = 0;

int32_t dct_value_r = 0xffffffff/2;
int32_t dct_value_ir = 0xffffffff/2;
const int32_t dct_alpha = 2143192972;
const int32_t dct_alpha_inv = 2147483647 - 2143192972;
int64_t tmp;

volatile uint8_t led_r_brightness = 10;
volatile uint8_t led_ir_brightness = 100;

void main(void)
{

	Initialize();

	  /*
	   * GPIO SET MODE
	   */

	ScreenInit();

	SetScreen(0, 0);

	GPIO_Init(LED_R_PORT, LED_R_PIN, GPIO_MODE_OUT_OD_LOW_FAST);
	GPIO_Init(LED_IR_PORT, LED_IR_PIN, GPIO_MODE_OUT_OD_LOW_FAST);

//	/* not sure if needed but set adc input pin to floating input*/
	GPIO_Init(ADC_IN_PORT, ADC_IN_PIN, GPIO_MODE_IN_FL_NO_IT);


	/* enable interrupts */
	enableInterrupts();

	/* Enable timer 1*/
	TIM1->CR1 |= TIM1_CR1_CEN;

	int32_t data_r, data_ir;

#if INTERRUPT_BASED != 1
	volatile int8_t adc_val = 0;
	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);
#endif

	uint8_t screen_val = 0, screen_loop_cnt = 0;

	while (1)
	{

#if EXPORT_RAW == 0



#if INTERRUPT_BASED == 1

		data_r = databuffer_get_new_data();

#else

		//toggle
		TOGGLE_STAGE_GATE_BIT(int_gate_state, STAGE1_GATE);
		if(GET_STAGE_GATE_BIT(int_gate_state, STAGE1_GATE)){
			(void)stage1_fir_filter_advance(adc_val, 0);
			continue;
		}

		int16_t stage1_out = stage1_fir_filter_advance(adc_val, 1);

		//toggle
		TOGGLE_STAGE_GATE_BIT(int_gate_state, STAGE2_GATE);
		if(GET_STAGE_GATE_BIT(int_gate_state, STAGE2_GATE)){
			(void)stage2_fir_filter_advance(stage1_out, 0);
			continue;
		}

		data_r = stage2_fir_filter_advance(stage1_out, 1);
#endif


		/*
		 * MIXING
		 */
		int64_t mix_tmp;
		mix_tmp = data_r * mixing_table[mixing_table_ir_index];
		data_ir = ((int32_t *)(&mix_tmp))[0];

		mix_tmp = data_r * mixing_table[mixing_table_r_index];
		data_r = ((int32_t *)(&mix_tmp))[0];

		//increment indexes
		if(++mixing_table_r_index == mixing_table_size){
			mixing_table_r_index = 0;
		}
		if(++mixing_table_ir_index == mixing_table_size){
			mixing_table_ir_index = 0;
		}

		/******/

		//toggle
		TOGGLE_STAGE_GATE_BIT(main_gate_state, STAGE3_GATE);
		if(GET_STAGE_GATE_BIT(main_gate_state, STAGE3_GATE)){
			(void)stage3_fir_filter_advance(data_r, 0, stage3_r_filter_memory);
			(void)stage3_fir_filter_advance(data_ir, 0, stage3_ir_filter_memory);

			continue;
		}

		data_r = stage3_fir_filter_advance(data_r, 1, stage3_r_filter_memory);
		data_ir = stage3_fir_filter_advance(data_ir, 1, stage3_ir_filter_memory);


//		//toggle
//		TOGGLE_STAGE_GATE_BIT(main_gate_state, STAGE4_GATE);
//		if(GET_STAGE_GATE_BIT(main_gate_state, STAGE4_GATE)){
//			(void)stage4_fir_filter_advance(data_r, 0, stage4_r_filter_memory);
//			(void)stage4_fir_filter_advance(data_ir, 0, stage4_ir_filter_memory);
//
//			continue;
//		}
//
//		data_r = stage4_fir_filter_advance(data_r, 1, stage4_r_filter_memory);
//		data_ir = stage4_fir_filter_advance(data_ir, 1, stage4_ir_filter_memory);


		//toggle
		TOGGLE_STAGE_GATE_BIT(main_gate_state, STAGE4_GATE);
		if(GET_STAGE_GATE_BIT(main_gate_state, STAGE4_GATE)){
			(void)generic_fir_rom_opt_advance(data_r, 0, stage4_r_filter_memory, stage4_coeff, STAGE4_ORDER);
			(void)generic_fir_rom_opt_advance(data_ir, 0, stage4_ir_filter_memory, stage4_coeff, STAGE4_ORDER);

			continue;
		}

		data_r = generic_fir_rom_opt_advance(data_r, 1, stage4_r_filter_memory, stage4_coeff, STAGE4_ORDER);
		data_ir = generic_fir_rom_opt_advance(data_ir, 1, stage4_ir_filter_memory, stage4_coeff, STAGE4_ORDER);


		//toggle
		TOGGLE_STAGE_GATE_BIT(main_gate_state, STAGE5_GATE);
		if(GET_STAGE_GATE_BIT(main_gate_state, STAGE5_GATE)){
			(void)generic_fir_rom_opt_advance(data_r, 0, stage5_r_filter_memory, stage5_coeff, STAGE5_ORDER);
			(void)generic_fir_rom_opt_advance(data_ir, 0, stage5_ir_filter_memory, stage5_coeff, STAGE5_ORDER);

			continue;
		}

		data_r = generic_fir_rom_opt_advance(data_r, 1, stage5_r_filter_memory, stage5_coeff, STAGE5_ORDER);
		data_ir = generic_fir_rom_opt_advance(data_ir, 1, stage5_ir_filter_memory, stage5_coeff, STAGE5_ORDER);

		ScreenDisplayNext();

		//toggle
		TOGGLE_STAGE_GATE_BIT(main_gate_state, STAGE6_GATE);
		if(GET_STAGE_GATE_BIT(main_gate_state, STAGE6_GATE)){
			(void)generic_fir_rom_opt_advance(data_r, 0, stage6_r_filter_memory, stage6_coeff, STAGE6_ORDER);
			(void)generic_fir_rom_opt_advance(data_ir, 0, stage6_ir_filter_memory, stage6_coeff, STAGE6_ORDER);
			continue;
		}

		data_r = generic_fir_rom_opt_advance(data_r, 1, stage6_r_filter_memory, stage6_coeff, STAGE6_ORDER);
		data_ir = generic_fir_rom_opt_advance(data_ir, 1, stage6_ir_filter_memory, stage6_coeff, STAGE6_ORDER);


		//toggle
		TOGGLE_STAGE_GATE_BIT(main_gate_state, STAGE7_GATE);
		if(GET_STAGE_GATE_BIT(main_gate_state, STAGE7_GATE)){
			(void)generic_fir_rom_opt_advance(data_r, 0, stage7_r_filter_memory, stage7_coeff, STAGE7_ORDER);
			(void)generic_fir_rom_opt_advance(data_ir, 0, stage7_ir_filter_memory, stage7_coeff, STAGE7_ORDER);
			continue;
		}

		data_r = generic_fir_rom_opt_advance(data_r, 1, stage7_r_filter_memory, stage7_coeff, STAGE7_ORDER);
		data_ir = generic_fir_rom_opt_advance(data_ir, 1, stage7_ir_filter_memory, stage7_coeff, STAGE7_ORDER);


		tmp = (dct_alpha * dct_value_r) + (dct_alpha_inv * data_r);
		dct_value_r = ((int32_t *)(&tmp))[0];

		tmp = (dct_alpha * dct_value_ir) + (dct_alpha_inv * data_ir);
		dct_value_ir = ((int32_t *)(&tmp))[0];

		if(++screen_loop_cnt == 25){
			screen_loop_cnt = 0;
			if(++screen_val == 251){
				screen_val = 0;
			}
			SetScreen(screen_val, 0);
		}

#if INTERRUPT_BASED == 1

				uart_tx_reg[0] = ((int8_t)((data_r >> 28) & 0x0f)) | UART1_TX_RED_INDICATOR;
				uart_tx_reg[1] = ((int8_t)((data_r >> 21) & 0x7f)) | UART1_TX_RED_INDICATOR;
				uart_tx_reg[2] = ((int8_t)((data_r >> 14) & 0x7f)) | UART1_TX_RED_INDICATOR;
				uart_tx_reg[3] = ((int8_t)((data_r >> 7) & 0x7f)) | UART1_TX_RED_INDICATOR;
				uart_tx_reg[4] = ((int8_t)((data_r) & 0x7f)) | UART1_TX_RED_INDICATOR;

				uart_tx_reg[5] = ((int8_t)((data_r >> 28) & 0x0f)) | UART1_TX_IR_INDICATOR;
				uart_tx_reg[6] = ((int8_t)((data_ir >> 21) & 0x7f)) | UART1_TX_IR_INDICATOR;
				uart_tx_reg[7] = ((int8_t)((data_ir >> 14) & 0x7f)) | UART1_TX_IR_INDICATOR;
				uart_tx_reg[8] = ((int8_t)((data_ir >> 7) & 0x7f)) | UART1_TX_IR_INDICATOR;
				uart_tx_reg[9] = ((int8_t)((data_ir) & 0x7f)) | UART1_TX_IR_INDICATOR;

				uart1_tx_index = 10;
				UART1->DR = uart_tx_reg[9];
#else
				GPIO_WriteReverse(GPIOD, GPIO_PIN_3);
#endif

#else
				data_r = 0;
				data_ir = 0;

#endif

	}

}


#define LED_BRIGHTNESS(procent)							((100 - procent) * 24)

#define LED_R_VALUE_FROM_MOD_TABLE(table_entry)			((uint8_t)((table_entry >> 4) & 0xf))
#define LED_IR_VALUE_FROM_MOD_TABLE(table_entry)		((uint8_t)(table_entry & 0xf))


volatile uint16_t led_mod_table_idx = 0;


/**
 *  Timer 1 update event interrupt
 *
 *	Write new values timer 1 CCR 3,4 to modulate leds
 */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, ITC_IRQ_TIM1_OVF)
{
	//clear interrupt pending flag
	TIM1_ClearITPendingBit(TIM1_IT_UPDATE);

	if(TIM1->CR1 & (1 << 4)){
//		// write new values from lookup table to compare registers to modulate leds
//		TIM1_SetCompare4((uint16_t)(LED_R_VALUE_FROM_MOD_TABLE(led_mod_table[led_mod_table_idx]) * led_r_brightness));
//		TIM1_SetCompare3((uint16_t)(LED_IR_VALUE_FROM_MOD_TABLE(led_mod_table[led_mod_table_idx]) * led_ir_brightness));
//
//		//increment index

		switch(led_mod_table_idx / 10){
		case 0:

			TIM1_SetCompare3(LED_BRIGHTNESS(led_ir_brightness));
			TIM1_SetCompare4(LED_BRIGHTNESS(0));

			break;
		case 1:

			TIM1_SetCompare3(LED_BRIGHTNESS(led_ir_brightness));
			TIM1_SetCompare4(LED_BRIGHTNESS(led_r_brightness));

			break;
		case 2:

			TIM1_SetCompare3(LED_BRIGHTNESS(0));
			TIM1_SetCompare4(LED_BRIGHTNESS(led_r_brightness));

			break;
		case 3:

			TIM1_SetCompare3(LED_BRIGHTNESS(0));
			TIM1_SetCompare4(LED_BRIGHTNESS(0));

			break;
		default:

			break;
		}


		++led_mod_table_idx;
		if(led_mod_table_idx == 40){
			led_mod_table_idx = 0;
		}
	}
}

/**
 *  ADC end of conversion interrupt
 *
 *	Calculate and write values to timer 2 CCR 1 for ADC feedback generation
 *	Start timer 2
 *	Write sampled value to UART
 *	Further High speed calculations would be implemented here
 */
INTERRUPT_HANDLER(ADC1_IRQHandler, ITC_IRQ_ADC1)
{
	//clear interrupt pending flag
	ADC1_ClearITPendingBit(ADC1_IT_EOC);

	//extract adc value
	int8_t adc_val = ADC1->DRH;

	//set time 2 to create pulse of size (adc_val * 8)
	TIM2_SetCompare1(TIM2_RELOAD_VALUE - (((uint8_t)adc_val) * 8) + 1);

	//enable timer 2
	TIM2_Cmd(ENABLE);


	/**
	 * at this point further calculations will take place
	 */


#if EXPORT_RAW == 0

#if INTERRUPT_BASED == 1
		//toggle
		TOGGLE_STAGE_GATE_BIT(int_gate_state, STAGE1_GATE);
		if(GET_STAGE_GATE_BIT(int_gate_state, STAGE1_GATE)){
			(void)stage1_fir_filter_advance(adc_val, 0);
			return;
		}

		int16_t stage1_out = stage1_fir_filter_advance(adc_val, 1);


		//toggle
		TOGGLE_STAGE_GATE_BIT(int_gate_state, STAGE2_GATE);
		if(GET_STAGE_GATE_BIT(int_gate_state, STAGE2_GATE)){
			(void)stage2_fir_filter_advance(stage1_out, 0);
			return;
		}

		int32_t stage2_out = stage2_fir_filter_advance(stage1_out, 1);

		databuffer_write_new_data(stage2_out);
#else

#endif

#else

		uart1_tx_index = 1;
		UART1->DR = adc_val;

#endif


	return;
}

/**
 * Timer 2 update event interrupt
 *
 * Stop timer and force output low
 */
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, ITC_IRQ_TIM2_OVF)
{
	//disable timer 2
	TIM2_Cmd(DISABLE);

	//set counter value to 0 to ensure the pwm output is low
	TIM2_SetCounter(0);

	//clear interrupt pending flag
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);

}



/**
 * UART transfer complete
 */
INTERRUPT_HANDLER(UART1_TX_IRQHandler, ITC_IRQ_UART1_TX)
{
	UART1->SR &= ~(1U<<6U);

	if(--uart1_tx_index == 0){
		return;
	}

	UART1->DR = uart_tx_reg[uart1_tx_index-1];
}
