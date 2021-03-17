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
#include <set>
#include "ReseauInterurbain.h"
using namespace std;
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

        vector<unsigned int> distance(unReseau.getNombreSommets(), numeric_limits<unsigned int>::max()); //on met les distances a infini au debut
        vector<size_t> predecesseur(unReseau.getNombreSommets(), numeric_limits<size_t>::max()); //on met les sommets inconnue
        vector<bool> sommetSolutionne(unReseau.getNombreSommets(), false); //on met tous les sommets a non parcourus

        //Creation d'un set pour notre ensemble de sommet non solutionnés
        set<size_t> nonSolutionne;

        size_t sommetCourant = unReseau.getNumeroSommet(origine);

        distance[sommetCourant] = 0;


        nonSolutionne.insert(unReseau.getNumeroSommet(origine));

        while (!nonSolutionne.empty())
        {
            for(auto sommetAdjacent: unReseau.listerSommetsAdjacents(sommetCourant))
            {
                float poidMinimum = obtenirCoutDistance(sommetCourant,sommetAdjacent,dureeCout);
                //relachement des arcs et mise a jour des poids et predecesseur
                if (distance[sommetAdjacent] > distance[sommetCourant] + poidMinimum)
                { //Si le poid est plus grand que ce qu'on a, on l'actualise
                    distance[sommetAdjacent] = distance[sommetCourant] + poidMinimum;
                    predecesseur[sommetAdjacent] = sommetCourant;
                }
                //On met notre ville en tant que solutionnée
            }
            sommetSolutionne[sommetCourant] = true;




        }

        }

    std::vector<std::vector<std::string> > ReseauInterurbain::algorithmeKosaraju()
    {
        return std::vector<std::vector<std::string>>();
    }

    float ReseauInterurbain::obtenirCoutDistance(size_t source, size_t destination, bool dureeCout) const
    {
        Ponderations poid = unReseau.getPonderationsArc(source,destination);

        float valeurDeRetour;
        if (dureeCout){
            valeurDeRetour = poid.duree;
        }
        else{
            valeurDeRetour = poid.cout;
        }
        return valeurDeRetour;
    }


}//Fin du namespace
