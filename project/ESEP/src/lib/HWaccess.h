/*
 * Steuerung ob Simulation oder HW Plattform (C Interface)
 *
 * Diese Datei MUSS ueberall includiert werden, wo auf die HW zugegriffen wird.
 *
 */


/*
 * Ueber SIMULATION wird zwischen Simulation und HW umgeschaltet
 *
 * Damit die Aenderungen beachtet werden, muss Check Dependencies On/Off -> Check User Headers Only
 * gesetzt sein.
 *
 * Zur Not : Ein Clean auf das Projekt ausführen
 */

// Die drei folgenden includes dürfen nur in dieser Datei stehen

#include <stdio.h>
#include <sys/neutrino.h>
#include <hw/inout.h>

// End of File

#ifdef SIMULATION
#include <ioaccess.h>
#endif
