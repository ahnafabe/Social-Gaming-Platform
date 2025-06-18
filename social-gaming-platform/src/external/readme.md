### How to run

1. Web-socket-networking is included as submodule within external > src, and must be initialized upon cloning the repository. The following command can be run at the root folder:
````
    git submodule update --init --recursive
````
2. cmake and make

3. To run the server, within the build folder, run
````
    ./bin/privateServer <port number> ../src/external/src/external/web-socket-networking/webchat.html
````
4. Go to web browers, enter
````
     localhost:<port number> 
````
### Testing Requests

The following example of requests is the proper way to join a game:

1. Player creates a new game
````
    {"action":"3","body":"This text doesn't matter here", "request_id":"1"}
````
The player who created a game will receive a message with the Join Code

Other players can now join the session using the join code.

2. Joining as a new player (open new terminal/chat)

New player to use the 'Join Code' host got in the body text.
````
    {"action":"0","body":"JY9757", "request_id":"2"}
````    
3. Test Echo Broadcast to all users in session
````
    {"action":"2","body":"Text To Echo", "request_id":"3"}
````

### How to Test
After build and make, 
``./bin/external_tests``
