#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

#include "Pet.h"
#include "Garfield.h"
#include "ScoobyDoo.h"
#include "Gary.h"
using namespace std;

int menu(){
    int start;
    cout << "MY A-LIST PETS" << endl;       //try centering everything after
    cout << "[1] New Game" <<endl;          //maybe add a phrase or something at the begining    
    cout << "[2] Load Game" << endl;
    cout << "[3] Quit / Exit" << endl;
    cin >> start;
    return start;
}
string newGame(){
    srand(time(0));
    string petchoice,filename;
    int level, ngMood, ngHunger, ngWellness;
    bool fileNOTopen = true; 
    bool wrongLevel = true;
    
    cout << "Welcome to MY A-LIST PETS, where the main characters are our little ones." << endl;
    cout << "Here, you are the proud owner of one of the most beloved stars all over the world." << endl; //write more for intro
    cout << "To get started, please choose a character from below" << endl;
    cout << "Garfield" << endl;     
    cout << "ScoobyDoo" << endl;
    cout << "Gary(From Spongebob)" << endl;
    while(fileNOTopen){
    cin >> petchoice;
    filename = petchoice + ".txt";          //creates file name

    fstream userData(filename);        
   
    if(!userData.is_open()){
        cout << "ERROR : Please make a valid choice" << endl;       //loops when file does not open
    }
    else{
        cout << "New Game Successfully Created" << endl;
        cout << "Please select difficulty level. Note that this will determine the starting status of " << petchoice << "!" << endl;
        cout << "The more difficult the level, the lower overall pet status" << endl;
        cout << "[1] New pet owner" << endl;            //creates levels of difficulty to choose from
        cout << "[2] Not my first rodeo" << endl;
        cout << "[3] Animal whisperer" << endl;
        cin >> level;

        while(wrongLevel){     //loops if an incorrect level from above is inputted , creates random starting status for the three according to difficulty
        if(level == 1){         
            ngMood = (rand() % 25) + 75; //random num 75-100
            ngHunger = (rand() % 25); //0-25
            ngWellness = (rand() % 25) + 75; 
            wrongLevel = false;
        }
        else if(level == 2){
            ngMood = (rand() % 20) + 40; //random num 40-60
            ngHunger = (rand() % 15) + 25; //25-40
            ngWellness = (rand() % 15) + 50;  //50-65
            wrongLevel = false;
        }
        else if(level == 3){
            ngMood = (rand() % 40) + 20; //random num 20-60
            ngHunger = (rand() % 30)+ 50; //50-80
            ngWellness = (rand() % 30) + 30; //30-60
            wrongLevel = false;
        }
        else{
            cout << "ERROR: Level does not exist, please input valid level" << endl;      
            cin >> level;
        }
        }
        cout << "Before getting started, there are a few things that should be noted. There are three statuses that will be changed according to the actions you choose."
                " Mood shows the overall happiness of " << petchoice << ". Hunger, shows how hungry, and wellness is the overall wellbeing."
                " For some pets, wellness may mean getting some rest, while others it may be getting stimulated in some form activity."
                " Wellness is determined by the characters personality. Without further ado, lets begin!" << endl;

        userData.clear();       //will clear data if there is any
        userData << ngMood << endl;
        userData << ngHunger << endl;
        userData << ngWellness << endl;
        userData.close();

        fileNOTopen = false;
    }
    }
    return petchoice; //will return string to open in main
}
void loadFile(string fname, int *m, int *h, int *w){    //used to load the info of a file, where the statuses are assignmed with mood, hunger, wellness
    fstream file;   
    file.open(fname);
    while(!file.eof()){
        file >> *m >> *h >> *w;
    }
    cout << endl;
    cout << "Current Status" << endl;       //prints current status for new game that was loaded from above
    cout << "---------------" << endl;
    cout << "Mood: " << *m << endl;
    cout << "Hunger: " << *h << endl;
    cout << "Wellness: " << *w << endl;
    cout << endl;

    file.close();
    
}
void actionList(string petType){        //menu for different actions according to the type of pet
    cout << endl;
    cout << "Narrator : Choose an action" << endl;
    cout << endl;
    if(petType == "Garfield"){
        cout << "[1] Give Lasagna" << endl;
        cout << "[2] Watch TV" << endl;
        cout << "[3] Play with Odie" << endl; 
    }
    else if(petType == "ScoobyDoo"){
        cout << "[1] Give a ScoobySnack" << endl;
        cout << "[2] Solve a Mystery" << endl;
        cout << "[3] Rest" << endl;
    }
    else{
        cout << "[1] Give Snail-Po" << endl;
        cout << "[2] Play with his favorite toy ball" << endl;
        cout << "[3] Give a Bath" << endl;
    }
    cout << "[4] Speed up time" << endl;  
    cout << "[5] Save and load" << endl;
    cout << "[6] Quit/Main Menu" << endl;  
}
void execute(string petName, int mood, int hunger, int wellness, int choice){
    int t;
    bool go = true;     //sets loop for different actions until they exit to main menu
    if(petName == "Garfield"){  
        Garfield userPet;       //creates derived class
        userPet.setHunger(hunger);
        userPet.setMood(mood);
        userPet.setWellness(wellness);
        while(go){      //loop for actions
            if(choice == 1){
                userPet.Action1();  //choice 1= Action 1
                userPet.checkStatus(); //checks if the statuses are below or above 100 and fixes them. Also prints prompt if doing poorly.
                userPet.printStatus();  //prints Pets status
                actionList(petName);    //shows menu again
                cin >> choice; 
            }
            else if(choice == 2){
                userPet.Action2();  //choice 2 = action 2
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 3){
                userPet.Action3();      //choice 3 = action 3
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
           }
            else if(choice == 4){
                cout << "Narator: Please Specify how many hours you want to pass by" << endl;
                cin >> t;   
                userPet.Time(t);        //choice 4 = lets time pass
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 5){
                userPet.saveload();     //choice 5 = save and load
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 6){   
                go = false; //choice 6 = quit, returns to main menu and closes inner loop
            }
            else{       //if any number that is not on the loop is entered
                cout << endl;
                cout << "Please type valid action" << endl;
                actionList(petName);
                cin >> choice; 
            }  
        }
    }
    else if(petName == "ScoobyDoo"){
        ScoobyDoo userPet;
        userPet.setHunger(hunger);
        userPet.setMood(mood);
        userPet.setWellness(wellness);

        while(go){
            if(choice == 1){
                userPet.Action1();
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 2){
                userPet.Action2();
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 3){
                userPet.Action3();
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
            }
             else if(choice == 4){
                cout << "Narator: Please Specify how many hours you want to pass by" << endl;
                cin >> t;
                userPet.Time(t);
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 5){
                userPet.saveload();
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 6){
                go = false;
            }
            else{
                cout << endl;
                cout << "Please type valid action" << endl;
                actionList(petName);
                cin >> choice; 
            }  
        }
    }
    else{
        Gary userPet;
        userPet.setHunger(hunger);
        userPet.setMood(mood);
        userPet.setWellness(wellness); 

        while(go){
            if(choice == 1){
                userPet.Action1();
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
            }   
            else if(choice == 2){
                userPet.Action2();
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 3){
                userPet.Action3();
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 4){
                cout << "Narator: Please Specify how many hours you want to pass by" << endl;
                cin >> t;
                userPet.Time(t);
                userPet.checkStatus();
                userPet.printStatus();
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 5){
                userPet.saveload();
                actionList(petName);
                cin >> choice; 
            }
            else if(choice == 6){
                go = false;
            }
            else{
                cout << endl;
                cout << "Please type valid action" << endl;
                actionList(petName);
                cin >> choice; 
            }  
            } 
        }

}

