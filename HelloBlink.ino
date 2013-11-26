/*
  Hello Blink
 Flashes Hello World in Morse Code.
 
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int dot = 200; // milliseconds. Start slow
int dash = 3*dot;
int codeSpace = dot;
int letterSpace = dash;
int wordSpace = 7*dot;

// store the alphabet etc. in binary bytes. dot=0, dash=1.
// Currently only do letters and numbers.
// First non zero signals start, everything following is significant.
// e.g. Letter A is dot dash = 00000101, thie first one says that the 
// following 01 are the letter.



// store the alphabet etc. in binary bytes. dot=0, dash=1.
// Currently only do letters and numbers, so can fit in byte.
// First 3 bits determine length. Code starts at bit 4
const int numCodes = 36; // 26 letters, 10 numbers
byte code[numCodes];
int numStart = 0; // where do the numbers start in the code array? Initialized in setup()

// the setup routine runs once when you press reset:
void setup() {                
    // initialize the digital pin as an output.
    pinMode(led, OUTPUT);

    // now set up the array with morse code.
    code[0] = B00000101;	// dot dash, letter A
    code[1] = B00011000;	// dash dot dot dot, letterB
    code[2] = B00011010;	// dash dot dash dot, C
    code[3] = B00001100; 	// dash dot dot, D
    code[4] = B00000010;	// dot, E
    code[5] = B00010010;	// dot dot dash dot, F
    code[6] = B00001110;	// dash dash dot, G
    code[7] = B00010000;	// dot dot dot dot, H
    code[8] = B00000100;	// dot dot, I
    code[9] = B00010111;	// dot dash dash dash, J
    code[10] = B00001101;	// dash dot dash, K
    code[11] = B00010100;	// dot dash dot dot, L
    code[12] = B00000111;	// dash dash, M
    code[13] = B00000110;	// dash dot, N
    code[14] = B00001111;	// dash dash dash, O
    code[15] = B00010110;	// dot dash dash dot, P
    code[16] = B00011101;	// dash dash dot dash, Q
    code[17] = B00001010;	// dot dash dot, R
    code[18] = B00001000;	// dot dot dot, S
    code[19] = B00000011;	// dash, T
    code[20] = B00001001;		// dot dot dash, U
    code[21] = B00010001;	// dot dot dash, V
    code[22] = B00001011;	// dot dash dash, W
    code[23] = B00011001;	// dash dot dot dash, X
    code[24] = B00011011;	// dash dot dash dash, Y
    code[25] = B00011100;	// dash dash dot dot, letter z
    numStart = 26;
    code[26] = B00111111;	// dash dash dash dash dash, number 0
    code[27] = B00101111;	// dot dash dash dash dash, number 1
    code[28] = B00100111;	// dot dot dash dash dash
    code[29] = B00100011;	// dot dot dot dash dash
    code[30] = B00100001;	// dot dot dot dot dash
    code[31] = B00100000;	// dot dot dot dot dot
    code[32] = B00110000;	// dash dot dot dot dot
    code[33] = B00111000;	// dash dash dot dot dot 
    code[34] = B00111100;	// dash dash dash dot dot
    code[35] = B00111110;	// dash dash dash dash dot, number 9
}
const int codeArraySize = 36;

int flashSpace(int space)
{
    digitalWrite(led, LOW);
    delay(space);
}

void flashCode(int codePoint)
{
    if (codePoint > -1 && codePoint < codeArraySize){
        byte thisCode = code[codePoint];
        int charSize = 8;
        int start = 0;
        byte mask = B10000000;
        for (int outerLooper=0; outerLooper<charSize; ++outerLooper){
            if (mask & thisCode){
                start = outerLooper + 1;
                break;
            }
            mask >>= 1;
        }
        mask >>= 1;
        for (int i=start; i<charSize-1; ++i){ // go to charSize-1 since we don't want a codeSpace after the last piece of code.
            digitalWrite(led, HIGH); // turn on the led
            // now determine how long to leave it on.
            if (mask & thisCode) // its a dash
                delay(dash);
            else
                delay(dot);

            digitalWrite(led, LOW); // turn off the led
            delay(codeSpace);

            mask >>= 1;
        }
        // do the final letter, with no code space.
        digitalWrite(led, HIGH); // turn on the led
        if (mask & thisCode){ // its a dash
            delay(dash);
        }
        else {
            delay(dot);
        }
    }
}
void flashChar(char character)
{
    // Assume ascii, upper case and lower case use the same morse code.
    do{
        int offset = -1;
        // skip unknown chars
        if (character >= '0' && character <= '9') {
            offset = character - '0' + numStart;
        } else if (character >= 'A'  && character <= 'Z'){
            offset = character - 'A';
        } else if (character >= 'a'  && character <= 'z') {
            offset = character - 'a';
        } else {
            flashSpace(wordSpace); // if we don't recognize the char, assume it's an end of word. 
        }
        flashCode(offset);
    } 
    while (false);
}
void flashString(char* word)
{

    flashChar(word[0]);
    int i=1;
    char mychar = word[i];
    while (mychar != '\0'){
        flashSpace(letterSpace);
        flashChar(mychar);
        ++i;
        mychar = word[i];
    }
}

// the loop routine runs over and over again forever:
void loop() {
    /**
     * digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
     * delay(2000);               // wait for a second
     * digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
     * delay(1000);               // wait for a second
     **/
    char string[] = "Hello World";
    flashString(string);
    flashSpace(wordSpace);
    flashSpace(wordSpace);  
}
