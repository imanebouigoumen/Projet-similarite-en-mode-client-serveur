#include "distance.hpp"
#include <cmath>
#include <limits>
#include <algorithm>
#include <map>
#include <vector>
#include <iostream>
#include "bird.hpp"
#include <cfloat>

// Prototype de la fonction de distance à définir par l'utilisateur
double distance(Point point1, Point point2)
{

    return std::sqrt(std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2));
}

// Fonction pour calculer la matrice de distance optimisée

std::vector<std::vector<double>> calculerMatriceDistanceOptimisee(std::vector<Point> trajectoire1, std::vector<Point> trajectoire2)
{

    int NbreLigne = trajectoire1.size();
    int NbreColonne = trajectoire2.size();

    std::vector<std::vector<double>> MD(NbreLigne, std::vector<double>(NbreColonne, 0));
    double DiagMax = 0;

    // Calcul de la diagonale et de DiagMax
    int i = 0, j = 0;
    while (i < NbreLigne - 1 && j < NbreColonne - 1)
    {
        MD[i][j] = distance(trajectoire1[i], trajectoire2[j]);
        if (MD[i][j] > DiagMax)
            DiagMax = MD[i][j];
        if (i < NbreLigne - 1)
            i++;
        if (j < NbreColonne - 1)
            j++;
    }

    // Calcul de la partie haut-droit de MD
    int jprecedant = 0;
    for (i = 0; i < NbreLigne; i++)
    {
        j = i;
        while ((j < NbreColonne && DiagMax > distance(trajectoire1[i], trajectoire2[j])) ||
               (j < NbreColonne && j < jprecedant))
        {
            MD[i][j] = distance(trajectoire1[i], trajectoire2[j]);
            j++;
        }
        jprecedant = j;
    }

    // Calcul de la partie bas-gauche de MD
    int iprecedant = 0;
    for (j = 0; j < NbreLigne; j++)
    {
        i = j;
        while ((i < NbreLigne && DiagMax > distance(trajectoire1[i], trajectoire2[j])) ||
               (i < NbreLigne && i < iprecedant))
        {
            MD[i][j] = distance(trajectoire1[i], trajectoire2[j]);
            i++;
        }
        iprecedant = i;
    }
    std::cout << "Matrice de distance optimisée : " << std::endl;
    for (int i = 0; i < NbreLigne; ++i)
    {
        for (int j = 0; j < NbreColonne; ++j)
        {
            std::cout << MD[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return MD;
}

// La fonction qui calcule la matrice de Fréchet
std::vector<std::vector<double>> calculerMatriceFrechet(const std::vector<std::vector<double>> &MD)
{
    int NbreLigne = MD.size();
    int NbreColonne = MD[0].size();
    std::vector<std::vector<double>> MF(NbreLigne, std::vector<double>(NbreColonne, -1));

    for (int i = 0; i < NbreLigne; ++i)
    {
        for (int j = 0; j < NbreColonne; ++j)
        {
            if (MD[i][j] != -1)
            {
                double val1 = (i > 0 && MD[i - 1][j] != -1) ? MF[i - 1][j] : DBL_MAX;
                double val2 = (j > 0 && MD[i][j - 1] != -1) ? MF[i][j - 1] : DBL_MAX;
                double val3 = (i > 0 && j > 0 && MD[i - 1][j - 1] != -1) ? MF[i - 1][j - 1] : DBL_MAX;
                double minVal = std::min({val1, val2, val3});
                MF[i][j] = minVal == DBL_MAX ? MD[i][j] : std::max(MD[i][j], minVal);
            }
        }
    }
    std::cout << "Matrice de Fréchet : " << std::endl;
    for (int i = 0; i < NbreLigne; ++i)
    {
        for (int j = 0; j < NbreColonne; ++j)
        {
            std::cout << MF[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return MF;
}

double calculerDistanceFrechet(const std::vector<Point> &trajectoire1, const std::vector<Point> &trajectoire2)
{
    std::vector<std::vector<double>> MD = calculerMatriceDistanceOptimisee(trajectoire1, trajectoire2);
    std::vector<std::vector<double>> MF = calculerMatriceFrechet(MD);
    cout << "Distance de frechet : " << MF.back().back() << endl;
    return MF.back().back();
}

vector<vector<Point>> trajectoiresSimilaires(const map<int, vector<Point>> &referenceData, const vector<Point> &clientTrajectory, float threshold)
{
    vector<vector<Point>> similarTrajectories;

    int d = 0;
    double mf;

    for (auto it = referenceData.begin(); it != referenceData.end(); ++it)
    {
        const vector<Point> &points = it->second;

        mf = calculerDistanceFrechet(clientTrajectory, points);
        if (mf <= threshold)
        {
            d++;
        }
    }
    cout << "le nombre de trajectoires similaires par rapport à t : " << d << endl;
    return similarTrajectories;
}

vector<pair<int, int>> trajectoiresSimilairesDonnees(const map<int, vector<Point>> &referenceData, float threshold)
{
    double mf = 100;
    vector<pair<int, int>> similarTrajectories; // Paires d'identifiants de trajectoires similaires
    if (referenceData.empty())
    {
        return similarTrajectories;
    }

    // Parcourir toutes les paires de trajectoires dans le jeu de données
    auto last = prev(referenceData.end());

    for (auto it1 = referenceData.begin(); it1 != last; ++it1)
    {
        int track_id1 = it1->first;
        vector<Point> points1 = it1->second;

        for (auto it2 = next(it1); it2 != referenceData.end(); ++it2)
        {
            int track_id2 = it2->first;
            vector<Point> points2 = it2->second;

            // Calculer la distance de Fréchet entre les deux trajectoires
            mf = calculerDistanceFrechet(points1, points2);
            if (mf <= threshold)
            {
                similarTrajectories.push_back({track_id1, track_id2});
                cout << "Trajectoire " << track_id1 << " et trajectoire " << track_id2 << " sont similaires." << endl;
            }
        }
    }
    if (similarTrajectories.empty())
    {
        cout << "pas de trajectoires similaires par rapport au seuil fourni" << endl;
    }
    return similarTrajectories;
}
vector<Point> trajectoireProche(const map<int, vector<Point>> &referenceData, const vector<Point> &clientTrajectory)
{
    double mf;
    int trackproche;
    vector<Point> similaire;

    double min = calculerDistanceFrechet(referenceData.begin()->second, clientTrajectory);

    auto it = referenceData.begin();
    ++it;
    for (; it != referenceData.end(); ++it)
    {
        int track_id = it->first;
        const vector<Point> &points = it->second;

        mf = calculerDistanceFrechet(clientTrajectory, points);
        if (mf <= min)
        {
            min = mf;
            similaire = points;
            trackproche = track_id;
        }
    }
    cout << "Trajectoire proche : ";
    if (similaire.empty())
    {
        similaire = referenceData.begin()->second;
        trackproche = referenceData.begin()->first;
    }

    cout << "[";
    for (const auto &point : similaire)
    {
        cout << "(" << point.x << ", " << point.y << ") ";
    }
    cout << "]" << endl;
    cout << "track id de le trajectoire la plus proche :  " << trackproche << endl;

    return similaire;
}
