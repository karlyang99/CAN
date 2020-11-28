# CAN related practice, 2020-11-21

- Requires installation and setup of influxDB and kafka
https://kafka.apache.org/downloads

-written in golang

2020-11-24

https://www.youtube.com/watch?v=quZWGSEMyD0

HAL #8: HowTo - Timer PWM

HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);


2020-11-27

  HAL_TIM_Base_Start_IT(&htim16);
  
  htim16.Instance = TIM16;
  
  htim16.Init.Prescaler = 48-1;
  
  htim16.Init.Period = 1000-1;
  
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)

{

  /* USER CODE BEGIN Callback 0 */
  
  static int tim16_step = 0;
  
  static int tim16_counter  = 0;
  
  static int tim16_edge[18] =   {   80,   80,  100,  160,  160,  160,  100,   80,   80,
		  	  	  	  	  	  	    80,   80,  100,  160,  160,  160,  100,   80,   80 };
								    
  static int tim16_output[18] = { 0x00, 0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08, 0x00,
  	  	  	  	  	  	  	      0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xE0, 0xC0, 0x80, 0x00 };
							      

  /* USER CODE END Callback 0 */
  
  if (htim->Instance == TIM6) {
  
    HAL_IncTick();
    
  }
  
  /* USER CODE BEGIN Callback 1 */
  
  if (htim == &htim16)
  
  {
  
	if (++tim16_counter >= tim16_edge[tim16_step])
	
	{
	
	  tim16_counter  = 0;
	  
      // temp = tim16_output[tim16_step +1];

	  tim16_step ++;
	  
	  if (tim16_step >= 18)
	  {
		  tim16_step = 0;
		  tim16_counter  = 0;
	  }

	  if (tim16_step%9 == 0)
	  {
		  HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	  }
	}
  }
  
  /* USER CODE END Callback 1 */
  
}
