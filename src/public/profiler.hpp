#pragma once


#ifdef TRACY_ENABLE
    #include <tracy/Tracy.hpp>

    #define PROFILER_SCOPE_NAME(name) ZoneScopedN(name)

    #define PROFILER_SCOPE_TEXT( txt, size ) ZoneText( txt, size )
    #define PROFILER_SCOPE_TEXT_F( fmt, ... ) ZoneTextF( fmt, ... )

    #define PROFILER_FRAME_UPDATE() FrameMark
    #define PROFILER_FRAME_START(name) FrameMarkStart(name) 
    #define PROFILER_FRAME_END(name) FrameMarkEnd(name)

#else

    #define PROFILER_SCOPE_NAME(name)

    #define PROFILER_SCOPE_TEXT( txt, size ) 
    #define PROFILER_SCOPE_TEXT_F( fmt, ... )


    #define PROFILER_FRAME_UPDATE()
    #define PROFILER_FRAME_START(name)
    #define PROFILER_FRAME_END(name)


#endif