int main(){
    int selection, mood, hunger, wellness,choice, t;
    bool start = true;      //loop for startup menu
    string petName, filename;

    while(start){           //creates loop until option [3] Quit is submitted
        selection = menu();         //calls for main menu
        if(selection == 1){     //New Game
            petName = newGame();        //returns the pet name from the newGame function
            filename = petName + ".txt";  //adds .txt to create file name
            loadFile(filename, &mood, &hunger, &wellness); //loads statuses to respective names inside this function
            
            actionList(petName); //shows actions given the type of pet
            cin >> choice;
            execute(petName, mood, hunger, wellness, choice);       //executes the actions
        }

        else if(selection == 2){    //selection 2 = load
            bool correctName = false;
            cout << "Please enter the name of your pet" << endl; 

            while(!correctName){
            cin >> petName; //will be Garfield, ScoobyDoo or Gary
            filename = petName + ".txt";    //adds .txt to create filename
            if(petName == "Garfield" || petName == "ScoobyDoo" || petName == "Gary"){
                loadFile(filename, &mood, &hunger, &wellness); // loads file
                correctName = true;
            }
            else{
                cout << "ERROR : Please enter valid name" << endl; 
            }
            }

            actionList(petName);        //calls menu for given pet
            cin >> choice;

            execute(petName, mood, hunger, wellness, choice);
        
        } 
        else if(selection == 3){
            start = false; //option 3 = quit, closes main loop and closes everything
        }
        else{
            cout << "Invalid choice, try again" << endl; //if 1,2, or 3 is not entered
        }

    }
    
    return 0;
}