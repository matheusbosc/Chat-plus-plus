# Chat++

## What is it?
Chat++ is a small networked chat application built in C++. This is my first C++ project.

Chat++ is terminal-based, featuring a Text UI (made with ArthurSonzogni/FTXUI).

## How does it work?
You first start the server, which allows connections from clients. Whenever a client connects, the server creates a new thread that runs the listener loop

The listener loops listens for messages sent to the server, and then relays that message to all the clients in the same room.

When you start the client, it shows a connection page, which allows the user to put in their name, room, and the server's ip and port.

When you connect, your client sends a connection message to the server, which is relayed to the other clients. This serves as a test message and something
to let other users know you are in the room.

You can then send messages and view the messages in the panel.

## Why did I make it?
I made this application because I was trying to learn a new language, something more low-level, and something more widely used.

I chose this because it not only gave me lots of features to try and learn, but also because it deppened my understanding of networking, something i wanted to learn more about.

I also chose this project because it is cool, and a great addition to a resume!

---
Licensed under the [GNU AGPL license](https://github.com/matheusbosc/Chat-plus-plus?tab=AGPL-3.0-1-ov-file).

Made By Matheus - [matheusbosc.com](matheusbosc.com)
