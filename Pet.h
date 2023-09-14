#ifndef PET_H
#define PET_H

#include <iostream>


using namespace std;
//base class
class Pet{      
    protected:      //accesible to derived classes
        int mood;       
        int hunger;
        int wellness;

    public:
        Pet();      //default 
        Pet(int,int,int);      //override 
        
        
        void setMood(int);
        void setHunger(int);
        void setWellness(int);
        void Time(int);
        void printStatus();

        virtual void Action1();     //used polymorphsim, all are named the same and different attributes are displayed in maine menu
        virtual void Action2();
        virtual void Action3();
};
Pet::Pet(){
    mood = 0;
    hunger = 0;
    wellness = 0;
}
Pet::Pet(int m, int h, int w){
    mood = m;
    hunger = h;
    wellness = w;
}
void Pet::setMood(int m){
    mood = m;
}
void Pet::setHunger(int h){
    hunger = h;
}
void Pet::setWellness(int w){
    wellness = w;
}

void Pet::Action1(){
    mood += 0;     
    hunger += 0;
    wellness += 0; 
}
void Pet::Action2(){
    mood += 0;     
    hunger += 0;
    wellness += 0; 
}
void Pet::Action3(){
    mood += 0;     
    hunger += 0;
    wellness += 0; 
}
void Pet:: Time(int t){         
    mood -= (t * 5);
    hunger +=(t * 10);
    wellness -=( t * 5);
}
void Pet::printStatus(){
    cout << endl;
    cout << "Current Status" << endl;
    cout << "---------------" << endl;
    cout << "Mood: " << mood << endl;
    cout << "Hunger: " << hunger << endl;
    cout << "Wellness: " << wellness << endl;
    cout << endl;
}

#endif