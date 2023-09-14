#ifndef GARFIELD_H
#define GARFIELD_H

#include <iostream>
#include <fstream>


#include "Pet.h"

using namespace std;
class Garfield : public Pet{
    public:
        
        void Action1();     //Give Lasagna
        void Action2();     //watch TV
        void Action3();     //play w Odie

        void checkStatus();

        void saveload();
    
};
void Garfield :: Action1(){
    cout << endl;
    cout << "Garfield : love me, feed Me, never leave me" << endl;
    cout << endl;
    mood += 20;     
    hunger -= 20;
    wellness -= 10;  
}
void Garfield :: Action2(){
    mood += 10;
    wellness -= 15; 
}
void Garfield :: Action3(){
    mood -= 15;
    hunger += 20;
    wellness += 25;
}
void Garfield :: checkStatus(){
    if(mood < 0)
        mood = 0;
    else if(mood > 100)
        mood = 100;
    if(mood <= 20){
        cout << endl;
        cout << "Narrator: Garfield is in one of his moods, better do something and fast!" << endl;
        cout << endl;
    }

    if(hunger < 0)
        hunger = 0;
    else if(hunger > 100)
        hunger = 100;
    if(hunger >= 60){
        cout << endl;
        cout << "Narrator: Someone get this cat some lasagna stacked!" << endl; //may change if food does
        cout << endl;
    }

    if(wellness < 0)
        wellness = 0;
    else if(wellness > 100){
        wellness = 100;
    }
    if(wellness <= 20){
        cout << endl;
        cout << "Narrator: We all have to do things we don't want, and today Garfield needs some physical activity" << endl;
        cout << endl;
    }
    
}
void Garfield :: saveload(){
    fstream file("Garfield.txt");

    if(!file.is_open()){
        cout << "Could not save data" << endl;
    }
    else{
        file.clear();

        file << mood << endl;
        file << hunger << endl;
        file << wellness;
    
        file.close();
    }
    cout << endl;
    cout << "Game successfully saved" << endl;
    cout << endl;
}

#endif