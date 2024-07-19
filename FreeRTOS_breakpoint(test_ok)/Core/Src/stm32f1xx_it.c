/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim8;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
#if 0
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}
#else

#define rt_uint32_t unsigned int
#define uint16_t    unsigned short
struct exception_info
{
    rt_uint32_t exc_return;
    rt_uint32_t r4;
    rt_uint32_t r5;
    rt_uint32_t r6;
    rt_uint32_t r7;
    rt_uint32_t r8;
    rt_uint32_t r9;
    rt_uint32_t r10;
    rt_uint32_t r11;
    rt_uint32_t r0;
    rt_uint32_t r1;
    rt_uint32_t r2;
    rt_uint32_t r3;
    rt_uint32_t r12;
    rt_uint32_t lr;
    rt_uint32_t pc;
    rt_uint32_t psr;
};

/* check the disassembly instruction is 'BL' or 'BLX' */
static int disassembly_ins_is_bl_blx(rt_uint32_t addr) {
    uint16_t ins1 = *((uint16_t *)addr);
    uint16_t ins2 = *((uint16_t *)(addr + 2));

#define BL_INS_MASK         0xF800
#define BL_INS_HIGH         0xF800
#define BL_INS_LOW          0xF000
#define BLX_INX_MASK        0xFF00
#define BLX_INX             0x4700

    if ((ins2 & BL_INS_MASK) == BL_INS_HIGH && (ins1 & BL_INS_MASK) == BL_INS_LOW) {
        return 1;
    } else if ((ins2 & BLX_INX_MASK) == BLX_INX) {
        return 1;
    } else {
        return 0;
    }
}

static int is_in_text(unsigned int addr)
{
	extern int * Image$$ER_IROM1$$Base;
	extern int * Image$$ER_IROM1$$Length;

	unsigned int text_start = (unsigned int)&Image$$ER_IROM1$$Base;
	unsigned int text_len = (unsigned int)&Image$$ER_IROM1$$Length;

	if ((addr >= text_start) && (addr < text_start + text_len))
		return 1;
	else
		return 0;
}

/*
 * fault exception handler
 */
void usage_fault_exception(struct exception_info * exception_info)
{
	unsigned int *app_sp;

	int i;
	app_sp = (unsigned int *)(exception_info + 1);  /* context + 16*4 */
	
    printf("psr: 0x%08x\r\n", exception_info->psr);
    printf("r00: 0x%08x\r\n", exception_info->r0);
    printf("r01: 0x%08x\r\n", exception_info->r1);
    printf("r02: 0x%08x\r\n", exception_info->r2);
    printf("r03: 0x%08x\r\n", exception_info->r3);
    printf("r04: 0x%08x\r\n", exception_info->r4);
    printf("r05: 0x%08x\r\n", exception_info->r5);
    printf("r06: 0x%08x\r\n", exception_info->r6);
    printf("r07: 0x%08x\r\n", exception_info->r7);
    printf("r08: 0x%08x\r\n", exception_info->r8);
    printf("r09: 0x%08x\r\n", exception_info->r9);
    printf("r10: 0x%08x\r\n", exception_info->r10);
    printf("r11: 0x%08x\r\n", exception_info->r11);
    printf("r12: 0x%08x\r\n", exception_info->r12);
    printf(" lr: 0x%08x\r\n", exception_info->lr);
    printf(" pc: 0x%08x\r\n", exception_info->pc);




    while (1);
}


