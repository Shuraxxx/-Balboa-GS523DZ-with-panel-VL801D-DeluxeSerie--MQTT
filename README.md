# -Balboa-GS523SZ-with-panel-VL801D-DeluxeSerie-MQTT
 Balboa-GS523SZ с панелью VL801D (DeluxeSerie)

<img src="https://github.com/Shuraxxx/-Balboa-GS523SZ-with-panel-VL801D-DeluxeSerie-/blob/main/src/img/balboa_GS523DZ-VL810D.jpg" width="400"> <img src="https://github.com/Shuraxxx/-Balboa-GS523SZ-with-panel-VL801D-DeluxeSerie-/blob/main/src/img/VL801D-balboa-panel-spa-studio-WEB_ex.jpg" width="400">
## RJ45 pins
<img src="https://github.com/Shuraxxx/-Balboa-GS523SZ-with-panel-VL801D-DeluxeSerie-/blob/main/src/img/RJ45.png" width="200">

| PIN           | Description             | 
| ------------- | ------------------------|
| PIN 1         | Functionality unclear!  |
| PIN 2         | Functionality unclear!  |
| PIN 3         | Button data             |  
| PIN 4         | GND                     |
| PIN 5         | Display data            | 
| PIN 6         | Clock                   |
| PIN 7         | 5 VDC                   |  
| PIN 8         | Floating output         |

##PIN 6 (Yellow - clock), PIN 5 (Blue - display data), PIN 3 (Pink - button data)

<img src="https://github.com/Shuraxxx/-Balboa-GS523SZ-with-panel-VL801D-DeluxeSerie-/blob/main/src/img/DATA-1.jpg" width="800"> 
<img src="https://github.com/Shuraxxx/-Balboa-GS523SZ-with-panel-VL801D-DeluxeSerie-/blob/main/src/img/DATA-2.jpg" width="800"> 

#### Conclusion

- The clock is pulsing 7 times for each chunck of display data, totaly 71 bits and 4 times for button data. A complete cycle 75 pulses. <br />
- Totaly of 6 display data chunks in each cycle <br />
- Each cycle is repeted every 10 ms <br />
- One total cycle duration 3.54 ms
- Bit duration for display data is 23.75 microseconds for display data <br />
- Bit duration for button data is 71.5 microseconds <br />
- 
#### Display Data

Display shows **29.5C** - How does that translates into the bitstream? The first 4 chunks is coded in BCD to 7 segment LCD (see reference) but only with 7 bits sinse the LSB always is (0). <br />
2 = 1101101(0) = (Chunk 1) <br />
9 = 1110011(0) = (Chunk 2) <br />
5 = 1011011(0) = (Chunk 3) <br />
C = 1001110(0) = (Chunk 4) <br />

| Chunk 4 - bit(0-7) | Chunk 3 - bit(7-14) | Chunk 2 - bit(14-21) | Chunk 4 - bit(21-28) | Chunk 5 - bit(29-71) | 
| ------------------ | ------------------- | ------------------   | -------------------- | -------------------- |                     
| LCD segment 1      |  LCD segment 2      |   LCD Segment 3      |   LCD segment 4      | 29 - Button down/up  |                    
|                    |                     |                      |                      |                      |       
|                    |                     |                      |                      |                      | 
|                    |                     |                      |                      | 59 - Standard mode   | 
|                    |                     |                      |                      |                      |  
|                    |                     |                      |                      |                      |   
|                    |                     |                      |                      |                      | 
<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/extras/docs/Balboa_GS_Interface.jpg" width="1000">
## PCB fabrication

<img src="(https://github.com/Shuraxxx/-Balboa-GS523SZ-with-panel-VL801D-DeluxeSerie-/blob/main/extras/PCB%20layout/PCB_3D_view.jpg" width="303"> <img src="(https://github.com/Shuraxxx/-Balboa-GS523SZ-with-panel-VL801D-DeluxeSerie-/blob/main/extras/PCB%20layout/PCB_2D_view.jpg)" width="400">

Gerber files located here: [PCB Balboa-GS510SZ](https://github.com/Shuraxxx/-Balboa-GS523SZ-with-panel-VL801D-DeluxeSerie-/tree/main/extras/PCB%20layout )

## References of great help!
- https://github.com/NickB1/OpenSpa/blob/master/documents/Balboa/Balboa_Display_Measurements.pdf
- https://www.olivierhill.ca/archives/72-The-Internet-of-Spas.html
- https://create.arduino.cc/projecthub/meljr/7-segment-led-displays-102-using-a-shift-register-6b6976

## Other Balboa projects 
- GL2000 Series https://github.com/netmindz/balboa_GL_ML_spa_control
- BP Series https://github.com/ccutrer/balboa_worldwide_app
- https://github.com/MagnusPer/Balboa-GS510SZ/
