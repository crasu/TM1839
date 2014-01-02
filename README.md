# HTTP Controller for TM1839 #

This code can be used to control a TM1839 based led strip with an arduino-ethernet via http requests.
The stripes have been sold by ALDI in december 2013.


## Getting started ##
1. Flash this code to an arduino ethernet
2. Connect the VCC and GND pins of the stripe with a 12V power source
3. Connect the Data and GND pins with pin 7 of an arduino ethernet
4. Adapt IPs in the code
5. Run to set led groups 2 and 10 to white:

  wget -q -O - 192.168.100.177/SET=02FFFFFF0AFFFFFF

6. Run this to reset the stripe:

  wget -q -O - 192.168.100.177/BLACK 

## Credits ##
Based on neobob code from https://github.com/chriszero/neobob
