  // Function: This program measures heart rate that triggers 3 servos. The lowest pulse in the program be set to 30.
  // Hardware: Grove - Ear-clip Heart Rate Sensor, Arduino - Uno
  // Author: Nadhif Satriana       
  // Date: March 5th, 2020
  // Version: v1.0
  // References: Heart Rate sensor code has been cloned from http://wiki.seeedstudio.com/Grove-Ear-clip_Heart_Rate_Sensor/
  
  #include <Servo.h>
  #define LED 4            // indicator, Grove - LED is connected with D4 of Arduino
  boolean led_state = LOW; // state of LED, each time an external interrupt will change the state of LED
  unsigned char counter;
  unsigned long temp[21];
  unsigned long sub;
  bool data_effect=true;
  unsigned int heart_rate; // the measurement result of heart rate

  // you can change it follow your system's request. 2000 meams 2 seconds. System return error if the duty overtrip 2 second.
  const int max_heartpluse_duty = 2000;
  
  Servo topHeart;
  Servo leftHeart;
  Servo rightHeart;
  float delayRate = 1000;
  float heartRateF = 0;
  void setup() {
    Serial.begin(9600); // Serial connection for debugging
    
    // heart mechanism servos
    topHeart.attach(4);
    leftHeart.attach(5);
    rightHeart.attach(6);

    // Start the program
    Serial.println("Please ready your chest belt.");
    delay(5000);
    arrayInit();
    Serial.println("Heart rate test begin.");
    attachInterrupt(0, interrupt, RISING);//set interrupt 0,digital port 2
  }
  
  void loop() {
    // Write servo positions 
    topHeart.write(0); 
    delay(100);
    leftHeart.write(90);
    delay(100);
    rightHeart.write(0); //makes sure right and left heart follow each other's movements
    delay(delayRate);
    topHeart.write(90);  
    delay(100);
    leftHeart.write(0);
    delay(100);
    rightHeart.write(90);
    delay(delayRate);
  }

  /* Cloned from http://wiki.seeedstudio.com/Grove-Ear-clip_Heart_Rate_Sensor/ */
  /* Function: calculate the heart rate */
  void sum()
  {
   if(data_effect)
      {
        heart_rate=1200000/(temp[20]-temp[0]);//60*20*1000/20_total_time 
        Serial.print("Heart_rate_is:\t");
        Serial.println(heart_rate);
        heartRateF = 1200000/(temp[20]-temp[0]);//60*20*1000/20_total_time
        delayRate = (60.0 / heartRateF) * 1000.0;
        Serial.print("Delay rate _is:\t");
        Serial.println(delayRate);
        
      }
     data_effect=1;//sign bit
  }
  
  /* Function: Interrupt service routine.Get the sigal from the external interrupt */
  void interrupt()
  {
      temp[counter]=millis();
      Serial.println(counter,DEC);
      Serial.println(temp[counter]);
      switch(counter)
      {
          case 0:
              sub=temp[counter]-temp[20];
              Serial.println(sub);
              break;
          default:
              sub=temp[counter]-temp[counter-1];
              Serial.println(sub);
              break;
      }
      if(sub>max_heartpluse_duty)//set 2 seconds as max heart pluse duty
      {
          data_effect=0;//sign bit
          counter=0;
          Serial.println("Heart rate measure error,test will restart!" );
          arrayInit();
      }
      if (counter==20&&data_effect)
      {
          counter=0;
          sum();
      }
      else if(counter!=20&&data_effect)
      counter++;
      else 
      {
          counter=0;
          data_effect=1;
      }

  }
  
  /* Function: Initialization for the array(temp) */
  void arrayInit()
  {
      for(unsigned char i=0;i < 20;i ++)
      {
          temp[i]=0;
      }
      temp[20]=millis();
  }
