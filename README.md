# Internet Relay Chat (IRC) Server

## Overview

This project is a custom Internet Relay Chat (IRC) server implemented in C++. This project allows users to connect via IRC clients and communicate in real-time using a text-based protocol. The server supports multiple clients, user authentication, and channel management. Additionally, you can use the implemented **OpenAI bot** or the **OpenWeather bot**, which require your API key.

## Features

- **Real-time Messaging:** Users can send and receive messages instantly.
- **Channel Support:** Join public or private channels to chat with multiple users.
- **User Authentication:** Clients can authenticate with a password.
- **Operator Commands:** Channel operators can manage users with commands like `KICK`, `INVITE`, and `MODE`.
- **Non-blocking I/O:** The server uses non-blocking operations to handle multiple clients simultaneously.
- **BOT Integration:** The code includes bots utilizing the OpenAI API and the OpenWeather API.

## Requirements

- C++98 compliant compiler (e.g., g++, clang++)
- Make utility for building the project
- Basic networking tools (e.g., `nc` for testing)

## Installation

1. **Clone the Repository:**
   ```bash
   git clone <repository-url>
   cd IRC/IRC_SERVER
   ```

2. **Build the Server:**
   ```bash
   make
   ```

## Configuration

You can run the server using the following command:

```bash
./ircserv <port> <password>
```

- `<port>`: The port number on which the server listens for incoming connections (e.g., `6667`).
- `<password>`: The connection password required for clients to connect.

### Example:

```bash
./ircserv 6667 mysecurepassword
```

## Connecting with an IRC Client

To connect to your server, use any IRC client, such as:

- HexChat
- mIRC (Windows)
- Irssi (Linux)
- Textual (macOS)

### Connection Details:

- **Server:** `127.0.0.1` (or your server's IP address)
- **Port:** The port you specified (e.g., `6667`)
- **Password:** The password set during server launch (e.g., `mysecurepassword`)

## Testing the Server

Once connected, you can test the following functionalities:

- Set a nickname and username.
- Join channels using the command `/join <channel>`.
- Send private messages using `/msg <user> <message>`.
- Use operator commands if you have operator privileges.

## Setting up the Bots

### What is a Bot?

Bots are automated programs that can perform tasks in the IRC environment. In this project, we have two bots:

- **OpenAI Bot:** This bot leverages the OpenAI API to respond to user queries and provide conversational capabilities.
- **OpenWeather Bot:** This bot utilizes the OpenWeather API to fetch and provide weather updates based on user input.

### Steps to Set Up the Bots

1. **Set up your API key:**
   Get your OpenAI API key or your OpenWeather API key depending on which bot you plan to use. At the root of the repo directory, run:
   ```bash
   touch .env && echo "API_KEY=yourapikey" > .env
   ```

2. **Build the Bot:**
   If you want to use the OpenAI bot:
   ```bash
   cd IRC_BOT
   make openai
   ```
   If you want to use the OpenWeather bot:
   ```bash
   cd IRC_BOT
   make weather
   ```

3. **Start the Bot:**
   ```bash
   ./executable <server ip> <server port> <server password>
   ```

4. **Testing the Bot:**
   Now, connect to the server with an IRC client and send a private message to the bot with this command: 
   ```bash
   /msg smartboi <message>
   ```
   If your bot is using OpenWeather, send it a city name you want to check the weather for, like:
   ```bash
   /msg smartboi Paris
   ```
   If your bot is using OpenAI, send it a message, and it will respond to you:
   ```bash
   /msg smartboi Hello, how are you doing today?
   ```

## Contributing

Feel free to fork the repository and submit pull requests for enhancements or bug fixes.

## Acknowledgments

This project was developed collaboratively with [Andrii Syvash](https://github.com/redarling).
