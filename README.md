# DBUS Session Bus Example

This repository contains a small GLib/GIO D-Bus demo that shows how to:

- register a service name on the D-Bus session bus,
- export a D-Bus interface from a server process,
- call that interface from a client process,
- receive and print the returned result using an asynchronous callback.

The program is intentionally simple and is meant as a learning example for D-Bus service/client communication in C using GLib.

## Project Overview

The example uses a generated D-Bus binding from the XML description in `com.DBUS_gen.xml`.

- Service name: `com.DBUS_gen`
- Interface name: `com.DBUS_gen.server_client_network`
- Object path: `/`
- Method used by the client: `client`

The client sends an integer value `num1` to the server. The server computes:

```text
ans = num1 + 1
```

and returns `ans` to the client only when `num1 > 1`.

If the client passes a value less than or equal to `1`, the server does not send a response.

## Repository Structure

- `server.c` — server-side implementation that owns the D-Bus name and exports the interface.
- `client.c` — client-side implementation that creates a proxy, calls the remote method, and waits for the async callback.
- `com.DBUS_gen.xml` — D-Bus interface definition used by `gdbus-codegen`.
- `mydbus.c` and `mydbus.h` — generated GLib binding files produced from the XML file.
- `Makefile` — builds the server and client binaries.
- `run.sh` — convenience script that regenerates bindings and builds the binaries.

## Build Requirements

Make sure the following are installed on the system:

- `gcc`
- `glib-2.0`
- `gio-2.0`
- `gdbus-codegen`
- `pkg-config`

## Build Instructions

You can build the project in either of these ways:

### Option 1: Use the provided script

```sh
chmod +x run.sh
./run.sh
```

The script will:

1. remove previous build artifacts,
2. regenerate `mydbus.c` and `mydbus.h`,
3. compile `server` and `client`.

### Option 2: Use Make directly

```sh
make clean
make gen
make all
```

## How to Run

Start the server first:

```sh
./server
```

Then, in another terminal, run the client with an integer argument:

```sh
./client 3
```

Example behavior:

- Input: `3`
- Server receives `3`
- Server computes `4`
- Client prints:

```text
Answer = 4
```

## Complete Flow Diagram

The following plain-text flow explains the full lifecycle of this D-Bus example more clearly:

```text
Build Phase
-----------
com.DBUS_gen.xml
    -> gdbus-codegen
    -> mydbus.c + mydbus.h
    -> make builds server and client

Server Startup
--------------
./server
    -> server.c main()
    -> create GLib main loop
    -> register D-Bus name: com.DBUS_gen on session bus
    -> on_name_acquired()
    -> create skeleton interface
    -> connect handle-client signal
    -> export interface on object path /
    -> wait for incoming D-Bus method calls

Client Startup
--------------
./client 3
    -> client.c main()
    -> create proxy for com.DBUS_gen
    -> call remote method client(num1 = 3)
    -> wait for async callback

Request Processing
-----------------
Server receives num1
    -> if num1 > 1
        -> ans = num1 + 1
        -> send ans back to client
    -> if num1 <= 1
        -> no response is sent

Result
------
client callback receives ans
    -> print Answer = 4
```

## Server Behavior

The server performs the following steps:

1. Creates a GLib main loop.
2. Registers the D-Bus name `com.DBUS_gen` on the session bus.
3. When the name is acquired, creates a skeleton object for the interface.
4. Connects the `handle-client` signal to the callback `on_handle_network`.
5. Exports the interface on the object path `/`.
6. Waits in the main loop for incoming D-Bus requests.

Inside `on_handle_network()`:

- it receives `num1`,
- computes `ans = num1 + 1`,
- sends `ans` back only if `num1 > 1`.

## Client Behavior

The client:

1. checks that a command-line argument is provided,
2. creates a D-Bus proxy for the service,
3. calls the remote method `client`,
4. waits in a GLib main loop until the async callback completes,
5. prints the returned answer and exits.

## D-Bus Interface Definition

The interface is declared in `com.DBUS_gen.xml`:

```xml
<interface name="com.DBUS_gen.server_client_network">
  <method name="server">
    <arg name="num1" direction="in" type="i" />
    <arg name="ans" direction="out" type="i" />
  </method>
  <method name="client">
    <arg name="num1" direction="in" type="i" />
    <arg name="ans" direction="out" type="i" />
  </method>
</interface>
```

In practice, the implemented code path uses the `client` method and returns `num1 + 1` as the answer.

## Notes

- This project demonstrates a basic D-Bus service/client exchange.
- It uses the session bus, not the system bus.
- `mydbus.c` and `mydbus.h` are generated files and should not be hand-edited.
- If the argument value is `1` or less, the current server logic does not emit a response.

## Clean Build

To remove all generated binaries and object files:

```sh
make clean
```

## Summary

This repository is a compact example of a GLib D-Bus application where:

- the server registers a well-known name,
- the client connects to that service through a proxy,
- the server handles a method call asynchronously,
- the result is returned over the session bus.

