// TPXParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <format>
#include <cmath>
#include "rapidxml.hpp"

struct trackpoint {
    char* time;
    float latitude;
    float longitude;
    float altitudeMeters;
    float distanceMeters;
    float speed;
    std::pair<float, float> latlongdist;
};


// TODO Make these string targets string views.  
// 
// returns node ptr if found while transversing first child of graph.
// returns nullptr if not found
rapidxml::xml_node<>* findNode(rapidxml::xml_node<>*& node, std::string_view target) {
    rapidxml::xml_node<>* track = node;
    while (track->name() != target) {
        track = track->first_node();
        if (track == nullptr) return track;
    }
    return track;
}

// returns node ptr if found while transversing the adjacent nodes of the current node
// returns nullptr if not found
rapidxml::xml_node<>* findAdjacentNode(rapidxml::xml_node<>*& node, std::string_view target) {
    rapidxml::xml_node<>* track = node;
    while (track->name() != target) {
        track = track->next_sibling();
        if (track == nullptr) return track;
    }
    return track;
}

// Asumes "Trackpoint" node as param
trackpoint constructTrackpoint(rapidxml::xml_node<>*& trackpoint_node) {
    rapidxml::xml_node<>* node = trackpoint_node->first_node();
    char* timec_str = node->value();
    std::string_view latStr(node->next_sibling()->first_node()->value());
    std::string_view lonStr(node->next_sibling()->first_node()->next_sibling()->value());
    std::string_view altiStr(findAdjacentNode(node, "AltitudeMeters")->value());
    std::string_view distStr(findAdjacentNode(node, "DistanceMeters")->value());
    std::string_view spdStr(findAdjacentNode(node, "Extensions")->first_node()->first_node()->value());

    float lat;
    float lon;
    float alti;
    float dist;
    float spd;

    std::from_chars(latStr.data(), latStr.data() + latStr.size(), lat);
    std::from_chars(lonStr.data(), lonStr.data() + lonStr.size(), lon);
    std::from_chars(altiStr.data(), altiStr.data() + altiStr.size(), alti);
    std::from_chars(distStr.data(), distStr.data() + distStr.size(), dist);
    std::from_chars(spdStr.data(), spdStr.data() + spdStr.size(), spd);

    return trackpoint{ 
        timec_str, 
        lat, 
        lon,
        alti, 
        dist, 
        spd
    };
}

void constructVectorFromLap(rapidxml::xml_node<>*& lap_node, std::vector<trackpoint>& tps) {
    rapidxml::xml_node<>* node = lap_node->first_node(); //TotalTimeSeconds;
    /*target = "Track";*/
    node = findAdjacentNode(node, "Track");

    node = node->first_node(); //TrackPoint

    while (node != nullptr) {
        tps.push_back(constructTrackpoint(node));
        node = node->next_sibling();
    }
}
// https://stackoverflow.com/a/16271669

// center_lat is cos(center_latitude)
float longitudeToLocalX(float cos_center_lat, float lon) {
    return lon * cos_center_lat;
}

int main()
{
    char text[] = "hello";

    std::ifstream myfile("test.tcx");

    rapidxml::xml_document<> doc;

    /* "Read file into vector<char>"  See linked thread above  https://stackoverflow.com/questions/2808022/how-to-parse-an-xml-file-with-rapidxml*/
    std::vector<char> buffer((std::istreambuf_iterator<char>(myfile)), std::istreambuf_iterator<char>());

    buffer.push_back('\0');


    //std::cout << &buffer[0] << std::endl; /* see if file is read*/

    doc.parse<0>(&buffer[0]);

    rapidxml::xml_node<>* track = doc.first_node();


    std::cout << "Name of my first node is: " << doc.first_node()->first_attribute()->next_attribute()->name() << "\n";
    std::cout << " " << track->name() << "\n";

    std::string target = "Id";
    track = findNode(track, target);
    
    target = "Lap";
    track = findAdjacentNode(track, target);

    std::vector<trackpoint> tps;
    while (track != nullptr) {
        constructVectorFromLap(track, tps);
        track = track->next_sibling();
        track = findAdjacentNode(track, target);
    }

    // min/max latitude used to find center for the rectangular approximation of coords instead of the first coordinate
    float minLat = 181.0f;
    float maxLat = -181.0f;
    for (int i = 0; i < tps.size(); i++) {
        minLat = std::min(minLat, tps[i].latitude);
        maxLat = std::max(maxLat, tps[i].latitude);
    }

    float prevLat = tps[0].latitude;
    //float cosCenterLat = std::cos(prevLat);
    float centerLat = (maxLat - minLat) * 0.5f + minLat;
    float cosCenterLat = std::cos(centerLat);
    std::cout << "\nGREP centerLat " << centerLat << " firstLat " << prevLat << "\n";
    //cosCenterLat = 1;

    float prevLon = tps[0].longitude * cosCenterLat;

    
    for (int i = 0; i < tps.size(); i++) {
        float tempLon = tps[i].longitude * cosCenterLat;
        tps[i].latlongdist = { tps[i].latitude - prevLat, tempLon - prevLon };
        prevLat = tps[i].latitude;
        prevLon = tempLon;
        std::cout << "[" << std::format("{:.2f}", tps[i].latlongdist.first * 10000) << ", " << std::format("{:.2f}", tps[i].latlongdist.second * 10000) << "], ";
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
