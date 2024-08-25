int ldrPin = A0;
int led = 7;
int threshold = 70;
void setup()
{
 Serial.begin(9600);
 pinMode(led, OUTPUT);
}
void loop()
{
 int data = analogRead(ldrPin);
 Serial.println("");
 Serial.print("Light Sensor ");
 Serial.print("Value = ");
 Serial.print(data);
 if(data <= threshold)
 {
 digitalWrite(led, LOW);
 }
 else
 {
 digitalWrite(led, HIGH);
 }
}