#include "mbed.h"
#include "RGBmatrixPanel.h"
#include "DigitalIn.h"                  
#include <cstdlib>

//pin inputs to connect the k64f pins to the led matrix
DigitalIn one(D7);
DigitalIn two(D8);
DigitalIn three(D9);
DigitalIn four(D10);
DigitalIn five(D11);
DigitalIn six(D12);
DigitalIn seven(D13);
DigitalIn eight(D14);
DigitalIn nine(D15);
PinName ur1=A4;
PinName ug1=A5;
PinName ub1=D2;
PinName lr2=D3;
PinName lg2=D4;
PinName lb2=D5;
PinName a = A0;
PinName b = A1;
PinName c = A2;
PinName d = A3;
PinName latch = D0;
PinName clk = D6;
PinName oe = D1;
RGBmatrixPanel matrix(ur1,lr2,ug1,lg2,ub1,lb2,a,b,c,d,clk,latch,oe,false);
//RGBmatrixPanel::RGBmatrixPanel(r1, r2, g1, g2, b1, b2, a, b, c, d, sclk, latch, oe,bool dbuf)

//delay function that updates the display and busy waits
void delay(int x)
{
    for(int i=0; i<x*1000;i++)
    {
        wait_us(100);
        matrix.updateDisplay();
    }
}

//gets the starting x coordinate of the mole based upon which mole, 1-9, it is
int getX(int mole){
        if (mole<4) return (mole-1)*10;
        else if (mole<7) return (mole-4)*10;
        else return (mole-7)*10;
        }

//gets the starting x coordinate of the mole based upon which mole, 1-9, it is
int getY(int mole){
        if (mole<4)   return 1;
         else if(mole<7)   return 11;   
        else  return 21;
        }

//function that draws the mole and has a mole pop up. Coloration based upon easy or hard game           
void popMole(int mole,bool easy){
    int startX=0;
    int startY=0;
    int red=rand() % 8;
    int green=rand() % 8;
    int blue=rand() % 8;
    if (!easy){
        red=7;
        green=0;
        blue=0;
    }
        
        startX=getX(mole);
        startY=getY(mole);
    matrix.fillRect(startX,startY,9,9,matrix.Color333(0,0,0));
    matrix.drawLine(startX+3,startY+1,startX+6,startY+1,matrix.Color333(red, green, blue));
    matrix.drawLine(startX+1,startY+3,startX+1,startY+7,matrix.Color333(red, green, blue));
    matrix.drawLine(startX+8,startY+3,startX+8,startY+7,matrix.Color333(red, green, blue));
    matrix.drawLine(startX+3,startY+8,startX+3,startY+9,matrix.Color333(red, green, blue));
    matrix.drawLine(startX+6,startY+8,startX+6,startY+9,matrix.Color333(red, green, blue));
    matrix.drawLine(startX+4,startY+6,startX+5,startY+6,matrix.Color333(7,0,0));
    matrix.drawPixel(startX+3, startY+4, matrix.Color333(7, 7, 7));
    matrix.drawPixel(startX+6, startY+4, matrix.Color333(7, 7, 7));
    matrix.drawPixel(startX+2, startY+2, matrix.Color333(red, green, blue));
    matrix.drawPixel(startX+7, startY+2, matrix.Color333(red, green, blue));
    matrix.drawPixel(startX+1, startY+1, matrix.Color333(red, green, blue));
    matrix.drawPixel(startX+8, startY+1, matrix.Color333(red, green, blue));
}

//takes a string input and converts to char array and then print outputs the letters based on color specifications
//colorful states if to make multi-colored. If not, then choose whether color should be inputted. If inputted, it takes the
//color written in redi,greeni, and bluei
void wordLine(bool colorful,bool input,char word[],int redi,int greeni,int bluei){
        int red,green,blue=0;
        if (!colorful){
            if (input){
                red=redi;
                green=greeni;
                blue=bluei;
                }
            else{
                red=rand() %8;
                green=rand() %8;
                blue=rand() %8;
                }
            }
        for (int x=0;x<strlen(word);x++){
            if (colorful){
            red=rand() %8;
            green=rand() %8;
            blue=rand() %8;
            }
            matrix.setTextColor(matrix.Color333(red,green,blue));
            matrix.putc(word[x]);
            }
    }

