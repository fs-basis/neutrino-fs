/* helper for different display CVFD implementations */
#if HAVE_DUCKBOX_HARDWARE
#include <driver/vfd.h>
#endif
#if HAVE_ARM_HARDWARE || HAVE_MIPS_HARDWARE
#if BOXMODEL_E4HD
#include <driver/lcdd.h>
#define CVFD CLCD
#else
#include <driver/simple_display.h>
#endif
#endif
#ifdef ENABLE_GRAPHLCD
#include <driver/nglcd.h>
#endif
