#pragma once
//Linked List
class Node {
public:
    char name[15];
    SOCKET ClientSocket;
    Node* next;
    long int id;
    thread* t;
    Node() {
        name[0] = '\0';
    }
    ~Node() {
        //this->t->join();
    }
};

class Clients {
public:
    Node* head = NULL;
    int length;
    int wait;
    char name[DEFAULT_BUFLEN];
    Clients() {
        length = 0; 
        wait = 0;
    }
    void myname(SOCKET ClientSocketmyname, char name[15]) {
        Node* thisNode = head;
        while (thisNode != NULL) {
            if (ClientSocketmyname == thisNode->ClientSocket) {
                char message[50] = "\0";
                sprintf_s(message, "func(%s), list(%s)\0", name, thisNode->name);
                if (!winsocket->sendd(ClientSocketmyname, message, thisNode->name))return;
                break;
            }
            thisNode = thisNode->next;
        }
    }
    void remove(SOCKET ClientSocket) {
        while (this->wait) {
            cout << "Remove waiting...." << endl;
            Sleep(50);
        }
        this->wait = 1;
        Node* prev = head; // empty header
        Node* current = head->next; // the first valid node
        while (current != NULL && current->next != NULL) {
            if (current->ClientSocket == ClientSocket) {
                break;
            }
            else {
                prev = current;
                current = current->next; // go to next value
            }
        }
        if (head != NULL && head->next == NULL && head->ClientSocket == ClientSocket) {
            cout << "Deleting(" << head->name << ")" << endl;
            //head->t->join();
            head = NULL;
        }
        else if (current == NULL) { // if we reached end of list or the list is empty
            cout << "Can't remove value: no match found.\n";
        }
        else {
            cout << "Deleting(";
            prev->next = current->next; // unlink the node you remove
            //current->t->join();
            cout << current->name << ")" << endl;
            delete current; // delete the node
        }
        Sleep(100);
        this->wait = 0;
    }
    void append(SOCKET ClientSocket) {
        while (this->wait) {
            cout << "Append waiting...." << endl;
            Sleep(50);
        }
        this->wait = 1;
        Node* currentNode = new Node();
        currentNode->ClientSocket = ClientSocket;
        char name[15];
        name[0] = '\0'; //name should be null until user login
        currentNode->t = new thread(threadreceive(ClientSocket, winsocket->CountClientNumber, name), 1);
        strcpy_s(currentNode->name, name);
        currentNode->next = NULL;
        if (head == NULL) {
            cout << "Append successful\n";
            head = currentNode;
        }
        else {
            Node* thisNode = head;
            while (thisNode != NULL && thisNode->next != NULL) {
                //cout << thisNode->name << endl;
                thisNode = thisNode->next;
            }
            //cout << thisNode->name << endl;
            cout << "Append successful\n";
            thisNode->next = currentNode;
        }
        length++;
        Sleep(100);
        this->wait = 0;
    }
    int changename(SOCKET ClientSocketchangename, char chngenome[15], int num = 1) {
        Node* thisNode = head;
        while (thisNode != NULL) {
            if (ClientSocketchangename == thisNode->ClientSocket) {
                strcpy_s(thisNode->name, chngenome);
                char message[30];
                if (num == 1) {
                    strcpy_s(message, "#Login Successful");
                }
                else if (num == 2) {
                    strcpy_s(message, "#Account Deleted");
                }
                else if (num == 3) {
                    strcpy_s(message, "#Account Deactivated");
                }
                else if (num == 4) {
                    strcpy_s(message, "#Account Activated");
                }
                else {
                    strcpy_s(message, "#Logged out");
                }
                if (!winsocket->sendd(ClientSocketchangename, message, thisNode->name))return 0;
                break;
            }
            thisNode = thisNode->next;
        }
        return num;
    }
    void getonline(SOCKET ClientSocketgetonline, char SendName[15]) {
        Node* thisNode = head;
        name[0] = '\0';

        while (thisNode != NULL) {
            if (ClientSocketgetonline != thisNode->ClientSocket && thisNode->name[0] != '\0') {
                //cout << thisNode->name << endl;
                if (strlen(name) + strlen(thisNode->name) >= DEFAULT_BUFLEN) {
                    if (!winsocket->sendd(ClientSocketgetonline, name, SendName))return;
                    sprintf_s(name, "\0");//empty string
                }

                int j = strlen(name);
                int i;
                for (i = 0; i < strlen(thisNode->name); i++) {
                    name[j + i] = thisNode->name[i];
                }
                name[j + i] = '\n';
                name[j + i + 1] = '\0';
            }
            thisNode = thisNode->next;

        }
        if (!winsocket->sendd(ClientSocketgetonline, name, SendName))return;
    }
    SOCKET Find(char name[15]) {
        Node* thisNode = head;
        char message[DEFAULT_BUFLEN] = "\0";
        while (thisNode != NULL) {
            if (strcmp(thisNode->name, name) == 0) {
                return thisNode->ClientSocket;
            }
            thisNode = thisNode->next;
        }
        return INVALID_SOCKET;
    }
};


