/**
 * \file ReseauInterurbain.cpp
 * \brief Implémentattion de la classe ReseauInterurbain.
 * \author IFT-2008 & ?
 * \version 0.1
 * \date mars 2021
 *
 *  Travail pratique numéro 2
 *
 */
#include <sstream>
#include <fstream>
#include "ReseauInterurbain.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{

// Méthode fournie
void ReseauInterurbain::chargerReseau(std::ifstream & fichierEntree)
{
    if (!fichierEntree.is_open())
        throw std::logic_error("ReseauInterurbain::chargerReseau: Le fichier n'est pas ouvert !");

    std::string buff;

	getline(fichierEntree, nomReseau);
	nomReseau.erase(0, 20); // Enlève: Reseau Interurbain:

	int nbVilles;

	fichierEntree >> nbVilles;
	getline(fichierEntree, buff); //villes

	unReseau.resize(nbVilles);

	getline(fichierEntree, buff); //Liste des villes

	size_t i = 0;

    getline(fichierEntree, buff); //Premiere ville

    while(buff != "Liste des trajets:")
    {
        unReseau.nommer(i, buff);
        getline(fichierEntree, buff);
        i++;
    }

    while(!fichierEntree.eof())
    {
        getline(fichierEntree, buff);
		std::string source = buff;
		getline(fichierEntree, buff);
		std::string destination = buff;

		getline(fichierEntree, buff);
		std::istringstream iss(buff);

		float duree;
		iss >> duree;

		float cout;
		iss >> cout;

		unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout);
	}
}
/**
 * \brief Constructeur de la classe ReseauInterurbain
 * @param nomReseau un string, le nom de notre reseau
 * @param nbVilles un size_t, le nombre de ville dans le reseau
 */
    ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles): nomReseau(nomReseau),unReseau(nbVilles)
    {

    }
/**
 * \brief Destructeur de la classe ReseauInterurbain
 *
 */
    ReseauInterurbain::~ReseauInterurbain()
    {}
/**
 * \brief Change la taille du réseau en utilisant un nombre de villes = nouvelleTaille
 * @param nouvelleTaille la nouvelle taille de notre reseau urbain
 */
    void ReseauInterurbain::resize(size_t nouvelleTaille)
    {
        unReseau.resize(nouvelleTaille);
    }
/**
 * \brief Trouve le plus court chemin entre une ville source et une ville destination en utilisant l'algorithme de Dijkstra
 * @param origine la ville ou l'on part
 * @param destination la ville ou l'on veut aller
 * @param dureeCout true, on utilise la duree comme ponderation des trajets, false, on utilise le cout en $
 * @return
 */
    Chemin ReseauInterurbain::rechercheCheminDijkstra(const std::string &origine, const std::string &destination,
                                                      bool dureeCout) const
    {
        Chemin cheminPlusCourt;
        cheminPlusCourt.coutTotal = 0;
        cheminPlusCourt.dureeTotale= 0;
        cheminPlusCourt.reussi = false;

        size_t indexVilleDepart = unReseau.getNumeroSommet(origine);
        size_t indexVilleArrivee = unReseau.getNumeroSommet(destination);

        cheminPlusCourt.listeVilles.push_back(origine);
    }


}//Fin du namespace
