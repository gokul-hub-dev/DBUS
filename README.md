# DBUS

#run
./run.sh

this script will generate server and client bin file, execute those bin file/
./server
./client 3

if client argument value lessthen 1 server won't response.

##################################server######################################


+---------------------+             +---------------------------+
|   Start Program     |             |   Register D-Bus Name      |
|                     |             |   "com.DBUS_gen"           |
+----------+----------+             +--------------+------------+
           |                                         |
           v                                         |
+---------------------+             +----------------+-----------+
|  Initialize GLib    |             |  on_name_acquired Callback |
|  Main Loop          |             |  (D-Bus Name Acquisition)  |
+----------+----------+             +--------------+-------------+
           |                                         |
           v                                         |
+---------------------+             +----------------+----------+
|  Print "Server      |             |  Create D-Bus Interface   |
|  running..."        |             |  Skeleton                 |
+----------+----------+             +--------------+------------+
           |                                         |
           v                                         |
+---------------------+             +----------------+------------+
|  g_bus_own_name     |             |  Export Interface on D-Bus  |
|  (Register D-Bus    |             |  (Interface at "/")         |
|  Name Callback)     |             +----------------+------------+
+---------------------+                              |
           |                                         |
           v                                         |
+---------------------+             +----------------+------------+
|  g_main_loop_run    |<------------+  Handle Client Method       |
|  (Start GLib Main   |             |  Calls (on_handle_network)  |
|  Loop)              |             +----------------+------------+
+---------------------+                              |
           |                                         |
           v                                         |
+---------------------+             +----------------+------------+
|                     |             |  Respond to Client        |
|                     +------------>+  Message Sent!            |
|  End Program        |             +---------------------------+
+---------------------+

1.Initialization:
Start the program.
Initialize GLib main loop (g_main_loop_new).

2.D-Bus Name Registration:
Register the D-Bus name "com.DBUS_gen" on the session bus using g_bus_own_name.
Provide a callback (on_name_acquired) for handling the acquisition of the name.
on_name_acquired Callback:

3.Upon successful acquisition of the D-Bus name:
Create a new instance of namespaceDBUSServer.client_network interface skeleton.
Connect the handle-client signal of the interface to the on_handle_network callback function.
Export the interface on D-Bus at path "/" using g_dbus_interface_skeleton_export.

4.Main Event Loop:
Print "Server running..." to indicate the server is operational.
Enter the GLib main loop (g_main_loop_run) to handle D-Bus events and signals.

5.Handling Client Requests:
When a client sends a method call on the namespaceDBUSServer.client_network interface:
The on_handle_network callback function is invoked.
It processes the method call (num1), computes ans = num1 + 1, and sends ans back to the client if num1 > 1.
Prints relevant messages indicating the received number and the completion of message sending.

6.Continued Operation:
The server remains active, continuously handling D-Bus events and client requests within the main loop.

7.End of Program:
The program terminates when explicitly stopped or upon encountering an exit condition.


####################################client###########################################

  +-----------------+
  |   Start Program |
  +--------+--------+
           |
           v
  +-----------------+
  |  Check Command  |
  |  Line Arguments |
  +--------+--------+
           |
           v
  +-----------------+
  |  Create D-Bus   |
  |  Proxy          |
  +--------+--------+
           |
           v
  +--------------------------+
  |  Register Async          |
  |  Callback                |
  |  (callback_client_async) |
  +--------+-----------------+
           |
           v
  +-----------------+
  |  Parse Command  |
  |  Line Argument  |
  +--------+--------+
           |
           v
  +-----------------------------------------------------+
  |  Make D-Bus Call                                    |
  |  (namespace_dbus_server_client_network_call_client) |
  +--------+--------------------------------------------+
           |
           v
  +-----------------+
  |  Initialize     |
  |  GLib Main Loop |
  +--------+--------+
           |
           v
  +-----------------+
  |  Run GLib Main  |
  |  Loop           |
  +--------+--------+
           |
   +-------+--------+
   |       |
   v       |
+--+-----------------------+
|  Handle D-Bus            |
|  Responses and           |
|  Events                  |
|  (callback_client_async) |
+--------------------------+
           |
           v
  +-----------------+
  |  Print Result   |
  +-----------------+
           |
           v
  +-----------------+
  |  Cleanup and    |
  |  Release        |
  +-----------------+
           |
           v
  +-----------------+
  |   Exit Program  |
  +-----------------+


1.Argument Check:
Checks if the program is provided with the correct number of arguments (num1).
Prints usage instructions if arguments are insufficient.

2.Proxy Initialization:
Creates a proxy object (proxy) for the namespaceDBUSServer.client_network interface.
Synchronously connects to the D-Bus session bus and obtains the proxy object.
Handles errors if the proxy creation fails.

3.Callback Function (callback_client_async):
Defined to handle the asynchronous completion of the D-Bus method call.
Prints a message indicating the callback has been invoked (callback_client_async called!).
Retrieves the return value (retval) from the method call using namespace_dbus_server_client_network_call_client_finish.
If successful, prints the returned value (Answer = <retval>) and exits the program.
If there's an error during the callback, prints an error message and frees the error object (error).

4.Main Function:
Parses the command-line argument (num1) as an integer (a).
Initiates the asynchronous D-Bus method call (namespace_dbus_server_client_network_call_client), passing a as an argument.
Registers callback_client_async as the callback function to handle the asynchronous response.
Creates a GLib main loop (loop) to handle events until the callback completes.
Runs the main loop (g_main_loop_run) to keep the program alive and responsive to D-Bus events.

5.Cleanup:
Once the callback completes, releases resources associated with the proxy (g_object_unref(proxy)).
Unreferences the main loop object (g_main_loop_unref(loop)).

6.Program Exit:
Returns 0 to indicate successful program execution.
