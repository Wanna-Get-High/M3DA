#include "nurbs.h"
#include <iostream>
#include "UtilGL.h"

using namespace std;

Nurbs::Nurbs() {}

/**
 * @brief computeBSpline Evalue la courbe BSpline a un instant t.
 *
 * @param t l'instant voulue.
 *
 * @return un vecteur contenant la position du point de la courbe a un instant t.
 */
Vector2 Nurbs::computeBSpline(double t) {
    Vector2 ret;

    int size = _controlPoints.size();

    for (int k = 0; k < size; k++) {
        ret = ret + (_controlPoints[k] * computeN(k,_degree,t));
    }

    return ret;
}

/**
 * @brief computeN evalue N en un point du vecteur nodale
 * @param k l'indice du vecteur nodale
 * @param degreeP le degree de la courbe
 * @param t l'instant
 *
 * @return la valeur sur l'axe y de la courbe.
 */
double Nurbs::computeN(double k, double degreeP, double t) {

    if (degreeP == 0) {
        if (t >= _nodalVector[k] && t < _nodalVector[k+1] ) {
            return 1;
        } else {
            return 0;
        }
    }

    double denominateur1 = (_nodalVector[k+degreeP] - _nodalVector[k]);
    double denominateur2 = (_nodalVector[k+degreeP+1] - _nodalVector[k+1]);
    double part1 = 0;
    double part2 = 0;

    // on evite la division part 0;
    if (denominateur1 != 0)
        part1 = (t - _nodalVector[k]) / denominateur1;

    if (denominateur2 != 0)
        part2 = (_nodalVector[degreeP+k+1] - t) / denominateur2;

    return part1*computeN(k,degreeP-1,t) + part2*computeN(k+1,degreeP-1,t);
}

/**
 * @brief drawN dessine la courbe N utilisant le calcule de Cox-DeBoor.
 * @param k l'indice du vecteur nodale
 */
void Nurbs::drawN(int k) {

    int size = _nodalVector.size();
    int tmin = _nodalVector[0];
    int tmax = _nodalVector[size-1];

    glPushMatrix();

    // on récupère la couleur suivante.
    std::vector<double> color = Nurbs::getNextColor();
    glColor3f(color[0],color[1],color[2]);
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);

    for (double t = tmin; t < tmax ; t+=0.1) {

        double eval = computeN(k,_degree,t);

        // on reviens dans le bon repert ([0,0] est le centre, [-1,-1] en haut a gauche, [1,1] en bas à droite)
        // -1 pour mettre la courbe du coté gauche
        double newT = t/size -1;

        glVertex2f(newT,eval);
    }

    glEnd();

    glPopMatrix();
}

/**  @brief drawPoints dessine les points de controle sur l'axe x de la representation graphique de N. */
void Nurbs::drawPoints() {
    glPushMatrix();

    int size = _nodalVector.size();

    glPointSize(10);
    glColor3f(0,1,0);
    glBegin(GL_POINTS);

    for (int i = 0; i < size; ++i) {
        glVertex2f(_nodalVector[i]/size - 1 ,0);
    }

    glEnd();
    glPopMatrix();
}

/**
 * @brief initializeColorVector Initialise le tableau de couleurs en fonction du nombre
 * de point de controle demande
 */
void Nurbs::initializeColorVector() {

    int size  = _nodalVector.size();

    std::vector< std::vector<double> > vec(size, std::vector<double>(3) );

    _randomColors = vec;

    for (int i = 0; i < size; i++) {

        // creer un veteur contenant les composante de la couleur aleatoire
        std::vector<double> row(3);

        double r = (rand() % 10) / 10.0;
        double g = (rand() % 10) / 10.0;
        double b = (rand() % 10) / 10.0;

        row[0] = r;
        row[1] = g;
        row[2] = b;

        // on ajoute ce vecteur.
        _randomColors[i] = row;
    }
}

/**
 * @brief getNextColor
 *
 *  retourne la prochaine couleur du tableau _randomColors afin d'avoir la meme couleur lors
 *  d'un rafraichissement de la fenetre.
 *
 * @return un vecteur de taille 3 contenant les 3 composante RGB.
 */
std::vector<double> Nurbs::getNextColor() {
    std::vector<double> ret = _randomColors.at(_arrayOfColorIndex);
    _arrayOfColorIndex++;
    return ret;
}

/**
 * @brief Nurbs::initialize
 *
 *  Initialise le vecteur nodal _knot en fonction du degre et du nombre de point de controle.
 *  Initialise aussi le tableau de couleur utilise our l'affichage des courbes.
 *
 * @param nbPointControle_m le nombre de point de controle
 * @param degre_p           le degre de la courbe
 */
