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

int32_t generic_fir_rom_opt_advance(uint32_t input, char output_cycle, uint32_t *filter_memory, uint32_t *filter_coeff, uint8_t filter_order);

int32_t generic_2ord_iir_advance(int32_t input, int32_t *filter_memory, int32_t *coeff);

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
#define STAGE7_ORDER	36
#define STAGE8_ORDER	6
#define STAGE_DCT_ORDER	6


static const int32_t stage4_coeff[STAGE4_ORDER + 1] = {-21671168, -57446912, 86228736, 515199488, 786390528, 515199488, 86228736, -57446912, -21671168};
static const int32_t stage5_coeff[STAGE5_ORDER + 1] = {4534784, 10265344, -26586880, -84064768, 65555456, 500018688, 765431552, 500018688, 65555456, -84064768, -26586880, 10265344, 4534784};
static const int32_t stage6_coeff[STAGE6_ORDER + 1] = {-560384, -2084096, 1233920, 15193088, 11409408, -49801472, -85861120, 95791104, 472342016, 678930944, 472342016, 95791104, -85861120, -49801472, 11409408, 15193088, 1233920, -2084096, -560384};
static const int32_t stage7_coeff[STAGE7_ORDER + 1] = {5120, 53504, 110336, -148992, -819968, -543232, 2222336, 4315392, -1463808, -12609280, -9195520, 19376640, 38183424, -4659968, -83977472, -71265280, 128718848, 417668352, 556807936, 417668352, 128718848, -71265280, -83977472, -4659968, 38183424, 19376640, -9195520, -12609280, -1463808, 4315392, 2222336, -543232, -819968, -148992, 110336, 53504, 5120};
static const int32_t stage8_coeff[STAGE8_ORDER][6] = {			{1090773676, 2147483647, 1090773676, 2147483647, -135395880, 15073542},
		       	   	   	   	   	   	   	 	 					{1229528409, 2147483647, 1229528409, 2147483647, -199920484, 120399219},
																{1517206275, 2147483647, 1517206275, 2147483647, -320336811, 330931847},
																{1953851202, 2147483647, 1953851202, 2147483647, -482300155, 651288925},
																{2147483647, 1866145324, 2147483647, 2147483647, -670197788, 1100739223},
																{2147483647, 1612882853, 2147483647, 2147483647, -871569473, 1731055081}};

static const int32_t stage_dct_coeff[STAGE_DCT_ORDER][6] = {	{1073741824, -2147483647,  1073741824,  1107726926, -2147483647, 1040816898},
																{1073741824, -2147483647,  1073741824,  1105447022, -2147483647, 1043096802},
																{1073741824, -2147483647,  1073741824,  1101042586, -2147483647, 1047501238},
																{1073741824, -2147483647,  1073741824,  1094813773, -2147483647, 1053730051},
																{1073741824, -2147483647,  1073741824,  1087185065, -2147483647, 1061358758},
																{1073741824, -2147483647,  1073741824,  1078676348, -2147483647, 1069867476}};


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

int32_t stage8_ir_filter_memory[STAGE8_ORDER][2] = {{0},{0},{0},{0},{0},{0},};
int32_t stage8_r_filter_memory[STAGE8_ORDER][2] = {{0},{0},{0},{0},{0},{0},};

int32_t stage_dct_ir_filter_memory[STAGE_DCT_ORDER][2] = {{0},{0},{0},{0},{0},{0},};
int32_t stage_dct_r_filter_memory[STAGE_DCT_ORDER][2] = {{0},{0},{0},{0},{0},{0},};


/* gating filter function, implements decimation of frequency */
#define STAGE1_GATE								(0x00U)
#define STAGE2_GATE								(0x01U)

