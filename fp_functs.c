#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fp.h"
int getFrac(float val);
float fracCalculation(int val);

//Preston Desbordes
//10/1/2017 11PM

int computeFP(float val) {
// input: float value to be represented
// output: integer version in our representation
//
// Perform this the same way we did in class -
//    either dividing or multiplying the value by 2
//    until it is in the correct range (between 1 and 2).
//    Your exponent is the number of times this operation
//    was performed.   
// Deal with rounding by simply truncating the number.
// Check for overflow and underflow - 
//    For underflow, return 0 
//    For overflow, return -1

int exp,frac,finalAnswer,shiftingNum1,shiftingNum2;
int bias = 31; //bias is always this num
int s = 0; //sign bit is always 0
int e = 0;
int trunc = val;//get the non fraction number
float correction = val;

if(val > 2147483647) //check for overflow
{
printf("overflow!\n");
return -1;	
}

if(val > 1 && val < 2)
{
exp = 31;
exp = exp << 8;
}
if(val > 2) //divide in here
{
e = 0;
	while(correction > 2)
	{
	correction = correction/2;
	e++; //E = this num
	}
exp = e + bias; //our exp value
exp = exp << 8; //shifting exp into plac
}
else if(val < 1) //multiply here
{
e = 0;
	while(correction < 1)
	{
	correction = correction*2;
	e--; //E = this num
	}
exp = e + bias; //our exp value
exp = exp << 8; //shifting exp into plac
}
if(e == 0)
{
printf("PROCEED\n");

trunc = trunc << 7;
}
else if(trunc > 0)
{
trunc -= pow(2,e);

trunc = trunc << (8-e);
}

if(exp == 0 || val<0)//underflow check
{
printf("UNDERFLOW!\n");
return 0;
}

frac = getFrac(val);
frac = frac >> e;
frac = trunc | frac; 
finalAnswer = s | exp;
finalAnswer = finalAnswer | frac;

return finalAnswer;
}

float getFP(int val) {
// Using the defined representation, compute the floating point
//    value
// For denormalized values (including 0), simply return 0.
// For special values, return -1;

int e,exp;
int bias = 31;
float m,floatingFrac;//fraction that we want

exp = val >> 8; //get the exp value
e = exp - bias;// get the e value

if(exp == 0 || val == 0)//Denormalize check
{
printf("DENORMALIZED VALUE!\n");
}
if(exp == 64)//special value check
{
printf("SPECIAL VALUE!\n");
return -1;
}

m = fracCalculation(val);//does shifting, and calculates M 
if(e==0)
{
m += .5;
}
floatingFrac = m * pow(2,e);

 return floatingFrac;
}

int
multVals(int source1, int source2) {
// You must implement this by using the algorithm
//    described in class:
//    Add the exponents:  E = E1+E2 
//    multiply the fractional values: M = M1*M2
//    if M too large, divide it by 2 and increment E
//    save the result
// Be sure to check for overflow - return -1 in this case
// Be sure to check for underflow - return 0 in this case
int e1,e2,exp1,exp2,finalE,finalEXP,finalAnswer;
int bias = 31;
float m1,m2,finalM;

exp1 = source1 >> 8;//begin calculating both exp values
exp2 = source2 >> 8;
e1 = exp1 - bias;//begin calculating both e values
e2 = exp2 - bias;
finalE = e1+e2;
m1 = fracCalculation(source1);//utilize function to obtain m
m2 = fracCalculation(source2);
finalM = m1*m2;
while(finalM > 2)
{
finalM = finalM/2;
finalE++;
}
finalEXP = finalE + bias;
finalEXP = finalEXP << 8;
finalAnswer = getFrac(finalM);


finalAnswer = finalEXP | finalAnswer;

if(finalAnswer > 2147483647)
{
printf("OVERFLOW!\n");
return -1;
}

if(finalAnswer < 0)
{
printf("UNDERFLOW\n");
return 0;
}

 return finalAnswer;
}

int
addVals(int source1, int source2) {
// Do this function last - it is the most difficult!
// You must implement this as described in class:
//     If needed, adjust one of the two number so that 
//        they have the same exponent E
//     Add the two fractional parts:  F1' + F2 = F
//              (assumes F1' is the adjusted F1)
//     Adjust the sum F and E so that F is in the correct range
//     
// As described in the handout, you only need to implement this for
//    positive, normalized numbers
// Also, return -1 if the sum overflows

int e1,e2,exp1,exp2,finalE,finalEXP,finalAnswer;
int shift1 = 0, shift2 = 0;
int bias = 31;
float m1,m2,finalM;

exp1 = source1 >> 8;//begin calculating both exp values
exp2 = source2 >> 8;
e1 = exp1 - bias;//begin calculating both e values
e2 = exp2 - bias;

if(e1 > e2)
{
shift2 = e1-e2;
finalE = e1;
}
else if(e2 > e1)
{
shift1 = e2-e1;
finalE = e2;
}
else
{
finalE = e1;
}

while(shift2 > 0)
{
source2 = source2/2;
break;
shift2--;
	if(shift2 == 0)
	{
	//source2 -= pow(2,e2);
	}
}
while(shift1 > 0)
{
source1 = source1/2;
break;
shift1--;
	if(shift1 == 0)
	{
	source1 -= pow(2,e1);
	}
}

m1 = fracCalculation(source1);//utilize function to obtain m
m2 = fracCalculation(source2);

finalM = m1+m2;

while(finalM > 2)
{
finalM = finalM/2;
finalE++;
}
finalEXP = finalE + bias;
finalEXP = finalEXP << 8;
finalAnswer = getFrac(finalM);

finalAnswer = finalEXP | finalAnswer;

if(finalAnswer > 2147483647)
{
printf("OVERFLOW!\n");
return -1;
}

  return finalAnswer;
}

int getFrac(float val)
{
int counter = 7;
int answer = 0;
int trunc = val; //truncated num so that I can get the fraction
float rawFrac = val - trunc;
        while(counter>=0)
        {
        rawFrac = rawFrac*2;
                if(rawFrac>=1)//determines final number for frac
                {
                answer += pow(2,counter);
                rawFrac = rawFrac - 1;
		}
        counter--;
        }
return answer;
}

float fracCalculation(int val)
{
int fracBits;
float floatingFrac;//fraction that we want
float fracVal = 256;//fractional value for our fractional bits
if(val << 23 > 0)
{
	fracBits = val << 23;
	fracBits = fracBits >> 23;
}
else if(val << 23 < 0)
{
	if(val << 24 < 0)
	{
	fracBits = val << 25;
	fracBits = fracBits >> 25;
	}
	else
	{
	fracBits = val << 24;
	fracBits = fracBits >> 24;	
	}
}

floatingFrac = fracBits;
floatingFrac /= fracVal;//.113 now

if(floatingFrac < 1)
{
floatingFrac = fracBits;//SHOULD be 29
floatingFrac += fracVal;//SHOULD be 285
}

floatingFrac /= fracVal;
return floatingFrac;
}
