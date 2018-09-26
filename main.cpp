#include <gst/gst.h>

#include "GlibPtr.h"
#include "GstPtr.h"

extern "C" {
GST_PLUGIN_STATIC_DECLARE(interpipe);
}

int main(int argc, char *argv[])
{
    gst_init(0, nullptr);

    GST_PLUGIN_STATIC_REGISTER(interpipe);

    GMainLoopPtr loopPtr(g_main_loop_new(nullptr, FALSE));

    GstElementPtr srcPipelinePtr(
        gst_parse_launch(
            "videotestsrc ! interpipesink name=src sync=true",
            nullptr));
    GstElementPtr dstPipelinePtr(
        gst_parse_launch(
            "interpipesrc listen-to=src is-live=true ! autovideosink",
            nullptr));

    if(!srcPipelinePtr || !dstPipelinePtr)
        return -1;

    gst_element_set_state(srcPipelinePtr.get(), GST_STATE_PLAYING);
    gst_element_set_state(dstPipelinePtr.get(), GST_STATE_PLAYING);

    g_main_loop_run(loopPtr.get());

    return 0;
}
