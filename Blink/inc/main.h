#include "stm32f401ccu6.h"

#define RCC_CR                  *(volatile uint32_t*) 0x40023800
#define RCC_PLLCFGR             *(volatile uint32_t*) 0x40023804
#define RCC_CFGR                *(volatile uint32_t*) 0x40023808
#define RCC_AHB1ENR             *(volatile uint32_t*) 0x40023830
#define RCC_APB1ENR             *(volatile uint32_t*) 0x40023840
#define RCC_APB2ENR             *(volatile uint32_t*) 0x40023844

#define PWR_CR                  *(volatile uint32_t*) 0x40007000

#define FLASH_ACR               *(volatile uint32_t*) 0x40023C00

#define GPIOC_MODER             *(volatile uint32_t*) 0x40020800
#define GPIOC_OTYPER            *(volatile uint32_t*) 0x40020804
#define GPIOC_OSPEEDR           *(volatile uint32_t*) 0x40020808
#define GPIOC_BSRR              *(volatile uint32_t*) 0x40020818

#define TIM11_CR1               *(volatile uint32_t*) 0x40014800
#define TIM11_SR                *(volatile uint32_t*) 0x40014810
#define TIM11_CNT               *(volatile uint32_t*) 0x40014824
#define TIM11_PSC               *(volatile uint32_t*) 0x40014828
#define TIM11_ARR               *(volatile uint32_t*) 0x4001482C

#define RCC_CR_HSION            1
#define RCC_CR_HSIRDY           (1 << 1)
#define RCC_APB1ENR_PWREN       (1 << 28)
#define RCC_CFGR_HPRE_DIV2      (1 << 7)
#define RCC_CFGR_PPRE1_DIV1     (0 << 12)
#define RCC_CFGR_PPRE2_DIV1     (0 << 15)
#define RCC_CR_PLLON            (1 << 24)
#define RCC_CR_PLLRDY           (1 << 25)
#define RCC_CFGR_SW_PLL         2
#define RCC_CFGR_SWS_PLL        8
#define RCC_CFGR_SWS            (3 << 2)
#define RCC_AHB1ENR_GPIOCEN     (1 << 2)
#define FLASH_ACR_LATENCY_1WS   1
#define FLASH_ACR_DCEN          (1 << 10)
#define FLASH_ACR_ICEN          (1 << 9)
#define FLASH_ACR_PRFTEN        (1 << 8)
#define PWR_CR_VOS              0
#define MODER_PIN_13            (1U << 26)
#define SET_PIN_13              (1U << 13)
#define RESET_PIN_13            (1U << 29)
#define TIM11_EN                (1U << 18)
#define PSC_VALUE               41
#define ARR_VALUE               0xFFFF