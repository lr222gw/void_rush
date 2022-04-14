#pragma once

#include "Generate.hpp"
#include <fstream>
#include <iostream>

void generation_proto ()
{
    
    int seed = time(0);
    std::ifstream seed_file("void_rush/proto_outputs/seed.txt");
    std::string tempStr;
    //getline(seed_file, tempStr);
    seed_file >> tempStr;
    seed = std::atoi(tempStr.c_str());
    seed_file.close();
    std::ofstream output_stream ("void_rush/proto_outputs/debugoutput.txt");
    

    
    output_stream << "Platform generation test\n Input seed: " << seed << std::endl;
    
    player *pl = new player ();
    generation levelGen (seed, 20);
    levelGen.assignPlayer (pl);
    levelGen.start (1);
    std::vector<platform *> platforms = levelGen.getPlatforms ();

    std::ofstream out ("void_rush/proto_outputs/graph.dot");
    out << "digraph {\n";
    int scale = 20;

    int abs_X = 0;
    int abs_Y = 0;
    player p;
    for (int i = 0; i < platforms.size (); i++)
    {
        auto d = Vector3 (1.f, 2.f, 3.f);
        p.distance (d);
        output_stream << i << " platform.\nXPos: " << platforms[i]->getPos ().x
                  << " YPos: " << platforms[i]->getPos ().y
                  << " ZPos: " << platforms[i]->getPos ().z << "\n"
                  << "Rotation: " << platforms[i]->getRotation () << "\n";
        if (i != platforms.size () - 1)
        {
            output_stream << i << " Distance to next "
                      << platforms[i]->distance (platforms[i]->next->getPos ())
                      << std::endl;
        }
        // abs_X += platforms[i]->getPos().at(0) ;
        // abs_Y += platforms[i]->getPos().at(1) ;

        abs_X = platforms[i]->getPos ().y;
        abs_Y = platforms[i]->getPos ().x;

        out << "Platform_" << i << " [\n";
        out << "label = \"P_" << i << "\n";
        out << "X: " << platforms[i]->getPos ().x << "\n";
        out << "Y: " << platforms[i]->getPos ().y << "\n";
        out << "Z: " << platforms[i]->getPos ().z << "\"\n";

        // out << "pos = \""<< platforms[i]->getPos().at(0) * scale <<","
        // << platforms[i]->getPos().at(1) * scale <<"!\"\n"; out << "pos =
        // \""<< abs_X
        // + scale <<"," << abs_Y + scale <<"!\"\n";

        out << "pos = \"" << abs_X * scale << "," << abs_Y * scale << "!\"\n";
        out << "]\n";
        if (i != 0)
        {
            out << "Platform_" << i - 1 << " -> "
                << "Platform_" << i;
            out << "[ label = \"Dist: "
                << platforms[i - 1]->distance (
                       platforms[i - 1]->next->getPos ())
                << "\" ]"
                << "\n";
        }
    }
    out << "}\n";

    out.close ();
    output_stream.close();

    delete pl;
}