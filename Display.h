 /* ******** Display.h ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 4/2/2018
 * images and data points
 */
#include <stdint.h>

/****** displayWeatherIcon ***********
 * Display weather icon from corresponding weather code
 * Inputs: an integer from the weather api that corresponds to weather status
 * Output: None
 */
void displayWeatherIcon(int icon);

/****displayDigits *********
* Print out digits
* Inputs: hour and minute numbers
* Outputs: None
*/
void displayDigits(int hour, int min);
