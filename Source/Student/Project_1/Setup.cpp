#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"

void ProjectOne::setup()
{
    // Create your inital agents
    
    
    for (int i = 0; i < 15; i++)
    {
        BehaviorAgent* civilian = agents->create_behavior_agent("Civilian", BehaviorTreeTypes::Civilian);
        civilian->set_position(RNG::world_position());
        civilian->set_scaling(2.f);
    }
    

    for (int i = 0; i < 1; i++)
    {
        BehaviorAgent* police = agents->create_behavior_agent("Police", BehaviorTreeTypes::Police);
        police->set_color(Vec3(0.094, 0.592, 0.749));
        police->set_position(RNG::world_position());
    }

    BehaviorAgent* detective = agents->create_behavior_agent("Detective", BehaviorTreeTypes::Detective);
    detective->set_color(Vec3(0.250, 0.776, 0.141));
    detective->set_position(RNG::world_position());
    
    for (int i = 0; i < RNG::range(5,7); i++)
    {
        BehaviorAgent* criminal = agents->create_behavior_agent("Criminal", BehaviorTreeTypes::Criminal);
        criminal->set_color(Vec3(0.658f, 0.043f, 0.058f));
        criminal->set_position(RNG::world_position());
    }
    
    // you can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code that yourself
    // that's the realm of project 2 though
    terrain->goto_map(0);

    // you can also enable the pathing layer and set grid square colors as you see fit
    // works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    // camera position can be modified from this default as well
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees
}