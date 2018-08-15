# Speaker
Embedded Systems Project
Lab 5 Report
Samuel Zhang (shz96), Zi Zhou Wang (zw3948)
Saadallah
3/1/2018

Objectives
Requirements document
As always, feel free to adjust the syntax and format of your requirements document as you think appropriate. The goal of the document is to provide a clear an unambiguous description of what the project does.  
1. Overview
  1.1. Objectives: Why are we doing this project? What is the purpose?
The objectives of this project are to design, build and test a music player. Educationally, students are learning how to interface a DAC, how to design a speaker amplifier, how to store digital music in ROM, and how to perform DAC output in the background. Your goal is to play your favorite song.
 
  1.2. Process: How will the project be developed?
The project will be developed using the TM4C123 board. There will be two or three switches that the operator will use to control the music player. The system will be built on a solderless breadboard and run on the usual USB power. The system may use the on board switches or off-board switches. A hardware/software interface will be designed that allows software to control the player. There will be at least three hardware/software modules: switch input, DAC output, and the music player.  The process will be to design and test each module independently from the other modules. After each module is tested, the system will be built and tested.
 
  1.3. Roles and Responsibilities: Who will do what?  Who are the clients?
EE445L students are the engineers and the TA is the client. Students are expected to make minor modifications to this document in order to clarify exactly what they plan to build. Students are allowed to divide responsibilities of the project however they wish, but, at the time of demonstration, both students are expected to understand all aspects of the design.
Everything was done together in person. Joel was responsible for the Music.h and Music.c and creating the structs for the music itself. Samuel compiled all the drivers together for the timers, ADC, and DAC. The hardware was put together by both students. The music playing functionality was done together in person so that each person understood the code.

  1.4. Interactions with Existing Systems: How will it fit in?
            	The system will use the TM4C123 board, a solderless breadboard, and the speaker as shown in Figure 5.1. It will be powered using the USB cable. You may use a +5V power from the lab bench, but please do not power the TPA731 or the speaker with a voltage above +5V.
 
  1.5. Terminology: Define terms used in the document.
