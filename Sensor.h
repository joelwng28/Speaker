/* ******** Sensor.h ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 3/28
 * ultrasonic sensor 
 * Final Lab
 * Saadallah
 */
 
//*************Sensor_Init********************************************
//  Initialize US Sensor
//  Inputs: None
//  Output: none
void Sensor_Init(void);

//*************Check_For_Movement********************************************
//  Use sensor to check if object is within the way
//  Inputs: None
//  Output: True/False
uint8_t Check_For_Movement(void);

//true or false flag for if movement is detected
extern uint8_t movement_Detected;
