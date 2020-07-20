//Insta Chatbot
//Written by Dylan Webb 
//Version 1.3 -- 7/19/2020

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <ctime>

using namespace std;

//formats input to match text
string refineText(const string text);
//converts .json file to .txt file and populates a vector for mapping
void jsonToTxt (const string name);
//orders vector of text chronologically
vector<string> orderText(vector<string> text);
//searches convo for similar text
vector<string> parseConvo(string input);
//creates a random response using maps and initial words
void generateResponse(const vector<string> sampleText);
//checks if a file exists
bool existsTest(const string& name);
//converts string to a vector
vector<string> stringToVector(const string text);
//checks if username is in a file
bool nameInFile(const string name); 

vector<string> wordvect; //vector used to store all text in lines
string inputText; //string of concatonated text from parseConvo

int main(int argc, char** argv) {
  //generate txt files for the given username
  string input;
  
  //checks that necessary files exist and runs setup
  if (!existsTest("convo.txt")) {
    cout << "------First time setup------" << endl;
    if (argc == 1) {
      cout << "Enter your instagram username" << endl << " : ";
      getline(cin, input);
      if (!nameInFile(input)) {
        cout << "Error: username not found" << endl;
        getline(cin, input);
        return 0;
      }
    }
    else if (argc == 2) {
      input = argv[1];
    }
    else {
      cout << "Error: incorrect number of arguments" << endl;
      getline(cin, input);
      return 0;
    }

    if (!existsTest("messages.json")) {
      cout << "Error: messages.json file missing" << endl;
      getline(cin, input);
      return 0;
    }
    else {
      cout << "Creating necessary files..." << endl;
      cout << "Generating chatbot..." << endl;
      jsonToTxt(input);
    }
  }
  cout << "Type \"quit\" to exit the program" << endl;

  //copies user text into a vector
  string word;
  ifstream fin;
  fin.open("convo.txt");
  while(fin >> word) {
    wordvect.push_back(word);
  }
  fin.close();

  //loop for text input and response output
  while (input != "quit") {
    cout << "Q: ";
    getline(cin, input);
    if (input != "quit") {
      input = refineText(input);
      vector<string> inputVect = parseConvo(input);
      generateResponse(inputVect);
    }
  }
  return 0;
}

bool nameInFile(const string name) {
  string word;
  string username = "\"" + name + "\",";
  ifstream fin; //opens designated file
  fin.open("messages.json");
  //searches file for name
  while (fin >> word) {
    if (word == username) {
      return true;
    }
  }
return false;
}

vector<string> stringToVector(const string text) {
  vector<string> output;
  int j = 0;
  //convert string into vector for output
  for (int i = 0; i < text.length(); ++i) {
    if (text.at(i) == ' ') { //at each space the last word is added
      output.push_back(text.substr((i - j), j));
      j = 0;
    }
    else {
      ++j;
    }
  }
  output.push_back(text.substr(text.length() - j, j + 1));
  return output;
}


bool existsTest(const string& name) {
  if (FILE *file = fopen(name.c_str(), "r")) {
      fclose(file);
      return true;
  } else {
      return false;
  }   
}

string refineText(const string text) {
  bool space = false;
  string newText;
  //loops to keep only spaces and letters all lowercase
  for (int i = 0; i < text.length(); ++i) {
    if (isspace(text.at(i))) {
      if (!space) {
        space = true;
        newText.push_back(text.at(i));
      }
    }
    else {
      space = false;
    }
    if (isalpha(text.at(i))) {
      newText.push_back(tolower(text.at(i)));
    }
    else if (isdigit(text.at(i)) || text.at(i) == '%') {
        newText.push_back(text.at(i));
      }
  }
  //adds an extra space at the end if there isn't one already
  if (newText != "") {
    if (newText.at(newText.length() - 1) != ' ') {
      newText.push_back(' ');
    }
  }
  return newText;
}

