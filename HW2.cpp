#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

using namespace std;

/*RESOURCES USED FOR SOCKETS: 
1) Files given by instructor, in class activity
2)https://www.linuxhowtos.org/C_C++/socket.htm
3)https://www.geeksforgeeks.org/design-a-concurrent-server-for-handling-multiple-clients-using-fork/*/

struct node{        //huffman tree node
    char character;
    int frequency;  

    //pointers for huffman tree
    node *right;
    node *left;
};
class v_nodes{
    private:
        vector<node*> items;    //class consist of a vector of nodes
        node *root;     //saves the root once Huffman tree is created
    public:
        void create(char, int);     //creates new node and adds it to vector

    //functions to sort the vector in descending order
        static bool compare(const  node *a, const node *b){
            if(a->frequency == b->frequency){   //if both nodes have the same frequency it will compare by ascii
                int ascii_a = int(a->character);
                int ascii_b = int(b->character);

                return ascii_a > ascii_b;

            }
            else
                return a->frequency > b->frequency;
        }
        void sort_vector(){
           sort(items.begin(), items.end(),compare);
        }
        
        void Huffman();     //creates Huffman
        void Decompress(node*, string);     //decompresses and prints binary codes
        char find_c(string);        //finds character given binary code(string)
};
void v_nodes::create(char c, int freq){
    //creates new node and sets initial values for node...
    node *newNode = new node;
    newNode->character = c;
    newNode->frequency = freq;
    newNode->left = newNode->right = NULL;

    items.push_back(newNode);       //adds the new node to the vector of nodes
}
void v_nodes::Huffman(){
    //creates huffman...
    while(items.size() > 1){        //continuous loop until there is only the root left in the vector
        sort_vector();      //sorts in descending order

        node *l = items.back();     //first node popped will be labeled as left
        items.pop_back();           //removes last node

        node *r = items.back();     //last node will now be the right node
        items.pop_back();       //removes the last node

        node *newNode = new node;       //creates a new node
        newNode->character = '\0'; //'\0 == asciii value 0 so sorting will still place is correctly
        newNode->frequency = l->frequency + r->frequency;       //new node frequency
        
        //sets left and right pointer or the leafs to the node created
        newNode->left = l;      
        newNode->right = r;

        items.push_back(newNode);   //adds the new node into the vector
       
    }
    root = items[0];
    string s;

    //calls decompress function
    Decompress(root, s);
}
void v_nodes::Decompress(node* root, string s){
    if(!root)
        return;
    if (root->character != '\0')  //will print character if it is not NULL
    {
        cout << "Symbol: " << root->character << ", Frequency: " << root -> frequency << ", Code: " << s<< endl;
        string s;
    }

  Decompress(root->left,s+'0');   //tranverse left of huffman
  Decompress(root->right,s+'1');   //tranverse right of huffman
}
char v_nodes::find_c(string bin_code){
    node* temp = root;      //used to transverse through huffman tree

    //transverse through tree given binary code...
    for(int i = 0;i < bin_code.size(); i++){
        char t_num = bin_code[i];
        if(t_num == '0')
            temp = temp->left;
        else
            temp = temp->right;
    }
    return temp->character; //returns the character of the node received after transversing
}

int main(int argc, char** argv){
    int port_no = atoi(argv[1]);        //port number
    
    string text;

    char c;
    int freq;

    v_nodes list;
    int num_values = 0;     //counts number of values, used to create child processes
    while(getline(cin, text)){
        int size = text.size();
        c = text[0];        //gets character
        text.erase(0,2);        //removes character and space
        freq = stoi(text);

        //insert to vector of nodes
        list.create(c,freq);
        num_values++; 
    }

    list.Huffman();         //creates the huffman tree

    //createing child processes...
    //****used references/ links stated at top
    int sockfd, newsockfd, clilen,n;        //integers used to create processes : file descriptors, n = return values to compare for read/write
    struct sockaddr_in serv_addr, cli_addr;     //structs containing internet address for server ad client

    //did not add the if statement for argc because Homework stated that input would be in proper fromat

    sockfd = socket(AF_INET, SOCK_STREAM, 0);       //created new socket of type stream and  with domain to internet
    if(sockfd < 0){     ///error message is socket is unable to be created /opened
        cerr << "ERROR : UNABLE TO OPEN SOCKET" << endl;
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));       //sets values in buffer to 0
    //sets server vals...
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){ //prints error message if socket is unable to bind to server address
        cerr << "ERROR : BINDING" << endl;
        exit(1);
    }
    listen(sockfd,5);   //lets process to listen on socket for a connection
    clilen = sizeof(cli_addr);      //sets the int clilen to the length of the client address

    int pid;    //for fork
    
    for(int i = 0; i < num_values; i++){    //creates child processses for num_values
       
        newsockfd = accept(sockfd, (struct sockaddr *)& cli_addr, (socklen_t *)&clilen);        //causes the child process to block until a client connects0
        if(newsockfd < 0){  //if newsockfd is not able to return new file descriptor, error on accept will display and terminate program0
            cerr << "ERROR : ACCEPT" << endl;
            exit(1);
        }

        pid = fork();
        //if it is a child process...
        if(pid == 0){      
            //reads size of the string/ char[] sent by client...
            int size; 
            n = read(newsockfd, &size, sizeof(int));        
            if(n < 0){  //if unable to read the size of item sent...
                cerr << "ERROR : reading socket size" << endl;
                exit(1);
            }
          
            char *buffer = new char[size+1];        //sets buffer as a char array with the size that was read from the client
            bzero(buffer, size+1);      //sets all values in the char array = buffer to 0
            n = read(newsockfd, buffer,size);       //reads from newsockfd, places in buffer char[], and sets size as the on received before
            if(n < 0){      //if unable to read the binary code from client
                cerr << "ERROR : reading socket buffer" << endl;
                exit(1);
            }

            string read_code(buffer);       //turns the buffer from char[] to a string for simplicity when transversing through tree
            const char c = list.find_c(read_code);      //finds the character givent the string of binary code, sets as a const char to be able to write to client(client takes const void*)
            
            n = write(newsockfd, &c, sizeof(int));      //writes/ sends character gotten to the client. Send as &c because functions takes a pointer
            if(n < 0){      //if unable to send the character to the client
                cerr << "ERROR : WRITING SOCKET" << endl;
                exit(1);
            }

            close(newsockfd);       //closes the newsockfd to let the other newsockfd/ child processes to run
            delete[] buffer;        
            _exit(0);

        }
   
    }
    //wait...
    for(int i = 0; i < num_values; i++)
        wait(nullptr);     
       
    return 0;
}