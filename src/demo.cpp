#include "stm32-template.h"

Gpio led_green(GPIOC, GPIO_Pin_9, RCC_APB2Periph_GPIOC );
Gpio led_blue(GPIOC, GPIO_Pin_8, RCC_APB2Periph_GPIOC );

void setup() {

	led_green.init(GPIO_Mode_Out_PP);
	led_blue.init(GPIO_Mode_Out_PP);

	nvic.configure(TIM2_IRQn, 0, 3, ENABLE);
	Tim t2(TIM2, RCC_APB1Periph_TIM2, RCC_APB1PeriphClockCmd);
	t2.init(1000, 1000);
	t2.configureIT(TIM_IT_Update);
	t2.setState();

	fprintf(stdout, "char: %d bytes.\r\n", sizeof(char));
	fprintf(stdout, "short: %d bytes.\r\n", sizeof(short));
	fprintf(stdout, "int: %d bytes.\r\n", sizeof(int));
	fprintf(stdout, "long: %d bytes.\r\n", sizeof(long));
	fprintf(stdout, "float: %d bytes.\r\n", sizeof(float));
	fprintf(stdout, "double: %d bytes.\r\n", sizeof(double));

}

void loop() {

	static uint8_t i = 0;
	while (usart.available()) {
		char c = usart.read();
		fprintf(stdout, "0x%02X\r\n", c);
		led_blue.toggle();
	}

	fprintf(stdout, "0x%02x\r\n", i++);

	//led_blue.toggle();
	delayMicroseconds(500000);
}

