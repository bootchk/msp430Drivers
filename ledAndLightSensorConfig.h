
/*
 * Definitions for LEDAndLightSensor
 * Used in msp430Drivers/LEDAndLightSensor/
 *
 * Defines NSide and PSide
 *
 * PSide high and NSide low to light LED
 * PSide low and Nside high to reverse bias LED
 */

// requires board defined (see config.h)



#ifdef MYPCB_BOARD_R5


#define NSideLEDPort GPIO_PORT_P1
#define NSideLEDPin  GPIO_PIN7

#define PSideLEDPort GPIO_PORT_P1
#define PSideLEDPin  GPIO_PIN5


#elif defined(BLINKERBQ_BOARD)


#define NSideLEDPort GPIO_PORT_P2
#define NSideLEDPin  GPIO_PIN1

#define PSideLEDPort GPIO_PORT_P2
#define PSideLEDPin  GPIO_PIN0


#elif defined(PROTO_BOARD)


// P1.0 is redm P1.1 is green led on launchpad
#define NSideLEDPort GPIO_PORT_P1
#define NSideLEDPin  GPIO_PIN0

#define PSideLEDPort GPIO_PORT_P1
#define PSideLEDPin  GPIO_PIN1


#elif defined(FULL_PROTO_LAUNCHPAD)


#ifdef OLD
#define NSideLEDPort GPIO_PORT_P1
#define NSideLEDPin  GPIO_PIN7

#define PSideLEDPort GPIO_PORT_P1
#define PSideLEDPin  GPIO_PIN5
#else
#define NSideLEDPort GPIO_PORT_P2
#define NSideLEDPin  GPIO_PIN1

#define PSideLEDPort GPIO_PORT_P2
#define PSideLEDPin  GPIO_PIN0
#endif



#else
#error "No board defined."
#endif
