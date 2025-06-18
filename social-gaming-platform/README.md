# 373-24-hachee

## Cloning Repository

Web-socket-networking is included as submodule within external > src, and must be initialized upon cloning the repository. The following command can be run at the root folder:

    git submodule update --init --recursive

## Running server

Once make is complete, the following code can be used to run the server

    bin/privateServer 8000 ../373-24-hachee/src/external/src/external/web-socket-networking/webchat.html

## Testing Requests

The following example requests will receive a response from the server:

    {"action":"start","body":"any text","sessionID":"123", "request_id":"1"}
    {"action":"end","body":"any text","sessionID":"123", "request_id":"2"}
    {"action":"newGame","body":"any text","sessionID":"123", "request_id":"3"}
    {"action":"echo","body":"text to echo back","sessionID":"123", "request_id":"4"}

Defining sessionIDs will make the user join that room.

Putting a non-defined action as the value will return a 400 Bad Request:
    {"action":"xxx","body":"any text"}