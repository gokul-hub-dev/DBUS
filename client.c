#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "mydbus.h"

static void
callback_client_async(GObject *proxy,
                      GAsyncResult *res,
                      gpointer user_data)
{
    g_print("callback_client_async called!\n");
    gint retval;
    GError *error = NULL;

    if (namespace_dbus_server_client_network_call_client_finish(NAMESPACE_DBUS_SERVER_CLIENT_NETWORK(proxy), &retval, res, &error))
    {
        g_print("Answer = %d\n", retval);
        exit(0);
    }
    else
    {
        g_printerr("Error in callback: %s\n", error->message);
        g_error_free(error);
    }

    // Quit the main loop when callback is finished
    if (user_data != NULL)
    {
        g_main_loop_quit((GMainLoop *)user_data);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: ./client num1\n");
        return 0;
    }

    namespaceDBUSServer_client_network *proxy;
    GError *error = NULL;
    gint retval;

    proxy = namespace_dbus_server_client_network_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, 0, "com.DBUS_gen", "/", NULL, &error);

    if (error != NULL)
    {
        g_printerr("Error creating proxy: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    gint a = atoi(argv[1]);

    error = NULL;
    namespace_dbus_server_client_network_call_client(proxy, a, NULL, callback_client_async, NULL);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    
    // Run the main loop until the callback finishes
    g_main_loop_run(loop);

    g_object_unref(proxy); // Release resources before exiting

    g_main_loop_unref(loop); // Unreference the main loop object

    return 0;
}
