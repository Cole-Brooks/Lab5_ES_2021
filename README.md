# Lab5_ES_2021
### Objective:
Gain some experience with C-based programming of AVR microcontrollers, serial interface protocols, ADC, and DAC

### Lab Description:
In this lab, you will build a remotely controllable analog signal/data logging system using the built-in A/D converter of the ATmega328P controller and the MAX518, an external two-channel D/A converter with an I2C interface. Important aspects of the utilized ADC and DAC will be covered in class (review the relevant lecture notes). The MAX518 chip and associated components are included in the kit.

The system will have an RS232 interface (9600 8N1) that will be connected to your labtop/computer. The computer user will be able to trigger analog voltage measurements and set the output voltage for both channels of the DAC by means of commands sent through the RS232 interface.

The system will implement the following commands.

![image](https://user-images.githubusercontent.com/55246154/114051864-195a7c00-9853-11eb-95e2-79df866d0b35.png)

Here is the nominal message exchange between the person typing on the terminal keyboard and analog interface system, which will send information back to the terminal screen (use the Arduino IDE Serial Monitor functionality as terminal emulator). Text shown in BOLD is typed by the user on the keyboard. Non-bold text represents responses by the analog interface system.

![image](https://user-images.githubusercontent.com/55246154/114051951-2e370f80-9853-11eb-848f-5a1e13c40764.png)

After completing the assignment, upload your C source code and report to ICON (note that one source code file and report per group is sufficient). If you plan to see a TA on the day the lab is due, a sign up for check offis  required (see calendar on ICON). 

Note that you can use available libraries for I2C and RS232 communication. Do not forget to mention the used libraries in your report. 

For check off, develop a test approach so that you can demonstrate to the TA that the read ADC as well as generated DAC voltages are correct.

Mid-lab review - Show a TA your progress towards completing Lab5 (use the ICON calendar for sign up). It is expected that you can demonstrate that your program can successfully demonstrate RS232-based communication and reading of ADC voltage values. 
