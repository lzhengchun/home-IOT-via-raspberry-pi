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

5. fill in your database information in *dht11_to_mysql.c* and compile the driver (dht11_to_mysql.c) with * gcc -Wall -o dht11_to_mysql dht11_to_mysql.c -lwiringPi `mysql_config --cflags --libs`*. the application should read temperature and humidity once measured by DHT11 and store them in your database. Since it only read once, you should creat a regular task in order to measure in a given time interval, one of an easy solution will be shown in the next step.

6. set up a regular task via Crontab. (e.g.: run sudo crontab -e and add " * * * * * echo `date +\%Y\%m\%d\%H\%M\%S`,`/home/pi/home_therm/dht11/dht11_to_mysql` >> /home/pi/home_therm/dht11/temp.log " to the end of file(quote not included) will create a regular task with interval of one minute (asterisk stands for any minute))

7. fill in your database information in file *get_data_from_db.php* (in web folder)

8. copy all files in web folder to your www folder (or sub folder of your web server). This part use a highcharts library (http://www.highcharts.com) to visulize your data. 

# have more fun

the data are stored in your database, you can do many funny things with this data, for example, check your room temperature and humidity via whatsapp (I did these via Chinese wechat, when I send an command to my subscripte account, current temp. or humity will be sent back).

a demo: http://zliu.info:8080/home_status/24hours.html

My system is running since August 1st, 2014. temperature and humidity are monitoring every minute.
