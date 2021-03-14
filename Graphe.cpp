/**
 * \file Graphe.cpp
 * \brief Implémentation d'un graphe orienté.
 * \author IFT-2008 & ?
 * \version 0.1
 * \date mars 2021
 *
 *  Travail pratique numéro 2
 *
 */

#include "Graphe.h"

using namespace std;
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
    //Mettez l'implémentation de vos méthodes ici.

    /**
 * \brief Constructeur par defaut du graphe, il initialise l'attribut nbSommets a 10 par défault
 */
    Graphe::Graphe(size_t nbSommets) : nbSommets(10), listesAdj(std::vector<std::list<Arc>>()), nbArcs(0)
    {

    }

    /**
 * \brief Destructeur du graphe
 */
    Graphe::~Graphe()
    {}

    /**
        * \brief Modifie le nombre de sommets du graphe
        *
        * \post Le nombre de sommet a augmenter
        * \param[in] nouvelleTaille le nombre de sommets du graphe a modifier
        */
    void Graphe::resize(size_t nouvelleTaille)
    {
        noms.resize(nouvelleTaille);
        listesAdj.resize(nouvelleTaille);
        nbSommets = nouvelleTaille;
    }
/**
 * \brief Donne un nom à un sommet en utlisant son numéro (indice dans le vector).
 * @param sommet l'indice du sommet
 * @param nom Le nom que l'on veut assigner au sommet
 */
    void Graphe::nommer(size_t sommet, const string &nom)
    {
        noms[sommet] = nom;
    }
/**
 * \brief Ajoute un arc au graphe, lance exception si arc existe deja ou sommet trop grand
 * @param source le point de départ
 * @param destination La destination où l'arc mène
 * @param duree pour la pondération
 * @param cout pour la pondération
 */
    void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout)
    {
        if (source > nbSommets || destination > nbSommets)
        {
            throw logic_error("L'indice du sommet ou de la destination est plus excède la grosseur du graphe");
        }
        if (arcExiste(source,destination))
        {
            throw logic_error("L'arc existe déjà dans le graphe");
        }
        Ponderations ponderations(duree,cout);
        Arc arcAjoute(destination,ponderations);

        listesAdj[source].push_back(arcAjoute);
        nbArcs++;

    }
/**
 * \brief Supprime un arc du graphe, lance exception si arc existe pas ou sommet trop grand
 * @param source le point de départ
 * @param destination La destination où l'arc mène
 */
    void Graphe::enleverArc(size_t source, size_t destination)
    {
        if (source > nbSommets || destination > nbSommets)
        {
            throw logic_error("L'indice du sommet ou de la destination est plus excède la grosseur du graphe");
        }
        if (!arcExiste(source,destination))
        {
            throw logic_error("L'arc n'existe pas dans le graphe");
        }

        list<Arc>:: iterator arcPourSuppression;
        for(auto it = listesAdj[source].begin(); it != listesAdj[source].end(); it++)
        {
            if (it->destination == destination)
            {
                arcPourSuppression = it;
            }
        }
        listesAdj[source].erase(arcPourSuppression);

    }
/**
 * \brief Vérifie si un arc existe
 * @param source Le sommet que l'on cherche dans la liste
 * @param destination La destination de l'arc
 * @return
 */
    bool Graphe::arcExiste(size_t source, size_t destination) const
    {
        bool valeurPourRetour = false;

        for(auto it = listesAdj[source].begin(); it != listesAdj[source].end(); it++)
        {
            if (it->destination == destination)
            {
                valeurPourRetour = true;
            }
        }
        return valeurPourRetour;
    }

}//Fin du namespace
