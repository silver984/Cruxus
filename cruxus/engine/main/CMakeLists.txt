target_sources(cruxus
    PRIVATE
        AudioSys.cc
        Context.cc
        # CrashManager.cc <- to remove
        Director.cc
        Game.cc
        InputSys.cc
        ResourceSys.cc
        Window.cc
)

add_subdirectory(objects)