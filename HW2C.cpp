#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <vector>
#include <string>
#include <pthread.h>
#include <sstream>

using namespace std;

struct arg{     //multithread arguments
    string bin_code;        
    string pos;

    int port_num;
    string server;

    vector<char> *message;      //stores the original message here
};
//multithreaf function...
void *getMessage(void *xval){
    struct arg *x = (struct arg *)xval;     //turns the void * into the struct of arg

    vector<int> pos;        
    string temp;

    //turns the string of positions into a vector of integers...
    stringstream ss(x->pos);
    while (getline(ss,temp,' ')){
        pos.push_back(stoi(temp));  
    }

    //creating socket....
    //values needed to create the socket
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;


    sockfd = socket(AF_INET,SOCK_STREAM, 0);        //creates new socket type stream and domain internet

    if(sockfd < 0){     //if unable to create/ open socket
        cerr << "ERROR: UNABLE TO OPEN SOCKET" << endl;
        exit(1);
    }
    
    //turns the string from struct of the server into a char arr
    string s = x->server;
    char arr[s.length()+1];
    strcpy(arr, s.c_str());
    
    server = gethostbyname(arr);    //assigns the struct of the server by the char array above, sees if given host is found
    if(server == NULL){     //if host ins unable to be gotten/found
        cerr << "ERROR: NO HOST" << endl;
        exit(1);
    }

    //sets the fields/ values for the server...
    bzero((char*)&serv_addr, sizeof(serv_addr));   
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr,server->h_length);       //copies length bytes from server_addr struct to the strcut of server's host length 
    serv_addr.sin_port = htons(x->port_num);        //sets server port number to the one saved in argument
    
    
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){  //if connection unable t be established between client and server
        cerr << "ERROR: CONNECTING" << endl;
        exit(1);
    }
    
    //sets binary code from the struct to a char array of name message
    string b_code = x->bin_code;
    char message[b_code.length()+1];
    strcpy(message,b_code.c_str());
    int sMessage = strlen(message); //length of message
    n = write(sockfd, &sMessage, sizeof(int));      //writes/sends to the server the size of the message
    if(n < 0){      //if unable to write to socket
        cerr << "ERROR: WRITING TO SOCKET" << endl;
        exit(1);
    }
    
    n = write(sockfd, message, sMessage);       //sends actual message
    if(n < 0){      //if message is unable to be written to socket
        cerr << "ERROR: WRITING TO SOCKET" << endl;
        exit(1);
    }

    char *buffer = new char;        //where the value received from server will be stored
    bzero(buffer, 1);   //sets the buffer char to value of zero
    
    n = read(sockfd, buffer,1); //reads into the buffer the character that the server sent
 

    //places the character gotten from server in the positions that they are supposed to be
    for(int i = 0; i < pos.size(); i++){
        x->message->at(pos[i]) = *buffer;   //sets message at the certain position as the *buffer to get the actual char
    }
    delete buffer;
    close(sockfd);

    return NULL;
}

int main(int argc, char *argv[]){
    string hostname = argv[1];
    int port_no = atoi(argv[2]);

    string text;
    int num_values = 0;     //number of threads
    int size = 0;       //size of original message

    vector<string> bin;
    vector<string> positions;

    while(getline(cin, text)){
        size += count(text.begin(), text.end(), ' ');   //counts the instance of spaces to see the size of the original message
        
        //sets the string for the binary code and puts it in the vector of binary codes...
        string bin_t = text.substr(0, text.find(" "));
        bin.push_back(bin_t);
        text.erase(0, bin_t.length()+1);

        positions.push_back(text);  //sets rest of the string as a string of location and puts in the positions vector

        num_values++;     
    }

    //creating threads...
    struct arg x[num_values];
    pthread_t tid[num_values];
    vector<char> answer(size);

    for(int i = 0; i < num_values; i++){
        //values to find char...
        x[i].bin_code = bin[i];
        x[i].pos = positions[i];
        //values to make socket...
        x[i].port_num = port_no;
        x[i].server = hostname;

        x[i].message = &answer; //final answer

        if(pthread_create(&tid[i],NULL,getMessage,&x[i])){
            fprintf(stderr,"Error creating thread \n");
            return 1;
        }
    }

    //wait
    for(int i = 0; i < num_values; i++)
        pthread_join(tid[i],NULL);

    cout << "Original message: ";
    for(int i = 0; i< answer.size(); i++)
        cout << answer[i];

    return 0;
}