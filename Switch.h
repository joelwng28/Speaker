/* ******** Switch.h ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 2/20/2018
 * switch init, debouncing, and function control
 * Lab 5
 * Saadallah
 * 2/20/2018
 */

//*************Switches_Init********************************************
//  Initialize "Up" "Down" and "Select" Switches
//  Inputs: None
//  Output: none
void Switches_Init(void);

//*************Pause********************************************
// Pauses music
//  Inputs: None
//  Output: none
void Pause(void);

//*************Play********************************************
//  Plays music if music was paused
//  Inputs: None
//  Output: none
void Play(void);

//*************Mode********************************************
//  Changes the voice/instrument of the sound
//  Inputs: None
//  Output: none
void Mode(void);

//*************Rewind********************************************
//  Play music from the start
//  Inputs: None
//  Output: none
void Rewind(void);
