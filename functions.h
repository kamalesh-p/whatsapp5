#pragma once

#include <ctime> 

// Commands for Client
const int command_len = 20;
char commands[command_len][20] = {
	"name",			//0		//name_Client-1_hello world
	"names",		//1		//names_Client-1,Client-2,Client-3_hello world
	"signup",		//2		//signup_name_passsword
	"login",		//3		//login_name_passsword
	"logout",		//4		//logout
	"delete",		//5		//delete
	"remove",		//6		//remove
	"myname",		//7		//myname
	"nom",			//8		//name
	"others",		//9		//others
	"del",			//10	//del_id
	"read",			//11	//read_id
	"unread",		//12	//unread_id
	"emoji",		//13	//emoji_id_Emoji-1,Emoji-2,Emoji-3
	"rememoji",		//14    //rememoji_id_Emoji-1,Emoji-2,Emoji-3
	"notemoji",		//15	//notemoji_id_Emoji-1,Emoji-2,Emoji-3
	"edit",			//16    //edit_id_message
	"deactivate",	//17	//deactivate
	"activate",		//18	//activate_name_password
	"search",		//19	//search_word or sentence
};

char emojicommands[emoji_len][20] = {
	"thumbsup", //0
	"heart",	//1
	"star"		//2
};

void replaceChar(char s[DEFAULT_BUFLEN], char oldchar, char newchar) {
	int i;
	for (i = 0; i < strlen(s); i++) {
		if (s[i] == oldchar) {
			s[i] = newchar;
		}
	}
	s[i] = '\0';
}

void replaceChar(char newstring[DEFAULT_BUFLEN], char oldstring[DEFAULT_BUFLEN], char oldchar, char newchar) {
	int i;
	for (i = 0; i < strlen(oldstring); i++) {
		if (oldstring[i] == oldchar) {
			newstring[i] = newchar;
			continue;
		}
		newstring[i] = oldstring[i];
	}
	newstring[i] = '\0';
}

void gettime(char currenttime[50]) {
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	ctime_s(currenttime, 50, &end_time);
	replaceChar(currenttime, '\n', '\0');
	replaceChar(currenttime, ' ', '*');
}

int stoi(char s[15]) {
	int num;
	sscanf_s(s, "%d", &num);
	return num;
}

void split(char word[][30], char sentence[DEFAULT_BUFLEN], int* num) {
	int i, j = 0, count = 0;
	for (i = 0; i < strlen(sentence); i++) {
		if (sentence[i] == ',') {
			word[count][j] = '\0';
			count++;
			j = 0;
			continue;
		}
		word[count][j] = sentence[i];
		j++;
	}
	word[count][j] = '\0';
	*num = count + 1;
}

int split(char word[15][DEFAULT_BUFLEN], char buff[DEFAULT_BUFLEN * 15]) {
	int count = 0, j = 0, i;
	for (i = 0; i < strlen(buff); i++) {
		if (buff[i] == '_') {
			word[count][j] = '\0';
			j = 0;
			count++;
			continue;
		}
		word[count][j] = buff[i];
		j++;
	}
	word[count][j] = '\0';
	return count;
}

void stringcpy(char word1[DEFAULT_BUFLEN], char word2[DEFAULT_BUFLEN]) {
	int i;
	for (i = 0; i < strlen(word2); i++) {
		word1[i] = word2[i];
	}
	word1[i] = '\0';
}

