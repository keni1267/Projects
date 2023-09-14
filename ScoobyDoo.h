#ifndef SCOOBYDOO_H
#define SCOOBYDOO_H

#include <iostream>
#include <fstream>

#include "Pet.h"

using namespace std;

class ScoobyDoo : public Pet{
    public:


        void Action1();     //ScoobySnack
        void Action2();     //Solve Mystery
        void Action3();     //Rest

        void checkStatus();
        void saveload();


};
void ScoobyDoo::Action1(){      
    mood += 10;
    hunger -= 10;
}
void ScoobyDoo::Action2(){
    cout << endl;
    cout << "Scooby : Scooby-Dooby-Doo!" << endl;
    cout << endl;
    mood += 10;
    hunger += 15;
    wellness -= 20;         
}
void ScoobyDoo::Action3(){
    mood += 20;
    wellness += 20;
} 
void ScoobyDoo :: checkStatus(){
    if(mood < 0)
        mood = 0;
    else if(mood > 100)
        mood = 100;
    if(mood <= 40){
        cout << endl;
        cout << "Narrator: Scooby is in dire need of a boost! Its unlike him to be this sad!" << endl;
        cout << endl;
    }

    if(hunger < 0)
        hunger = 0;
    else if(hunger > 100)
        hunger = 100;
    if(hunger >= 70){
        cout << endl;
        cout << "Narrator: I think it's time for a Scooby Snack" << endl; 
        cout << endl;
    }

    if(wellness < 0)
        wellness = 0;
    else if(wellness > 100){
        wellness = 100;
    }
    if(wellness <= 40){
        cout << endl;
        cout << "Narrator: Solving mysteries is a hard job, looks like someone needs to rest" << endl;
        cout << endl;
    }
}
void ScoobyDoo :: saveload(){
    fstream file("ScoobyDoo.txt");

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