#ifndef GARY_H
#define GARY_H

#include <iostream>
#include <fstream>

#include "Pet.h"

class Gary : public Pet{
    public:
        void Action1();     //Snail-Po
        void Action2();     //Toy Ball
        void Action3();     //Bath

        void checkStatus();
        void saveload();
};
void Gary ::Action1(){
    mood += 10;
    hunger -= 20;
}
void Gary :: Action2(){
    mood += 20;
    hunger += 15;
    wellness += 20;
}
void Gary :: Action3(){
    mood -= 20;
    wellness += 10;
}
void Gary :: checkStatus(){
    if(mood < 0)
        mood = 0;
    else if(mood > 100)
        mood = 100;
    if(mood <= 40){
        cout << endl;
        cout << "Narrator: Gary is down in the dumps, remember: happy pet, happy life" << endl;
        cout << endl;
    }

    if(hunger < 0)
        hunger = 0;
    else if(hunger > 100)
        hunger = 100;
    if(hunger >= 70){
        cout << endl;
        cout << "Narrator: For a snail, he sure eats a lot! Gary is hungry!" << endl; //may change if food does
        cout << endl;
    }

    if(wellness < 0)
        wellness = 0;
    else if(wellness > 100){
        wellness = 100;
    }
    if(wellness <= 20){
        cout << endl;
        cout << "Narrator: There is a smelly smell coming from somewhere, and it is not me. It's time for a bath" << endl;
        cout << endl;
    }
}
void Gary:: saveload(){
    fstream file("Gary.txt");

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