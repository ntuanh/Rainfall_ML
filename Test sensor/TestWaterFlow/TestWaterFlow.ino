#define CONSTANT_FACTOR 7.5 //covert hall sensor signal to lit / minute

volatile int flowFrequency;  
unsigned int lPerHour; // lit per hour
unsigned char flowSensorPin = 2; 
unsigned long currentTime;
unsigned long cloopTime;


void flowCount() // Interrupt function
{
    flowFrequency++;
}


void setup()
{
    pinMode(flowSensorPin, INPUT_PULLUP);
    Serial.begin(9600);
    attachInterrupt(0, flowCount, RISING);
    sei(); // Enable interrupts
    currentTime = millis();
    cloopTime = currentTime;
}
int Waterflow()
{
    currentTime = millis();
    //after 1s read the sensor signal count and reset it
    if (currentTime >= (cloopTime + 1000))
    {
        cloopTime = currentTime;  // update referent time
        lPerHour = (flowFrequency * 60 / CONSTANT_FACTOR);
        flowFrequency = 0;
        return lPerHour;
    }
    else
    return 0; 
}
void loop(){
  int flow = Waterflow();
if ( flow != 0){
    Serial.print("Water Flow : ");
    Serial.println(flow);
  }
  delay(10);
}