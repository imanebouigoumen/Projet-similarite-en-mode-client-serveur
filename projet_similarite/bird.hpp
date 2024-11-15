#ifndef BIRD_HPP
#define BIRD_HPP
using namespace std;
#include "distance.hpp"
struct BirdData
{
    int pos_id;
    int track_id;
    int pair_id;
    int bird_num;
    string path;
    string bird_id;
    double x;
    double y;
    double t;
    bool dp_10m;
};
map<int, vector<Point>> readBird();

#endif