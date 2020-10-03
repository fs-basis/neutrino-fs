/* helper for different display CVFD implementations */
#if HAVE_DUCKBOX_HARDWARE
#include <driver/vfd.h>
#endif
<<<<<<< HEAD
#if HAVE_TRIPLEDRAGON
#include <driver/lcdd.h>
#endif
#if HAVE_SPARK_HARDWARE || HAVE_AZBOX_HARDWARE || HAVE_GENERIC_HARDWARE  || HAVE_ARM_HARDWARE
=======
#if HAVE_SPARK_HARDWARE || HAVE_GENERIC_HARDWARE  || HAVE_ARM_HARDWARE || HAVE_MIPS_HARDWARE
>>>>>>> c531f49e3... coolstream hardware remove part 9
#include <driver/simple_display.h>
#endif
