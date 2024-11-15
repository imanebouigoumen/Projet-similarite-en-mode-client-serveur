#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include "bird.hpp"
#include <vector>
#include <cstddef>
#include <cbor.h>
#include <cstring>
#include <utility>
#include "distance.hpp"

using namespace std;

void traitement(cbor_item_t *MessageRecu)
{

    vector<Point> L1;
    cbor_pair *cbor_pairs = cbor_map_handle(MessageRecu);
    char *key;
    float val, val2, frechet;
    int k = 0;
    size_t taille = cbor_map_size(MessageRecu);
    int choix = cbor_get_uint16(cbor_pairs[0].value);
    cout << "Votre choix  " << choix << endl;

    if (choix == 1 || choix == 3)
    {
        for (size_t i = 1; i < cbor_map_size(MessageRecu) - 1; ++i)
        { // parcourt toutes les paires
            key = (char *)malloc(sizeof(char) * (cbor_string_length(cbor_pairs[i].key) + 1));
            memcpy(key, cbor_string_handle(cbor_pairs[i].key), cbor_string_length(cbor_pairs[i].key) + 1);
            if (key)
            {

                val = cbor_float_get_float(cbor_pairs[i].value);
                val2 = cbor_float_get_float(cbor_pairs[i + 1].value);
                L1.push_back({});
                L1[k].x = val;
                L1[k].y = val2;
                /*cout << L1[k].x << endl
                     << L1[k].y;*/
                ++k;
                ++i;
            }

            free(key);
        }
    }

    map<int, vector<Point>> map = readBird();
    if (choix == 1 || choix == 2)
    {
        frechet = cbor_float_get_float(cbor_pairs[taille - 1].value);
        cout << "frechet " << frechet << endl;
    }
    switch (choix)
    {
    case 1:
        trajectoiresSimilaires(map, L1, frechet);

        break;
    case 2:
        trajectoiresSimilairesDonnees(map, frechet);
        break;
    case 3:
        trajectoireProche(map, L1);
        break;

    default:
        break;
    }

    cout << "----------------" << endl;
}

int main(void)
{
    // Création du socket TCP
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        cout << "Erreur lors de la création du socket" << endl;
        return 1;
    }

    // Liaison du socket à l'adresse et au port
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080); // Port arbitraire
    if (::bind(server_socket, (sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        cerr << "Erreur lors de la liaison du socket" << endl;
        close(server_socket);
        return 1;
    }

    // Écoute de nouvelles connexions entrantes
    if (listen(server_socket, SOMAXCONN) != 0)
    {
        cerr << "Erreur lors de l'écoute du socket" << endl;
        close(server_socket);
        return 1;
    }

    cout << "Serveur en attente de connexions..." << endl;

    while (true)
    {
        // Accepter une nouvelle connexion
        sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (sockaddr *)&client_address, &client_address_size);
        if (client_socket == -1)
        {
            cerr << "Erreur lors de l'acceptation de la connexion" << endl;
            continue;
        }

        cout << "Nouvelle connexion acceptée" << endl;

        // Recevoir les données du client

        char buffer[1024];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1)
        {
            cerr << "Erreur lors de la réception des données du client" << endl;
        }
        else if (bytes_received == 0)
        {
            cout << "Client déconnecté" << endl;
        }
        else
        {

            cbor_item_t *MessageRecu;
            struct cbor_load_result result;
            MessageRecu = cbor_load((const unsigned char *)buffer, bytes_received, &result);
            cout << "la taille : " << MessageRecu->refcount << endl;

            cbor_describe(MessageRecu, stdout);

            fflush(stdout);
            traitement(MessageRecu);
        }
        close(client_socket);
    }

    // Fermer le socket du serveur
    close(server_socket);

    return 0;
}