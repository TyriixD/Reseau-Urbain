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
#include <algorithm>
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
        if(!unReseau.sommetExiste(origine) || !unReseau.sommetExiste(destination))
        {
            throw logic_error("L'origine ou la destination est absente du reseau");
        }

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
        size_t sommetArrivee = unReseau.getNumeroSommet(destination);

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
                // Si le sommet n'est pas solutionner et qu'il est la plus petite distance
                if(!sommetSolutionne[sommetAdjacent])
                {
                    nonSolutionne.insert(sommetAdjacent);
                }

            }

            //On cherche la prochaine ville a explorer en prenant celle avec la plus petite distance et pas dans le sous-graphe
            sommetSolutionne[sommetCourant] = true;
            nonSolutionne.erase(sommetCourant);
            float poidMinimum = 1e6;
            for (auto sommet: nonSolutionne)
            {
                if(distance[sommet] < poidMinimum)
                {
                    poidMinimum = distance[sommet];
                    sommetCourant = sommet;
                }
            }
            if (sommetCourant == sommetArrivee)
            {
                vector<size_t> cheminInverse;
                cheminInverse.push_back(sommetCourant);
                do
                {
                    cheminInverse.push_back(predecesseur[sommetCourant]);
                    sommetCourant = predecesseur[sommetCourant];
                }
                while(sommetCourant != unReseau.getNumeroSommet(origine));

                reverse(cheminInverse.begin(), cheminInverse.end()); //On remet le chemin dans le bon ordre, du debut à la fin
                for(size_t index = 0; index < cheminInverse.size(); index++)
                {
                    if(index > 0)
                    {
                        cheminPlusCourt.dureeTotale += obtenirCoutDistance(cheminInverse[index-1], cheminInverse[index],true);
                        cheminPlusCourt.coutTotal += obtenirCoutDistance(cheminInverse[index-1], cheminInverse[index], false);
                    }
                    cheminPlusCourt.listeVilles.push_back(unReseau.getNomSommet(cheminInverse[index]));
                }
                cheminPlusCourt.reussi = true;
                return cheminPlusCourt;
            }

        }


        }
/**
 * \brief Trouve l’ensemble des composantes fortement connexes en utilisant l'algorithme de Kosaraju
 * @return un vecteur de vecteurs de chaînes caractères
 */
    std::vector<std::vector<std::string> > ReseauInterurbain::algorithmeKosaraju()
    {
        vector<size_t> parcoursProfondeurGraphe = parcoursProfondeurDuGraphe();
        reverse(parcoursProfondeurGraphe.begin(),parcoursProfondeurGraphe.end());

        vector<vector<string> > composantes; //Un vecteur contenant nos ensemble de composante, les composantes sont sous formes de vecteur
        vector<bool> parcouru(unReseau.getNombreSommets(),false);
        stack<size_t> pile;
        stack<size_t> pileInverser;


        for(auto sommet: parcoursProfondeurGraphe){
            std::vector<std::string> composante_fortement_connexe;

            if(!parcouru[sommet]){
                for(auto noeud : parcoursProfondeurInverse(sommet,pileInverser, parcouru))
                {
                    composante_fortement_connexe.push_back(unReseau.getNomSommet(noeud));
                }
                composantes.push_back(composante_fortement_connexe);
            }
        }



        return composantes;
    }

    /**
     * \brief Vient chercher la ponderation d'un arc et calcul le cout de distance selon le booleen donnee
     * @param source le sommet de depart
     * @param destination le sommet d'arrive
     * @param dureeCout un booleen true, si on veut la duree, false si on veut le cout
     * @return le poid d'un arc
     */
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
/**
 * \brief fait le parcour en prodonfeur a partir d'un sommet et sa liste d'adjacence
 * @param source le sommet de depart
 * @param pile, une pile vide
 * @param parcouru un vecteur de booleen si les sommets sont parcourus ou non
 * @return un vecteur contenant les sommets du parcour en ordre
 */
    std::vector<size_t>
    ReseauInterurbain::parcoursProfondeur(size_t source, stack<size_t> &pile, vector<bool> &parcouru) const
    {
        vector<size_t> resultat;
        //On commence le parcours en visitant le premier sommet et en l'ajoutant à la pile
        parcouru[source] = true;
        pile.push(source);

        while(!pile.empty())
        {
            size_t prochainSommet = pile.top();
            pile.pop();
            resultat.push_back(prochainSommet);

            vector<size_t> sommetAdjacents = unReseau.listerSommetsAdjacents(prochainSommet);

            for(unsigned long & sommetAdjacent : sommetAdjacents)
            {
                if(!parcouru[sommetAdjacent])
                {
                    pile.push(sommetAdjacent);
                    parcouru[sommetAdjacent] = true;
                }
            }


        }
        return resultat;
    }
/**
 * \brief fait le parcour en prodonfeur a partir d'un sommet et sa liste d'adjacence inverse
 * @param source le sommet de depart
 * @param pile, une pile vide
 * @param parcouru un vecteur de booleen si les sommets sont parcourus ou non
 * @return un vecteur contenant les sommets du parcour en ordre
 */
    std::vector<size_t> ReseauInterurbain::parcoursProfondeurInverse(size_t source, stack<size_t> &pile, vector<bool> &parcouru) const
    {
        vector<size_t> resultat;

        parcouru[source] = true;
        pile.push(source);

        while(!pile.empty())
        {
            size_t prochainSommet = pile.top();
            pile.pop();
            resultat.push_back(prochainSommet);

            vector<size_t> sommetAdjacentsInverse = unReseau.GetSommetAdjacentInverse(prochainSommet);

            for(unsigned long & sommetAdjacent : sommetAdjacentsInverse)
            {
                if(!parcouru[sommetAdjacent])
                {
                    pile.push(sommetAdjacent);
                    parcouru[sommetAdjacent] = true;
                }
            }


        }
        return resultat;
    }
/**
 * \brief Fait le parcour en profondeur pour chaque sommet du graphe
 * @return un vecteur avec les sommets du parcour
 */
    std::vector<size_t> ReseauInterurbain::parcoursProfondeurDuGraphe() const
    {
        std::vector<size_t> DFS;
        std::vector<bool> parcouru(unReseau.getNombreSommets(),false);
        std::stack<size_t> pile;

        for(size_t index=0; index < unReseau.getNombreSommets(); index++){
            if(!parcouru[index]){
                for(auto noeud : parcoursProfondeur(index,pile, parcouru)){
                    DFS.push_back(noeud);
                }
            }
        }
        return DFS;
    }




}//Fin du namespace