//creates welcome screen by having moles pop up in shorter and shorter delays and then pop up words saying hello and asking if one wants to
//play a game by using the wordLine function
void welcomeScreen (){
    for (int x=0;x<8;x++){
    popMole(rand()%9+1,true);
    delay(8-x);
    matrix.fillRect(0,0,32,32,matrix.Color333(0,0,0));
    }
    for (int x=0;x<16;x++){
    popMole(rand()%9+1,true);
    delay(1);
    matrix.fillRect(0,0,32,32,matrix.Color333(0,0,0));
    }
    matrix.setCursor(1, 1);   // start at top left, with one pixel of spacing
    matrix.setTextSize(1);    // size 1 == 8 pixels high
    wordLine(false,false,"HELLO",0,0,0);
          delay(15);
    matrix.fillRect(0,0,32,32, matrix.Color333(0,0,0));
        matrix.setCursor(1, 1);
    wordLine(false,false,"DO ",0,0,0);
        delay(3);
    wordLine(false,false,"YA",0,0,0);
        delay(3);
    wordLine(false,false,"WANNA",0,0,0);
        delay(3);
    wordLine(false,false,"PLAY",0,0,0);
    wordLine(false,false,"A",0,0,0);
        delay(3);
    wordLine(true,true,"GAME?",0,0,0);
          delay(15);
    matrix.fillRect(0,0,32,32,matrix.Color333(0,0,0));
    }

/*
while named increment, the score incrementation is already done. Instead, this function displays the score in realtime during the game
The function centers the score so if the score is double digit, it shifts the score over to center it. The score also takes into account
whether it is displaying during the game or in final score display. If during game, the score is shown in white, but after, it is made
to change color randomly.
*/
void increment(int score,bool final){
        int red=1;
        int green=1;
        int blue=1;
        if (final){
            red=rand() %8;
            green=rand() %8;
            blue=rand() %8;
            }
        matrix.fillRect(10,24,10,10,matrix.Color333(0,0,0));
        if (score<11){
        matrix.setCursor(12, 24);   // start at top left, with one pixel of spacing
        matrix.setTextSize(1);    // size 1 == 8 pixels high
        // printff each letter with a rainbow color
        matrix.setTextColor(matrix.Color333(red,green,blue));
        matrix.putc(score+47);
        }
        else if (score<100){
        int tens=(score-1)/10+1;
        int ones=(score-1)%10+1;
        matrix.setCursor(9, 24);   // start at top left, with one pixel of spacing
        matrix.setTextSize(1);    // size 1 == 8 pixels high
        // printff each letter with a rainbow color
        matrix.setTextColor(matrix.Color333(tens,green,blue));
        matrix.putc(tens+47);
        matrix.putc(ones+47);
        }
        }

/*
This function creates the holes that the moles will pop up from. There are two methods in this. One for if it is to initialize the holes,
where all the holes are created and one where it just redraws the hole in one location where a mole has erased the hole
This function also takes in whether the game is easy to choose whether the hole should be made red or green
*/
void holesForMoles(int moleNegate, bool easy,bool init){
    int red=0;
    int green=7;
    int blue=0;
    if (!easy){
        red=7;
        green=0;
    }
    if (init){
        for (int x=1;x<10;x++){
            matrix.drawCircle(getX(x)+4, getY(x)+4, 4, matrix.Color333(red, green, blue));
        }
    }
    else{
        matrix.fillRect(getX(moleNegate),getY(moleNegate),10,10,matrix.Color333(0,0,0));
        matrix.drawCircle(getX(moleNegate)+4, getY(moleNegate)+4, 4, matrix.Color333(red, green, blue));
    }
}

/*
this function happens after the mole pops up and delays it for the delaytime it is to stay up for and checks for whether the mole position
has been clicked by the button. If clicked, the mole time stays just as long but a block is shown over the mole, showing that it has been
hit. Easy input again taken to choose colorway of the mole and choose difficulty. It is tested to find whether a button is hit and if correct
one hit. If in easy mode, if wrong button is clicked, nothing happens but in difficult mode, the game also ends and returns false for "hit"
if the wrong button is clicked
*/        
bool bopMole(int mole,int delaytime,bool easy){
    bool hit=false;
    
    for(int i=0; i<delaytime*1000;i++){
        wait_us(100);
        if(!(one&&two&&three&&four&&five&&six&&seven&&eight&&nine)){
            if (mole==1&&!one||mole==2&&!two||mole==3&&!three||mole==4&&!four||mole==5&&!five||mole==6&&!six||mole==7&&!seven||mole==8&&!eight||mole==9&&!nine){
                int ex=getX(mole);
                int why=getY(mole);
                if (easy){
                    matrix.fillRect(ex,why,10,10,matrix.Color333(0,0,7));
                    }
                else{
                    matrix.fillRect(ex,why,10,10,matrix.Color333(7,0,0));
                    }
                hit=true;
            }
            else if(!easy){
                return false;
                }
        }    
    matrix.updateDisplay();
    }
    holesForMoles(mole,easy,false);
    return hit;
    }