void Nurbs::initialize(int numberOfControlePoint_m, int degre_p) {

    _nodalVector.clear();

    _arrayOfColorIndex = 0;

    _numberOfControlPoint = numberOfControlePoint_m;

    _degree = degre_p;

    for (int var = 1; var <= numberOfControlePoint_m+degre_p+1; ++var) {
        _nodalVector.push_back(var);
    }

    Nurbs::initializeColorVector();
}

/**
 * @brief addControlePoint ajoute un point de controle au vecteur _controlPoints.
 *
 *  Reinitialise le veteur nodale en consequence.
 *
 * @param x la position x du point (doit etre dans les coordonnes [-1,-1] -> [1,1] )
 * @param y la position y du point (doit etre dans les coordonnes [-1,-1] -> [1,1] )
 */
void Nurbs::addControlePoint(double x, double y) {
    _controlPoints.push_back(Vector2(x,y));
    Nurbs::initialize(_controlPoints.size(),_degree);
}

/**
 * @brief removeControlePoint eenleve le dernier point de controle ajoute au vecteur _controlPoints.
 *
 *  Reinitialise le veteur nodale en consequence.
 */
void Nurbs::removeControlePoint() {
    if (_controlPoints.size() > 0) {
        _controlPoints.pop_back();
        Nurbs::initialize(_controlPoints.size(),_degree);
    }
}

/**
 * @brief drawAnimation met a jour les courbes.
 *
 * @param t l'instant t (voir updateData de CLView)
 */
void Nurbs::updateView(double t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0,0,0);

    char text[200];
    sprintf(text,"nb pt de controle : %d",_numberOfControlPoint);
    prog3d::ugl::drawText(text,0,0.90);
    sprintf(text,"degre : %d",_degree);
    prog3d::ugl::drawText(text,0,0.95);

    Nurbs::drawAllN();
    Nurbs::drawControlPoint();
    Nurbs::drawAnimation(t);
}

/**
 * @brief drawAnimation dessine le point qui va parcourir a la fois
 * la courbe BSpline et ls courbe de l'evaluation de N.
 * @param t l'instant t (voir updateData de CLView)
 */
void Nurbs::drawAnimation(double t) {
    glPushMatrix();

    glColor3f(0,0,0);
    glPointSize(10);
    glBegin(GL_POINTS);


    int size = _controlPoints.size();
    double val = (_nodalVector[_degree] + (_nodalVector[size] - _nodalVector[_degree]) *t);

    Vector2 vec = Nurbs::computeBSpline(val);
    glVertex2f(vec.x(), vec.y());

    size = _nodalVector.size();
    val = (_nodalVector[0]+ (_nodalVector[size-1] - _nodalVector[0])*t) / size -1;
    glVertex2f(val ,0);

    glEnd();
    glPopMatrix();
}

/** @brief drawControlPoint dessine le polyogne de controle ainsi que la courbe BSpline. */
void Nurbs::drawControlPoint() {

    int size = _controlPoints.size();

    // on remet l'indice de parcour du vecteur des couleurs a 0;
    _arrayOfColorIndex = 0;

    glPushMatrix();


    // dessine le polygone de controle
    glColor3f(1,0,0);
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i < size ; i++) {
        Vector2 vec = _controlPoints.at(i);
        glVertex2f(vec.x(),vec.y());
    }

    glEnd();

    glBegin(GL_POINTS);

    // dessine les point de controle du polygone de la meme couleur que les courbes de N.
    for (int i = 0; i < size ; i++) {
        std::vector<double> color = Nurbs::getNextColor();
        glColor3f(color[0],color[1],color[2]);

        Vector2 vec = _controlPoints.at(i);
        glVertex2f(vec.x(),vec.y());
    }

    glEnd();

    // dessine la courbe BSpline du polygone de point de controle
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);

    int tmin = _nodalVector[_degree];
    int tmax = _nodalVector[_controlPoints.size()];

    for (double t = tmin; t < tmax; t+=0.1) {
        Vector2 vec = Nurbs::computeBSpline(t);
        glVertex2f(vec.x(),vec.y());
    }

    glEnd();

    glPopMatrix();
}

/**
 * @brief drawAllN dessine les courbes ainsi que les points de controle
 * de l'evaluation de N en fonction du vecteur nodale
 */
void Nurbs::drawAllN() {

    _arrayOfColorIndex = 0;

    int size = _nodalVector.size() - _degree -1;

    for (int i = 0; i < size; ++i) {
        Nurbs::drawN(i);
    }

    Nurbs::drawPoints();
}

/**
 * @brief incrDegree incremente le degree de la courbe.
 * Reinitialise le veteur nodale en consequence.
 */
void Nurbs::incrDegree() {
    _degree++;
    Nurbs::initialize(_controlPoints.size(),_degree);
}

/**
 * @brief decrDegree decremente le degree de la courbe.
 * Reinitialise le veteur nodale en consequence.
 */
void Nurbs::decrDegree() {
    if (_degree > 0) {
        _degree--;
        Nurbs::initialize(_controlPoints.size(),_degree);
    }
}
