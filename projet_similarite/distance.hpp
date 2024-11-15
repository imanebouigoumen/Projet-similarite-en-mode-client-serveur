#ifndef DISTANCE_HPP
#define DISTANCE_HPP

#include <cmath>
#include <limits>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;

struct Point
{
    double x;
    double y;
};

/*double euclideanDistance(const Point &p1, const Point &p2);
std::vector<std::vector<double>> computeFrechetMatrix(const std::vector<Point> &t1, const std::vector<Point> &t2);
std::vector<double> computeFrechetDistances(const std::map<int, std::vector<Point>> &referenceData, const std::vector<Point> &clientTrajectory);
vector<vector<Point>> findSimilarTrajectories(const map<int, vector<Point>> &referenceData, const vector<Point> &clientTrajectory, float threshold);
int countSimilarTrajectories(const map<int, vector<Point>> &referenceData, const vector<Point> &clientTrajectory, double threshold);
*/
std::vector<std::vector<double>> calculerMatriceDistanceOptimisee(std::vector<Point> trajectoire1, std::vector<Point> trajectoire2);

std::vector<std::vector<double>> calculerMatriceFrechet(const std::vector<std::vector<double>> &MD);

double calculerDistanceFrechet(const std::vector<Point> &trajectoire1, const std::vector<Point> &trajectoire2);
vector<vector<Point>> trajectoiresSimilaires(const map<int, vector<Point>> &referenceData, const vector<Point> &clientTrajectory, float threshold);
vector<pair<int, int>> trajectoiresSimilairesDonnees(const map<int, vector<Point>> &referenceData, float threshold);
vector<Point> trajectoireProche(const map<int, vector<Point>> &referenceData, const vector<Point> &clientTrajectory);
#endif