/*
initiates the game by letting the user choose easy or hard difficulty by inputs into 1-3 buttons or 7-9 buttons, ignoring inputs to 4-6 
buttons. A dramatic display then pops up saying which mode was chosen. A click to start function is then shown which only procedes to the 
function once any button is clicked. Once the game starts, a while loop begins that does function popmole() to make a mole, and bopmole to
hit the mole until bopmole returns that the mole was not hit and the while loop ends. There is a counter within the loop that displays the
score in increment() and a delaytime variable dependent upon the score and the difficulty mode that inputs into the bopmole function and
shortens delay time. After the game ends, it is stated that game ends in different colorways for easy or hard and the user is asked
whether to play again with the score displayed underneath. Any button input will begin the game again by recalling this function
*/
void initiateGame(){
    bool easy=true;
    matrix.setCursor(3, 1);   // start at top left, with one pixel of spacing
    matrix.setTextSize(1);    // size 1 == 8 pixels high
    wordLine(true,true,"EASY",0,0,0);
    delay(1);
    matrix.setCursor(2, 12);  
    wordLine(false,true," OR  ",7,7,7);
    delay(1);
    matrix.setCursor(3, 24); 
    wordLine(false,true,"HARD ",7,0,0);
    while (one&&two&&three&&seven&&eight&&nine){
        delay(1);
    }
    easy=(seven&&eight&&nine);
    matrix.fillRect(0,0,32,32,matrix.Color333(0,0,0));
    delay(1);
    if (easy){
        matrix.setCursor(1, 1);   // start at top left, with one pixel of spacing
        matrix.setTextSize(1);   
        wordLine(true,true,"EASY ",0,0,0); 
        wordLine(true,true,"ROAD ",0,0,0);
        wordLine(true,true,"TAKEN",0,0,0);
    }
    else{
        matrix.setCursor(1, 1);   // start at top left, with one pixel of spacing
        matrix.setTextSize(1);   
        wordLine(false,true,"ROAD ",7,0,0); 
        wordLine(false,true,"LESS ",7,0,0);
        wordLine(false,true,"TAKEN",7,0,0);
    }
    delay(15);   
    matrix.fillRect(0,0,32,32, matrix.Color333(0,0,0));
    matrix.setCursor(1, 1);   // start at top left, with one pixel of spacing
    matrix.setTextSize(1);    // size 1 == 8 pixels high
    wordLine(false,false,"CLICK",0,0,0);
    delay(1);
    wordLine(false,false," TO  ",0,0,0);
    delay(1);
    wordLine(false,false,"START",0,0,0);
    delay(1);
    while(one&&two&&three&&four&&five&&six&&seven&&eight&&nine){
        matrix.updateDisplay();
    }
    matrix.fillRect(0,0,32,32, matrix.Color333(0,0,0));
    holesForMoles(0,easy,true);
    //starts game after button is clicked
    int counter=1;
    int mole=2;
    int delaytime=0;
    bool hit=true;
    bool win=false;
    while (hit) {
        increment(counter,false);
        counter=counter+1;
        delay(3);
        mole=rand() %9 +1;
        if (easy){
            delaytime=20-counter/4;
        }
        else{
            delaytime=8-counter/4;
        }
        if (delaytime<1){
            delaytime=1;
        }
        popMole(mole,easy);
        hit=bopMole(mole,delaytime,easy);
        if (counter==55){
            hit=false;
            win=true;
        }
    }
    matrix.updateDisplay();
    matrix.setCursor(1, 1);   // start at top left, with one pixel of spacing
    matrix.setTextSize(1);    // size 1 == 8 pixels high
    if (win){
        matrix.fillRect(0,0,32,32,matrix.Color333(0,0,0));
        wordLine(true,false,"YOU  ",0,0,0);
        delay(8);
        wordLine(true,false," WIN ",0,0,0);
        delay(8);
        wordLine(true,false,"<3333",0,0,0);
        delay(12);
    }
    else if (easy){
        matrix.fillRect(0,0,32,32,matrix.Color333(0,0,0));
        wordLine(false,false,"GAME ",0,0,0);
        delay(8);
        wordLine(false,false,"OVER ",0,0,0);
        delay(12);
    }
    else{
        for (int x=0;x<32;x++){
        matrix.fillRect(0,0,32,x,matrix.Color333(7,0,0));
        delay(1);
    }
    delay(2);
    wordLine(false,true,"GAME ",0,0,0);
    delay(8);
    wordLine(false,true,"OVER ",0,0,0);
    delay(12);
    }
    matrix.fillRect(0,0,32,32, matrix.Color333(0,0,0));
    delay(2);
    matrix.updateDisplay();
    matrix.setCursor(1, 1);   // start at top left, with one pixel of spacing
    //matrix.setTextSize(1);    // size 1 == 8 pixels high
    wordLine(false,false,"PLAY ",0,0,0);
    delay(4);
    wordLine(false,false,"AGAIN",0,0,0);
    delay(4);
    wordLine(false,false,"  ?",0,0,0);
    delay(4);
    while (1){
        matrix.updateDisplay();
        delay(1);
        increment(counter-1,true);
        if (!(one&&two&&three&&four&&five&&six&&seven&&eight&&nine)){
            matrix.fillRect(0,0,32,32, matrix.Color333(0,0,0));
            matrix.updateDisplay();
            initiateGame();
        }
   
    }
}

