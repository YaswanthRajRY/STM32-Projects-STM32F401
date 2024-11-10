#ifndef STM32F401CCU6_H_
#define STM32F401CCU6_H_

#include <stdint.h>

/*----------------------------------------------------------*/
/*      FLASH register                                      */
/*----------------------------------------------------------*/
typedef struct
{
    volatile uint32_t ACR;      /*!< FLASH access control register,   Address offset: 0x00 */
    volatile uint32_t KEYR;     /*!< FLASH key register,              Address offset: 0x04 */
    volatile uint32_t OPTKEYR;  /*!< FLASH option key register,       Address offset: 0x08 */
    volatile uint32_t SR;       /*!< FLASH status register,           Address offset: 0x0C */
    volatile uint32_t CR;       /*!< FLASH control register,          Address offset: 0x10 */
    volatile uint32_t OPTCR;    /*!< FLASH option control register ,  Address offset: 0x14 */
    volatile uint32_t OPTCR1;   /*!< FLASH option control register 1, Address offset: 0x18 */
} FLASH_TypeDef;

/*----------------------------------------------------------*/
/*      Power control register                              */
/*----------------------------------------------------------*/
typedef struct
{
    volatile uint32_t CR;   /*!< PWR power control register,        Address offset: 0x00 */
    volatile uint32_t CSR;  /*!< PWR power control/status register, Address offset: 0x04 */
} PWR_TypeDef;

/*----------------------------------------------------------*/
/*      Reset and control clock (RCC)                       */
/*----------------------------------------------------------*/
typedef struct
{
    volatile uint32_t CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
    volatile uint32_t PLLCFGR;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
    volatile uint32_t CFGR;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
    volatile uint32_t CIR;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
    volatile uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
    volatile uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
    volatile uint32_t AHB3RSTR;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
             uint32_t RESERVED0;     /*!< Reserved, 0x1C                                                                    */
    volatile uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
    volatile uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
             uint32_t RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
    volatile uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
    volatile uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
    volatile uint32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
             uint32_t RESERVED2;     /*!< Reserved, 0x3C                                                                    */
    volatile uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
    volatile uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
             uint32_t RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
    volatile uint32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
    volatile uint32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
    volatile uint32_t AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
             uint32_t RESERVED4;     /*!< Reserved, 0x5C                                                                    */
    volatile uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
    volatile uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
             uint32_t RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
    volatile uint32_t BDCR;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
    volatile uint32_t CSR;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
             uint32_t RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
    volatile uint32_t SSCGR;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
    volatile uint32_t PLLI2SCFGR;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
             uint32_t RESERVED7[1];  /*!< Reserved, 0x88                                                                    */
    volatile uint32_t DCKCFGR;       /*!< RCC Dedicated Clocks configuration register,                 Address offset: 0x8C */
} RCC_TypeDef;

/*----------------------------------------------------------*/
/*      General purpose input output register (GPIO)        */
/*----------------------------------------------------------*/
typedef struct
{
    volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
    volatile uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
    volatile uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
    volatile uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    volatile uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
    volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
    volatile uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
    volatile uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
    volatile uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;

/*----------------------------------------------------------*/
/*      Timer/Counter Register                              */
/*----------------------------------------------------------*/
typedef struct
{
    volatile uint32_t CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
    volatile uint32_t CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
    volatile uint32_t SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
    volatile uint32_t DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
    volatile uint32_t SR;          /*!< TIM status register,                 Address offset: 0x10 */
    volatile uint32_t EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
    volatile uint32_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
    volatile uint32_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
    volatile uint32_t CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
    volatile uint32_t CNT;         /*!< TIM counter register,                Address offset: 0x24 */
    volatile uint32_t PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
    volatile uint32_t ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
    volatile uint32_t RCR;         /*!< TIM repetition counter register,     Address offset: 0x30 */
    volatile uint32_t CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
    volatile uint32_t CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
    volatile uint32_t CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
    volatile uint32_t CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
    volatile uint32_t BDTR;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
    volatile uint32_t DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
    volatile uint32_t DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
    volatile uint32_t OR;          /*!< TIM option register,                 Address offset: 0x50 */
} TIM_TypeDef;

/*----------------------------------------------------------*/
/*      UART register                                       */
/*----------------------------------------------------------*/
typedef struct
{
    volatile uint32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
    volatile uint32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
    volatile uint32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
    volatile uint32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
    volatile uint32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
    volatile uint32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
    volatile uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_TypeDef;

/*********************************************************************************************************************/
/*      Peripheral memory map                                                                                        */
/*********************************************************************************************************************/

#define PWR_BASE_ADDR       (0x40007000)

#define FLASH_BASE_ADDR     (0x40023C00)

#define RCC_BASE_ADDR       (0x40023800)

#define GPIOA_BASSE_ADDR    (0x40020000)
#define GPIOC_BASSE_ADDR    (0x40020800)

#define TIM11_BASE_ADDR     (0x40014800)

#define USART1_BASE_ADDR    (0x40011000)

/*********************************************************************************************************************/
/*      Peripheral declaration                                                                                               */
/*********************************************************************************************************************/

#define PWR         ((PWR_TypeDef*) PWR_BASE_ADDR)

#define FLASH       ((FLASH_TypeDef*) FLASH_BASE_ADDR)

#define RCC         ((RCC_TypeDef*) RCC_BASE_ADDR)

#define GPIOA       ((GPIO_TypeDef*) GPIOA_BASSE_ADDR)
#define GPIOC       ((GPIO_TypeDef*) GPIOC_BASSE_ADDR)

#define TIM11       ((TIM_TypeDef*) TIM11_BASE_ADDR)

#define UART1       ((USART_TypeDef*) USART1_BASE_ADDR)

/*********************************************************************************************************************/
/*      Bit definition                                                                                               */
/*********************************************************************************************************************/
/*---------
|  FLASH  |
----------*/
/* FLASH->ACR */
#define DCEN         (0x1U << 10)
#define ICEN         (0x1U << 9)
#define PRFTEN       (0x1U << 8)
#define LATENCY_2WS  (0x2U)

/*---------
|  PWR    |
----------*/
/* PWR->CR */
#define VOS_SCALE_2_MODE (0x2U << 14)

/*---------
|  RCC    |
----------*/
/* RCC->CR */
#define HSI_ON      (0x1U)
#define HSI_RDY     (0x1U << 1)
#define HSE_ON      (0x1U << 16)
#define HSE_RDY     (0x1U << 17)
#define PLLON       (0x1U << 24)
#define PLLRDY      (0x1U << 24)
/* RCC->PLLCFGR */
#define PLLSRC_HSE  (0x1U << 22)
/* RCC->CFGR*/
#define AHB_PRE_DIV1    (0x0U << 7)
#define APB1_PRE_DIV2   (0x0U << 12)
#define APB2_PRE_DIV1   (0x0U << 15)
#define SWS             (0xCU)
#define SW_PLL          (0x2U)
#define SW_PLL_MASK     (SW_PLL << 2)
/* RCC->AHB1ENR*/

/* RCC->APB1ENR */
#define PWR_EN      (0x1U << 28)

/*----------
|  GPIO    |
-----------*/
/* GPIO->MODER */

#endif