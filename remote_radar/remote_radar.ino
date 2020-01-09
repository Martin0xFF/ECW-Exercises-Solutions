#define TRIGGER_PIN  7
#define ECHO_PIN     8
#define SERVO_PIN    3
#define MAX_DISTANCE 200
#define BUFFERSIZE 6
#include <NewPing.h>
#include <Servo.h>     

#include <SPI.h>
#include <nrf_hal.h>

// if 1 - write some info to the serial output
#define INFO_ON_SERIAL  0

// nRF communication channel (1MHz wide)
#define CHANNEL     20

// nRF Chip Enable (CE) pin
#define CE_PIN      9
// SPI CS (slave select) pin
#define CS_PIN      10


#define chip_enable()   digitalWrite(CE_PIN, HIGH)
#define chip_disable()  digitalWrite(CE_PIN, LOW)

int readings[6];
Servo zRotate;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);


static uint8_t tx[NRF_MAX_PL] = {};
static int cnt = 0;
static uint8_t led_val = LOW;

     
void setup() {
    
    // CE as output
    pinMode(CE_PIN, OUTPUT);
    digitalWrite(CE_PIN, LOW);

   
#if INFO_ON_SERIAL
    Serial.begin(115200);
#endif

    // SPI init
    SPI.begin();
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    // init NRF HAL
    hal_nrf_set_cs_pin(CS_PIN);

#if INFO_ON_SERIAL
    init_tx();

    uint8_t addr[5];
    char sp_buf[48];

    hal_nrf_get_address(HAL_NRF_PIPE0, addr);
    sprintf(sp_buf, "P0 addr: %02x:%02x:%02x:%02x:%02x\r\n",
        addr[0], addr[1], addr[2], addr[3], addr[4]);
    Serial.print(sp_buf);

    hal_nrf_get_address(HAL_NRF_TX, addr);
    sprintf(sp_buf, "TX addr: %02x:%02x:%02x:%02x:%02x\r\n",
        addr[0], addr[1], addr[2], addr[3], addr[4]);
    Serial.print(sp_buf);
#endif
    
    zRotate.attach(SERVO_PIN);
    zRotate.write(0);
    Serial.begin(115200);
    }

     //angle, distance
void loop() {
    int mAngle = 0;
    int xAngle = 180;
    int distance = 0;
    for (int angle=mAngle; angle<xAngle; angle= angle + 5){
      distance = moveRead(angle);
      tx_loop(distance, angle);
      delay(10);
    }

    for (int angle=xAngle; angle>mAngle ; angle= angle - 5){
      distance = moveRead(angle);
      tx_loop(distance, angle);
      delay(10);
    }
    
  }


int nofilterMR(int angle){
  int reading;
  zRotate.write(angle);
  delay(10);
  
  reading = sonar.ping();
  while(true){
    reading = sonar.ping();
    if(reading>0){
      return reading;
      }
    }
  }

int moveRead(int angle){
  zRotate.write(angle);
  delay(10);
  uint32_t startTime = millis();
  int reading;
  int count=0;
  
  while (true){
    delay(10);
    reading = sonar.ping();
    
    if (millis() - startTime > 10000){
      return -1;
      }
    else if (reading > 0){
 
        readings[count] = reading;
        count++;
        
        if (count > BUFFERSIZE - 1){
            int mean=0;
            for (int i=0; i<BUFFERSIZE; i++){
                mean = mean + readings[i];
            }
            return mean/BUFFERSIZE ;
        }
      }
      
    }
  }


  /* TX loop */
void tx_loop(int distance, int angle)
{
    int i;
    uint8_t irq_flg;
    char ibuf[10];

    hal_nrf_set_power_mode(HAL_NRF_PWR_UP);
    delayMicroseconds(1500);

    // although not necessary it's safer to re-initialize TX after
    // transceiver power-up due to observed transceiver lock-ups
    init_tx();

    tx[0] = 0xAB;   // magic
    sprintf(ibuf, "%d,%d", distance,angle);
    sprintf((char *)&tx[1], "ard-msg no. %s", ibuf);
    hal_nrf_write_tx_payload(tx, sizeof(tx));

    hal_nrf_get_clear_irq_flags();

    // pulse CE for transmission
    chip_enable();
    delayMicroseconds(100);
    chip_disable();

    // wait 2ms for transmission status
    for (i=0; i<20; i++) {
        irq_flg = hal_nrf_get_clear_irq_flags();
        if (irq_flg & ((1U<<HAL_NRF_TX_DS)|(1U<<HAL_NRF_MAX_RT))) break;
        delayMicroseconds(100);
    }

    if ((irq_flg & (1U<<HAL_NRF_MAX_RT)) || i>=20)
    {
        hal_nrf_flush_tx();
#if INFO_ON_SERIAL
        Serial.print("TX timeout\r\n");
#endif
    } else {
        
#if INFO_ON_SERIAL
        Serial.print("Sent message no. ");
        Serial.print(cnt, DEC);
        Serial.print("\r\n");
#endif
    }

    // go sleep up to the next transmission
    hal_nrf_set_power_mode(HAL_NRF_PWR_DOWN);
    delay(1000);

    cnt++;
}

void init_tx()
{
    hal_nrf_flush_rx();
    hal_nrf_flush_tx();

    // clear all extra features for the purpose of this example
    hal_nrf_enable_dynamic_payload(false);
    hal_nrf_enable_ack_payload(false);
    hal_nrf_enable_dynamic_ack(false);

    hal_nrf_set_rf_channel(CHANNEL);
    hal_nrf_set_datarate(HAL_NRF_1MBPS);
    hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);

    // TX setup: use default address, max power with no retransmissions
    hal_nrf_set_operation_mode(HAL_NRF_PTX);
    hal_nrf_config_tx(NULL, HAL_NRF_0DBM, 0, 0);
}