#define STAGE3_GATE								(0x00U)
#define STAGE4_GATE								(0x01U)
#define STAGE5_GATE								(0x03U)
#define STAGE6_GATE								(0x04U)
#define STAGE7_GATE								(0x05U)
#define STAGE8_GATE								(0x06U)

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


		for(uint8_t i = 0; i < STAGE8_ORDER; i++){
			data_ir = generic_2ord_iir_advance(data_ir, stage8_ir_filter_memory[i], stage8_coeff[i]);
			data_r = generic_2ord_iir_advance(data_r, stage8_ir_filter_memory[i], stage8_coeff[i]);
		}

		for(uint8_t i = 0; i < STAGE_DCT_ORDER; i++){
			data_ir = generic_2ord_iir_advance(data_ir, stage_dct_ir_filter_memory[i], stage_dct_coeff[i]);
			data_r = generic_2ord_iir_advance(data_r, stage_dct_r_filter_memory[i], stage_dct_coeff[i]);
		}


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

#define LED_R_VALUE_FROM_MOD_TABLE(table_entry)			((uint8_t)((table_entry >> 4) & 0xf))
#define LED_IR_VALUE_FROM_MOD_TABLE(table_entry)		((uint8_t)(table_entry & 0xf))

volatile uint16_t led_r_brightness = 16;
volatile uint16_t led_ir_brightness = 160;

//static const uint8_t led_mod_table[] = {16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16};
static const uint8_t led_mod_table[] = {136,136,153,169,186,202,219,219,236,236,253,253,254,254,254,254,239,239,223,223,207,191,175,159,143,142,126,110,94,77,61,44,44,27,27,10,10,9,9,8,8,23,22,38,37,53,68,84,99,115,130,130,145,161,177,193,208,208,224,224,240,240,240,240,240,241,225,225,209,210,194,179,163,148,132,133,117,102,86,71,56,40,41,25,26,10,11,11,12,12,13,29,30,46,46,62,79,95,111,127,143,143,159,175,191,206,222,222,238,237,253,252,252,251,251,250,234,233,217,216,200,183,166,150,133,133,116,100,83,67,50,34,33,17,17,1,0,0,0,0,0,16,16,32,32,49,65,81,97,114,130,131,147,164,180,197,213,214,230,231,248,248,249,249,250,250,235,235,220,220,205,189,174,158,142,142,127,111,95,79,63,47,47,31,31,14,14,14,14,13,13,28,28,43,43,58,74,89,105,120,136,135,150,166,181,197,212,212,227,227,242,242,241,241,241,241,224,224,208,208,192,176,160,144,128,129,113,97,81,66,50,35,35,20,20,5,5,6,6,7,8,24,25,41,42,58,75,91,108,124,141,141,158,174,190,206,223,223,239,239,255,255,255,255,255,254,238,238,222,221,205,188,172,155,139,138,122,105,89,72,56,39,38,22,21,5,4,4,3,3,2,18,17,33,33,49,64,80,96,112,128,128,144,160,176,193,209,209,225,226,242,243,243,244,244,245,229,230,214,215,200,184,169,153,138,138,123,107,92,76,61,45,46,30,30,14,15,15,15,15,15,31,31,47,47,62,78,94,110,125,141,140,156,171,187,202,218,217,233,232,248,247,246,246,245,245,228,228,211,211,194,178,161,145,129,129,112,96,80,64,48,32,32,16,16,1,1,1,1,2,2,19,19,36,36,53,69,86,102,119};
static const uint16_t led_mod_table_size = sizeof(led_mod_table)/sizeof(led_mod_table[0]);
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

	// write new values from lookup table to compare registers to modulate leds
	TIM1_SetCompare4((uint16_t)(LED_R_VALUE_FROM_MOD_TABLE(led_mod_table[led_mod_table_idx]) * led_r_brightness));
	TIM1_SetCompare3((uint16_t)(LED_IR_VALUE_FROM_MOD_TABLE(led_mod_table[led_mod_table_idx]) * led_ir_brightness));

	//increment index
	++led_mod_table_idx;
	if(led_mod_table_idx == led_mod_table_size){
		led_mod_table_idx = 0;
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
