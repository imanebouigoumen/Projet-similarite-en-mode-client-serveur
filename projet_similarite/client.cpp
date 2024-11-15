#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstddef>
#include <cbor.h>
#include <cstring>

using namespace std;

int main()
{
    // Création du socket TCP
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        cerr << "Erreur lors de la création du socket" << endl;
        return 1;
    }

    // Définition de l'adresse IP et du port du serveur
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);                     // Port du serveur
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr); // Adresse IP du serveur

    // Connexion au serveur
    if (connect(client_socket, (sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        cerr << "Erreur lors de la connexion au serveur" << endl;
        close(client_socket);
        return 1;
    }

    cout << "Connecté au serveur" << endl;

    // Envoyer des données au serveur
    size_t i = 0;
    float lmbd;
    float coord_x, coord_y;
    int choix;
    size_t nbr;

    cbor_item_t *monMessage = cbor_new_indefinite_map();
    std::cout << "Bienvenue sur le test de similarité " << std::endl;
    std::cout << " que voulez vous ?" << std::endl;
    std::cout << "1 : nombre de trajectoires similaires," << std::endl;
    std::cout << "2 : les trajectoires similaires," << std::endl;
    std::cout << "3 : trajectoire la plus proche" << std::endl;
    cin >> choix;
    struct cbor_pair cchoix;
    cchoix.key = cbor_move(cbor_build_string("choix"));
    cchoix.value = cbor_move(cbor_build_uint16(choix));
    cbor_map_add(monMessage, cchoix);

    if (choix == 1 || choix == 3)
    {
        cout << "donner le nombre de points de la trajectoire" << endl;
        cin >> nbr;
        while (i < nbr)
        {
            std::cout << "Entrez la coordonnée X : ";
            std::cin >> coord_x;
            std::cout << "Entrez la coordonnée Y : ";
            std::cin >> coord_y;

            struct cbor_pair x, y;
            string keyx = "X" + std::to_string(i);
            string keyy = "Y" + std::to_string(i);
            const char *key_x = keyx.c_str();
            const char *key_y = keyy.c_str();

            x.key = cbor_move(cbor_build_string(key_x));
            x.value = cbor_move(cbor_build_float8(coord_x));
            cbor_map_add(monMessage, x);
            y.key = cbor_move(cbor_build_string(key_y));
            y.value = cbor_move(cbor_build_float8(coord_y));
            cbor_map_add(monMessage, y);
            i++;
        }
    }
    if (choix == 1 || choix == 2)
    {
        struct cbor_pair z;
        std::cout << "Entrez distance frechet ";
        std::cin >> lmbd;
        z.key = cbor_move(cbor_build_string("frechet"));
        z.value = cbor_move(cbor_build_float8(lmbd));
        cbor_map_add(monMessage, z);
    }

    unsigned char *buffer;
    size_t buffer_size;
    buffer_size = cbor_serialize_alloc(monMessage, &buffer, &buffer_size);

    // Envoyer la séquence de bytes au serveur
    if (send(client_socket, buffer, buffer_size, 0) == -1)
    {
        std::cerr << "Erreur lors de l'envoi de données au serveur" << std::endl;
        close(client_socket);
        return 1;
    }
    std::cout << "Coordonnées envoyées au serveur" << std::endl;

    // Fermer la connexion avec le serveur
    close(client_socket);

    return 0;
}