void formatFromMessage(char storemessage[DEFAULT_BUFLEN], char mesg[DEFAULT_BUFLEN], char id[25], char to[15], char sent[50], char receive[50], char read[50], char emoji[50], char note[50]) {
	char sentence[] = "\n(id)        %s\n(message)   %s\n(to)        %s\n(sent)	    %s\n(delivered) %s\n(read)      %s\n(my emoji)  %s\n(oth emoji) %s\n(note)      %s\n\0";
	char thissent[50], thisreceive[50], thisread[50], thisfromemoji[emoji_len * 10] = "\0", thistoemoji[emoji_len * 10] = "\0";
	char newmessage[DEFAULT_BUFLEN];
	char thisemoji[emoji_len * 2][30];
	if (sent == NULL || strcmp(sent, "false") == 0) {
		sprintf_s(thissent, "_");
	}
	else {
		sprintf_s(thissent, "%s", sent);
	}
	if (receive == NULL || strcmp(receive, "false") == 0) {
		sprintf_s(thisreceive, "_");
	}
	else {
		sprintf_s(thisreceive, "%s", receive);
	}
	if (read == NULL || strcmp(read, "false") == 0) {
		sprintf_s(thisread, "_");
	}
	else {
		sprintf_s(thisread, "%s", read);
	}
	if (emoji == NULL || strcmp(emoji, "false") == 0) {
		sprintf_s(thisfromemoji, "_");
		sprintf_s(thistoemoji, "_");
	}
	else {
		int num;
		split(thisemoji, emoji, &num);
		for (int i = 0; i < emoji_len; i++) {
			if (strcmp(thisemoji[i], "1") == 0) {
				sprintf_s(thisfromemoji, "%s%s, ", thisfromemoji, emojicommands[i]);
			}
			if (strcmp(thisemoji[i + emoji_len], "1") == 0) {
				sprintf_s(thistoemoji, "%s%s, ", thistoemoji, emojicommands[i]);
			}
		}
		if (thisfromemoji[0] != '\0') {
			thisfromemoji[strlen(thisfromemoji) - 2] = '\0';
		}
		else {
			sprintf_s(thisfromemoji, "_");
		}
		if (thistoemoji[0] != '\0') {
			thistoemoji[strlen(thistoemoji) - 2] = '\0';
		}
		else {
			sprintf_s(thistoemoji, "_");
		}
	}
	sprintf_s(newmessage, sentence, id, mesg, to, thissent, thisreceive, thisread, thisfromemoji, thistoemoji, note);
	replaceChar(newmessage, '*', ' ');
	stringcpy(storemessage, newmessage);
}

void formatToMessage(char storemessage[DEFAULT_BUFLEN], char mesg[DEFAULT_BUFLEN], char id[25], char from[15], char emoji[50], char note[50]) {
	char sentence[] = "\n(id)        %s\n(message)   %s\n(from)      %s\n(my emoji)  %s\n(oth emoji) %s\n(note)      %s\n\0";
	char newmessage[DEFAULT_BUFLEN];
	char thisfromemoji[emoji_len * 10] = "\0", thistoemoji[emoji_len * 10] = "\0";
	char thisemoji[emoji_len * 2][30];
	if (emoji == NULL || strcmp(emoji, "false") == 0) {
		sprintf_s(thisfromemoji, "_");
		sprintf_s(thistoemoji, "_");
	}
	else {
		int num;
		split(thisemoji, emoji, &num);
		for (int i = 0; i < emoji_len; i++) {
			if (strcmp(thisemoji[i], "1") == 0) {
				sprintf_s(thisfromemoji, "%s%s, ", thisfromemoji, emojicommands[i]);
			}
			if (strcmp(thisemoji[i + emoji_len], "1") == 0) {
				sprintf_s(thistoemoji, "%s%s, ", thistoemoji, emojicommands[i]);
			}
		}
		if (thisfromemoji[0] != '\0') {
			thisfromemoji[strlen(thisfromemoji) - 2] = '\0';
		}
		else {
			sprintf_s(thisfromemoji, "_");
		}
		if (thistoemoji[0] != '\0') {
			thistoemoji[strlen(thistoemoji) - 2] = '\0';
		}
		else {
			sprintf_s(thistoemoji, "_");
		}
	}
	sprintf_s(newmessage, sentence, id, mesg, from, thistoemoji, thisfromemoji, note);
	replaceChar(newmessage, '*', ' ');
	stringcpy(storemessage, newmessage);
}

// num = 0 (Only splitting) (Used to decode from login.txt)
// num = 1 (splitting and analysing) (Used to decode from clients input message)
void analyse(char buff[DEFAULT_BUFLEN], char word0[DEFAULT_BUFLEN], char word1[DEFAULT_BUFLEN], char word2[DEFAULT_BUFLEN], int* num) {
	int count = 0, j = 0, i;
	for (i = 0; i < strlen(buff); i++) {
		if (buff[i] == '_') {
			switch (count) {
			case 0:
				word0[j] = '\0';
				break;
			case 1:
				word1[j] = '\0';
				break;
			case 2:
				word2[j] = '\0';
				break;
			}
			j = 0;
			count++;
			continue;
		}
		switch (count) {
		case 0:
			word0[j] = buff[i];
			break;
		case 1:
			word1[j] = buff[i];
			break;
		case 2:
			word2[j] = buff[i];
			break;
		}
		j++;
	}
	if (count == 2)
		word2[j] = '\0';
	else if (count == 0)
		word0[j] = '\0';
	else if (count == 1)
		word1[j] = '\0';

	if (*num == 1) {
		*num = -1;
		for (j = 0; j < command_len; j++) {
			if (strcmp(word0, commands[j]) == 0) {
				*num = j;
			}
		}
	}
}

