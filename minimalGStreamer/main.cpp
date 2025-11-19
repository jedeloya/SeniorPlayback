#include <gst/gst.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

// Change this to any public DASH/HLS test stream.
static const char* TEST_STREAM =
    "https://bitdash-a.akamaihd.net/content/sintel/sintel.mpd";  // DASH example
// "https://test-streams.mux.dev/x36xhzz/x36xhzz.m3u8";      // HLS example

// Bus callback — handles async events (errors, state changes, etc.)
static gboolean bus_callback(GstBus* bus, GstMessage* msg, gpointer data) {
    switch (GST_MESSAGE_TYPE(msg)) {

    case GST_MESSAGE_ERROR: {
        GError* err;
        gchar* debug;
        gst_message_parse_error(msg, &err, &debug);
        std::cerr << "[ERROR] " << err->message << std::endl;
        std::cerr << "Details: " << (debug ? debug : "none") << std::endl;
        g_free(debug);
        g_error_free(err);

        // Quit main loop on error
        GMainLoop* loop = static_cast<GMainLoop*>(data);
        g_main_loop_quit(loop);
        break;
    }

    case GST_MESSAGE_WARNING: {
        GError* err;
        gchar* debug;
        gst_message_parse_warning(msg, &err, &debug);
        std::cerr << "[WARN] " << err->message << std::endl;
        g_free(debug);
        g_error_free(err);
        break;
    }

    case GST_MESSAGE_STATE_CHANGED: {
        if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data)) break;

        GstState old_state, new_state, pending;
        gst_message_parse_state_changed(msg, &old_state, &new_state, &pending);

        std::cout << "[STATE] "
                  << gst_element_state_get_name(old_state)
                  << " -> "
                  << gst_element_state_get_name(new_state)
                  << std::endl;
        break;
    }

    default:
        break;
    }

    return TRUE;
}

int main(int argc, char* argv[])
{
    gst_init(&argc, &argv);

    // Create main loop
    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);

    // Create pipeline using playbin (handles HLS/DASH automatically if plugins installed)
    GstElement* pipeline = gst_element_factory_make("playbin", "player");
    if (!pipeline) {
        std::cerr << "Failed to create playbin. Check GStreamer installation." << std::endl;
        return -1;
    }

    g_object_set(pipeline, "uri", TEST_STREAM, nullptr);

    // Attach bus watches
    GstBus* bus = gst_element_get_bus(pipeline);
    gst_bus_add_watch(bus, bus_callback, loop);
    gst_object_unref(bus);

    std::cout << "Starting playback: " << TEST_STREAM << std::endl;

    // Set PLAYING state
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Enter main loop (blocks until quit)
    g_main_loop_run(loop);

    // Cleanup
    std::cout << "Stopping playback..." << std::endl;
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    return 0;
}
