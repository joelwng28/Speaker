 /* ******** Display.c ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 4/2/2018
 * images and data points
 */

#include <stdint.h>
#include "AlarmClock.h"
#include "ST7735.h"
#include "string.h"
#include "Globals.h"

uint8_t dayTime = 0;

/****** displayWeatherIcon ***********
 * Display weather icon from corresponding weather code
 * Inputs: an integer from the weather api that corresponds to weather status
 * Output: None
 */
void displayWeatherIcon(int icon){
	if((icon/100) == 2){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon38B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon38W, 16, 16);
		}
	}
	else if((icon/100) == 3){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon8B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon8W, 16, 16);
		}
	}
	else if((icon/100) == 5){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon12B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon12W, 16, 16);
		}
	}
	else if((icon/100) == 6){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon7B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon7W, 16, 16);
		}
	}
	else if((icon/100) == 7){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon20B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon20W, 16, 16);
		}
	}
	else if(icon == 800){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon32B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon31W, 16, 16);
		}
	}
	else if((icon/100) == 8){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon28B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon27W, 16, 16);
		}
	}
	else if(icon == 800){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon32B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon31W, 16, 16);
		}
	}
	else if(icon == 900){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon0B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon0W, 16, 16);
		}
	}
	else if(icon == 901 || icon == 902){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon1B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon1W, 16, 16);
		}
	}
	else if(icon == 903){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon13B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon13W, 16, 16);
		}
	}
	else if(icon == 904){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon25B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon25W, 16, 16);
		}
	}
	else if(icon == 905){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon22B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon22W, 16, 16);
		}
	}
	else if(icon == 906){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon19B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon19W, 16, 16);
		}
	}
	else if((icon/10) == 95){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon23B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon23W, 16, 16);
		}
	}
	else if((icon/10) == 96){
		if(dayTime){
			ST7735_DrawBitmap(112, 15, icon1B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon1W, 16, 16);
		}
	}
	else{
		if(dayTime){
			ST7735_DrawBitmap(112, 15, naB, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, naW, 16, 16);
		}
	}
}

//Draws digits to display time
/****displayDigits *********
* Print out digits
* Inputs: hour and minute numbers
* Outputs: None
*/
void displayDigits(int hour, int min){
	if(hour < 10){
		ST7735_DrawBitmap(0, 105, zero, 25, 50);
	}
	else if(hour < 20){
		ST7735_DrawBitmap(0, 105, one, 25, 50);
	}
	else{
		ST7735_DrawBitmap(0, 105, two, 25, 50);
	}
	int digit = (hour % 10);
	if(digit == 0){
		ST7735_DrawBitmap(25, 105, zero, 25, 50);
	}
	else if(digit == 1){
		ST7735_DrawBitmap(25, 105, one, 25, 50);
	}
	else if(digit == 2){
		ST7735_DrawBitmap(25, 105, two, 25, 50);
	}
	else if(digit == 3){
		ST7735_DrawBitmap(25, 105, three, 25, 50);
	}
	else if(digit == 4){
		ST7735_DrawBitmap(25, 105, four, 25, 50);
	}
	else if(digit == 5){
		ST7735_DrawBitmap(25, 105, five, 25, 50);
	}
	else if(digit == 6){
		ST7735_DrawBitmap(25, 105, six, 25, 50);
	}
	else if(digit == 7){
		ST7735_DrawBitmap(25, 105, seven, 25, 50);
	}
	else if(digit == 8){
		ST7735_DrawBitmap(25, 105, eight, 25, 50);
	}
	else if(digit == 9){
		ST7735_DrawBitmap(25, 105, nine, 25, 50);
	}
	ST7735_DrawBitmap(59, 105, colon, 10, 50);
	if(min < 10){
		ST7735_DrawBitmap(77, 105, zero, 25, 50);
	}
	else if(min < 20){
		ST7735_DrawBitmap(77, 105, one, 25, 50);
	}
	else if(min < 30){
		ST7735_DrawBitmap(77, 105, two, 25, 50);
	}
	else if(min < 40){
		ST7735_DrawBitmap(77, 105, three, 25, 50);
	}
	else if(min < 50){
		ST7735_DrawBitmap(77, 105, four, 25, 50);
	}
	else{
		ST7735_DrawBitmap(77, 105, five, 25, 50);
	}
	digit = (min % 10);
	if(digit == 0){
		ST7735_DrawBitmap(102, 105, zero, 25, 50);
	}
	else if(digit == 1){
		ST7735_DrawBitmap(102, 105, one, 25, 50);
	}
	else if(digit == 2){
		ST7735_DrawBitmap(102, 105, two, 25, 50);
	}
	else if(digit == 3){
		ST7735_DrawBitmap(102, 105, three, 25, 50);
	}
	else if(digit == 4){
		ST7735_DrawBitmap(102, 105, four, 25, 50);
	}
	else if(digit == 5){
		ST7735_DrawBitmap(102, 105, five, 25, 50);
	}
	else if(digit == 6){
		ST7735_DrawBitmap(102, 105, six, 25, 50);
	}
	else if(digit == 7){
		ST7735_DrawBitmap(102, 105, seven, 25, 50);
	}
	else if(digit == 8){
		ST7735_DrawBitmap(102, 105, eight, 25, 50);
	}
	else if(digit == 9){
		ST7735_DrawBitmap(102, 105, nine, 25, 50);
	}

}


