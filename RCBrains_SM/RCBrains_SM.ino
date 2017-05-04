/*
Final group project code for Grant Picker and Ben Shaffer.
This file configures the Arduino to read or control the transmitter of an RC
Car. The system is able to read and record the input of the driver, then play
it back.
*/

//Needed to read the SD card.
#include <SD.h>
#include <SPI.h>
#include <String.h>

//Needed for interrupts
#include <avr/interrupt.h>
#include <avr/io.h>

/* connect MOSI to 11
MISO to 12
SCK to 13 */

int csPin = 10;//Chip select pin. All other pin assignments not optional.

File file;

//State Machine variables.
const byte RUN=0, PLAY = 1, REC=2;//State placeholders
int state, nstate = RUN;//State variables
int l, r, u, d, i = 0, line = 0;
String instr, L = "L", R = "R";

//Button pins
const byte play = 2;
const byte rec = 3;

//input pins
const byte leftI = A3;
const byte rightI = A2;
const byte upI = A1;
const byte downI = A0;

//output pins
const byte leftO = 4;
const byte rightO = 5;
const byte upO = 6;
const byte downO = 7;

//Setup the button pin interrupts, and the SD card reader.
void setup() {

	Serial.begin(9600);//Open a connection to the serial monitor
  
	//Setup the SM buttons to generate interrupts.
	pinMode(play, INPUT);
	pinMode(rec, INPUT);
	attachInterrupt(digitalPinToInterrupt(play), PlayButton, RISING);
	attachInterrupt(digitalPinToInterrupt(rec), RecButton, RISING);

	//Setup input and output pins
	pinMode(leftI, INPUT);
	pinMode(rightI, INPUT);
	pinMode(upI, INPUT);
	pinMode(downI, INPUT);

	pinMode(leftO, OUTPUT);
	pinMode(rightO, OUTPUT);
	pinMode(upO, OUTPUT);
	pinMode(downO, OUTPUT);

	//Setup the SD file to read and write driving instructions.
	initializeSD();
  
}

void PlayButton() {
  Serial.println("Play button pressed");
  while(digitalRead(play) == HIGH) {}
  file.close();
  i=0;
  String filename = "file";
  filename += i;
  filename += ".txt";
  char buf[10];
  filename.toCharArray(buf, 10);
  while(SD.exists(buf)){
    i++;
    filename = "file";
    filename += i;
    filename += ".txt";
    filename.toCharArray(buf, 10);
  }
  filename = "file";
  filename += i-1;
  filename += ".txt";
  filename.toCharArray(buf, 10);
  openFile(buf);
  
  Serial.println("Success");
	switch(state){
		case RUN:
			nstate = PLAY;
			break;

		case PLAY:
			nstate = RUN;
			break;

		case REC:
			nstate = PLAY;
			break;

		default:
			nstate = RUN;
			break;
	}
}

void RecButton() {
  Serial.println("Record button pressed");
  while(digitalRead(rec) == HIGH) {}
	switch(state){
		case RUN:
			nstate = REC;
			
			newRecording();

			break;

		case PLAY:
			nstate = REC;
			
			newRecording();

			break;

		case REC:
			nstate = RUN;
			break;

		default:
			nstate = RUN;
			break;
	}
}

//Playback File reading and writing functions here.

void loop() {
	//First, update the state
	state = nstate;

	//Next, do one update of the state.
	switch(state){
		case RUN:
			
			//Turn off output to all pins.
			digitalWrite(leftO, LOW);
			digitalWrite(rightO, LOW);
			digitalWrite(upO, LOW);
			digitalWrite(downO, LOW);
			
			break;
			
		case PLAY:
			
			instr = readLine();
			instr.trim();
                        Serial.println(instr);

			if(instr == L){
				digitalWrite(leftO, HIGH);
				digitalWrite(rightO, LOW);
			
			} else if(instr.equals(R)){
  				digitalWrite(leftO, LOW);
				digitalWrite(rightO, HIGH);
			
			} else if(instr.equals("Straight")){
  				digitalWrite(leftO, LOW);
				digitalWrite(rightO, LOW);
			}
	
			if(instr.equals("U")){
				digitalWrite(upO, HIGH);
				digitalWrite(downO, LOW);
			
			} else if(instr.equals("D")){
  				digitalWrite(upO, LOW);
				digitalWrite(downO, HIGH);
			
			} else if(instr.equals("Stop")){
  				digitalWrite(upO, LOW);
				digitalWrite(downO, LOW);
			}

			break;
			
		case REC:
			
			l = analogRead(leftI);
			r = analogRead(rightI);
			u = analogRead(upI);
			d = analogRead(downI);
			
			if(l < 300) {
				writeToFile("L");
			} else if(r < 300) {
				writeToFile("R");
			} else {
				writeToFile("Straight");
			}
			
			if(u < 300) {
				writeToFile("U");
			} else if(d < 300) {
				writeToFile("D");
			} else {
				writeToFile("Stop");
			}	

			break;
			
		default:
			//Do nothing. Automatically changes state to RUN.
			break;
	}

	delay(50);
	
	return;
}

void newRecording() {

	//Finds the number of the next file.
	String filename = "file";
	filename += i;
	filename += ".txt";
	char buf[10];
	filename.toCharArray(buf, 10);
	while(SD.exists(buf)){
		i++;
		filename = "file";
		filename += i;
		filename += ".txt";
		filename.toCharArray(buf, 10);
	}
	
	createFile(buf);

}

void initializeSD() {

	pinMode(csPin, OUTPUT);

	if(SD.begin()) {
		Serial.println("SD card is ready.");
	} else {
		Serial.println("SD card is not ready.");
	}
	return;
}

int createFile(char filename[]) {

	file = SD.open(filename, FILE_WRITE);

	if (file) {
		Serial.println("File created.");
		return 1;
	} else {
		Serial.println("File could not be created.");
		return 0;
	}
}

int writeToFile(char text[]) {
	if (file) {
		file.println(text);
		Serial.println((String)"Writing " + text + " to file.");
		return 1;
	} else {
		Serial.println("Couldn't write to file.");
		return 0;
	}
}

void closeFile() {
	if (file) {
		file.close();
		Serial.println("File closed");
	}
}

int openFile(char filename[]) {
	file = SD.open(filename);
	if (file) {
		Serial.println("File opened successfully.");
		return 1;
	} else {
		Serial.println("File failed to be opened.");
		return 0;
	}
}

String readLine() {
	String received = "";
	char ch;

	while (file.available()) {
		ch = file.read();
		if (ch == '\n') {
			return String(received);
		} else {
			received += ch;
		}
	}

	return "";
}
