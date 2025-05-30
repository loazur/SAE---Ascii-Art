#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <sstream>
#include <iomanip>

int main(int argc, char* argv[])
{
    #ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    #endif // _WIN32

    // *Création des variables
    std::string nomFichierInput, nomFichierSortie, nomFichierPalette;
    
    //* Regarde les arguments du programmes
    for (int i = 0; i < argc; ++i)
    {
        if (argc == 1)
        {
            break;
        }

        if (std::string(argv[1]) == "--help")
        {
            std::cout << "Usage :\n"
                << argv[0] << " [options] \n\n"
                << "Options : \n"
                << "--input fichier    Spécifie le fichier image à convertir \n"
                << "                   Si ce paramètre n'est pas spécifié, le fichier est demandé via la \n"
                << "                   console. \n\n"
                << "--output fichier   Spécifie le nom du fichier texte qui contiendra l'Ascii Art. \n"
                << "                   Si ce paramètre n'est pas spécifié, le fichier est demandé via la \n"
                << "                   console.\n\n"
                << "--palette fichier  Spécifie un fichier texte contenant la palette de couleur Ascii. \n"
                << "                   Chaque ligne du fichier contient un charactère en UTF-8, du plus \n"
                << "                   sombre au plus clair. \n"
                << "                   Si ce paramètre n'est pas spécifié, la palette par défaut est\n"
                << "                   \"W\", \"w\", \"l\", \"i\", \":\", \",\", \".\", \" \".\n\n"
                << "--help             Affiche cette aide \n\n";
            return 0;
        }

        if (std::string(argv[i]) == "--input")
        {
            nomFichierInput = std::string(argv[i + 1]);
        }

        if (std::string(argv[i]) == "--output")
        {
            nomFichierSortie = std::string(argv[i + 1]);
        }

        if (std::string(argv[i]) == "--palette")
        {
            nomFichierPalette = std::string(argv[i + 1]);
        }
    }

    // * Vérification des arguments 
    if (nomFichierInput == "")
    {
        std::cout << "Ecrivez le nom du fichier : ";
        std::getline(std::cin, nomFichierInput);
    }

    if(nomFichierSortie == "")
    {
        std::cout << "Ecrivez le nom du fichier de sortie : ";
        std::cin >> nomFichierSortie;
    }

    // *Ouverture du fichier de l'utilisateur en mode binaire :
    std::ifstream in(nomFichierInput, std::ios_base::binary);

    // Vérification de la validité des fichiers
    if (!in.is_open())
    {
        std::cerr << "Problème d'ouverture du fichier '" << nomFichierInput << "'. \n";
        return 1;
    }

    // On le fais aptrès le fichier input pour pas que ça crée un fichier pour rien
    std::ofstream out(nomFichierSortie);
    std::ifstream fichierPalette(nomFichierPalette);

    // Suite vérification
    if (!out.is_open())
    {
        std::cerr << "Erreur à la création du nouveau fichier.";
        return 1;
    }

    std::vector<std::string> palette;

    bool usedInArgs = true;

    // * Si 0 argument --palette ou mauvais argument
    if (!fichierPalette.is_open())
    {
        nomFichierPalette = "palette.txt";
        palette = {"W", "w", "l", "i", ":", ",", ".", " "};

        usedInArgs = false;
    }

    // *Charger la palette
    std::string lignePalette;

    // Si la palette par défaut n'a pas été prise
    if (usedInArgs)
    {
        while (!fichierPalette.eof())
        {
            std::getline(fichierPalette, lignePalette);
            palette.push_back(lignePalette);
        }
    }


    // *Décodage de l'entetes
    // On récupére les premieres lignes
    std::string premierL;
    std::getline(in, premierL);

    std::string deuxiemeL;
    std::getline(in, deuxiemeL);

    // On récupère les dimensions
    std::string ligneDimensions;

    std::getline(in, ligneDimensions);
    std::stringstream ssDimensions(ligneDimensions);

    // Largeur / Hauteur
    std::array<int, 2> dimensions;

    ssDimensions >> dimensions[0];
    ssDimensions >> dimensions[1];

    int totalDonnees = dimensions[0] * dimensions[1];

    // *Création d'une mémoire:
    std::vector<char> donnees(totalDonnees);

    //Lecture des octets depuis le fichier et stockage dans le tableau donnees :
    in.read(donnees.data(), totalDonnees);

    // *Stockage du tableau encodé avec les nouveaux contrastes
    std::vector<std::string> donneesDecoder(totalDonnees);

    int niveaux = 256 / palette.size();

    // *Boucle de convertion de données
    for (size_t i = 0; i < donnees.size(); ++i)
    {
        int valeurPixel = static_cast<unsigned char>(donnees[i]);
        int indexPalette = valeurPixel / niveaux;

        if (indexPalette >= palette.size())
        {
            indexPalette = palette.size() - 1; // Pour gérer les valeurs maximales
        }

        donneesDecoder[i] = palette[indexPalette]; // Premier caractère de la ligne de palette
    }

    // Écrire dans le fichier de sortie
    for (int i = 0; i < dimensions[1]; ++i)
    {
        for (int j = 0; j < dimensions[0]; ++j)
        {
            out << donneesDecoder[i * dimensions[0] + j];
            // i * largeur : Saute tous les éléments des lignes précédentes pour arriver à la ligne i
            // + j : Accède à la colonne j dans la ligne actuelle
        }
        out << "\n";
    }

    std::cout << "Le fichier \""<< nomFichierSortie << "\" a bien été crée.\n";
    return 0;
}