void jsonToTxt (const string name) {
  vector<string> text;
  vector<string> tempText = {"/"};
  string word;
  string username = "\"" + name + "\",";

  bool scan = true;
  bool process = false;
  bool first = false;
  bool user = false;
  bool userSpoke = false;
  bool appendText = false;
  
  vector<int> Q;
  vector<int> A;
  bool userText = false;

  int count = 0;
  
  ifstream fin; //opens designated file
  
  fin.open("messages.json");

  //populates set with text
  while (fin >> word) {
    
    if (word == "\"likes\":" || word == "\"media\":" 
    || word == "\"link\":" ) { //skip over liked messages and media
      scan = false;
    }
    else if (word == "\"date\":") { //stop skipping
      scan = true;
    }
    else if (word == "{\"participants\":") {
      //add a blank response to divide conversations
      tempText.push_back("/");
      vector<string> orderedText = orderText(tempText);
      for (int i = 0; i < orderedText.size(); ++i) {
        text.push_back(orderedText.at(i));
      }
      tempText.clear();
      text.push_back("");
      Q.push_back(count);
      ++count;
      
    }

    //adds words to text and processes them
    if (process) {
      string newWord = "";
      if (first) { //remove extra bracket and designate speaker
        word.erase(0, 1);
        tempText.push_back("/");
        //add a Q or A depending on speaker and which bot is created
        if (user) {
          A.push_back(count);
          if (!userSpoke) { //boolean tracks who spoke last
            appendText = true;
            userSpoke = true;
          }
        }
        else {
          Q.push_back(count);
          if (userSpoke) {
            appendText = true;
            userSpoke = false;
          }
        }
        first = false;
      }

      //loop to rearrange tempText in chronological order
      //and append tempText to text
      if (appendText) {
        vector<string> orderedText = orderText(tempText);
        for (int i = 0; i < orderedText.size(); ++i) {
          text.push_back(orderedText.at(i));
        }
        tempText.clear();
        tempText.push_back("/");
        appendText = false;
      }

      //loop to add words to tempText
      for (int i = 0; i < word.size(); ++i) {
        if (word.at(i) == '}') { // if close bracket stop adding
          process = false;
          user = false;
        }
        else if (isalpha(word.at(i))) { //remove all punctuation
          newWord.push_back(tolower(word.at(i)));
        }
        else if (isdigit(word.at(i)) || word.at(i) == '%') {
          newWord.push_back(word.at(i));
        }
      }
      //removes hyperlinks from text
      if (word.substr(0,5) != "\"http" 
      && word.substr(0,4) != "http") {
        tempText.push_back(newWord);
        ++count;
      }
    }

    //scans for words to add to text
    else if (scan) {
      //checks if user or other is speaking
      if (word == username) {
        user = true;
      }
      else if (word == "\"text\":") { //ready to add text
        process = true;
        first = true;
      }
    }
  }
  //adds final lines unsorted
  for (int i = 0; i < tempText.size(); ++i) {
    if (tempText.at(i) != "/") {
      text.push_back(tempText.at(i));
    }
  }
  fin.close(); //close the file

  //output text as a conversation switching lines between speakers
  ofstream fout;
  fout.open("convo.txt");

  for (int i = 0; i < text.size(); ++i) {
    if (!userText) {
      //checks if user is speaking
      for (int a = 0; a < A.size(); ++a) { 
        if (A.at(a) == i) {
          userText = true;
          if (i > 0) {
            fout << endl;
          }
        }
      }
    }
    else if (userText) {
      //checks if other person is replying
      for (int q = 0; q < Q.size(); ++q) { 
        if (Q.at(q) == i) {
          userText = false;
          if (i > 0) {
            fout << endl;
          }
        }
      }
    }
    if (text.at(i) != "") {
      fout << text.at(i) << " "; //outputs text
    }
  }
  fout.close(); //close file
}

vector<string> orderText(vector<string> text) {
  //loops backward until new line symbol / and then forward
  vector<string> orderedText;
  text.pop_back();
  int endline = text.size();
  for (int i = text.size() - 1; i >= 0; --i) {
    if (text.at(i) == "/") {
      for (int j = i + 1; j < endline; ++j) {
        orderedText.push_back(text.at(j)); //adds words to text
      }
      endline = i;
    }
  }
  return orderedText;
}