void analyse(char buff[DEFAULT_BUFLEN], char word1[DEFAULT_BUFLEN], char word2[DEFAULT_BUFLEN], int* num) {
	char specifier[DEFAULT_BUFLEN];
	*num = 1;
	analyse(buff, specifier, word1, word2, &*num);
}

int process_output(SOCKET localClientSocket, char name[15], int clientnumber, int num, char word1[DEFAULT_BUFLEN], char word2[DEFAULT_BUFLEN]) {
	int i, boolean = 0;
	int value;
	int numb;
	char read[50];
	int alertboolean = 0;
	char letter[50] = "\0";
	switch (num) {
	case 0://send one person
	case 1://send many person
		//word1 -> name/names
		//word2 -> message
		int number;
		char names[10][30];
		char sent[50], receive[50];
		if (num == 1)
			split(names, word1, &number);
		else {
			number = 1;
			strcpy_s(names[0], word1);
		}
		char message[DEFAULT_BUFLEN];
		char newmessage[DEFAULT_BUFLEN];
		strcpy_s(message, word2); // message
		for (int i = 0; i < number; i++) {
			cout << i + 1 << ")" << names[i] << endl;
			if (strcmp(name, names[i]) == 0) {
				cout << "Same User" << endl;
				char alertmessage[70] = "unable to send message - same user";
				winsocket->sendd(localClientSocket, alertmessage, name);
				continue;
			}
			gettime(sent);
		label:
			int value = myfiles->messagefile.loginid.Find(names[i]);
			char id[50];
			char note[50];
			if (value) {
				cout << "Registered User" << endl;
				SOCKET valueClientSocket = client->Find(names[i]);
				if (valueClientSocket != INVALID_SOCKET) {
					gettime(receive);
					gettime(read);
					strcpy_s(id, myfiles->messagefile.Write(name, names[i], message, sent, receive, read));
					strcpy_s(note, "message delivered, message seen");
					formatFromMessage(newmessage, message, id, names[i], sent, receive, read, NULL, note);
					char tonewmessage[DEFAULT_BUFLEN];
					strcpy_s(note, "new message");
					formatToMessage(tonewmessage, message, id, name, NULL, note);
					if (!winsocket->sendd(valueClientSocket, tonewmessage, names[i])) goto label;//Incase sent failure repeat from label
					cout << "Sent Online" << endl;
				}
				else {
					strcpy_s(id, myfiles->messagefile.Write(name, names[i], message, sent, NULL, NULL));
					strcpy_s(note, "message sent");
					formatFromMessage(newmessage, message, id, names[i], sent, NULL, NULL, NULL, note);
					cout << "Sent Offline" << endl;
				}
				winsocket->sendd(localClientSocket, newmessage, name);
			}
			else {
				cout << "Unregistered User" << endl;
				char alertmessage[70] = "***unable to send message - user not registered***";
				winsocket->sendd(localClientSocket, alertmessage, name);
			}
		}
		break;
	case 2:  //signup
	case 3:  //login
	case 17: //deactivate
	case 18: //activate
		//word1 -> name
		//word2 -> password
		if (num == 2) {//signup
			boolean = myfiles->messagefile.loginid.Find(word1, NULL, 1);
			if (boolean) boolean = 0;
			else boolean = 1;
		}
		else if (num == 3)//login
			boolean = myfiles->messagefile.loginid.Find(word1, word2);
		else if (num == 17) {//deactivate
			boolean = myfiles->messagefile.loginid.Find(name, NULL, 1);
		}
		else if (num == 18) {//activate
			char newword1[15];
			sprintf_s(newword1, "#%s", word1);
			boolean = myfiles->messagefile.loginid.Find(newword1, word2, 1);
		}
		if (boolean) {
			if (num == 2 || num == 3) {
				for (i = 0; i < strlen(word1); i++) {
					name[i] = word1[i];
				}
				name[i] = '\0';
				client->changename(localClientSocket, name);
			}
			if (num == 2) {//signup
				myfiles->messagefile.loginid.Append(word1, word2);
				cout << "New user created" << endl;
			}
			else if (num == 3) {//login
				myfiles->messagefile.Fetch(localClientSocket, name);
				cout << "Sent fetched messages" << endl;
				myfiles->messagefile.recent_5_conversation_chats(name, localClientSocket);
				cout << "Sent recent 5 conversation chats" << endl;
			}
			else if (num == 17) {//deactivate
				myfiles->messagefile.loginid.Edit(name, 0);
				cout << name << "-Account deactivated" << endl;
				name[0] = '\0';
				client->changename(localClientSocket, name, 3);
			}
			else if (num == 18) {//activate
				for (i = 0; i < strlen(word1); i++) {
					name[i] = word1[i];
				}
				name[i] = '\0';
				char newword1[15];
				sprintf_s(newword1, "#%s", word1);
				myfiles->messagefile.loginid.Edit(newword1, 1);
				client->changename(localClientSocket, name, 4);
				myfiles->messagefile.Fetch(localClientSocket, name);
				cout << "Sent fetched messages" << endl;
				myfiles->messagefile.recent_5_conversation_chats(name, localClientSocket);
				cout << "Sent recent 5 conversation chats" << endl;
			}
		}
		else {
			char message[25];
			char tempname[15];
			sprintf_s(tempname, "Client-%d\0", clientnumber);
			if (num == 2)
				strcpy_s(message, "#User Already Exist");
			else if (num == 3 || num == 17 || num == 18)
				strcpy_s(message, "#Fail");
			return winsocket->sendd(localClientSocket, message, tempname);
		}
		break;
	case 4: //logout
		name[0] = '\0';
		client->changename(localClientSocket, name, 0);
		break;
	case 5: //delete
	case 6: //remove
		myfiles->messagefile.loginid.Delete(name);
		client->changename(localClientSocket, name, 2);
		name[0] = '\0';
		break;
	case 7: //myname
	case 8: //nom
		client->myname(localClientSocket, name);
		break;
	case 9: //others
		client->getonline(localClientSocket, name);
		break;
	case 10: //del_id
		value = myfiles->messagefile.Edit(word1, name);
		if (value) {
			char newmessage[] = "message deleted";
			winsocket->sendd(localClientSocket, newmessage, name);
		}
		break;
	case 11: // read
		gettime(read);
		numb = 1;
		alertboolean = myfiles->messagefile.Edit(word1, name, numb, read);
		break;
	case 12: // unread
		numb = 2;
		alertboolean = myfiles->messagefile.Edit(word1, name, numb);
		break;
	case 13: // emoji
	case 14: // rem emoji
	case 15: // not emoji
		if (num == 13) {
			number = 3; //4
		}
		else if (num == 14) {
			number = 5; //5
		}
		else if (num == 15) {
			number = 5; //5
		}
		char thisemoji[emoji_len][30];
		int check[emoji_len];
		split(thisemoji, word2, &numb);
		int j;
		for (j = 0; j < emoji_len; j++) {
			check[j] = 0;
		}
		for (i = 0; i < numb; i++) {
			for (j = 0; j < emoji_len; j++) {
				if (strcmp(thisemoji[i], emojicommands[j]) == 0) {
					check[j] = 1;
				}
			}
		}
		for (j = 0; j < emoji_len; j++) {
			sprintf_s(letter, "%s%d,", letter, check[j]);
		}
		letter[strlen(letter) - 1] = '\0';
		alertboolean = myfiles->messagefile.Edit(word1, name, number, letter, localClientSocket);
		break;
	case 16: // edit message
		number = 7;
		alertboolean = myfiles->messagefile.Edit(word1, name, number, word2, localClientSocket);
		break;
	//case 17: //deactivate
	//case 18: //activate    
		//near case 2: case 3:
	case 19:
		myfiles->messagefile.Search(name, word1, localClientSocket); 
		cout << "Search sent" << endl;
		break;
	}
	if (num == 11 || num == 12) {
		if (alertboolean) {
			char alertmessage[50] = "message updated";
			winsocket->sendd(localClientSocket, alertmessage, name);
		}
	}
	if ((num == 11 || num == 12 || num == 13 || num == 14 || num == 15 || num == 16) && !alertboolean) {
		char alertmessage[50] = "task not completed";
		winsocket->sendd(localClientSocket, alertmessage, name);
	}
	return 1;
}

void encode(char s[DEFAULT_BUFLEN * 5]) {
	int i, num;
	//cout << s << endl;
	for (i = 0; i < strlen(s); i++) {
		if (s[i] == ' ') continue;
		num = s[i];
		num += 4;
		if (num > 127) {
			num -= 127 - 1;
		}
		s[i] = num;
	}
	//cout << s << endl << endl;
}

void decode(char s[DEFAULT_BUFLEN * 5]) {
	int i, num;
	//cout << s << endl;
	for (i = 0; i < strlen(s); i++) {
		if (s[i] == ' ') continue;
		num = s[i];
		num -= 4;
		if (num < 0) {
			num = 127 - num + 1;
		}
		s[i] = num;
	}
	//cout << s << endl << endl;
}
