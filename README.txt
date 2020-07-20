Insta Chatbot
Written by Dylan Webb 
V 1.0 -- 5/22/2020
V 1.1 -- 5/23/2020
  fixed hyperlinks for shared media appearing in convo.txt
  added support for numbers
V 1.2 -- 7/18/2020
  fixed order of text by same speaker to be chronological
  fixed hyperlinks ending messages appearing in convo.txt
  output length now depends on output accuracy
V 1.3 -- 7/19/2020
  added support for Mac

PURPOSE AND METHOD

This project was inspired by my interest in creating a personalized chatbot, sort of like Jarvis in Iron Man. It mimics human speech and responds to input with related output. Chatbots are sometimes used therapeutically, but this one is mostly intended for enjoyment. It converses with the user, however, and can provide basic company. Although this chatbot may not be as accurate as others, it is more personal as it responds based on sample text provided by the user--a more personalized chatbot.

The program relies on the user's log of direct messages, or DMs, for the database from which responses are drawn. This file, called messages.json, is accessible to all Instagram users for free, whereas methods for compiling sms texts and iMessages often cost money or come with viruses. The program matches user-inputted text to relevant text in the user's Instagram conversations, and responds based off of this. In this way it mimics the language of the user and provides a more personalized conversation.

INSTRUCTIONS FOR USE

InstaChatbot.exe and main.cpp are included
Download InstaChatbot.exe or compile main.cpp and place it in a folder on your computer
Request your data from instagram and locate the messages.json file in the folder you are sent
Place the messages.json file in the same folder as the InstaChatbot.exe program

Run the program, and enter your instagram username (this only occurs on first time setup, which will take a little longer)
This will not in any way affect your account--it is just necessary for parsing the conversation and separating the user's messages and others' replies
The program will create a convo.txt file which it will reference in the future

Now enter anything you would like to say, and the chatbot will respond!