vector<string> parseConvo(string input) {
  int lineNum = 0;
  string line;
  string lastLine;
  vector<int> wordPos;
  vector<string> matches;
  vector<string> output;
  vector<string> lines;
  bool firstSearch = true;
  bool found = false;

  //create vector of position of spaces
  for (int i = 0; i < input.length(); ++i) {
    if (input.at(i) == ' ') {
      wordPos.push_back(i);
    }
  }

  //subtract a word from the input text until match found in convo
  for (int i = wordPos.size() - 1; i > -2; --i) {

    ifstream fin; //opens designated file
    fin.open("convo.txt");

    if (firstSearch) {
      //loop to find a line which exactly matches input
      while (getline(fin, line)) {
        lineNum++;
        lines.push_back(line);
        if (line == input 
        && lastLine.size() <= 12*sqrt(input.size()) 
        && input.length() > 0) {
          matches.push_back(lastLine);
          found = true;
        }
        else {
          //saves response in case following line matches
          lastLine = line;
        }
      }
      firstSearch = false;
    }
    else {
      //loop to find a line which contains input or most of it
      while (getline(fin, line) && input.length() > 0) {
        //looks for input in line
        if (line.find(input) != std::string::npos 
        && line.size() <= 10*sqrt(input.size())
        && lastLine.size() <= 14*sqrt(input.size())) {
          matches.push_back(lastLine);
          found = true;
        }
        else {
          //saves response in case following line matches
          lastLine = line;
        }
      }
    }

    fin.close(); //close file
    
    //if a match is found end loop and output response
    if (found) {
      srand(time(NULL));
      int index = rand() % matches.size();
      string text = matches.at(index);
      output = stringToVector(text);
      inputText = input;
      return output;
    }
    else { //if match not found, remove last word and retry
      if (i > -1) {
        input = input.substr(0, (wordPos.at(i)));
      }
    }
  }

  //generates random response when no match found
  srand(time(NULL));
  int randLine = rand() % lineNum;
  output = stringToVector(lines[randLine]);
  inputText = input;
  return output;
}

void generateResponse(const vector<string> sampleText) {
  map<vector<string>, vector<string> > wordmap;
  vector<string> state;
  vector<string> addList;
  srand(time(NULL));
  int M;
  
  //adds more randomness when not much sample text provided
  if (sampleText.size() <= 2 || inputText.size() <= 5) {
    M = 2;
  }
  else {
    M = 3;
  }

  for (int i = 0; i < M; ++i) {
    state.push_back(""); //populates the vector of states
  }

  //adds the sample text to the front of the vector to start there
  for (int i = sampleText.size() - 1; i >= 0; --i) {
    wordvect.insert(wordvect.begin(), sampleText.at(i));
  }

  //creates a map of associated words
  for (vector<string>::iterator it = wordvect.begin(); it != 
  wordvect.end(); it++) {
    wordmap[state].push_back(*it);
    state.push_back(*it);
    state.erase(state.begin());
  }
  
  state.clear();
  for (int i = 0; i < M; ++i) {
    state.push_back("");
  }

  //prints the wordmap
  cout << "A: ";
  for (int i = 0; i < ((M * M) + 1); ++i) {
    int ind = rand() % wordmap[state].size();
    cout << wordmap[state][ind] << " ";

    //list of words which can't finish a sentence
    addList = {"the", "a", "an", "and", "but", "for", "or", "so",
    "i", "im", "to", "too", "above", "around", "at", "before", "by",
    "from", "in", "into", "of", "on", "to", "with", "what", "whats",
    "was", "is", "are", "will", "be", "were", "like", "likes", "my",
    "liked", "how", "howd", "where", "when", "who", "your", "youre", 
    "their", "theyre", "theyd", "theyll", "hes", "hed", "hell",
    "shes", "shed", "shell", "its", "itd", "itll", "about",
    "thats", "thatd", "thatll", "wed", "well", "id", "ill", "am",
    "arent", "dont", "wont", "cant", "do", "didnt", "wouldnt"};
    if (i == (M * M)) { //extends sentence if word incomplete
      for (int j = 0; j < addList.size(); ++j) {
        if (wordmap[state][ind] == addList.at(j)) {
          i--;
          break;
        }
      }
    }

    state.push_back(wordmap[state][ind]);
    state.erase(state.begin());
  }
  cout << endl;
}