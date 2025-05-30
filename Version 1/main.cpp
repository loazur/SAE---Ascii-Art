#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <sstream>

int main()
{
    #ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    #endif // _WIN32

    std::string nomFichier;

    std::cout << "Ecrivez le nom du fichier : ";
    std::getline(std::cin, nomFichier);

    //Ouverture du fichier de l'utilisateur en mode binaire :
    std::ifstream fichier(nomFichier, std::ios_base::binary);

    if (!fichier.is_open())
    {
        std::cerr << "Problème d'ouverture du fichier '" << nomFichier << "'. \n";
        return 0;
    }

    //Décodage de l'entetes
    // On récupére les premieres lignes
    std::string premierL;
    std::getline(fichier, premierL);

    std::string deuxiemeL;
    std::getline(fichier, deuxiemeL);

    // On récupère les dimensions
    std::string ligneDimensions;

    std::getline(fichier, ligneDimensions);
    std::stringstream ssDimensions(ligneDimensions);

    // Largeur / Hauteur
    std::array<std::string, 2> dimensions;

    std::getline(ssDimensions, dimensions[0], ' ');
    std::getline(ssDimensions, dimensions[1], ' ');

    std::cout << "Dimensions : " << dimensions[0] << "L / " << dimensions[1] << "H \n";

    double totalDonnees = std::stod(dimensions[0]) * std::stod(dimensions[1]);

    //Création d'une mémoire:
    std::vector<char> donnees(totalDonnees);

    //Lecture des octets depuis le fichier et stockage dans le tableau donnees :
    fichier.read(donnees.data(), totalDonnees);

    //Stockage du tableau encodé avec les nouveaux contrastes
    std::vector<char> donneesDecoder(totalDonnees);

    for (int i = 0; i < totalDonnees; ++i)
    {
        if (donnees[i] < 0)
        {
            donnees[i] *= -1;
        }

        // 0 - 32 - 'W'
        if (donnees[i] < 32 && donnees[i] >= 0)
        {
            donneesDecoder[i] = 'W';
        }

        // 32 - 64 - 'w'
        if (donnees[i] < 64 && donnees[i] >= 32)
        {
            donneesDecoder[i] = 'w';
        }

        // 64 - 96  - 'l'
        if (donnees[i] < 96 && donnees[i] >= 64)
        {
            donneesDecoder[i] = 'l';
        }

        // 96 - 128 - 'i'
        if (donnees[i] < 128 && donnees[i] >= 96)
        {
            donneesDecoder[i] = 'i';
        }

        // 128 - 160 - ':'
        if (donnees[i] < 160 && donnees[i] >= 128)
        {
            donneesDecoder[i] = ':';
        }

        // 160 - 191 - ','
        if (donnees[i] < 191 && donnees[i] >= 160)
        {
            donneesDecoder[i] = ',';
        }

        // 191 - 223 - '.'
        if (donnees[i] < 223 && donnees[i] >= 191)
        {
            donneesDecoder[i] = '.';
        }

        // 223 - 255 - ' '
        if (donnees[i] <= 255 && donnees[i] >= 223)
        {
            donneesDecoder[i] = ' ';
        }

    }

    // Affichage du ASCII Art
    int larg = 0;
    for (char lettre : donneesDecoder)
    {
        std::cout << lettre;

        ++larg;

        if (larg == std::stod(dimensions[0]))
        {
            std::cout << "\n";

            larg = 0;
        }

    }
}