int PulseSensorPurplePin = 0;
int LED13 = 13;
int Signal;
int Threshold = 570;
int count = 0;
int value = 0;
int heartbeat = 0;
int j = 0;
#include <SPI.h>
#include<SD.h>
int sign;
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial.begin(9600);
  pinMode(LED13, OUTPUT);
  Serial.print("Initialising SD Card...");
  Serial.println("Card intitialized");
  system("/sbin/fdidk -1 > /dev/ttyGSO");
  system("date 2017.05.17-10:00::35");
}

void loop() {
  //  // put your main code here, to run repeatedly:
  if (value < 30000) {
    value += 300;
    Signal = analogRead(PulseSensorPurplePin);
    if (Signal > Threshold && Signal < 700) {
      digitalWrite(LED13, HIGH);
      count = count + 1;
      Serial.print(count);
      Serial.print("     Signal value    ");
      Serial.println(Signal);
    }
    else {
      digitalWrite(LED13, LOW);
    }
  }
  if (value >= 30000) {
    heartbeat = count * 2;
    value = 0;
    count = 0;
    Serial.println(heartbeat);
    String datastring = "";
    system("date '+%H:%M:%S' > /home/root/time.txt");
    FILE *fp;
    fp = fopen("/home/root/time.txt", "r");
    char buf[20];
    fgets(buf, 9, fp);
    fclose(fp);
    datastring += String(buf) + ",";
    String phour, pminute;
    int i = 0;
    for (i = 0; i < 2; i++) {
      phour += buf[i];
      pminute += buf[i + 3];
    }
    int presenthour = phour.toInt();
    int presentminute = pminute.toInt();
    datastring += String(heartbeat);
    File datafile = SD.open("heart.csv", FILE_WRITE);
    if (datafile) {
      datafile.println(datastring);
      datafile.close();
    }
    else {
      Serial.println("error opening heart.csv");
    }
    j++;
    if (j==10) {
      datafile = SD.open("heart.csv");
      if (datafile) {
        while (datafile.available()) {
          String s = datafile.readStringUntil('\n');
          int hour = s.substring(0, 2).toInt();
          int minute = s.substring(3, 5).toInt();
          if ((hour==presenthour-1)&&(minute==presentmiute)) {
            int lastheart =  s.substring(10).toInt();
            j = 0;
            if (lastheart<heartbeat-8) {

              Serial1.println("ATD +918607442065;");//dial the number
              delay(100);
              Serial1.println();
              DialVoiceCall();
            } else
              break;

          }
        }
        datafile.close();
      }
      else {
        Serial.println("error opening heart.csv");
      }
    }
  }

  delay(300);
}
void DialVoiceCall()
{
  Serial1.println("ATD +918607442065;");//dial the number
  delay(100);
  Serial1.println();
}
void ShowSerialData()
{
  while (Serial1.available() != 0)
    Serial.write(Serial1.read());
}

