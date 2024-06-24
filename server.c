#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "mydbus.h"

static gboolean
on_handle_network(namespaceDBUSServer_client_network *interface,
                  GDBusMethodInvocation *invocation,
                  const gint num1,
                  gpointer user_data)
{
    gint ans = num1 + 1;
    if (num1 > 1)
    {
        g_print("Received from client: %d.\n", num1);
        g_print("Processing to send.......\n");
        namespace_dbus_server_client_network_complete_client(interface, invocation, ans);
        g_print("Message sent!\n");
    }
    return TRUE; // Always return TRUE to indicate handling
}

static void
on_name_acquired(GDBusConnection *connection,
                 const gchar *name,
                 gpointer user_data)
{
    namespaceDBUSServer_client_network *interface;
    GError *error = NULL;

    interface = namespace_dbus_server_client_network_skeleton_new();
    g_signal_connect(interface, "handle-client", G_CALLBACK(on_handle_network), NULL);

    if (!g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(interface), connection, "/", &error))
    {
        g_printerr("Failed to export interface: %s\n", error->message);
        g_error_free(error);
        exit(1);
    }

    g_print("Name acquired and interface exported.\n");
}

int main()
{
    GMainLoop *loop;

    loop = g_main_loop_new(NULL, FALSE);
    g_bus_own_name(G_BUS_TYPE_SESSION, "com.DBUS_gen", G_BUS_NAME_OWNER_FLAGS_NONE, NULL, on_name_acquired, NULL, NULL, NULL);
    g_print("Server running...\n");
    g_main_loop_run(loop);

    return 0;
}
