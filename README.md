# room-temperature-humidity-monitoring-and-visualize-via-raspberry

This toy project let you install a temperature and humidity monitoring system in your room and visualize them in you web browser so as to check your room status anywhere via browser. A raspberry Pi and DHT11 sensor (or any other sensor with corresponding driver) are required.

# requiremnts
1. DHT11, I bought from aliexpress.com with less than two Euros, free shipping.

2. Raspberry Pi, I am using a Raspberry Pi 1 Model B, you can use any model but you should change the IO configuration in the driver code.

# Install
1. install an OS for your raspberry, I am using Raspbian with GUI removed.

2. Install apache, php and mysql.

3. create a databsed in mysql for stroing your temperature and humidity.

4. install Wiring Pi (http://wiringpi.com), this library lets you access to the GPIO of RPi easily.

5. fill in your database information in *dht11_to_mysql.c* and compile the driver (dht11_to_mysql.c) with *gcc -Wall -o dht11_to_mysql dht11_to_mysql.c -lwiringPi*

6. set up a regular task via Crontab

7. fill in your database information in file *get_data_from_db.php* (in web folder)

8. copy all files in web folder to your www folder (or sub folder of your web server).

