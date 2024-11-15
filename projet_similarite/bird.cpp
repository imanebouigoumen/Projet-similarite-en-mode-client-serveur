#include <fstream>
#include <sstream>
#include <cstddef>
#include <cbor.h>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "distance.hpp"
#include "bird.hpp"

using namespace std;

// Structure to hold data for each row

map<int, vector<Point>> readBird()
{

    string line;
    ifstream ip("pigeonspathA.csv");
    getline(ip, line);
    map<int, vector<Point>> trackMap;
    vector<Point> traj;

    while (getline(ip, line))
    {
        stringstream inputString(line);
        BirdData bird;
        string st;
        // Extrait chaque champ de la ligne séparée par des virgules
        getline(inputString, st, ';');
        bird.pos_id = std::stoi(st);
        getline(inputString, st, ';');
        bird.track_id = std::stoi(st);
        getline(inputString, st, ';');
        getline(inputString, st, ';');
        getline(inputString, st, ';');
        getline(inputString, st, ';');
        getline(inputString, st, ';');
        bird.x = std::stod(st);
        getline(inputString, st, ';');
        bird.y = std::stod(st);

        // cout << "track: " << bird.track_id << ", x: " << std::fixed << std::setprecision(4) << bird.x << ", y: " << bird.y << endl;
        if (trackMap.find(bird.track_id) != trackMap.end())
        {
            // Le track_id existe déjà, ajouter le point au vecteur correspondant
            trackMap[bird.track_id].push_back({bird.x, bird.y});
        }
        else
        {
            // Le track_id n'existe pas encore, créer une nouvelle entrée
            trackMap[bird.track_id] = {{bird.x, bird.y}};
        }
    }
    /*Affiche les valeurs extraites
    cout << "track id: " << bird.track_id << endl;
    cout << "x: " << bird.x << endl;
    cout << "y: " << bird.y << endl;*/

    /*for (const auto &entry : trackMap)
    {
        cout << "track_id: " << entry.first << endl;
        for (const auto &point : entry.second)
        {
            cout << "x: " << point.x << ", y: " << point.y << endl;
        }
    }*/
    return trackMap;
}
