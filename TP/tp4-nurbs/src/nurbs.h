#ifndef NURBS_H
#define NURBS_H

#include "UtilGL.h"
#include "vector2.h"

class Nurbs
{

private:

    /** @brief _degree le degre de la courbe */
    int _degree;

    /**
     * @brief _arrayOfColorIndex l'indexe utilise pour eviter de changer de couleur lors d'un rafraichissement
     * de la fenetre.
     */
    int _arrayOfColorIndex;

    /** @brief _numberOfControlPoint le nombre de point de controle. */
    int _numberOfControlPoint;

    /** @brief _nodalVector le vecteur nodale */
    std::vector<double> _nodalVector;

    /** @brief _controlPoints le tableau de Vecteur2 contenant les positions de points de controle */
    std::vector<Vector2> _controlPoints;

    /** @brief _randomColors le tableau de couleurs utilise pour l'affichage des points de controle */
    std::vector< std::vector<double> > _randomColors;

    /**
     * @brief computeBSpline Evalue la courbe BSpline a un instant t.
     *
     * @param t l'instant voulue.
     *
     * @return un vecteur contenant la position du point de la courbe a un instant t.
     */
    Vector2 computeBSpline(double t);

    /**
     * @brief computeN evalue N en un point du vecteur nodale
     * @param k l'indice du vecteur nodale
     * @param degreeP le degree de la courbe
     * @param t l'instant
     *
     * @return la valeur sur l'axe y de la courbe.
     */
    double computeN(double k, double degreeP, double t);

    /**
     * @brief drawN dessine la courbe N utilisant le calcule de Cox-DeBoor.
     * @param k l'indice du vecteur nodale
     */
    void drawN(int k);

    /**  @brief drawPoints dessine les points de controle sur l'axe x de la representation graphique de N. */
    void drawPoints();

    /**
     * @brief initializeColorVector Initialise le tableau de couleurs en fonction du nombre
     * de point de controle demande
     */
    void initializeColorVector();

    /**
     * @brief getNextColor
     *
     *  retourne la prochaine couleur du tableau _randomColors afin d'avoir la meme couleur lors
     *  d'un rafraichissement de la fenetre.
     *
     * @return un vecteur de taille 3 contenant les 3 composante RGB.
     */
    std::vector<double> getNextColor();

public:

    Nurbs();

    /**
     * @brief Nurbs::initialize
     *
     *  Initialise le vecteur nodal _knot en fonction du degre et du nombre de point de controle.
     *  Initialise aussi le tableau de couleur utilise our l'affichage des courbes.
     *
     * @param nbPointControle_m le nombre de point de controle
     * @param degre_p           le degre de la courbe
     */
    void initialize(int nbPointControle_m, int degre_p);

    /**
     * @brief addControlePoint ajote un point de controle au vecteur _controlPoints.
     *
     *  Reinitialise le veteur nodale en consequence.
     *
     * @param x la position x du point (doit etre dans les coordonnes [-1,-1] -> [1,1] )
     * @param y la position y du point (doit etre dans les coordonnes [-1,-1] -> [1,1] )
     */
    void addControlePoint(double x, double y);

    /**
     * @brief removeControlePoint eenleve le dernier point de controle ajoute au vecteur _controlPoints.
     *
     *  Reinitialise le veteur nodale en consequence.
     */
    void removeControlePoint();

    /**
     * @brief drawAnimation met a jour les courbes.
     *
     * @param t l'instant t (voir updateData de CLView)
     */
    void updateView(double t);

    /**
     * @brief drawAnimation dessine le point qui va parcourir a la fois
     * la courbe BSpline et ls courbe de l'evaluation de N
     * @param t l'instant t (voir updateData de CLView)
     */
    void drawAnimation(double t);

    /** @brief drawControlPoint dessine le polyogne de controle ainsi que la courbe BSpline. */
    void drawControlPoint();

    /**
     * @brief drawAllN dessine les courbes ainsi que les points de controle
     * de l'evaluation de N en fonction du vecteur nodale
     */
    void drawAllN();

    /**
     * @brief incrDegree incremente le degree de la courbe.
     * Reinitialise le veteur nodale en consequence.
     */
    void incrDegree();

    /**
     * @brief decrDegree decremente le degree de la courbe.
     * Reinitialise le veteur nodale en consequence.
     */
    void decrDegree();
};

#endif // NURBS_H
