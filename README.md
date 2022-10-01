# Door-Locker-Security-Systems-
Developing a system to unlock a door using a password.
Drivers: GPIO, Keypad, LCD, Timer, UART, I2C, EEPROM, Buzzer and
DC-Motor - 
Microcontroller: ATmega16.

This project uses Two Atmega16 microcontrollers communicating together through UART
-HMI controller which deals with the user and gets the required data as the password though the Keypad.
-CONTROL controller which deals with the data which was sent from MC1, it takes the password through the UART then compares it
 with the one stored in the External EEPROM if it is correct it opens a menu which allows the user to Change the Password or Open the door.