SSI - is a widely used serial interface standard for industrial applications between a master (e.g. controller) and a slave (e.g. sensor)
Linearity - the property of a mathematical relationship or function which means that it can be graphically represented as a straight line. Examples are the relationship of voltage and current across a resistor (Ohm's law), or the mass and weight of an object
Frequency response - the dependence on signal frequency of the output–input ratio of an amplifier or other device.
Loudness - That attribute of auditory sensation in terms of which sounds can be ordered on a scale extending from quiet to loud
Pitch - the quality of a sound governed by the rate of vibrations producing it; the degree of highness or lowness of a tone
Instrument - a tool or implement, especially one for delicate or scientific work.
Tempo - the speed at which a passage of music is or should be played.
Envelope - a covering or containing structure or layer
Melody - a sequence of single notes that is musically satisfying
Harmony - the combination of simultaneously sounded musical notes to produce chords and chord progressions having a pleasing effect
 
  1.6. Security: How will intellectual property be managed?
The system may include software from StellarisWare and from the book. No software written for this project may be transmitted, viewed, or communicated with any other EE445L student past, present, or future (other than the lab partner of course). It is the responsibility of the team to keep its EE445L lab solutions secure.
 
2. Function Description
  2.1. Functionality: What will the system do precisely?
If the operator presses the play/pause button the music will play or pause. If the operator presses the play/pause button once the music should pause. Hitting the play/pause again causes music to continue. The play/pause button does not restart from the beginning, rather it continues from the position it was paused. If the rewind button is pressed, the music stops and the next play operation will start from the beginning of the song. There is a mode switch that allows the operator to control some aspect of the player. The mode switch changes the “instrument” or “voice” of the music being played.

As extra credit, we will implement a slidepot that allows you to pick where to start in the music and the “fourth” ADC button will make the music start at that point.

There will be a default envelope to each note played that is independent of the frequency of the note.

There must be a C data structure to hold the music. There must be a music driver that plays songs. The length of the song should be at least 30 seconds and comprise of at least 8 different frequencies. Although you will be playing only one song, the song data itself will be stored in a separate place and be easy to change. The player runs in the background using interrupts. The foreground (main) initializes the player, then executes for(;;){} do nothing loop. If you wish to include LCD output, this output should occur in the foreground. The maximum time to execute one instance of the pitch ISR is 1.45 microseconds, this sends the voltage into the DAC. It takes 1.8 microseconds to change the current note. You will need public functions Rewind, Play and Stop, which perform operations like a cassette tape player. The Play function has an input parameter that defines the song to play. A background thread implemented with output compare will fetch data out of your music structure and send them to the DAC.  
There must be a C data structure to store the sound waveform or instrument. You are free to design your own format, as long as it uses a formal data structure (i.e., struct). The generated music must sound beautiful utilizing the SNR of the DAC. Although you only have to implement one instrument, it should be easy to change instruments.
 
  2.2. Scope: List the phases and what will be delivered in each phase.
Phase 1 is the preparation; phase 2 is the demonstration; and phase 3 is the lab report. Details can be found in the lab manual.
 
  2.3. Prototypes: How will intermediate progress be demonstrated?
A prototype system running on the TM4C123 board and solderless breadboard will be demonstrated. Progress will be judged by the preparation, demonstration and lab report.
 
  2.4. Performance: Define the measures and describe how they will be determined.
The system will be judged by three qualitative measures. First, the software modules must be easy to understand and well-organized. Second, the system must employ an abstract data structures to hold the sound and the music. There should be a clear and obvious translation from sheet music to the data structure. Backward jumps in the ISR are not allowed. Waiting for SSI output to complete is an acceptable backwards jump. Third, all software will be judged according to style guidelines. Software must follow the style described in Section 3.3 of the book. There are three quantitative measures. First, the SNR of the DAC output of a sine wave should be measured. Second, the maximum time to run one instance of the ISR will be recorded. Third, you will measure power supply current to run the system. There is no particular need to optimize any of these quantitative measures in this system.
 
  2.5. Usability: Describe the interfaces. Be quantitative if possible.
There will be four switch inputs and a slidepot. The DAC will be interfaced to a 8-ohm speaker. 
 
  2.6. Safety: Explain any safety requirements and how they will be measured.
    	If you are using headphones, please verify the sound it not too loud before placing the phones next to your ears.  
 
3. Deliverables
  3.1. Reports: How will the system be described?
A lab report described below is due by the due date listed in the syllabus. This report includes the final requirements document.
 
  3.2. Audits: How will the clients evaluate progress?
The preparation is due at the beginning of the lab period on the date listed in the syllabus.
 
  3.3. Outcomes: What are the deliverables? How do we know when it is done?
There are three deliverables: preparation, demonstration, and report. 


Hardware Design



Measurement Data
Our reference voltage is set to 1.5.
The resolution we calculated is 1.28 mV per increment value of our DAC. We found this using the equation: (2800mV - 2480mV) / (2000 - 1750) = 1.28mV
Our maximum DAC value is 2343, by the following equation: (2 * 1.5) V / 1.28 mV/DAC = 2343 DAC
The precision of our DAC is 4096. For a theoretical voltage for a DAC value of 1500, we got 1.465V using the equation: (1500 * 2 * 2) / 4096 = 1.465V
The actual voltage for a DAC output of 1500 is 2.16V.
Going by this calculation, our accuracy is 47.44%, via the equation: 1 - (2.16V - 1.465V) / 1.465V = 0.4744 = 47.44%
Here are 8 DAC output measurements:

DAC Output Voltage for 2000: 2.80V

DAC Output Voltage for 1750: 2.48V

DAC Output Voltage for 1500: 2.16V

DAC Output Voltage for 1250: 1.76V

DAC Output Voltage for 1000: 1.44V

DAC Output Voltage for 750: 1.04V

DAC Output Voltage for 500: 0.72V

DAC Output Voltage for 250: 0.32V
After measuring the time domain and frequency domain of our system at 440Hz, we captured the following output:

Given the above output, we calculate our SNR to be 42.4 dB


From the above measurements, the time our program spends in the ISR is (1.8us)/(31.8us) = 5.66%

Time domain at 440Hz

Frequency domain at 440Hz

+5V  Voltage

Current without sound

Current with sound
Analysis and Discussion
Three possible errors on a DAC could include the following:
noise produced at the same frequencies as the output frequencies
bad resistor choices for producing the DAC increments
the operational amplifier could produce noise or other artifacts in the output
With SPH and SPO set to 0, the data available interval is 25ns. The data required interval depends on both setup time and hold time, so 13ns in total. Given that our clock speed was set to 16MHz, our clock period interval was 62.5ns. Since the smallest clock low width is 25ns, then the shortest SSI period must be 62.5 + 25 = 87.5ns. This corresponds to a max SSI clock of 11.428MHz. We used an SSI clock of 8MHz, below that maximum.
Frequency range of a spectrum analyzer is determined by dividing the sampling frequency by 2 (Nyquist thm.)
We did not drive the speaker directly from the DAC because the current required for any decent volume is too high for the board's GPIO pins to supply. The amp (TPA731) helps take care of the current problem by providing a current gain but a voltage decrease.

