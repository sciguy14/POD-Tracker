#include <Narcoleptic.h>

int8_t answer;
int onModulePin= 2;
char gps_data[100];
int counter;
boolean gps_good = false;

void setup()
{
  //Boot the Device
  pinMode(onModulePin, OUTPUT);
  Serial.begin(115200);      
}


void loop()
{
  
  power_on();
  delay(3000);
   
  //sets the PIN code
  sendATcommand("AT+CPIN=****", "OK", 2000);
  delay(3000);
    
  //Sets up SMS Mode
  answer = 0;
  while (answer == 0)
  {
    answer = sendATcommand("AT+CMGF=1", "OK", 5000);
  }
    
  // starts GPS session in stand alone mode
  answer = 0;  
  while (answer == 0)
  {
    answer = sendATcommand("AT+CGPS=1,1","OK",5000); 
  }
  
  //Get GPS Info
  gps_good = false;
  memset(gps_data, 0, 100);
  int retries = 0;
  while (!gps_good && retries < 10)
  {
    answer = sendATcommand("AT+CGPSINFO","+CGPSINFO:",10000);    // request info from GPS
    if (answer == 1)
    {
      counter = 0;
      do
      {
        while(Serial.available() == 0);
        gps_data[counter] = Serial.read();
        counter++;
      }
      while(gps_data[counter - 1] != '\r');
      gps_data[counter] = '\0';
      if(gps_data[0] != ',')
      {
        gps_good = true;
      }       
    }
    retries++;
    delay(10000);
  }
    
  if (gps_good)
  {
    Serial.println(F("GPS Lock. Sending SMS."));
    
    
    //Connect to Mobile Network
    while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
            sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );
    
    answer = sendATcommand("AT+CMGS=\"XXXXXXXXXX\"", ">", 2000);    // send the SMS number
    if (answer == 1)
    {
      Serial.println(gps_data);
      Serial.write(0x1A);
      answer = sendATcommand("", "OK", 20000);
      if (answer == 1)
      {
        Serial.print(F("Sent "));    
      }
      else
      {
        Serial.print(F("error "));
      }
    }
    else
    {
      Serial.print(F("error "));
      Serial.println(answer, DEC);
    }
  }
  
  //power off then sleep for 10 minutes
  digitalWrite(onModulePin,HIGH);
  delay(3000);
  digitalWrite(onModulePin,LOW);
  
  for (int sleepy = 0; sleepy <20; sleepy++)
  {
    Serial.println(sleepy);
    delay(2500);
    Narcoleptic.delay(25000);
    delay(2500);
  }
}

void power_on(){

    uint8_t answer=0;
    
    // checks if the module is started
    answer = sendATcommand("AT", "OK", 2000);
    if (answer == 0)
    {
        // power on pulse
        digitalWrite(onModulePin,HIGH);
        delay(3000);
        digitalWrite(onModulePin,LOW);
    
        // waits for an answer from the module
        while(answer == 0){     // Send AT every two seconds and wait for the answer
            answer = sendATcommand("AT", "OK", 2000);    
        }
    }
    
}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialice the string
    
    delay(100);
    
    while( Serial.available() > 0) Serial.read();    // Clean the input buffer
    
    Serial.println(ATcommand);    // Send the AT command 


    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(Serial.available() != 0){    
            response[x] = Serial.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer) != NULL)    
            {
                answer = 1;
            }
        }
    // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;
}