unsigned int svc_exception(struct exception_info * exception_info)
{
	unsigned int addr;
	unsigned short swi_inst;
	unsigned int *old_sp;
	unsigned int val;
	unsigned int new_pos;
	unsigned int offset;
	struct exception_info stack_frame;
	
	/* 取出swi指令 */
	addr = exception_info->pc;
	addr -= 2;

	swi_inst = *((unsigned short *)addr);

    printf("use swi 0x%08x to emulat pop\r\n", swi_inst);

	/* 分析swi指令 */
	old_sp = (unsigned int *)(exception_info + 1);
	
	swi_inst = swi_inst & 0xff;

	/* 模拟swi指令 */
	if (swi_inst == 0xff)
	{
		// pop {pc}
		val = *old_sp;
		old_sp++;
		exception_info->pc = val;
	}
	else
	{
		
		if (swi_inst & (1<<0))
		{
			// pop {r0}
			val = *old_sp;
			old_sp++;
			exception_info->r0 = val;
		}

		if (swi_inst & (1<<1))
		{
			// pop {r1}
			val = *old_sp;
			old_sp++;
			exception_info->r1 = val;
		}

		if (swi_inst & (1<<2))
		{
			// pop {r2}
			val = *old_sp;
			old_sp++;
			exception_info->r2 = val;
		}
		

		if (swi_inst & (1<<3))
		{
			// pop {r3}
			val = *old_sp;
			old_sp++;
			exception_info->r3 = val;
		}

		if (swi_inst & (1<<4))
		{
			// pop {r4}
			val = *old_sp;
			old_sp++;
			exception_info->r4 = val;
		}

		if (swi_inst & (1<<5))
		{
			// pop {r5}
			val = *old_sp;
			old_sp++;
			exception_info->r5 = val;
		}

		if (swi_inst & (1<<6))
		{
			// pop {r6}
			val = *old_sp;
			old_sp++;
			exception_info->r6 = val;
		}

		if (swi_inst & (1<<7))
		{
			// pop {r7}
			val = *old_sp;
			old_sp++;
			exception_info->r7 = val;
		}

		// pop {pc}
		val = *old_sp;
		old_sp++;
		exception_info->pc = val;
		printf("exception_info->pc = 0x%08x\r\n", val);
		
	}

	/* 移动栈帧 */
	stack_frame = *exception_info;

	val = (unsigned int)(exception_info + 1); // old_sp
	new_pos = (unsigned int)old_sp;
	offset = new_pos - val;

	exception_info = (struct exception_info *)((unsigned int)exception_info + offset);
	*exception_info = stack_frame;
return offset;
	/* 返回 */
}

/*
 * fault exception handler
 */
void rt_hw_hard_fault_exception(struct exception_info * exception_info)
{
	unsigned int *app_sp;
	unsigned int lr;
	unsigned int pc;

	int i;
	app_sp = (unsigned int *)(exception_info + 1);  /* context + 16*4 */
	
    printf("psr: 0x%08x\r\n", exception_info->psr);
    printf("r00: 0x%08x\r\n", exception_info->r0);
    printf("r01: 0x%08x\r\n", exception_info->r1);
    printf("r02: 0x%08x\r\n", exception_info->r2);
    printf("r03: 0x%08x\r\n", exception_info->r3);
    printf("r04: 0x%08x\r\n", exception_info->r4);
    printf("r05: 0x%08x\r\n", exception_info->r5);
    printf("r06: 0x%08x\r\n", exception_info->r6);
    printf("r07: 0x%08x\r\n", exception_info->r7);
    printf("r08: 0x%08x\r\n", exception_info->r8);
    printf("r09: 0x%08x\r\n", exception_info->r9);
    printf("r10: 0x%08x\r\n", exception_info->r10);
    printf("r11: 0x%08x\r\n", exception_info->r11);
    printf("r12: 0x%08x\r\n", exception_info->r12);
    printf(" lr: 0x%08x\r\n", exception_info->lr);
    printf(" pc: 0x%08x\r\n", exception_info->pc);


#if 0
	printf("stacks: \r\n");
#else
	printf("use the command to get callback: arm-none-eabi-addr2line -e your.axf -a -f ");
#endif

	i = 0;
	for (i = 0; i < 1024; )
	{
#if 0		
		printf("%08x ", *app_sp);		
		app_sp++;
		i++;
		if (i % 16 == 0)
			printf("\r\n");
#else
		lr = *app_sp;
		app_sp++;
		i++;

		/* lr的bit0必定是1, 表示thumb指令集 
		 * lr必定处于代码段
		 */
		if ((lr & 1) && is_in_text(lr))
		{
			/* 返回地址前面必定是BL或BLX指令 */
			pc = (lr & ~1) - 4;
			if (disassembly_ins_is_bl_blx(pc))
			{
				printf("%08x ", pc);
			}
			
		}
#endif
	}
	printf("\r\n");

    while (1);
}


#endif
/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler_c(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    printf("UsageFault_Handler\r\n");
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles TIM8 update interrupt.
  */
void TIM8_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_UP_IRQn 0 */

  /* USER CODE END TIM8_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim8);
  /* USER CODE BEGIN TIM8_UP_IRQn 1 */

  /* USER CODE END TIM8_UP_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