/*
runs game
*/
int main(){
welcomeScreen();
while (1){
initiateGame();
}
}



//while (1){
//    if (one.read()){
//        matrix.fillRect(0,0,32,32, matrix.Color333(7,7,7));
//        delay(1);
//       matrix.fillRect(0,0,32,32, matrix.Color333(0,0,0));
//        delay(1);
//        }
//    }
        //welcomeScreen();
        //initiateGame();
   
/**
        for(int i=0; i<4; i++) {
        matrix.fillRect(0,0,32,32, matrix.Color333(0,0,0));
        matrix.setCursor(1, 1);   // start at top left, with one pixel of spacing
        matrix.setTextSize(1);    // size 1 == 8 pixels high
        // printff each letter with a rainbow color
                matrix.setTextColor(matrix.Color333(0,0,7));
        matrix.putc('B');
          delay(1);
                matrix.setTextColor(matrix.Color333(7,7,0));
        matrix.putc('R');
          delay(1);
                  matrix.setTextColor(matrix.Color333(4,4,4));
        matrix.putc('I');
          delay(1);
                  matrix.setTextColor(matrix.Color333(7,0,7));
        matrix.putc('A');
          delay(1);
        matrix.setTextColor(matrix.Color333(0,7,0));
        matrix.putc('N');
          delay(1);
        matrix.setTextColor(matrix.Color333(4,3,6));
        matrix.putc('J');
          delay(1);
                matrix.setTextColor(matrix.Color333(3,1,5));
        matrix.putc('E');
          delay(1);
                  matrix.setTextColor(matrix.Color333(7,4,1));
        matrix.putc('O');
          delay(1);
                  matrix.setTextColor(matrix.Color333(5,3,2));
        matrix.putc('N');
          delay(1);
        matrix.setTextColor(matrix.Color333(6,1,2));
        matrix.putc('G');
          delay(1);
        matrix.setTextColor(matrix.Color333(7,0,0));
        matrix.putc('I');
        delay(1);
        matrix.setTextColor(matrix.Color333(7,4,0));
        matrix.putc('S');
          delay(1);
        matrix.setTextColor(matrix.Color333(4,7,4));
        matrix.putc(' ');
          delay(1);
        matrix.setTextColor(matrix.Color333(0,4,7));
        matrix.putc('A');
          delay(1);
        matrix.setTextColor(matrix.Color333(0,0,0));
        matrix.putc(' ');
          delay(1);
        matrix.setTextColor(matrix.Color333(4,0,7));
        matrix.putc(' ');
          delay(1);
                matrix.setTextColor(matrix.Color333(4,4,4));
        matrix.putc('H');
          delay(1);
                  matrix.setTextColor(matrix.Color333(4,7,4));
        matrix.putc('O');
          delay(1);
                  matrix.setTextColor(matrix.Color333(7,0,7));
        matrix.putc('E');
          delay(1);
        matrix.setTextColor(matrix.Color333(0,0,0));
        matrix.putc(' ');
          delay(1);
          }
 /**
     int a = 0;
 
     int L = 6;
 
     while(1){
 
          while(a<32-L){
 
      matrix.fillRect(0,a,32,L, matrix.Color333(0,3,4));
 
      delay(1000);
 
      matrix.fillRect(0,0,32,32, matrix.Color333(0, 0, 0));  
 
      a++;
 
    }
 
          while(a>0){
 
      matrix.fillRect(0,a,32,L, matrix.Color333(0,3,4));
 
      delay(1000);
 
      matrix.fillRect(0,0,32,32, matrix.Color333(0, 0, 0)); 
 
      a--;
 
    }
 
}  
 */

