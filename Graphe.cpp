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
        nbArcs--;

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
/**
 * \brief Retourne la liste de successeurs d'un sommmet, lance une exception si le sommet est superieur a nb sommet
 * @param sommet Le sommet que l'on cherche sa liste d'adjacence
 * @return la liste de sommet adjacent
 */
    std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const
    {
        if (sommet > nbSommets)
        {
            throw logic_error("Le sommet est supérieur à nbSommets");
        }
        std::vector<size_t> listeSommetAdjacents;
        for(auto arc: listesAdj[sommet]){
            listeSommetAdjacents.push_back(arc.destination);
        }
        return listeSommetAdjacents;
    }
/**
 * \brief Retourne le nom d'un sommet
 * @param sommet l'indice du sommet que l'on cherche
 * @return le nom du sommet
 */
    std::string Graphe::getNomSommet(size_t sommet) const
    {
        if (sommet > nbSommets)
        {
            throw logic_error("Le sommet est supérieur à nbSommets");
        }

        return noms[sommet];
    }

/**
 * \brief Retourne le numéro d'un sommet
 * @param nom le nom du sommet dont on veut le numero
 * @return le nom du sommet
 */
    size_t Graphe::getNumeroSommet(const string &nom) const
    {
        for (size_t i = 0; i < noms.size(); i++)
        {
            if (noms[i] == nom){
                return i;
            }
        }
        throw logic_error("Le nom de l'arc est introuvable");

    }
/**
 * \brief Retourne le nombre de sommet du graphe
 * @return nbSommets
 */
    int Graphe::getNombreSommets() const
    {
        return nbSommets;
    }
/**
 * \brief Retourne le nombre des arcs du graphe
 * @return nbArcs
 */
    int Graphe::getNombreArcs() const
    {
        return nbArcs;
    }
/**
 * \brief Retourne les pondérations se trouvant dans un arc (source -> destination), lance une exception si la source supérieurs a nb sommet et si arc existe pas
 * @param source
 * @param destination
 * @return
 */
    Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const
    {
        if(source > nbSommets || destination > nbSommets){
            throw logic_error("La source ou la destination est plus grande que le nombre de sommet");
        }
        if(!arcExiste(source, destination)){
            throw logic_error("L'arc n'existe pas dans le graphe");
        }

        for(Arc arc : listesAdj[source]){
            if (arc.destination == destination){
                return arc.poids;
            }
        }
    }


}//Fin du namespace
