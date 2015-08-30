/*
 *  dht11_to_mysql.c
 *
 *
 *  dht11_to_mysql.c contains the code that read measured data from dht11, 
 *  data process (sensor protocol) and insert read value to database table 
 *  
 *  compile: gcc -Wall -o dht11_to_mysql dht11_to_mysql.c -lwiringPi `mysql_config --cflags --libs`
 */
#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <stdint.h>
#define MAX_TIME 85
#define DHT11PIN 7
#define ATTEMPTS 5
//extern int read_dht11(float *humidity, float *temperature);
/*
*********************************************************************
function name: read_dht11

description: read mesured temperature and humidity from DHT11 sensor
             via 1-wire bus
             The output data from dht11 contains 40-bits, explain as follows:
             |--------+--------+--------+--------+--------|
             |    Humidity     |   temperature   |checksum|
             |--------+--------+--------+--------+--------|
             |   int  |decimal |   int  | decimal|        |
             |--------+--------+--------+--------+ 8bits  |
             |  8bits |  8bits |  8bits |  8bits |        |
             |--------+--------+--------+--------+--------|

            reading trig timing sequence: same as 1-wire bus, bit read has been
            realized in wiringpi library 
            (redirect to https://en.wikipedia.org/wiki/1-Wire)


parameters: 
            *humidity: pointer of humidity 
            *temperature: temperature for output

return: 1 operate succefully, 0 failed to read
*********************************************************************
*/
int read_dht11(float *humidity, float *temperature)
{
    int dht11_val[5]={0,0,0,0,0};
    uint8_t lststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    for(i = 0; i < 5; i++)
    {
        dht11_val[i] = 0;
    }
    pinMode(DHT11PIN, OUTPUT);       // set data pin as output mode
    digitalWrite(DHT11PIN, LOW);     // set this pin as low state
    delay(18);
    digitalWrite(DHT11PIN,HIGH);     // delay 18 ms and pull to high in initialize communication

    delayMicroseconds(40);           // set data pin as input mode in order to read

    pinMode(DHT11PIN, INPUT);

    for(i = 0; i < MAX_TIME; i++)
    {
        counter = 0;
        while(digitalRead(DHT11PIN) == lststate)
        {
            counter++;
            delayMicroseconds(1);
            if(counter == 255)
            {
                break;
            }
        }

        lststate = digitalRead(DHT11PIN);

        if(counter == 255) // check if is time out (bus no response)
        {
            break;
        }

        // top 3 transistions are ignored, complete read bits should be even
        if((i >= 4) && (i % 2 == 0))
        {
            dht11_val[j/8] <<= 1;

            if(counter > 16)
            {
                dht11_val[j/8] |= 1;
            }
            j++;
        }
    }

    // verify checksum and generate the final results (temp and humidity value)
    if((j>=40) && (dht11_val[4] == ((dht11_val[0] + dht11_val[1] + dht11_val[2] + dht11_val[3])& 0xFF)))
    {
        *humidity = (float)dht11_val[0];
        *temperature = (float)dht11_val[2];
        printf("%d.%d,%d.%d\n",dht11_val[0],dht11_val[1],dht11_val[2],dht11_val[3]);
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
*********************************************************************
function name: finish_with_error

description: read mesured temperature and humidity from DHT11 sensor
             via 1-wire bus

parameters: 
            *con mysql: connection

return: none
*********************************************************************
*/
void finish_with_error(MYSQL *con)
{
  mysql_close(con);
  exit(1);        
}
/*
*********************************************************************
function name: insert_ht_into_db

description: insert read humidity and temperature to mysql database

parameters: 
            *con mysql: connection
            humidity: humidity to inserted
            temperature: temperature to insert

return: 1 operate succefully, 0 failed to read
*********************************************************************
*/
int insert_ht_into_db(MYSQL *con, float humidity, float temperature)
{
    char sql_str[200] = {"\0"};
    snprintf(sql_str, sizeof(sql_str), "insert into humidity_and_temperature values(now(), %f, %f);", 
    humidity, temperature);
    if(mysql_query(con, sql_str))
    {
        mysql_close(con);
        return 1;
    }
    return 0;
}

int main(void)
{  
    MYSQL *con = mysql_init(NULL);
    int dht_opr_attempts;
    float humidity, temperature;
    if(wiringPiSetup()==-1)
    {
        exit(1);
    }

    if (con == NULL) 
    {
        exit(1);
    }

    // connect to mysql server
    if (mysql_real_connect(con, "localhost", "youe username", "yourpassword", NULL, 0, NULL, 0) == NULL) 
    {
        mysql_close(con);
        exit(1);
    }
    else
    {
        printf("connect to db succeeded!\n");
    }

    // select databases
    if (mysql_query(con, "use home_status")) // !!! modify to your database name !!!
    {
        mysql_close(con);
        exit(1);
    }

    dht_opr_attempts = 5;
    while(dht_opr_attempts > 0)
    {
        if(1 == read_dht11(&humidity, &temperature))
        {
            break;
        }
        else
        {
            dht_opr_attempts --;
            sleep(5);
        }
    }
    if(dht_opr_attempts > 0)
    {
        insert_ht_into_db(con, humidity, temperature);
    }
    else
    {
        printf("DHT11 oprating failed!\n");
    }
    mysql_close(con);
    exit(0);
}
