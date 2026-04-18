target_sources(cruxus
    PRIVATE
        AudioSys.cc
        context.cc
        # CrashManager.cc <- to remove
        Director.cc
        Game.cc
        InputSys.cc
        ResourceSys.cc
        Window.cc
)

add_subdirectory(objects)